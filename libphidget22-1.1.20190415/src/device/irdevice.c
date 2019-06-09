/*
 * This file is part of libphidget22
 *
 * Copyright 2015 Phidgets Inc <patrick@phidgets.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/>
 */
#include "phidgetbase.h"
#include "device/irdevice.h"
#include "usb.h"

static PhidgetReturnCode analyze_data(PhidgetIRDeviceHandle phid, int trailgap_needed);
static PhidgetReturnCode learn_data(PhidgetIRDeviceHandle phid);
static PhidgetReturnCode _Transmit(mosiop_t iop, PhidgetIRDeviceHandle phid, const char *data, PhidgetIR_CodeInfoHandle codeInfo);
static PhidgetReturnCode _TransmitRepeat(mosiop_t iop, PhidgetIRDeviceHandle phid);
static PhidgetReturnCode _TransmitRaw(mosiop_t iop, PhidgetIRDeviceHandle phid, const int *data, int length, int carrierFrequency, double dutyCycle, int gap);

//initAfterOpen - sets up the initial state of an object, reading in packets from the device if needed
//				  used during attach initialization - on every attach
static PhidgetReturnCode CCONV
PhidgetIRDevice_initAfterOpen(PhidgetDeviceHandle device) {
	PhidgetIRDeviceHandle phid = (PhidgetIRDeviceHandle)device;
	assert(phid);

	phid->lastDataTime = mos_gettime_usec();

	switch (phid->phid.deviceInfo.UDD->uid) {
	case PHIDUID_1055:

		phid->lastGap = PUNK_BOOL;

		//set data to unknown
		phid->polarity = PUNK_BOOL;

		//reset data pointers
		phid->dataReadPtr = 0;
		phid->dataWritePtr = 0;
		phid->learnReadPtr = 0;

		phid->lastCodeKnown = PFALSE;
		phid->lastRepeat = PUNK_BOOL;
		phid->lastLearnedCodeKnown = PFALSE;
		phid->lastGap = PUNK_BOOL;

		memset(&phid->lastCodeInfo, 0, sizeof(phid->lastCodeInfo));
		memset(&phid->lastLearnedCodeInfo, 0, sizeof(phid->lastLearnedCodeInfo));

		break;

	default:
		MOS_PANIC("Unexpected device");
	}

	return (EPHIDGET_OK);
}

static void
codeStrToByteArray(uint8_t *arr, size_t arrLen, const char *str) {
	int i;
	char tmpStr[3] = { 0 };
	const char *strPtr = str;
	size_t len = strlen(str);

	//verify that arr is big enough
	assert(arrLen >= (len / 2 + ((len % 1) ? 1 : 0)));

	i = 0;
	if (len % 1) {
		tmpStr[0] = strPtr[0];
		arr[i++] = (uint8_t)strtol(tmpStr, NULL, 16);
		len--;
		strPtr++;
	}
	while (len) {
		tmpStr[0] = strPtr[0];
		tmpStr[1] = strPtr[1];
		arr[i++] = (uint8_t)strtol(tmpStr, NULL, 16);
		strPtr+=2;
		len-=2;
	}
}

//dataInput - parses device packets
static PhidgetReturnCode CCONV
PhidgetIRDevice_dataInput(PhidgetDeviceHandle device, uint8_t *buffer, size_t length) {
	PhidgetIRDeviceHandle phid = (PhidgetIRDeviceHandle)device;
	int data[IR_MAX_DATA_PER_PACKET];
	PhidgetChannelHandle channel;
	int i, dataLength;
	mostime_t duration;
	uint32_t us;

	assert(phid);
	assert(buffer);

	//Parse device packets - store data locally
	switch (phid->phid.deviceInfo.UDD->uid) {
	case PHIDUID_1055:
		dataLength = buffer[0];

		for (i = 1; i <= dataLength; i++) {
			us = (((buffer[i * 2 - 1] & 0x7F) << 8) + buffer[i * 2]) * 10; //us

			//this means it's over the length we can measure
			if (us >= IR_MAX_DATA_us)
				us = IR_RAWDATA_LONGSPACE;

			if (phid->polarity == PUNK_BOOL) {
				//first time, set it
				phid->polarity = buffer[i * 2 - 1] & 0x80;

				/*
				 * If the us is not IR_RAWDATA_LONGSPACE, we have to add a IR_RAWDATA_LONGSPACE
				 * otherwise we can't recognize the first code
				 */
				if (us != IR_RAWDATA_LONGSPACE) {
					phid->dataBuffer[phid->dataWritePtr] = IR_RAWDATA_LONGSPACE;

					phid->dataWritePtr++;
					phid->dataWritePtr &= IR_DATA_ARRAY_MASK;
				}
			} else {
				phid->polarity ^= 0x80; //switch it each time
			}

			data[i - 1] = us;
			phid->dataBuffer[phid->dataWritePtr] = us;

			phid->dataWritePtr++;
			phid->dataWritePtr &= IR_DATA_ARRAY_MASK;

			/*
			 * If we run into data that hasn't been read... too bad,
			 * we overwrite it and adjust the read pointer.
			 */
			if (phid->dataWritePtr == phid->dataReadPtr) {
				phid->dataReadPtr++;
				phid->dataReadPtr &= IR_DATA_ARRAY_MASK;
			}

			// make sure it's right
			if (phid->polarity != (buffer[i * 2 - 1] & 0x80)) {
				logerr("IR data has gotten out of sync!");

				//invalidate the buffer
				phid->dataReadPtr = phid->dataWritePtr;

				// TODO: Inform channel that it's buffer should be invalidated

				phid->polarity ^= 0x80;
			}
		}
		break;
	default:
		MOS_PANIC("Unexpected device");
	}

	if (dataLength > 0) {
		phid->lastDataTime = mos_gettime_usec();
		phid->delayCode = PTRUE;

		if ((channel = getChannel(phid, 0)) != NULL) {
			bridgeSendToChannel(channel, BP_RAWDATA, "%*U", dataLength, data);
			PhidgetRelease(&channel);
		}

		//analyze data
		analyze_data(phid, PTRUE);
		learn_data(phid);
	} else {
		duration = mos_gettime_usec() - phid->lastDataTime;

		if (duration > IR_MAX_GAP_LENGTH && phid->delayCode) {
			/*
			* We didn't get any data, so try and look at what we do have
			* run this if we haven't had data for > MAX_GAP
			*/
			analyze_data(phid, PFALSE);
			phid->delayCode = PFALSE;
		}
		if (duration > 300000) //300 ms for learning
			learn_data(phid); //didn't get any data, so try and look at what we do have
	}


	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
PhidgetIRDevice_bridgeInput(PhidgetChannelHandle ch, BridgePacket *bp) {
	PhidgetIR_CodeInfo codeInfo;
	PhidgetIRDeviceHandle phid;
	PhidgetReturnCode res;
	const char *codeStr;

	phid = (PhidgetIRDeviceHandle)ch->parent;

	assert(phid->phid.deviceInfo.class == PHIDCLASS_IR);
	assert(ch->class == PHIDCHCLASS_IR);
	assert(ch->index == 0);

	switch (bp->vpkt) {
	case BP_TRANSMIT:
		res = readCodeInfo(bp, &codeInfo);
		if (res != EPHIDGET_OK)
			return (res);

		codeStr = getBridgePacketString(bp, 0);

		if (strlen(codeStr) != IR_DATASIZE(codeInfo.bitCount) * 2)
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG, "Code sength does not match bit count."));

		return (_Transmit(bp->iop, phid, codeStr, &codeInfo));
	case BP_TRANSMITRAW:
		return (_TransmitRaw(bp->iop, phid, (const int *)getBridgePacketUInt32Array(bp, 0),
		  getBridgePacketArrayCnt(bp, 0), getBridgePacketUInt32(bp, 1), getBridgePacketDouble(bp, 2),
		  getBridgePacketUInt32(bp, 3)));
	case BP_TRANSMITREPEAT:
		return (_TransmitRepeat(bp->iop, phid));
	case BP_OPENRESET:
	case BP_CLOSERESET:
	case BP_ENABLE:
		return (EPHIDGET_OK);
	default:
		MOS_PANIC("Unexpected packet type");
	}
}

