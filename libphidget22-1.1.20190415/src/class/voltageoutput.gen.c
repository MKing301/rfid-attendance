/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

#include "device/analogdevice.h"
static void CCONV PhidgetVoltageOutput_errorHandler(PhidgetChannelHandle ch,
  Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetVoltageOutput_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetVoltageOutput_setStatus(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetVoltageOutput_getStatus(PhidgetChannelHandle phid,
  BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetVoltageOutput_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetVoltageOutput_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetVoltageOutput_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetVoltageOutput {
	struct _PhidgetChannel phid;
	int enabled;
	double voltage;
	double minVoltage;
	double maxVoltage;
	PhidgetVoltageOutput_VoltageOutputRange voltageOutputRange;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetVoltageOutputHandle ch;
	int version;

	ch = (PhidgetVoltageOutputHandle)phid;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 0) {
		logerr("%"PRIphid": bad version %d != 0", phid, version);
		return (EPHIDGET_BADVERSION);
	}

	ch->enabled = getBridgePacketInt32ByName(bp, "enabled");
	ch->voltage = getBridgePacketDoubleByName(bp, "voltage");
	ch->minVoltage = getBridgePacketDoubleByName(bp, "minVoltage");
	ch->maxVoltage = getBridgePacketDoubleByName(bp, "maxVoltage");
	ch->voltageOutputRange = getBridgePacketInt32ByName(bp, "voltageOutputRange");

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	PhidgetVoltageOutputHandle ch;

	ch = (PhidgetVoltageOutputHandle)phid;

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ",enabled=%d"
	  ",voltage=%g"
	  ",minVoltage=%g"
	  ",maxVoltage=%g"
	  ",voltageOutputRange=%d"
	  ,0 /* class version */
	  ,ch->enabled
	  ,ch->voltage
	  ,ch->minVoltage
	  ,ch->maxVoltage
	  ,ch->voltageOutputRange
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetVoltageOutputHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetVoltageOutputHandle)phid;
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
	case BP_SETVOLTAGE:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minVoltage, ch->maxVoltage);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->voltage = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Voltage");
		break;
	case BP_SETVOLTAGERANGE:
		if (!supportedVoltageOutputRange(phid,
		  (PhidgetVoltageOutput_VoltageOutputRange)getBridgePacketInt32(bp, 0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified VoltageOutputRange is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->voltageOutputRange = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "VoltageOutputRange");
		break;
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetVoltageOutputHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);
	ch = (PhidgetVoltageOutputHandle)phid;

	ret = EPHIDGET_OK;


	switch (phid->UCD->uid) {
	case PHIDCHUID_1002_VOLTAGEOUTPUT_100:
		ch->enabled = 1;
		ch->maxVoltage = 10;
		ch->minVoltage = -10;
		ch->voltage = 0;
		ch->voltageOutputRange = VOLTAGE_OUTPUT_RANGE_10V;
		break;
	case PHIDCHUID_OUT1000_VOLTAGEOUTPUT_100:
		ch->enabled = 1;
		ch->maxVoltage = 4.2;
		ch->minVoltage = 0;
		ch->voltage = 0;
		break;
	case PHIDCHUID_OUT1001_VOLTAGEOUTPUT_100:
		ch->enabled = 1;
		ch->maxVoltage = PUNK_DBL;
		ch->minVoltage = PUNK_DBL;
		ch->voltage = 0;
		ch->voltageOutputRange = VOLTAGE_OUTPUT_RANGE_10V;
		break;
	case PHIDCHUID_OUT1002_VOLTAGEOUTPUT_100:
		ch->enabled = 1;
		ch->maxVoltage = PUNK_DBL;
		ch->minVoltage = PUNK_DBL;
		ch->voltage = 0;
		ch->voltageOutputRange = VOLTAGE_OUTPUT_RANGE_10V;
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}


	return (ret);
}

static PhidgetReturnCode CCONV
_setDefaults(PhidgetChannelHandle phid) {
	PhidgetVoltageOutputHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ch = (PhidgetVoltageOutputHandle)phid;
	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_1002_VOLTAGEOUTPUT_100:
		ret = bridgeSendToDevice(phid, BP_SETENABLED, NULL, NULL, "%d", ch->enabled);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETVOLTAGE, NULL, NULL, "%g", ch->voltage);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_OUT1000_VOLTAGEOUTPUT_100:
		ret = bridgeSendToDevice(phid, BP_SETENABLED, NULL, NULL, "%d", ch->enabled);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETVOLTAGE, NULL, NULL, "%g", ch->voltage);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_OUT1001_VOLTAGEOUTPUT_100:
		ret = bridgeSendToDevice(phid, BP_SETVOLTAGERANGE, NULL, NULL, "%d", ch->voltageOutputRange);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_OUT1002_VOLTAGEOUTPUT_100:
		ret = bridgeSendToDevice(phid, BP_SETVOLTAGERANGE, NULL, NULL, "%d", ch->voltageOutputRange);
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
PhidgetVoltageOutput_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetVoltageOutput));
}

