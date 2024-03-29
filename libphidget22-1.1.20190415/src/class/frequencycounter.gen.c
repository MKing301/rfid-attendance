/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

#include "device/frequencycounterdevice.h"
static void CCONV PhidgetFrequencyCounter_errorHandler(PhidgetChannelHandle ch,
  Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetFrequencyCounter_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetFrequencyCounter_setStatus(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetFrequencyCounter_getStatus(PhidgetChannelHandle phid,
  BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetFrequencyCounter_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetFrequencyCounter_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetFrequencyCounter_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetFrequencyCounter {
	struct _PhidgetChannel phid;
	double totalTicksSinceLastCount;
	uint32_t frequencyPrecision;
	uint64_t count;
	int enabled;
	uint32_t dataInterval;
	uint32_t minDataInterval;
	uint32_t maxDataInterval;
	PhidgetFrequencyCounter_FilterType filterType;
	double frequency;
	double maxFrequency;
	double frequencyCutoff;
	double minFrequencyCutoff;
	double maxFrequencyCutoff;
	Phidget_InputMode inputMode;
	Phidget_PowerSupply powerSupply;
	double timeElapsed;
	PhidgetFrequencyCounter_OnCountChangeCallback CountChange;
	void *CountChangeCtx;
	PhidgetFrequencyCounter_OnFrequencyChangeCallback FrequencyChange;
	void *FrequencyChangeCtx;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetFrequencyCounterHandle ch;
	int version;

	ch = (PhidgetFrequencyCounterHandle)phid;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 2) {
		logerr("%"PRIphid": bad version %d != 2", phid, version);
		return (EPHIDGET_BADVERSION);
	}

	ch->totalTicksSinceLastCount = getBridgePacketDoubleByName(bp, "totalTicksSinceLastCount");
	ch->frequencyPrecision = getBridgePacketUInt32ByName(bp, "frequencyPrecision");
	ch->count = getBridgePacketUInt64ByName(bp, "count");
	ch->enabled = getBridgePacketInt32ByName(bp, "enabled");
	ch->dataInterval = getBridgePacketUInt32ByName(bp, "dataInterval");
	ch->minDataInterval = getBridgePacketUInt32ByName(bp, "minDataInterval");
	ch->maxDataInterval = getBridgePacketUInt32ByName(bp, "maxDataInterval");
	ch->filterType = getBridgePacketInt32ByName(bp, "filterType");
	ch->frequency = getBridgePacketDoubleByName(bp, "frequency");
	ch->maxFrequency = getBridgePacketDoubleByName(bp, "maxFrequency");
	ch->frequencyCutoff = getBridgePacketDoubleByName(bp, "frequencyCutoff");
	ch->minFrequencyCutoff = getBridgePacketDoubleByName(bp, "minFrequencyCutoff");
	ch->maxFrequencyCutoff = getBridgePacketDoubleByName(bp, "maxFrequencyCutoff");
	ch->inputMode = getBridgePacketInt32ByName(bp, "inputMode");
	ch->powerSupply = getBridgePacketInt32ByName(bp, "powerSupply");
	ch->timeElapsed = getBridgePacketDoubleByName(bp, "timeElapsed");

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	PhidgetFrequencyCounterHandle ch;

	ch = (PhidgetFrequencyCounterHandle)phid;

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ",totalTicksSinceLastCount=%g"
	  ",frequencyPrecision=%u"
	  ",count=%ul"
	  ",enabled=%d"
	  ",dataInterval=%u"
	  ",minDataInterval=%u"
	  ",maxDataInterval=%u"
	  ",filterType=%d"
	  ",frequency=%g"
	  ",maxFrequency=%g"
	  ",frequencyCutoff=%g"
	  ",minFrequencyCutoff=%g"
	  ",maxFrequencyCutoff=%g"
	  ",inputMode=%d"
	  ",powerSupply=%d"
	  ",timeElapsed=%g"
	  ,2 /* class version */
	  ,ch->totalTicksSinceLastCount
	  ,ch->frequencyPrecision
	  ,ch->count
	  ,ch->enabled
	  ,ch->dataInterval
	  ,ch->minDataInterval
	  ,ch->maxDataInterval
	  ,ch->filterType
	  ,ch->frequency
	  ,ch->maxFrequency
	  ,ch->frequencyCutoff
	  ,ch->minFrequencyCutoff
	  ,ch->maxFrequencyCutoff
	  ,ch->inputMode
	  ,ch->powerSupply
	  ,ch->timeElapsed
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetFrequencyCounterHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetFrequencyCounterHandle)phid;
	res = EPHIDGET_OK;

	switch (bp->vpkt) {
	case BP_SETENABLED:
		TESTBOOL_IOP(bp->iop, getBridgePacketInt32(bp, 0));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->enabled = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Enabled");
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
	case BP_SETFILTERTYPE:
		if (!supportedFilterType(phid, (PhidgetFrequencyCounter_FilterType)getBridgePacketInt32(bp,
		  0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified FilterType is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->filterType = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "FilterType");
		break;
	case BP_SETINPUTMODE:
		if (!supportedInputMode(phid, (Phidget_InputMode)getBridgePacketInt32(bp, 0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified InputMode is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->inputMode = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "InputMode");
		break;
	case BP_SETPOWERSUPPLY:
		if (!supportedPowerSupply(phid, (Phidget_PowerSupply)getBridgePacketInt32(bp, 0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified PowerSupply is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->powerSupply = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "PowerSupply");
		break;
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetFrequencyCounterHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);
	ch = (PhidgetFrequencyCounterHandle)phid;

	ret = EPHIDGET_OK;


	switch (phid->UCD->uid) {
	case PHIDCHUID_1054_FREQUENCYCOUNTER_000:
		ch->dataInterval = 256;
		ch->minDataInterval = 32;
		ch->maxDataInterval = 60000;
		ch->maxFrequency = 1000000;
		ch->maxFrequencyCutoff = 10;
		ch->minFrequencyCutoff = 0.01;
		ch->count = 0;
		ch->timeElapsed = 0;
		ch->frequencyCutoff = 1;
		ch->frequency = 0;
		ch->filterType = FILTER_TYPE_LOGIC_LEVEL;
		ch->enabled = 1;
		break;
	case PHIDCHUID_DAQ1400_FREQUENCYCOUNTER_100:
		ch->dataInterval = 250;
		ch->minDataInterval = 20;
		ch->maxDataInterval = 60000;
		ch->maxFrequency = 1000000;
		ch->maxFrequencyCutoff = 10;
		ch->minFrequencyCutoff = 0.01;
		ch->count = 0;
		ch->timeElapsed = 0;
		ch->frequencyCutoff = 1;
		ch->frequency = 0;
		ch->filterType = FILTER_TYPE_LOGIC_LEVEL;
		ch->powerSupply = POWER_SUPPLY_12V;
		ch->inputMode = INPUT_MODE_NPN;
		ch->enabled = 1;
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}

	ch->totalTicksSinceLastCount = 0;
	ch->frequencyPrecision = 0;

	return (ret);
}

static PhidgetReturnCode CCONV
_setDefaults(PhidgetChannelHandle phid) {
	PhidgetFrequencyCounterHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ch = (PhidgetFrequencyCounterHandle)phid;
	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_1054_FREQUENCYCOUNTER_000:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETFILTERTYPE, NULL, NULL, "%d", ch->filterType);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETENABLED, NULL, NULL, "%d", ch->enabled);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_DAQ1400_FREQUENCYCOUNTER_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETPOWERSUPPLY, NULL, NULL, "%d", ch->powerSupply);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETINPUTMODE, NULL, NULL, "%d", ch->inputMode);
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

}

static void CCONV
PhidgetFrequencyCounter_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetFrequencyCounter));
}