static PhidgetReturnCode
sendRAWData(mosiop_t iop, PhidgetIRDeviceHandle phid, int *data, int length, int carrier, double dutyCycle, int gap) {
	uint8_t buffer[8] = { 0 };
	int i, j, result;

	if (length > 0xff)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Invalid length, must be <= 255."));

	buffer[0] = IR_DEFINEDATA_PACKET;
	buffer[1] = (length >> 8) & 0xff;
	buffer[2] = length & 0xff;
	buffer[3] = (uint8_t)round(((double)(1 / (double)carrier) * 1600000.0 - 1)); //period
	buffer[4] = (uint8_t)round((((double)(1 / (double)carrier) * 1600000.0 - 1) * (dutyCycle))); //pulse width
	buffer[5] = (uint8_t)(round(gap / 10.0) >> 8) & 0xff;
	buffer[6] = (uint8_t)round(gap / 10.0) & 0xff;
	//can leave this out to receive while we transmit - TESTING ONLY!
	buffer[7] = IR_STOP_RX_WHILE_TX_FLAG;

	if ((result = PhidgetDevice_sendpacket(iop, (PhidgetDeviceHandle)phid, buffer, 8)) != EPHIDGET_OK) {
		return result;
	}

	for (i = 0; i < length; i++) {
		j = i % 8;
		buffer[j] = data[i];
		if (j == 7 || i == (length - 1)) {
			if ((result = PhidgetDevice_sendpacket(iop, (PhidgetDeviceHandle)phid, buffer, j + 1)) != EPHIDGET_OK) {
				return result;
			}
		}
	}

	return (EPHIDGET_OK);
}
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define pdiff(a, b) (ABS((signed)(a) - (signed)(b)) / (double)( ((a) + (b)) / 2.0 ))

#define COUNT_TIMES(highlow, index)								\
	if (highlow##s[index] == data[i] || !highlow##s[index]) {	\
		highlow##s[index] = data[i];							\
		highlow##Counts[index]++;								\
		if (highlow##count < (index + 1))						\
			highlow##count = (index + 1);						\
		continue;												\
	}

#define ORDER_TIMES(highlow, index)	do {										\
	if (highlow##s[index + 1] && (highlow##s[index + 1] < highlow##s[index])) {	\
		int temp = highlow##Counts[index];										\
		highlow##Counts[index] = highlow##Counts[index + 1];					\
		highlow##Counts[index + 1] = temp;										\
		temp = highlow##s[index];												\
		highlow##s[index] = highlow##s[index + 1];								\
		highlow##s[index + 1] = temp;											\
	}																			\
} while (0)

static int CCONV_CDECL
compare_int(const void *arg1, const void *arg2) {
	const int *int1 = (const int *)arg1;
	const int *int2 = (const int *)arg2;

	return (*int1 - *int2);
}

static void
codeToStr(char *str, size_t strLen, uint8_t *data, int bitCount) {
	size_t dataSize = IR_DATASIZE(bitCount);
	assert(strLen >= (dataSize * 2 + 1));

	// if dataSize is odd, we need to have an odd number of chars in the string,
	// so, we print the fisrt byte as a single char
	if (dataSize % 1) {
		sprintf(str, "%01x", *data++);
		str+=1;
		dataSize-=1;
	}

	while (dataSize--) {
		sprintf(str, "%02x", *data++);
		str+=2;
	}
}

