/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

#include "device/spatialdevice.h"
static void CCONV PhidgetMagnetometer_errorHandler(PhidgetChannelHandle ch,
  Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetMagnetometer_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetMagnetometer_setStatus(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetMagnetometer_getStatus(PhidgetChannelHandle phid,
  BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetMagnetometer_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetMagnetometer_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetMagnetometer_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetMagnetometer {
	struct _PhidgetChannel phid;
	int axisCount;
	uint32_t dataInterval;
	uint32_t minDataInterval;
	uint32_t maxDataInterval;
	double magneticField[3];
	double minMagneticField[3];
	double maxMagneticField[3];
	double magneticFieldChangeTrigger;
	double minMagneticFieldChangeTrigger;
	double maxMagneticFieldChangeTrigger;
	double timestamp;
	PhidgetMagnetometer_OnMagneticFieldChangeCallback MagneticFieldChange;
	void *MagneticFieldChangeCtx;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetMagnetometerHandle ch;
	int version;

	ch = (PhidgetMagnetometerHandle)phid;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 1) {
		logerr("%"PRIphid": bad version %d != 1", phid, version);
		return (EPHIDGET_BADVERSION);
	}

	ch->axisCount = getBridgePacketInt32ByName(bp, "axisCount");
	ch->dataInterval = getBridgePacketUInt32ByName(bp, "dataInterval");
	ch->minDataInterval = getBridgePacketUInt32ByName(bp, "minDataInterval");
	ch->maxDataInterval = getBridgePacketUInt32ByName(bp, "maxDataInterval");
	memcpy(&ch->magneticField, getBridgePacketDoubleArrayByName(bp, "magneticField"),
	  sizeof (double) * 3);
	memcpy(&ch->minMagneticField, getBridgePacketDoubleArrayByName(bp, "minMagneticField"),
	  sizeof (double) * 3);
	memcpy(&ch->maxMagneticField, getBridgePacketDoubleArrayByName(bp, "maxMagneticField"),
	  sizeof (double) * 3);
	ch->magneticFieldChangeTrigger = getBridgePacketDoubleByName(bp, "magneticFieldChangeTrigger");
	ch->minMagneticFieldChangeTrigger = getBridgePacketDoubleByName(bp,
	  "minMagneticFieldChangeTrigger");
	ch->maxMagneticFieldChangeTrigger = getBridgePacketDoubleByName(bp,
	  "maxMagneticFieldChangeTrigger");
	ch->timestamp = getBridgePacketDoubleByName(bp, "timestamp");

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	PhidgetMagnetometerHandle ch;

	ch = (PhidgetMagnetometerHandle)phid;

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ",axisCount=%d"
	  ",dataInterval=%u"
	  ",minDataInterval=%u"
	  ",maxDataInterval=%u"
	  ",magneticField=%3G"
	  ",minMagneticField=%3G"
	  ",maxMagneticField=%3G"
	  ",magneticFieldChangeTrigger=%g"
	  ",minMagneticFieldChangeTrigger=%g"
	  ",maxMagneticFieldChangeTrigger=%g"
	  ",timestamp=%g"
	  ,1 /* class version */
	  ,ch->axisCount
	  ,ch->dataInterval
	  ,ch->minDataInterval
	  ,ch->maxDataInterval
	  ,ch->magneticField
	  ,ch->minMagneticField
	  ,ch->maxMagneticField
	  ,ch->magneticFieldChangeTrigger
	  ,ch->minMagneticFieldChangeTrigger
	  ,ch->maxMagneticFieldChangeTrigger
	  ,ch->timestamp
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetMagnetometerHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetMagnetometerHandle)phid;
	res = EPHIDGET_OK;

	switch (bp->vpkt) {
	case BP_SETCORRECTIONPARAMETERS:
		res = DEVBRIDGEINPUT(phid, bp);
		break;
	case BP_RESETCORRECTIONPARAMETERS:
		res = DEVBRIDGEINPUT(phid, bp);
		break;
	case BP_SAVECORRECTIONPARAMETERS:
		res = DEVBRIDGEINPUT(phid, bp);
		break;
	case BP_SETDATAINTERVAL:
		TESTRANGE_IOP(bp->iop, "%"PRIu32, getBridgePacketUInt32(bp, 0), ch->minDataInterval,
		  ch->maxDataInterval);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->dataInterval = getBridgePacketUInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "DataInterval");
		break;
	case BP_SETCHANGETRIGGER:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minMagneticFieldChangeTrigger,
		  ch->maxMagneticFieldChangeTrigger);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->magneticFieldChangeTrigger = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "MagneticFieldChangeTrigger");
		break;
	case BP_FIELDSTRENGTHCHANGE:
		memcpy(&ch->magneticField, getBridgePacketDoubleArray(bp, 0), sizeof (double) * 3);
		ch->timestamp = getBridgePacketDouble(bp, 1);
		FIRECH(ch, MagneticFieldChange, ch->magneticField, ch->timestamp);
		break;
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetSpatialDeviceHandle parentSpatial;
	PhidgetMagnetometerHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);
	ch = (PhidgetMagnetometerHandle)phid;

	ret = EPHIDGET_OK;

	parentSpatial = (PhidgetSpatialDeviceHandle)phid->parent;

	switch (phid->UCD->uid) {
	case PHIDCHUID_1042_MAGNETOMETER_300:
		ch->dataInterval = 256;
		ch->maxDataInterval = 1000;
		ch->maxMagneticField[0] = 5.6;
		ch->maxMagneticField[1] = 5.6;
		ch->maxMagneticField[2] = 5.6;
		ch->maxMagneticFieldChangeTrigger = 10;
		ch->minDataInterval = 4;
		ch->minMagneticField[0] = -5.6;
		ch->minMagneticField[1] = -5.6;
		ch->minMagneticField[2] = -5.6;
		ch->minMagneticFieldChangeTrigger = 0;
		ch->magneticField[0] = parentSpatial->magneticField[ch->phid.index][0];
		ch->magneticField[1] = parentSpatial->magneticField[ch->phid.index][1];
		ch->magneticField[2] = parentSpatial->magneticField[ch->phid.index][2];
		ch->magneticFieldChangeTrigger = 0;
		ch->axisCount = 3;
		ch->timestamp = parentSpatial->timestamp[ch->phid.index];
		break;
	case PHIDCHUID_1044_MAGNETOMETER_400:
		ch->dataInterval = 256;
		ch->maxDataInterval = 1000;
		ch->maxMagneticField[0] = 5.6;
		ch->maxMagneticField[1] = 5.6;
		ch->maxMagneticField[2] = 5.6;
		ch->maxMagneticFieldChangeTrigger = 10;
		ch->minDataInterval = 4;
		ch->minMagneticField[0] = -5.6;
		ch->minMagneticField[1] = -5.6;
		ch->minMagneticField[2] = -5.6;
		ch->minMagneticFieldChangeTrigger = 0;
		ch->magneticField[0] = parentSpatial->magneticField[ch->phid.index][0];
		ch->magneticField[1] = parentSpatial->magneticField[ch->phid.index][1];
		ch->magneticField[2] = parentSpatial->magneticField[ch->phid.index][2];
		ch->magneticFieldChangeTrigger = 0;
		ch->axisCount = 3;
		ch->timestamp = parentSpatial->timestamp[ch->phid.index];
		break;
	case PHIDCHUID_1044_MAGNETOMETER_500:
		ch->dataInterval = 256;
		ch->maxDataInterval = 1000;
		ch->maxMagneticField[0] = 50;
		ch->maxMagneticField[1] = 50;
		ch->maxMagneticField[2] = 50;
		ch->maxMagneticFieldChangeTrigger = 100;
		ch->minDataInterval = 4;
		ch->minMagneticField[0] = -50;
		ch->minMagneticField[1] = -50;
		ch->minMagneticField[2] = -50;
		ch->minMagneticFieldChangeTrigger = 0;
		ch->magneticField[0] = parentSpatial->magneticField[ch->phid.index][0];
		ch->magneticField[1] = parentSpatial->magneticField[ch->phid.index][1];
		ch->magneticField[2] = parentSpatial->magneticField[ch->phid.index][2];
		ch->magneticFieldChangeTrigger = 0;
		ch->axisCount = 3;
		ch->timestamp = parentSpatial->timestamp[ch->phid.index];
		break;
	case PHIDCHUID_1056_MAGNETOMETER_000:
		ch->dataInterval = 256;
		ch->maxDataInterval = 1000;
		ch->maxMagneticField[0] = 4;
		ch->maxMagneticField[1] = 4;
		ch->maxMagneticField[2] = 4;
		ch->maxMagneticFieldChangeTrigger = 10;
		ch->minDataInterval = 4;
		ch->minMagneticField[0] = -4;
		ch->minMagneticField[1] = -4;
		ch->minMagneticField[2] = -4;
		ch->minMagneticFieldChangeTrigger = 0;
		ch->magneticField[0] = parentSpatial->magneticField[ch->phid.index][0];
		ch->magneticField[1] = parentSpatial->magneticField[ch->phid.index][1];
		ch->magneticField[2] = parentSpatial->magneticField[ch->phid.index][2];
		ch->magneticFieldChangeTrigger = 0;
		ch->axisCount = 3;
		ch->timestamp = parentSpatial->timestamp[ch->phid.index];
		break;
	case PHIDCHUID_1056_MAGNETOMETER_200:
		ch->dataInterval = 256;
		ch->maxDataInterval = 1000;
		ch->maxMagneticField[0] = 4;
		ch->maxMagneticField[1] = 4;
		ch->maxMagneticField[2] = 4;
		ch->maxMagneticFieldChangeTrigger = 10;
		ch->minDataInterval = 4;
		ch->minMagneticField[0] = -4;
		ch->minMagneticField[1] = -4;
		ch->minMagneticField[2] = -4;
		ch->minMagneticFieldChangeTrigger = 0;
		ch->magneticField[0] = parentSpatial->magneticField[ch->phid.index][0];
		ch->magneticField[1] = parentSpatial->magneticField[ch->phid.index][1];
		ch->magneticField[2] = parentSpatial->magneticField[ch->phid.index][2];
		ch->magneticFieldChangeTrigger = 0;
		ch->axisCount = 3;
		ch->timestamp = parentSpatial->timestamp[ch->phid.index];
		break;
	case PHIDCHUID_MOT1101_MAGNETOMETER_100:
		ch->dataInterval = 250;
		ch->maxDataInterval = 60000;
		ch->maxMagneticField[0] = 8;
		ch->maxMagneticField[1] = 8;
		ch->maxMagneticField[2] = 8;
		ch->maxMagneticFieldChangeTrigger = 8;
		ch->minDataInterval = 20;
		ch->minMagneticField[0] = -8;
		ch->minMagneticField[1] = -8;
		ch->minMagneticField[2] = -8;
		ch->minMagneticFieldChangeTrigger = 0;
		ch->magneticField[0] = PUNK_DBL;
		ch->magneticField[1] = PUNK_DBL;
		ch->magneticField[2] = PUNK_DBL;
		ch->magneticFieldChangeTrigger = 0;
		ch->axisCount = 3;
		ch->timestamp = PUNK_DBL;
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}


	return (ret);
}