API_PRETURN
PhidgetVoltageOutput_create(PhidgetVoltageOutputHandle *phidp) {

	CHANNELCREATE_BODY(VoltageOutput, PHIDCHCLASS_VOLTAGEOUTPUT);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetVoltageOutput_delete(PhidgetVoltageOutputHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}

API_PRETURN
PhidgetVoltageOutput_setEnabled(PhidgetVoltageOutputHandle ch, int enabled) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETENABLED, NULL, NULL, "%d", enabled));
}

API_PRETURN
PhidgetVoltageOutput_getEnabled(PhidgetVoltageOutputHandle ch, int *enabled) {

	TESTPTR_PR(ch);
	TESTPTR_PR(enabled);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	*enabled = ch->enabled;
	if (ch->enabled == (int)PUNK_BOOL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetVoltageOutput_setVoltage(PhidgetVoltageOutputHandle ch, double voltage) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETVOLTAGE, NULL, NULL, "%g", voltage));
}

API_VRETURN
PhidgetVoltageOutput_setVoltage_async(PhidgetVoltageOutputHandle ch, double voltage,
  Phidget_AsyncCallback fptr, void *ctx) {
	PhidgetReturnCode res;

	if (ch == NULL) {
		if (fptr) fptr((PhidgetHandle)ch, ctx, EPHIDGET_INVALIDARG);
		return;
	}
	if (ch->phid.class != PHIDCHCLASS_VOLTAGEOUTPUT) {
		if (fptr) fptr((PhidgetHandle)ch, ctx, EPHIDGET_WRONGDEVICE);
		return;
	}
	if (!ISATTACHED(ch)) {
		if (fptr) fptr((PhidgetHandle)ch, ctx, EPHIDGET_NOTATTACHED);
		return;
	}

	res = bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETVOLTAGE, fptr, ctx, "%g", voltage);
	if (res != EPHIDGET_OK && fptr != NULL)
		fptr((PhidgetHandle)ch, ctx, res);
}

API_PRETURN
PhidgetVoltageOutput_getVoltage(PhidgetVoltageOutputHandle ch, double *voltage) {

	TESTPTR_PR(ch);
	TESTPTR_PR(voltage);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	*voltage = ch->voltage;
	if (ch->voltage == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetVoltageOutput_getMinVoltage(PhidgetVoltageOutputHandle ch, double *minVoltage) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minVoltage);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	*minVoltage = ch->minVoltage;
	if (ch->minVoltage == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetVoltageOutput_getMaxVoltage(PhidgetVoltageOutputHandle ch, double *maxVoltage) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxVoltage);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	*maxVoltage = ch->maxVoltage;
	if (ch->maxVoltage == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetVoltageOutput_setVoltageOutputRange(PhidgetVoltageOutputHandle ch,
  PhidgetVoltageOutput_VoltageOutputRange voltageOutputRange) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETVOLTAGERANGE, NULL, NULL, "%d",
	  voltageOutputRange));
}

API_PRETURN
PhidgetVoltageOutput_getVoltageOutputRange(PhidgetVoltageOutputHandle ch,
  PhidgetVoltageOutput_VoltageOutputRange *voltageOutputRange) {

	TESTPTR_PR(ch);
	TESTPTR_PR(voltageOutputRange);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_VOLTAGEOUTPUT);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_OUT1000_VOLTAGEOUTPUT_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*voltageOutputRange = ch->voltageOutputRange;
	if (ch->voltageOutputRange == (PhidgetVoltageOutput_VoltageOutputRange)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}