//we can count on data having at most two high and two low lengths (except for RC-6)
//the first entry in data is a pulse, and the last is a pulse
//outlength will be in bits, but it should be specified in bytes (array size)
static PhidgetReturnCode
decode_data(int *data, int inlength, uint8_t *decoded_data, int *outlength, PhidgetIR_CodeInfoHandle codeInfo) {
	int i, highcount = 0, lowcount = 0, bitcount = 0, j = 0, n = 0;
	//room for 4
	int highs[5] = { 0 }, highCounts[5] = { 0 }, lows[5] = { 0 }, lowCounts[5] = { 0 };
	uint8_t decode_temp[IR_MAX_CODE_DATA_LENGTH] = { 0 };

	//get the numbers - only allow up to four each of high/low!!
	for (i = 0; i < inlength; i++) {
		if (!(i % 2)) {
			COUNT_TIMES(high, 0);
			COUNT_TIMES(high, 1);
			COUNT_TIMES(high, 2);
			COUNT_TIMES(high, 3);

			goto errorexit;
		} else {
			COUNT_TIMES(low, 0);
			COUNT_TIMES(low, 1);
			COUNT_TIMES(low, 2);
			COUNT_TIMES(low, 3);

			goto errorexit;
		}

		//logdebug("%07d,",data[i]);
	}

	//this sorts them
	for (i = 0; i < 5; i++) {
		ORDER_TIMES(high, 0);
		ORDER_TIMES(high, 1);
		ORDER_TIMES(high, 2);
		ORDER_TIMES(high, 3);
		ORDER_TIMES(low, 0);
		ORDER_TIMES(low, 1);
		ORDER_TIMES(low, 2);
		ORDER_TIMES(low, 3);
	}
	//sanity checks
	if (lowcount == 0 || highcount == 0)
		goto errorexit;

	//figure out the encoding
	//Bi-phase
	//either one high or two highs and one is twice the other (10% error)
	//and either one low or two lows and one is twice the other (10% error)
	//make sure that lows and highs are same length (30% error)
	if
		(
			(
				(highcount == 1) &&
				(lowcount == 1) &&
				(pdiff(highs[0], lows[0]) < 0.3)
				) ||
			(
				(highcount == 1) &&
				(lowcount >= 2) &&
				(pdiff(highs[0], lows[0]) < 0.3) &&
				(pdiff(lows[1] / 2.0, lows[0]) < 0.1)
				) ||
			(
				(highcount >= 2) &&
				(lowcount == 1) &&
				(pdiff(highs[0], lows[0]) < 0.3) &&
				(pdiff(highs[1] / 2.0, highs[0]) < 0.1)
				) ||
			(
				(highcount >= 2) &&
				(lowcount >= 2) &&
				(pdiff(highs[0], lows[0]) < 0.3) &&
				(pdiff(highs[1], lows[1]) < 0.3) &&
				(pdiff(highs[1] / 2.0, highs[0]) < 0.1) &&
				(pdiff(lows[1] / 2.0, lows[0]) < 0.1)
				)
			) {
		//could also be RC-5 or RC-6
		codeInfo->encoding = IR_ENCODING_BIPHASE;
		goto biphase;
	}
	//check for some special RC-6 cases
	//there will always be either 2 or 3 of high/low
	//there will always be a short high pulse
	else if (
		(
			//1x, 2x, 3x high, 2x low
			(highcount == 3) &&
			(lowcount == 1) &&
			(pdiff(highs[1], lows[0]) < 0.3) &&
			(pdiff(highs[2] / 3.0, highs[0]) < 0.1)
			) ||
		(
			//1x + 3x high, 1x, 2x, 3x low
			(highcount == 2) &&
			(lowcount == 3) &&
			(pdiff(highs[0], lows[0]) < 0.3) &&
			(pdiff(highs[1], lows[2]) < 0.3) &&
			(pdiff(lows[1] / 2.0, lows[0]) < 0.1) &&
			(pdiff(lows[2] / 3.0, lows[0]) < 0.1)
			) ||
		(
			//1x, 2x, 3x high, 2x, 3x low
			(highcount == 3) &&
			(lowcount == 2) &&
			(pdiff(highs[2], lows[1]) < 0.3) &&
			(pdiff(highs[1], lows[0]) < 0.3) &&
			(pdiff(highs[1] / 2.0, highs[0]) < 0.1) &&
			(pdiff(highs[2] / 2.0, highs[1]) < 0.1)
			)
		) {
		//RC-6 mode 0 always starts with bit0 == 1 (444us pulse - 444us space)
		//header is 2.666us pulse - 889us space
		//bit4 == toggle bit, and had double bit time (889us - 889us)
		//full length is 21 bits
		//trailing gap (signal free time) is 2.666us
		codeInfo->encoding = IR_ENCODING_RC6;
		goto biphase;
	} else {
		goto notbiphase;
	}

biphase:
	{
		int bit, startspace = 0, halfbit = 0, rc6temp = 0;

		//find pulse short pulse width - do an average if possible
		int pulse = lows[0];
		if (pdiff(lows[0], highs[0]) < 0.3) {
			pulse = round((lows[0] + highs[0]) / 2.0);
		} else if (highs[0] < pulse)
			pulse = highs[0];


		codeInfo->one[0] = pulse;
		codeInfo->one[1] = pulse;
		codeInfo->zero[0] = pulse;
		codeInfo->zero[1] = pulse;

		//make sure 1st pulse is short
		if (pdiff(data[0], pulse) > 0.3)
			goto notbiphase;

		//it's biphase! now decode..
		//note that there may be an 'invisible' starting space
		//- we can see this if the first long pulse is on an even i
		//note this could cause trouble for RC-6 when there isn't a 2x pulse before the TR bit,
		// or even for normal biphase/RC-5 when there isn't a 2x at all
		//default is 1 - this should be good for all but RC-6
		startspace = 1;
		for (i = 0; i < inlength; i++) {
			//found a double pulse
			if (pdiff(data[i] / 2.0, pulse) < 0.3) {
				if (!(i % 2))
					startspace = 1;
				else
					startspace = 0;

				break;
			}
		}

		//actually decode, we assume that the first bit is always 1
		//we are starting on the second pulse/space (first pulse/space is always short)
		bitcount = 0;
		bit = 1;
		halfbit = 1;
		for (i = 1 - startspace; i < inlength; i++) {
			//when bit is not changing we will set decode_temp bits twice - no worries there
			decode_temp[bitcount / 8] |= bit << (bitcount % 8);

			//found a triple pulse - this had better be RC-6
			if (pdiff(data[i] / 3.0, pulse) < 0.2) {
				if (!halfbit || (bitcount != 3 && bitcount != 4)) {
					goto notbiphase;
				}
				if (rc6temp == 0)
					rc6temp = 1;
				else
					rc6temp = 0;
				bit ^= 1;
				bitcount++;
				codeInfo->encoding = IR_ENCODING_RC6;
			}
			//found a double pulse
			else if (pdiff(data[i] / 2.0, pulse) < 0.2) {
				//probably this is PDM or PWM, but it could also be RC-6
				if (!halfbit) {
					//check for RC-6
					if (bitcount == 4) {
						halfbit ^= 1;
						rc6temp = 1;
						codeInfo->encoding = IR_ENCODING_RC6;
					} else
						goto notbiphase;
				} else {
					if (rc6temp == 1) {
						rc6temp = 0;
						halfbit ^= 1;
					} else {
						bit ^= 1;
					}
					bitcount++;
				}
			} else {
				if (halfbit)
					bitcount++;
				halfbit ^= 1;
			}
		}
		decode_temp[bitcount / 8] |= bit << (bitcount % 8);
		if ((halfbit && !bit) || (!startspace && bit))
			bitcount++;

		//success, presumably
		//see if it's RC-5 - 889 pulse time
		//TODO also check data length, etc.
		if (pdiff(pulse, 889) < 0.2
			&& codeInfo->encoding != IR_ENCODING_RC6
			&& startspace) {
			codeInfo->encoding = IR_ENCODING_RC5;
		}

		//TODO: if RC-6, make sure data length, bit times, etc. match, otherwise, it's not biphase!

		goto done;
	}

notbiphase:
	//make sure it's zeroed - we may have started filling it in in the biphase section
	memset(decode_temp, 0, IR_MAX_CODE_DATA_LENGTH);
	//Pulse Distance Modulation (PDM) - most common (NEC, etc.)
	//should be a trailing pulse that we just ignore for now - it just lets us get the trailing space width
	if (highcount == 1 && lowcount == 2) {
		int dataTimeZero = lows[1], dataTimeOne = lows[0];
		if (lows[1] > lows[0]) {
			dataTimeZero = lows[0];
			dataTimeOne = lows[1];
		}
		//get the data
		bitcount = 0;
		for (i = 0; i < (inlength - 1); i += 2) {
			if (data[i + 1] == dataTimeOne)
				decode_temp[bitcount / 8] |= (1 << (bitcount % 8));
			bitcount++;
		}

		codeInfo->one[0] = highs[0];
		codeInfo->one[1] = dataTimeOne;
		codeInfo->zero[0] = highs[0];
		codeInfo->zero[1] = dataTimeZero;

		codeInfo->trail = data[inlength - 1];
		codeInfo->encoding = IR_ENCODING_SPACE;
	}
	//PWM (SIRC)
	//in PWM, we take the trailing pulse to be a bit
	else if (highcount == 2 && lowcount == 1) {
		int dataTimeZero = highs[1], dataTimeOne = highs[0];
		if (highs[1] > highs[0]) {
			dataTimeZero = highs[0];
			dataTimeOne = highs[1];
		}
		//get the data
		bitcount = 0;
		for (i = 0; i < inlength; i += 2) {
			if (data[i] == dataTimeOne)
				decode_temp[bitcount / 8] |= (1 << (bitcount % 8));
			bitcount++;
		}

		codeInfo->one[0] = dataTimeOne;
		codeInfo->one[1] = lows[0];
		codeInfo->zero[0] = dataTimeZero;
		codeInfo->zero[1] = lows[0];

		codeInfo->encoding = IR_ENCODING_PULSE;
	} else
		goto errorexit;

done:
	//check size of output buffer
	if (*outlength < IR_DATASIZE(bitcount))
		goto errorexit;

	//limit - code have to be >= 4-bit
	if (bitcount < 4)
		goto errorexit;

	codeInfo->bitCount = bitcount;
	*outlength = IR_DATASIZE(bitcount);

	//data is LSB first now, lets flip it to MSB first - we don't change the Byte order though, just the bit order in the bytes
	memset(decoded_data, 0, *outlength);
	for (i = 0, j = bitcount - 1, n = (*outlength * 8) - 1; i < bitcount; i++, j--, n--)
		decoded_data[n / 8] |= ((decode_temp[j / 8] & (1 << (j % 8))) ? (0x01 << (i % 8)) : 0);

	return (EPHIDGET_OK);

errorexit:
	codeInfo->encoding = IR_ENCODING_UNKNOWN;
	return (EPHIDGET_UNEXPECTED);
}

//uses the data array
static void
dataTime(uint32_t *data, int *time, int ptr, int endptr) {
	int i;
	int length = endptr - ptr;
	if (length < 0)
		length += IR_DATA_ARRAY_SIZE;
	*time = 0;
	for (i = 0; i < length; i++) {
		*time += data[ptr];
		ptr = (ptr + 1)&IR_DATA_ARRAY_MASK;
	}
}

//uses the data array
static BOOL
compareDataArrays(uint32_t *data, int ptr1, int ptr2, int endptr1, int endptr2, double maxpdiff) {
	int i;

	int length1 = endptr1 - ptr1;
	int length2 = endptr2 - ptr2;

	if (length1 < 0)
		length1 += IR_DATA_ARRAY_SIZE;
	if (length2 < 0)
		length2 += IR_DATA_ARRAY_SIZE;

	if (length1 != length2)
		return PFALSE;

	for (i = 0; i < length1; i++) {
		if (pdiff(data[ptr1], data[ptr2]) > maxpdiff)
			return PFALSE;

		ptr1 = (ptr1 + 1)&IR_DATA_ARRAY_MASK;
		ptr2 = (ptr2 + 1)&IR_DATA_ARRAY_MASK;
	}
	return PTRUE;
}