static PhidgetReturnCode CCONV
_setDefaults(PhidgetChannelHandle phid) {
	PhidgetMagnetometerHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ch = (PhidgetMagnetometerHandle)phid;
	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_1042_MAGNETOMETER_300:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
		  ch->magneticFieldChangeTrigger);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_1044_MAGNETOMETER_400:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
		  ch->magneticFieldChangeTrigger);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_1044_MAGNETOMETER_500:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
		  ch->magneticFieldChangeTrigger);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_1056_MAGNETOMETER_000:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
		  ch->magneticFieldChangeTrigger);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_1056_MAGNETOMETER_200:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
		  ch->magneticFieldChangeTrigger);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_MOT1101_MAGNETOMETER_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
		  ch->magneticFieldChangeTrigger);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}

	return (ret);
}

static void CCONV
_fireInitialEvents(PhidgetChannelHandle phid) {
	PhidgetMagnetometerHandle ch;

	ch = (PhidgetMagnetometerHandle)phid;

	if(ch->magneticField[0] != PUNK_DBL &&
	  ch->magneticField[1] != PUNK_DBL &&
	  ch->magneticField[2] != PUNK_DBL &&
	  ch->timestamp != PUNK_DBL)
		FIRECH(ch, MagneticFieldChange, ch->magneticField, ch->timestamp);

}