API_PRETURN
PhidgetFrequencyCounter_create(PhidgetFrequencyCounterHandle *phidp) {

	CHANNELCREATE_BODY(FrequencyCounter, PHIDCHCLASS_FREQUENCYCOUNTER);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_delete(PhidgetFrequencyCounterHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}

API_PRETURN
PhidgetFrequencyCounter_getCount(PhidgetFrequencyCounterHandle ch, uint64_t *count) {

	TESTPTR_PR(ch);
	TESTPTR_PR(count);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*count = ch->count;
	if (ch->count == (uint64_t)PUNK_UINT64)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setEnabled(PhidgetFrequencyCounterHandle ch, int enabled) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETENABLED, NULL, NULL, "%d", enabled));
}

API_PRETURN
PhidgetFrequencyCounter_getEnabled(PhidgetFrequencyCounterHandle ch, int *enabled) {

	TESTPTR_PR(ch);
	TESTPTR_PR(enabled);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*enabled = ch->enabled;
	if (ch->enabled == (int)PUNK_BOOL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setDataInterval(PhidgetFrequencyCounterHandle ch, uint32_t dataInterval) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETDATAINTERVAL, NULL, NULL, "%u",
	  dataInterval));
}

API_PRETURN
PhidgetFrequencyCounter_getDataInterval(PhidgetFrequencyCounterHandle ch, uint32_t *dataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*dataInterval = ch->dataInterval;
	if (ch->dataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getMinDataInterval(PhidgetFrequencyCounterHandle ch,
  uint32_t *minDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*minDataInterval = ch->minDataInterval;
	if (ch->minDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getMaxDataInterval(PhidgetFrequencyCounterHandle ch,
  uint32_t *maxDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*maxDataInterval = ch->maxDataInterval;
	if (ch->maxDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setFilterType(PhidgetFrequencyCounterHandle ch,
  PhidgetFrequencyCounter_FilterType filterType) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETFILTERTYPE, NULL, NULL, "%d",
	  filterType));
}

API_PRETURN
PhidgetFrequencyCounter_getFilterType(PhidgetFrequencyCounterHandle ch,
  PhidgetFrequencyCounter_FilterType *filterType) {

	TESTPTR_PR(ch);
	TESTPTR_PR(filterType);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*filterType = ch->filterType;
	if (ch->filterType == (PhidgetFrequencyCounter_FilterType)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getFrequency(PhidgetFrequencyCounterHandle ch, double *frequency) {

	TESTPTR_PR(ch);
	TESTPTR_PR(frequency);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*frequency = ch->frequency;
	if (ch->frequency == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getMaxFrequency(PhidgetFrequencyCounterHandle ch, double *maxFrequency) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxFrequency);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*maxFrequency = ch->maxFrequency;
	if (ch->maxFrequency == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getFrequencyCutoff(PhidgetFrequencyCounterHandle ch, double *frequencyCutoff) {

	TESTPTR_PR(ch);
	TESTPTR_PR(frequencyCutoff);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*frequencyCutoff = ch->frequencyCutoff;
	if (ch->frequencyCutoff == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getMinFrequencyCutoff(PhidgetFrequencyCounterHandle ch,
  double *minFrequencyCutoff) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minFrequencyCutoff);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*minFrequencyCutoff = ch->minFrequencyCutoff;
	if (ch->minFrequencyCutoff == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getMaxFrequencyCutoff(PhidgetFrequencyCounterHandle ch,
  double *maxFrequencyCutoff) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxFrequencyCutoff);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*maxFrequencyCutoff = ch->maxFrequencyCutoff;
	if (ch->maxFrequencyCutoff == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setInputMode(PhidgetFrequencyCounterHandle ch, Phidget_InputMode inputMode) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETINPUTMODE, NULL, NULL, "%d",
	  inputMode));
}

API_PRETURN
PhidgetFrequencyCounter_getInputMode(PhidgetFrequencyCounterHandle ch, Phidget_InputMode *inputMode) {

	TESTPTR_PR(ch);
	TESTPTR_PR(inputMode);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_1054_FREQUENCYCOUNTER_000:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*inputMode = ch->inputMode;
	if (ch->inputMode == (Phidget_InputMode)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setPowerSupply(PhidgetFrequencyCounterHandle ch,
  Phidget_PowerSupply powerSupply) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETPOWERSUPPLY, NULL, NULL, "%d",
	  powerSupply));
}

API_PRETURN
PhidgetFrequencyCounter_getPowerSupply(PhidgetFrequencyCounterHandle ch,
  Phidget_PowerSupply *powerSupply) {

	TESTPTR_PR(ch);
	TESTPTR_PR(powerSupply);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_1054_FREQUENCYCOUNTER_000:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*powerSupply = ch->powerSupply;
	if (ch->powerSupply == (Phidget_PowerSupply)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_getTimeElapsed(PhidgetFrequencyCounterHandle ch, double *timeElapsed) {

	TESTPTR_PR(ch);
	TESTPTR_PR(timeElapsed);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);
	TESTATTACHED_PR(ch);

	*timeElapsed = ch->timeElapsed;
	if (ch->timeElapsed == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setOnCountChangeHandler(PhidgetFrequencyCounterHandle ch,
  PhidgetFrequencyCounter_OnCountChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);

	ch->CountChange = fptr;
	ch->CountChangeCtx = ctx;

	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetFrequencyCounter_setOnFrequencyChangeHandler(PhidgetFrequencyCounterHandle ch,
  PhidgetFrequencyCounter_OnFrequencyChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_FREQUENCYCOUNTER);

	ch->FrequencyChange = fptr;
	ch->FrequencyChangeCtx = ctx;

	return (EPHIDGET_OK);
}