//figure out the different times and their average values
//we'll need to use max error (30%?)
//we're going to keep track of average diff, and when we find a diff > 5xAvg, or > 20% switch to next section
//must have at least a 10% difference between lengths
//also if we find too many different times, error
static PhidgetReturnCode
get_times(int *timesin, int incount, int *timesout, int *timesoutcount, int *outcount) {
	int edge = 0, i = 0, j = 0, k = 0;
	double avgDiff = 0, diffSum = 0;
	for (i = 1; i < incount; i++) {
		double diff = pdiff(timesin[i], timesin[i - 1]);

		if (((avgDiff > 0) && (diff > (avgDiff * 10.0)) && diff > 0.1) || diff > 0.2) {
			//we've found a boundary
			//find average of all lower values
			double avg = 0;
			for (j = edge; j < i; j++)
				avg += timesin[j];
			avg /= (double)(i - edge);
			timesout[k] = round(avg);
			timesoutcount[k++] = (i - edge);

			if ((k + 1) > *outcount)
				return (EPHIDGET_INVALID);

			edge = i;
			diffSum = 0;
			avgDiff = 0;
		} else {
			diffSum += diff;
			avgDiff = diffSum / (i - edge);
		}
	}
	//get the last section...
	avgDiff = 0;
	for (j = edge; j < incount; j++)
		avgDiff += timesin[j];
	avgDiff /= (double)(incount - edge);
	timesoutcount[k] = (incount - edge);
	timesout[k++] = round(avgDiff);

	*outcount = k;
	return (EPHIDGET_OK);
}

//for now this just tries to get a data code
static PhidgetReturnCode
analyze_data(PhidgetIRDeviceHandle phid, int trailgap_needed) {
	int dataReader, dataWriter, highcount, lowcount, high_low, i, decodedcount = IR_MAX_CODE_DATA_LENGTH;
	int highs[IR_DATA_ARRAY_SIZE / 2], lows[IR_DATA_ARRAY_SIZE / 2], code_data[IR_DATA_ARRAY_SIZE];
	uint8_t decoded_data[IR_MAX_CODE_DATA_LENGTH] = { 0 };
	int highFinalscount = 20, lowFinalscount = 20;
	int highFinals[20], lowFinals[20];
	int highFinalsCounts[20], lowFinalsCounts[20];
	PhidgetIR_CodeInfo codeInfo = { 0 };
	PhidgetChannelHandle channel;

	//when read pointer != write pointer, there is new data to read
	//read pointer should point at first spot that's probably a gap
	while (phid->dataBuffer[phid->dataReadPtr] < IR_MIN_GAP_LENGTH) {
		//nothing to analyze yet
		if (phid->dataReadPtr == phid->dataWritePtr)
			return (EPHIDGET_OK);

		phid->dataReadPtr++;
		phid->dataReadPtr &= IR_DATA_ARRAY_MASK;
	}

	//nothing to analyze yet
	if (phid->dataReadPtr == phid->dataWritePtr)
		return (EPHIDGET_OK);

	dataReader = ((phid->dataReadPtr + 1) & IR_DATA_ARRAY_MASK);

	//nothing to analyze yet
	if (dataReader == (int)phid->dataWritePtr)
		return (EPHIDGET_OK);

	//gets the data up to the first probable gap
	highcount = 0;
	lowcount = 0;
	high_low = 1;
	while (dataReader != (int)phid->dataWritePtr) {
		//go till we find the next likely gap (long low pulse)
		if ((phid->dataBuffer[dataReader] >= IR_MIN_GAP_LENGTH) && (high_low == 0))
			goto analyze_step_two;

		//add data to high/low arrays
		//we start with a high pulse
		if (high_low == 1)
			highs[highcount++] = phid->dataBuffer[dataReader];
		else
			lows[lowcount++] = phid->dataBuffer[dataReader];

		high_low ^= 1;
		dataReader = ((dataReader + 1) & IR_DATA_ARRAY_MASK);
	}

	//hit the write pointer before finding a gap
	if (trailgap_needed)
		return (EPHIDGET_OK);

analyze_step_two:

	//nothing to analyze yet
	if (highcount == 0 && lowcount == 0)
		return (EPHIDGET_OK);

	phid->lastGap = phid->dataBuffer[phid->dataReadPtr];
	//should be one more high then low
	if ((highcount - 1) != lowcount) {
		loginfo("Unexpected number of high/low pulses between gaps");
		return (EPHIDGET_UNEXPECTED);
	}

	//sort the high/low arrays and extract their components
	qsort(highs, highcount, sizeof(int), compare_int);
	qsort(lows, lowcount, sizeof(int), compare_int);

	get_times(highs, highcount, highFinals, highFinalsCounts, &highFinalscount);
	get_times(lows, lowcount, lowFinals, lowFinalsCounts, &lowFinalscount);

	//go back through the data buffer and fills in dataBufferNormalized, doesn't include any gap data
	dataWriter = ((phid->dataReadPtr + 1) & IR_DATA_ARRAY_MASK);
	high_low = 1;
	while (dataWriter != dataReader) {
		//high time
		if (high_low) {
			int newtime = highFinals[0];
			double matchPercent = 1;
			for (i = 0; i < highFinalscount; i++) {
				//within 30% - we should never miss any...
				double diff = pdiff(phid->dataBuffer[dataWriter], highFinals[i]);
				if (diff <= 0.30 && diff < matchPercent) {
					newtime = highFinals[i];
					matchPercent = diff;
				}
			}
			phid->dataBufferNormalized[dataWriter] = newtime;
		}
		//low time
		else {
			int newtime = lowFinals[0];
			double matchPercent = 1;
			for (i = 0; i < lowFinalscount; i++) {
				//within 30% - we should never miss any...
				double diff = pdiff(phid->dataBuffer[dataWriter], lowFinals[i]);
				if (diff <= 0.30 && diff < matchPercent) {
					newtime = lowFinals[i];
					matchPercent = diff;
				}
			}
			phid->dataBufferNormalized[dataWriter] = newtime;
		}

		high_low ^= 1;
		dataWriter = ((dataWriter + 1) & IR_DATA_ARRAY_MASK);
	}

	//strip the header - if 1st pulse and/or the 1st space is unique, we assume first pulse/space is a header
	dataReader = (phid->dataReadPtr + 1) & IR_DATA_ARRAY_MASK;
	for (i = 0; i < highFinalscount; i++)
		if (phid->dataBufferNormalized[dataReader] == (uint32_t)highFinals[i] && highFinalsCounts[i] == 1) {
			dataReader = (phid->dataReadPtr + 3) & IR_DATA_ARRAY_MASK;
		}

	//put the code data into the code array
	i = 0;
	//include the trailing bit!
	//dataWriter = ((dataWriter - 1) & IR_DATA_ARRAY_MASK);
	while (dataWriter != dataReader) {
		code_data[i++] = phid->dataBufferNormalized[dataReader];
		dataReader = ((dataReader + 1) & IR_DATA_ARRAY_MASK);
	}

	//adjust the read pointer
	phid->dataReadPtr = dataReader;

	//try to decode
	if (!decode_data(code_data, i, decoded_data, &decodedcount, &codeInfo)) {
		//repeat logic
		int repeat = PFALSE;
		char codeStr[IR_MAX_CODE_STR_LENGTH];
		codeToStr(codeStr, IR_MAX_CODE_STR_LENGTH, decoded_data, codeInfo.bitCount);

		if (!strcmp(phid->lastCodeStr, codeStr)
			&& phid->lastGap <= IR_MAX_GAP_LENGTH && phid->lastGap >= IR_MIN_GAP_LENGTH
			&& phid->lastCodeInfo.bitCount == codeInfo.bitCount
			&& phid->lastCodeInfo.encoding == codeInfo.encoding)
			repeat = PTRUE;

		if ((channel = getChannel(phid, 0)) != NULL) {
			bridgeSendToChannel(channel, BP_CODE,
				"%s%d%d%d", codeStr, codeInfo.bitCount, repeat, codeInfo.encoding);
			PhidgetRelease(&channel);
		}

		//store to last code
		strcpy(phid->lastCodeStr, codeStr);
		phid->lastCodeInfo.bitCount = codeInfo.bitCount;
		phid->lastCodeInfo.encoding = codeInfo.encoding;
		phid->lastRepeat = repeat;
		phid->lastCodeKnown = PTRUE;
		//TODO: add header
	} //probably a repeat code (ie NEC code)
	else if ((i == 1 || i == 3) && phid->lastCodeKnown && phid->lastGap <= IR_MAX_GAP_LENGTH && phid->lastGap >= IR_MIN_GAP_LENGTH) {
		size_t dataSize = IR_DATASIZE(phid->lastCodeInfo.bitCount);
		if ((channel = getChannel(phid, 0)) != NULL) {
			bridgeSendToChannel(channel, BP_REPEAT, "%d", (int32_t)dataSize);
			PhidgetRelease(&channel);
		}
	}

	return (EPHIDGET_OK);
}