static void CCONV
PhidgetMagnetometer_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetMagnetometer));
}

API_PRETURN
PhidgetMagnetometer_create(PhidgetMagnetometerHandle *phidp) {

	CHANNELCREATE_BODY(Magnetometer, PHIDCHCLASS_MAGNETOMETER);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_delete(PhidgetMagnetometerHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}

API_PRETURN
PhidgetMagnetometer_setCorrectionParameters(PhidgetMagnetometerHandle ch, double magneticField,
  double offset0, double offset1, double offset2, double gain0, double gain1, double gain2, double T0, double T1, double T2, double T3, double T4, double T5) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	return bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETCORRECTIONPARAMETERS, NULL, NULL,
	  "%g%g%g%g%g%g%g%g%g%g%g%g%g", magneticField, offset0, offset1, offset2, gain0, gain1, gain2, T0, T1, T2, T3, T4, T5);
}

API_PRETURN
PhidgetMagnetometer_resetCorrectionParameters(PhidgetMagnetometerHandle ch) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	return bridgeSendToDevice((PhidgetChannelHandle)ch, BP_RESETCORRECTIONPARAMETERS, NULL, NULL,
	  NULL);
}

API_PRETURN
PhidgetMagnetometer_saveCorrectionParameters(PhidgetMagnetometerHandle ch) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	return bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SAVECORRECTIONPARAMETERS, NULL, NULL, NULL);
}