//this tries to lean a code
//we read staring at a long gap (PUNK_UINT32) until the next long gap, or time > 1 second
//need at least 1 code and 3 repeats to figure out the code completely
static PhidgetReturnCode
learn_data(PhidgetIRDeviceHandle phid) {
	int dataReader, dataWriter, highcount, lowcount, high_low, i, decodedcount = IR_MAX_CODE_DATA_LENGTH;
	int highs[IR_DATA_ARRAY_SIZE / 2], lows[IR_DATA_ARRAY_SIZE / 2], code_data[IR_DATA_ARRAY_SIZE];
	uint8_t decoded_data[IR_MAX_CODE_DATA_LENGTH];
	int highFinalscount = 20, lowFinalscount = 20;
	int highFinals[20], lowFinals[20];
	int highFinalsCounts[20], lowFinalsCounts[20];
	char codeStr[IR_MAX_CODE_STR_LENGTH];
	//PhidgetIR_Encoding encoding;
	PhidgetChannelHandle channel;
	PhidgetReturnCode res;
	BridgePacket *bp;

	PhidgetIR_CodeInfo codeInfo = { 0 };

	//keeps track of packets in the data stream
	int dataPackets[50];
	int dataPacketsCounter = 0;

	int timecounter = 0;

	//int oldLearPtr = phid->learnReadPtr;
	int readToPtr;

	//setup codeInfo with defaults
	codeInfo.carrierFrequency = 38000;
	codeInfo.encoding = IR_ENCODING_UNKNOWN;
	codeInfo.length = IR_LENGTH_UNKNOWN;
	//codeInfo.maxPdiff = 0.30;
	codeInfo.minRepeat = 1;
	codeInfo.dutyCycle = 0.5;

	//we have to have a BIG gap to start
	while (phid->dataBuffer[phid->learnReadPtr] < PUNK_UINT32) {
		//nothing to analyze yet
		if (phid->learnReadPtr == phid->dataWritePtr)
			return (EPHIDGET_OK);

		phid->learnReadPtr++;
		phid->learnReadPtr &= IR_DATA_ARRAY_MASK;
	}

	//nothing to analyze yet
	if (phid->learnReadPtr == phid->dataWritePtr)
		return (EPHIDGET_OK);

	dataReader = ((phid->learnReadPtr + 1) & IR_DATA_ARRAY_MASK);

	//nothing to analyze yet
	if (dataReader == (int)phid->dataWritePtr)
		return (EPHIDGET_OK);

	readToPtr = dataReader;
	//when read pointer != write pointer, there is new data to read
	//read pointer should point at first spot that's probably a gap
	while (phid->dataBuffer[readToPtr] != PUNK_UINT32) {
		//back up to last gap if we run into write pointer, or have > 1 sec. of data
		if (readToPtr == (int)phid->dataWritePtr || timecounter >= 2000000) {
			while (phid->dataBuffer[readToPtr] < IR_MIN_GAP_LENGTH) {
				//nothing to analyze yet
				if (readToPtr == dataReader)
					return (EPHIDGET_OK);

				readToPtr--;
				readToPtr &= IR_DATA_ARRAY_MASK;
			}
			goto analyze_step_one;
		}

		timecounter += phid->dataBuffer[readToPtr];

		readToPtr++;
		readToPtr &= IR_DATA_ARRAY_MASK;
	}

	//we should have at least enough data for one set plus its gap
analyze_step_one:

	//this grabs everything, including the gaps
	highcount = 0;
	lowcount = 0;
	high_low = 1;
	while (dataReader != readToPtr) {
		//add data to high/low arrays
		//we start with a high pulse
		if (high_low == 1)
			highs[highcount++] = phid->dataBuffer[dataReader];
		else
			lows[lowcount++] = phid->dataBuffer[dataReader];

		high_low ^= 1;
		dataReader = ((dataReader + 1) & IR_DATA_ARRAY_MASK);
	}

	//nothing to analyze yet
	if (highcount == 0 && lowcount == 0)
		goto advance_exit;

	//not enough data
	if ((highcount + lowcount) < 10)
		goto advance_exit;

	//sort the high/low arrays and extract their components
	qsort(highs, highcount, sizeof(int), compare_int);
	qsort(lows, lowcount, sizeof(int), compare_int);

	get_times(highs, highcount, highFinals, highFinalsCounts, &highFinalscount);
	get_times(lows, lowcount, lowFinals, lowFinalsCounts, &lowFinalscount);


	//go back through the data buffer and fill in dataBufferNormalized, doesn't include any gap data
	dataWriter = ((phid->learnReadPtr + 1) & IR_DATA_ARRAY_MASK);
	high_low = 1;
	dataPackets[dataPacketsCounter++] = dataWriter;
	while (dataWriter != dataReader) {
		//high time
		if (high_low) {
			int newtime = highFinals[0];
			double matchPercent = 1;
			for (i = 0; i < highFinalscount; i++) {
				//within 30% - we should never miss any...
				double diff = pdiff(phid->dataBuffer[dataWriter], highFinals[i]);
				if (diff <= 0.30 && diff < matchPercent) {
					newtime = highFinals[i];
					matchPercent = diff;
				}
			}
			phid->dataBufferNormalized[dataWriter] = newtime;
		}
		//low time
		else {
			int newtime = lowFinals[0];
			double matchPercent = 1;
			for (i = 0; i < lowFinalscount; i++) {
				//within 30% - we should never miss any...
				double diff = pdiff(phid->dataBuffer[dataWriter], lowFinals[i]);
				if (diff <= 0.30 && diff < matchPercent) {
					newtime = lowFinals[i];
					matchPercent = diff;
				}
			}
			phid->dataBufferNormalized[dataWriter] = newtime;

			if (newtime >= IR_MIN_GAP_LENGTH)
				dataPackets[dataPacketsCounter++] = ((dataWriter + 1) & IR_DATA_ARRAY_MASK);
		}

		high_low ^= 1;
		dataWriter = ((dataWriter + 1) & IR_DATA_ARRAY_MASK);
	}

	//at this point we have the data normalized, need to break off the first packet, figure out the gap, repeat, etc.
	//first make sure we have a data packet and at least 3 repeat packets.

	//not enough data
	if (dataPacketsCounter < 5)
		goto advance_exit;

	//strip the header - if 1st pulse and/or the 1st space is unique, we assume first pulse/space is a header
	//won't be unique because we have the repeats included now...
	dataReader = dataPackets[0];
	for (i = 0; i < highFinalscount; i++)
		if (phid->dataBufferNormalized[dataReader] == (uint32_t)highFinals[i] && (highFinalsCounts[i] == 1 || highFinalsCounts[i] == dataPacketsCounter)) {
			dataReader = (dataReader + 2) & IR_DATA_ARRAY_MASK;
			codeInfo.header[0] = phid->dataBufferNormalized[dataPackets[0]];
			codeInfo.header[1] = phid->dataBufferNormalized[(dataPackets[0] + 1) & IR_DATA_ARRAY_MASK];
		}

	//put the code data into the code array
	i = 0;
	//include the trailing bit!
	//dataWriter = ((dataWriter - 1) & IR_DATA_ARRAY_MASK);
	while (dataPackets[1] != dataReader) {
		code_data[i++] = phid->dataBufferNormalized[dataReader];
		dataReader = ((dataReader + 1) & IR_DATA_ARRAY_MASK);
	}

	//no gap for decoding
	i--;
	//try to decode
	if (decode_data(code_data, i, decoded_data, &decodedcount, &codeInfo)) {
		//couldn't find the encoding
		goto exit;
	}

	//get gap time
	{
		int time1 = 0, time2 = 0;
		codeInfo.length = IR_LENGTH_CONSTANT;
		dataTime(phid->dataBufferNormalized, &time1, dataPackets[0], dataPackets[1]);
		for (i = 1; i<(dataPacketsCounter - 1); i++) {
			dataTime(phid->dataBufferNormalized, &time2, dataPackets[i], dataPackets[i + 1]);
			if (pdiff(time1, time2) > 0.3)
				codeInfo.length = IR_LENGTH_VARIABLE;
		}
		if (codeInfo.length == IR_LENGTH_CONSTANT) {
			codeInfo.gap = time1;
		} else {
			int constgap = PTRUE;
			for (i = 1; i < (dataPacketsCounter - 1); i++) {
				if (phid->dataBufferNormalized[dataPackets[i] - 1] != phid->dataBufferNormalized[dataPackets[i + 1] - 1])
					constgap = PFALSE;
			}
			if (constgap)
				codeInfo.gap = phid->dataBufferNormalized[dataPackets[1] - 1];
			else {
				//not constant length and no constant gap - no sense!
				logdebug("Couldn't figure out the gap");
				goto exit;
			}
		}
	}

	//figure out repeat/toggle
	//first two don't match, so there may be some sort of repeat code or toggling
	if (compareDataArrays(phid->dataBufferNormalized, dataPackets[0], dataPackets[1], dataPackets[1], dataPackets[2], 0.3) != PTRUE) {
		int j;
		//packet 1 and 2 match - looks like a repeat code
		if (compareDataArrays(phid->dataBufferNormalized, dataPackets[1], dataPackets[2], dataPackets[2], dataPackets[3], 0.3) == PTRUE) {
			//See if the 'repeat code' is just the same code without the header (don't compare gaps)
			if (codeInfo.header[0] && compareDataArrays(
				phid->dataBufferNormalized, (dataPackets[0] + 2) & IR_DATA_ARRAY_MASK, dataPackets[1],
				(dataPackets[1] - 1) & IR_DATA_ARRAY_MASK, (dataPackets[2] - 1) & IR_DATA_ARRAY_MASK, 0.3)
				) {
				logdebug("No repeat code, just repeating without the header.");
				codeInfo.repeat[0] = 0;
			} else {
				for (i = dataPackets[1], j = 0; i != dataPackets[2]; i = (i + 1) & IR_DATA_ARRAY_MASK, j++) {
					//Make sure we don't go over the length of the repeat array!
					if (j >= IR_MAX_REPEAT_LENGTH) {
						logdebug("Couldn't figure out the repeat code");
						goto exit;
					}
					codeInfo.repeat[j] = phid->dataBufferNormalized[i];
				}
				//don't include the gap
				codeInfo.repeat[j - 1] = 0;
			}
		}
		//packets 0 and 2 match and 1 and 3 match - looks like a two packet data sequence, with some toggleing.
		else if (compareDataArrays(phid->dataBufferNormalized, dataPackets[0], dataPackets[2], dataPackets[1], dataPackets[3], 0.3) == PTRUE
			&& compareDataArrays(phid->dataBufferNormalized, dataPackets[1], dataPackets[3], dataPackets[2], dataPackets[4], 0.3) == PTRUE) {
			int decodedcount2 = IR_MAX_CODE_DATA_LENGTH;
			int code_data2[IR_DATA_ARRAY_SIZE];
			uint8_t decoded_data2[IR_MAX_CODE_DATA_LENGTH];
			PhidgetIR_CodeInfo codeInfo2 = { 0 };

			dataReader = dataPackets[1];
			//header
			if (codeInfo.header[0])
				dataReader = (dataReader + 2) & IR_DATA_ARRAY_MASK;
			//put the code data into the code array
			i = 0;
			//include the trailing bit!
			//dataWriter = ((dataWriter - 1) & IR_DATA_ARRAY_MASK);
			while (dataPackets[2] != dataReader) {
				code_data2[i++] = phid->dataBufferNormalized[dataReader];
				dataReader = ((dataReader + 1) & IR_DATA_ARRAY_MASK);
			}

			//no gap for decoding
			i--;
			//try to decode
			if (decode_data(code_data2, i, decoded_data2, &decodedcount2, &codeInfo2)) {
				goto exit;
			}

			if (codeInfo.encoding == codeInfo2.encoding &&
				codeInfo.bitCount == codeInfo2.bitCount &&
				decodedcount == decodedcount2) {
				uint8_t togglemask[IR_MAX_CODE_DATA_LENGTH];
				for (i = 0; i < decodedcount2; i++) {
					togglemask[i] = decoded_data[i] ^ decoded_data2[i];
				}
				codeToStr(codeInfo.toggleMask, sizeof(codeInfo.toggleMask), togglemask, codeInfo.bitCount);
				codeInfo.minRepeat = 2;
			} else {
				logdebug("Couldn't figure out the repeat code");
				goto exit;
			}
		} else {
			logdebug("Couldn't figure out the repeat code");
			goto exit;
		}
	}

	//send the event
	codeToStr(codeStr, sizeof(codeStr), decoded_data, codeInfo.bitCount);
	if ((channel = getChannel(phid, 0)) != NULL) {
		res = createBridgePacket(&bp, BP_LEARN, "%s", codeStr);
		if (res == EPHIDGET_OK) {
			res = writeCodeInfo(&codeInfo, bp);
			if (res == EPHIDGET_OK)
				bridgeSendBPToChannel(channel, bp); // destroys bridge packet
		}
		PhidgetRelease(&channel);
	}

	//store to last code
	strcpy(phid->lastLearnedCodeStr, codeStr);
	phid->lastLearnedCodeInfo = codeInfo;
	phid->lastLearnedCodeKnown = PTRUE;

exit:

	//adjust the learn read pointer
	phid->learnReadPtr = readToPtr;

	//Done
	return (EPHIDGET_OK);

advance_exit:
	//not enough data, and readToPtr == PUNK_UINT32, so we can't expect any more data, and increment learnPtr
	if (phid->dataBuffer[readToPtr] == PUNK_UINT32)
		phid->learnReadPtr = readToPtr;

	return (EPHIDGET_OK);
}

//Note that data is sent to board as 10's of us's
#define TIME_TO_PACKET(data, length, index, us) \
	do{ \
		if ((index + 2) >= length) \
			return (EPHIDGET_NOSPC); \
		if ((us) > 327670) \
			return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Pulse/Space/Gap times must be <= 327670us.")); \
		if ((us) > 1270) /*1270 == 0x7f << 1*/ \
			data[index++] = ((roundu((us)/10) >> 8) | 0x80); \
		data[index++] = roundu((us)/10) & 0xff; \
		*time += (us); \
	} while (0)

//returned data must start and end with pulses
static PhidgetReturnCode
RawTimeDataToRawData(mosiop_t iop, const int *rawTimeData, int rawTimeDataLength, int *rawData, int *rawDataLength, int *time) {
	int i = 0, j;

	for (j = 0; j < rawTimeDataLength; j++) {
		TIME_TO_PACKET(rawData, *rawDataLength, i, rawTimeData[j]);
	}

	*rawDataLength = i;

	return (EPHIDGET_OK);
}

//returned data must start and end with pulses
static PhidgetReturnCode
codeInfoToRawData(mosiop_t iop, uint8_t *code, PhidgetIR_CodeInfo codeInfo, int *rawData, int *rawDataLength,
	int *time, uint8_t repeat) {
	int i, j, k, lastbit;
	*time = 0;
	//special repeat code - simple!
	if (repeat && codeInfo.repeat[0]) {
		i = 0;
		j = 0;
		while (codeInfo.repeat[j]) {
			if (i >= *rawDataLength)
				return (EPHIDGET_NOSPC);

			TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.repeat[j]);
			j++;
		}
	}
	//actually do some work
	else {
		i = 0;
		if (codeInfo.header[0]) {
			TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.header[0]);
			TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.header[1]);
		}

		switch (codeInfo.encoding) {
		case IR_ENCODING_SPACE:
		case IR_ENCODING_PULSE:
			//k needs to be ie 1 for 15 bit codes, 0 for 16 bit codes, 7 for 9 bit codes...
			//because code[0] is MSB and may have less then 8 bit, where all other elements do have 8 bits
			for (j = (int)codeInfo.bitCount - 1, k = (8 - (codeInfo.bitCount % 8)) % 8; j >= 0; j--, k++) {
				if (code[k / 8] & (1 << (j % 8))) {
					TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[0]);
					TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1]);
				} else {
					TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[0]);
					TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1]);
				}
			}

			//add trail pulse for space encoding, delete last space for pulse encoding
			if (codeInfo.encoding == IR_ENCODING_SPACE) {
				TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.trail);
			} else {
				if (code[0] & 0x01) {
					if (codeInfo.one[1] > 1270)
						i--;
					i--;
					*time -= codeInfo.one[1];
				} else {
					if (codeInfo.zero[1] > 1270)
						i--;
					i--;
					*time -= codeInfo.zero[1];
				}
			}

			break;
		case IR_ENCODING_BIPHASE:
		case IR_ENCODING_RC5:
		case IR_ENCODING_RC6:
			//this uses space-pulse for 0 and pulse-space for 1, and has the long bit-6 (trailer bit)

			//1st bit is a special case
			j = (int)codeInfo.bitCount - 1;
			k = (8 - (codeInfo.bitCount % 8)) % 8;
			if (code[k / 8] & (1 << (j % 8))) {
				if (codeInfo.encoding == IR_ENCODING_RC6)
					TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[0]);
				lastbit = 1;
			} else {
				if (codeInfo.encoding != IR_ENCODING_RC6)
					TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[0]);
				lastbit = 0;
			}

			//now do the rest of the bits - each time trough,
			//we do the last hald of the last bit and the first half of this bit
			for (j--, k++; j >= 0; j--, k++) {
				int bit = (int)codeInfo.bitCount - j - 1;
				//1
				if (code[k / 8] & (1 << (j % 8))) {
					if (lastbit) {
						if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 5)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1] * 2);
						else
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1]);

						if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 4)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[0] * 2);
						else
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[0]);
					} else {
						if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 4)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1] + codeInfo.one[0] * 2);
						else if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 5)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1] * 2 + codeInfo.one[0]);
						else
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1] + codeInfo.one[0]);
					}

					lastbit = 1;
				}
				//0
				else {
					if (lastbit) {
						if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 4)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1] + codeInfo.zero[0] * 2);
						else if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 5)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1] * 2 + codeInfo.zero[0]);
						else
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1] + codeInfo.zero[0]);
					} else {
						if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 5)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1] * 2);
						else
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1]);

						if (codeInfo.encoding == IR_ENCODING_RC6 && bit == 4)
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[0] * 2);
						else
							TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[0]);
					}

					lastbit = 0;
				}
			}

			//finish up the data stream if we need to add a last pulse
			if (lastbit && codeInfo.encoding != IR_ENCODING_RC6)
				TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.one[1]);
			if (!lastbit && codeInfo.encoding == IR_ENCODING_RC6)
				TIME_TO_PACKET(rawData, *rawDataLength, i, codeInfo.zero[1]);

			break;
		//case IR_ENCODING_RAW:
		case IR_ENCODING_UNKNOWN:
		default:
			return (EPHIDGET_INVALIDARG);
		}
	}

	*rawDataLength = i;

	return (EPHIDGET_OK);
}