API_PRETURN
PhidgetMagnetometer_getAxisCount(PhidgetMagnetometerHandle ch, int *axisCount) {

	TESTPTR_PR(ch);
	TESTPTR_PR(axisCount);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*axisCount = ch->axisCount;
	if (ch->axisCount == (int)PUNK_INT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_setDataInterval(PhidgetMagnetometerHandle ch, uint32_t dataInterval) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETDATAINTERVAL, NULL, NULL, "%u",
	  dataInterval));
}

API_PRETURN
PhidgetMagnetometer_getDataInterval(PhidgetMagnetometerHandle ch, uint32_t *dataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*dataInterval = ch->dataInterval;
	if (ch->dataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMinDataInterval(PhidgetMagnetometerHandle ch, uint32_t *minDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*minDataInterval = ch->minDataInterval;
	if (ch->minDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMaxDataInterval(PhidgetMagnetometerHandle ch, uint32_t *maxDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*maxDataInterval = ch->maxDataInterval;
	if (ch->maxDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMagneticField(PhidgetMagnetometerHandle ch, double (*magneticField)[3]) {

	TESTPTR_PR(ch);
	TESTPTR_PR(magneticField);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	(*magneticField)[0] = ch->magneticField[0];
	if (ch->magneticField[0] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*magneticField)[1] = ch->magneticField[1];
	if (ch->magneticField[1] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*magneticField)[2] = ch->magneticField[2];
	if (ch->magneticField[2] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMinMagneticField(PhidgetMagnetometerHandle ch, double (*minMagneticField)[3]) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minMagneticField);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	(*minMagneticField)[0] = ch->minMagneticField[0];
	if (ch->minMagneticField[0] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*minMagneticField)[1] = ch->minMagneticField[1];
	if (ch->minMagneticField[1] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*minMagneticField)[2] = ch->minMagneticField[2];
	if (ch->minMagneticField[2] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMaxMagneticField(PhidgetMagnetometerHandle ch, double (*maxMagneticField)[3]) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxMagneticField);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	(*maxMagneticField)[0] = ch->maxMagneticField[0];
	if (ch->maxMagneticField[0] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*maxMagneticField)[1] = ch->maxMagneticField[1];
	if (ch->maxMagneticField[1] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*maxMagneticField)[2] = ch->maxMagneticField[2];
	if (ch->maxMagneticField[2] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_setMagneticFieldChangeTrigger(PhidgetMagnetometerHandle ch,
  double magneticFieldChangeTrigger) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
	  magneticFieldChangeTrigger));
}

API_PRETURN
PhidgetMagnetometer_getMagneticFieldChangeTrigger(PhidgetMagnetometerHandle ch,
  double *magneticFieldChangeTrigger) {

	TESTPTR_PR(ch);
	TESTPTR_PR(magneticFieldChangeTrigger);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*magneticFieldChangeTrigger = ch->magneticFieldChangeTrigger;
	if (ch->magneticFieldChangeTrigger == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMinMagneticFieldChangeTrigger(PhidgetMagnetometerHandle ch,
  double *minMagneticFieldChangeTrigger) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minMagneticFieldChangeTrigger);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*minMagneticFieldChangeTrigger = ch->minMagneticFieldChangeTrigger;
	if (ch->minMagneticFieldChangeTrigger == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getMaxMagneticFieldChangeTrigger(PhidgetMagnetometerHandle ch,
  double *maxMagneticFieldChangeTrigger) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxMagneticFieldChangeTrigger);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*maxMagneticFieldChangeTrigger = ch->maxMagneticFieldChangeTrigger;
	if (ch->maxMagneticFieldChangeTrigger == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_getTimestamp(PhidgetMagnetometerHandle ch, double *timestamp) {

	TESTPTR_PR(ch);
	TESTPTR_PR(timestamp);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);
	TESTATTACHED_PR(ch);

	*timestamp = ch->timestamp;
	if (ch->timestamp == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMagnetometer_setOnMagneticFieldChangeHandler(PhidgetMagnetometerHandle ch,
  PhidgetMagnetometer_OnMagneticFieldChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MAGNETOMETER);

	ch->MagneticFieldChange = fptr;
	ch->MagneticFieldChangeCtx = ctx;

	return (EPHIDGET_OK);
}