static void CCONV
PhidgetIRDevice_free(PhidgetDeviceHandle *phid) {

	mos_free(*phid, sizeof(struct _PhidgetIRDevice));
	*phid = NULL;
}

PhidgetReturnCode
PhidgetIRDevice_create(PhidgetIRDeviceHandle *phidp) {
	DEVICECREATE_BODY(IRDevice, PHIDCLASS_IR);
	return (EPHIDGET_OK);
}

static PhidgetReturnCode
_Transmit(mosiop_t iop, PhidgetIRDeviceHandle phid, const char *codeStr, PhidgetIR_CodeInfoHandle codeInfo) {
	uint8_t datatemp[IR_MAX_CODE_DATA_LENGTH];
	int dataBuffer[IR_DATA_ARRAY_SIZE];
	int dataBufferLength;
	size_t dataSize;
	int repcount;
	int time, i;
	PhidgetReturnCode retval;
	uint8_t togglemask[IR_MAX_CODE_DATA_LENGTH] = { 0 };

	assert(phid);
	assert(codeInfo);
	assert(codeStr);
	assert(phid->phid.deviceInfo.class == PHIDCLASS_IR);
	TESTATTACHED(phid);

	if (!codeInfo->bitCount || codeInfo->bitCount > IR_MAX_CODE_BIT_COUNT)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Bit count must be between 1 and %d.", IR_MAX_CODE_BIT_COUNT));

	//TODO: we're choosing arbitrary 10kHz - 1MHz range here
	if ((codeInfo->carrierFrequency && codeInfo->carrierFrequency < 10000) || codeInfo->carrierFrequency > 1000000)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Carrier Frequency must be between 10000 and 1000000."));

	//duty cycle between 10% and 50% - never allow higher then 50%
	if ((codeInfo->dutyCycle && codeInfo->dutyCycle < 0.1) || codeInfo->dutyCycle > 0.5)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Duty Cycle must be between 0.1 and 0.5"));

	//default encoding
	if (!codeInfo->encoding)
		codeInfo->encoding = IR_ENCODING_SPACE;
	if (!codeInfo->length)
		codeInfo->length = IR_LENGTH_CONSTANT;

	//fill in other defaults based on encoding
	switch (codeInfo->encoding) {
	case IR_ENCODING_SPACE:
		//we'll default to NEC coding
		if (!codeInfo->zero[0] && !codeInfo->one[0]) {
			codeInfo->zero[0] = 560;
			codeInfo->zero[1] = 560;
			codeInfo->one[0] = 560;
			codeInfo->one[1] = 1680;
			if (!codeInfo->header[0]) {
				codeInfo->header[0] = 9000;
				codeInfo->header[1] = 4500;
			}
			if (!codeInfo->repeat[0]) {
				codeInfo->repeat[0] = 9000;
				codeInfo->repeat[1] = 2250;
				codeInfo->repeat[2] = 560;
			}
			if (!codeInfo->trail)
				codeInfo->trail = 560;
			if (!codeInfo->gap)
				codeInfo->gap = 110000;
			if (!codeInfo->carrierFrequency)
				codeInfo->carrierFrequency = 38000;
		}
		if (!codeInfo->trail)
			return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Trail time must be specified."));
		break;
	case IR_ENCODING_PULSE:
		//default to SONY coding
		if (!codeInfo->zero[0] && !codeInfo->one[0]) {
			codeInfo->zero[0] = 600;
			codeInfo->zero[1] = 600;
			codeInfo->one[0] = 1200;
			codeInfo->one[1] = 600;
			if (!codeInfo->header[0]) {
				codeInfo->header[0] = 2400;
				codeInfo->header[1] = 600;
			}
			if (!codeInfo->gap)
				codeInfo->gap = 45000;
			if (!codeInfo->carrierFrequency)
				codeInfo->carrierFrequency = 40000;
		}
		break;
	case IR_ENCODING_BIPHASE:
		//no default here..
		break;
	case IR_ENCODING_RC5:
		if (!codeInfo->zero[0] && !codeInfo->one[0]) {
			codeInfo->zero[0] = 889;
			codeInfo->zero[1] = 889;
			codeInfo->one[0] = 889;
			codeInfo->one[1] = 889;
			if (!codeInfo->gap)
				codeInfo->gap = 114000;
			if (!codeInfo->carrierFrequency)
				codeInfo->carrierFrequency = 36000;
		}
		break;
	case IR_ENCODING_RC6:
		if (!codeInfo->zero[0] && !codeInfo->one[0]) {
			codeInfo->zero[0] = 444;
			codeInfo->zero[1] = 444;
			codeInfo->one[0] = 444;
			codeInfo->one[1] = 444;
			if (!codeInfo->header[0]) {
				codeInfo->header[0] = 2666;
				codeInfo->header[1] = 889;
			}
			if (!codeInfo->gap)
				codeInfo->gap = 105000;
			if (!codeInfo->carrierFrequency)
				codeInfo->carrierFrequency = 36000;
		}
		break;
	case IR_ENCODING_UNKNOWN:
	default:
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Invalid encoding."));
	}

	//fill in defaults for things that the user didn't specify
	if (!codeInfo->carrierFrequency)
		codeInfo->carrierFrequency = 38000;
	if (!codeInfo->dutyCycle)
		codeInfo->dutyCycle = 0.33;
	if (!codeInfo->minRepeat)
		codeInfo->minRepeat = 1;

	//make sure that other things are filled in
	if (!codeInfo->zero[0])
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Zero times must be specified."));
	if (!codeInfo->one[0])
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "One times must be specified."));
	if (!codeInfo->gap)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Gap time must be specified."));

	if (strlen(codeInfo->toggleMask) != 0)
		if (strlen(codeInfo->toggleMask) != strlen(codeStr))
			return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Toggle mask length must match code length"));

	dataBufferLength = IR_DATA_ARRAY_SIZE * sizeof(int);
	repcount = codeInfo->minRepeat;
	dataSize = IR_DATASIZE(codeInfo->bitCount);

	codeStrToByteArray(datatemp, sizeof(datatemp), codeStr);
	codeStrToByteArray(togglemask, sizeof(togglemask), codeInfo->toggleMask);

	//send out the number of times required
	while (repcount--) {
		//convert code into raw data array and send
		dataBufferLength = IR_DATA_ARRAY_SIZE * sizeof(int);
		retval = codeInfoToRawData(iop, datatemp, *codeInfo, dataBuffer, &dataBufferLength, &time, PFALSE);
		if (retval != EPHIDGET_OK)
			return retval;

		if (codeInfo->length == IR_LENGTH_CONSTANT)
			time = codeInfo->gap - time;
		else
			time = codeInfo->gap;

		retval = sendRAWData(iop, phid, dataBuffer, dataBufferLength, codeInfo->carrierFrequency,
			codeInfo->dutyCycle, time);
		if (retval != EPHIDGET_OK)
			return retval;

		memcpy(phid->lastSentCode, datatemp, dataSize);

		//toggle data
		for (i = 0; i < (int)dataSize; i++)
			datatemp[i] = datatemp[i] ^ togglemask[i];
	}

	//got here? success in sending
	phid->lastSentCodeInfo = *codeInfo;

	return (EPHIDGET_OK);
}

static PhidgetReturnCode
_TransmitRaw(mosiop_t iop, PhidgetIRDeviceHandle phid, const int *data, int dataLength, int carrierFrequency,
	double dutyCycle, int gap) {
	PhidgetReturnCode retval;
	int time = 0;
	int rawData[IR_DATA_ARRAY_SIZE];
	int rawDataLength = IR_DATA_ARRAY_SIZE;
	assert(phid);
	assert(phid->phid.deviceInfo.class == PHIDCLASS_IR);
	TESTATTACHED(phid);

	//needs to be uneven - start and end in a pulse
	if ((dataLength % 2) != 1)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Data length must be uneven."));

	//TODO: we're choosing arbitrary 10kHz - 1MHz range here
	if ((carrierFrequency && carrierFrequency < 10000) || carrierFrequency > 1000000)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Carrier Frequency must be between 10000 and 1000000."));

	//duty cycle between 10% and 50% - never allow higher then 50%
	if ((dutyCycle && dutyCycle < 0.10) || dutyCycle > 0.50)
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Duty Cycle must be between 0.1 and 0.5"));

	//defaults
	if (!carrierFrequency)
		carrierFrequency = 38000;
	if (!dutyCycle)
		dutyCycle = 0.33;

	if (dataLength > (1000 / 5))
		return (MOS_ERROR(iop, EPHIDGET_INVALIDARG, "Data length must be less then 200"));

	if ((retval = RawTimeDataToRawData(iop, data, dataLength, rawData, &rawDataLength, &time)) != EPHIDGET_OK)
		return retval;
	if ((retval = sendRAWData(iop, phid, rawData, rawDataLength, carrierFrequency ? carrierFrequency : 38000, dutyCycle ? dutyCycle : 0.33, gap)) != EPHIDGET_OK)
		return retval;

	return (EPHIDGET_OK);
}

static PhidgetReturnCode
_TransmitRepeat(mosiop_t iop, PhidgetIRDeviceHandle phid) {
	uint8_t datatemp[IR_MAX_CODE_DATA_LENGTH];
	int dataBuffer[IR_DATA_ARRAY_SIZE];
	int dataBufferLength;
	size_t dataSize;
	PhidgetReturnCode retval;
	int time, i;
	uint8_t togglemask[IR_MAX_CODE_DATA_LENGTH] = { 0 };

	assert(phid);
	assert(phid->phid.deviceInfo.class == PHIDCLASS_IR);
	TESTATTACHED(phid);

	dataBufferLength = IR_DATA_ARRAY_SIZE * sizeof(int);

	dataSize = IR_DATASIZE(phid->lastSentCodeInfo.bitCount);
	//assume that last code is valid
	if (dataSize > 0) {
		//get last data sent
		memcpy(datatemp, phid->lastSentCode, dataSize);

		//toggle data
		codeStrToByteArray(togglemask, sizeof(togglemask), phid->lastSentCodeInfo.toggleMask);
		for (i = 0; i < (int)dataSize; i++)
			datatemp[i] = datatemp[i] ^ togglemask[i];

		//construct the last code into a repeat code
		dataBufferLength = IR_DATA_ARRAY_SIZE * sizeof(int);
		retval = codeInfoToRawData(iop, datatemp, phid->lastSentCodeInfo, dataBuffer, &dataBufferLength,
			&time, PTRUE);
		if (retval != EPHIDGET_OK)
			return retval;

		if (phid->lastSentCodeInfo.length == IR_LENGTH_CONSTANT)
			time = phid->lastSentCodeInfo.gap - time;
		else
			time = phid->lastSentCodeInfo.gap;

		//send the data
		retval = sendRAWData(iop, phid, dataBuffer, dataBufferLength, phid->lastSentCodeInfo.carrierFrequency,
			phid->lastSentCodeInfo.dutyCycle, time);
		if (retval != EPHIDGET_OK)
			return retval;

		//store last sent code
		memcpy(phid->lastSentCode, datatemp, dataSize);
	} else {
		logwarn("Can't send a repeat until a code has been transmitted");
		return (MOS_ERROR(iop, EPHIDGET_UNKNOWNVAL, "Can't send a repeat until a code has been transmitted"));
	}

	return (EPHIDGET_OK);
}

