/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

static void CCONV PhidgetSoundSensor_errorHandler(PhidgetChannelHandle ch, Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetSoundSensor_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetSoundSensor_setStatus(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetSoundSensor_getStatus(PhidgetChannelHandle phid,
  BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetSoundSensor_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetSoundSensor_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetSoundSensor_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetSoundSensor {
	struct _PhidgetChannel phid;
	double lastdB;
	uint32_t dataInterval;
	uint32_t minDataInterval;
	uint32_t maxDataInterval;
	double dB;
	double maxdB;
	double dBA;
	double dBC;
	double noiseFloor;
	double octaves[10];
	double SPLChangeTrigger;
	double minSPLChangeTrigger;
	double maxSPLChangeTrigger;
	PhidgetSoundSensor_SPLRange SPLRange;
	PhidgetSoundSensor_OnSPLChangeCallback SPLChange;
	void *SPLChangeCtx;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetSoundSensorHandle ch;
	int version;

	ch = (PhidgetSoundSensorHandle)phid;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 1) {
		logerr("%"PRIphid": bad version %d != 1", phid, version);
		return (EPHIDGET_BADVERSION);
	}

	ch->lastdB = getBridgePacketDoubleByName(bp, "lastdB");
	ch->dataInterval = getBridgePacketUInt32ByName(bp, "dataInterval");
	ch->minDataInterval = getBridgePacketUInt32ByName(bp, "minDataInterval");
	ch->maxDataInterval = getBridgePacketUInt32ByName(bp, "maxDataInterval");
	ch->dB = getBridgePacketDoubleByName(bp, "dB");
	ch->maxdB = getBridgePacketDoubleByName(bp, "maxdB");
	ch->dBA = getBridgePacketDoubleByName(bp, "dBA");
	ch->dBC = getBridgePacketDoubleByName(bp, "dBC");
	ch->noiseFloor = getBridgePacketDoubleByName(bp, "noiseFloor");
	memcpy(&ch->octaves, getBridgePacketDoubleArrayByName(bp, "octaves"), sizeof (double) * 10);
	ch->SPLChangeTrigger = getBridgePacketDoubleByName(bp, "SPLChangeTrigger");
	ch->minSPLChangeTrigger = getBridgePacketDoubleByName(bp, "minSPLChangeTrigger");
	ch->maxSPLChangeTrigger = getBridgePacketDoubleByName(bp, "maxSPLChangeTrigger");
	ch->SPLRange = getBridgePacketInt32ByName(bp, "SPLRange");

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	PhidgetSoundSensorHandle ch;

	ch = (PhidgetSoundSensorHandle)phid;

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ",lastdB=%g"
	  ",dataInterval=%u"
	  ",minDataInterval=%u"
	  ",maxDataInterval=%u"
	  ",dB=%g"
	  ",maxdB=%g"
	  ",dBA=%g"
	  ",dBC=%g"
	  ",noiseFloor=%g"
	  ",octaves=%10G"
	  ",SPLChangeTrigger=%g"
	  ",minSPLChangeTrigger=%g"
	  ",maxSPLChangeTrigger=%g"
	  ",SPLRange=%d"
	  ,1 /* class version */
	  ,ch->lastdB
	  ,ch->dataInterval
	  ,ch->minDataInterval
	  ,ch->maxDataInterval
	  ,ch->dB
	  ,ch->maxdB
	  ,ch->dBA
	  ,ch->dBC
	  ,ch->noiseFloor
	  ,ch->octaves
	  ,ch->SPLChangeTrigger
	  ,ch->minSPLChangeTrigger
	  ,ch->maxSPLChangeTrigger
	  ,ch->SPLRange
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetSoundSensorHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetSoundSensorHandle)phid;
	res = EPHIDGET_OK;

	switch (bp->vpkt) {
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
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minSPLChangeTrigger,
		  ch->maxSPLChangeTrigger);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->SPLChangeTrigger = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "SPLChangeTrigger");
		break;
	case BP_SETSPLRANGE:
		if (!supportedSPLRange(phid, (PhidgetSoundSensor_SPLRange)getBridgePacketInt32(bp, 0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified SPLRange is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->SPLRange = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "SPLRange");
		break;
	case BP_DBCHANGE:
		ch->dB = getBridgePacketDouble(bp, 0);
		ch->dBA = getBridgePacketDouble(bp, 1);
		ch->dBC = getBridgePacketDouble(bp, 2);
		memcpy(&ch->octaves, getBridgePacketDoubleArray(bp, 3), sizeof (double) * 10);
		FIRECH(ch, SPLChange, ch->dB, ch->dBA, ch->dBC, ch->octaves);
		break;
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetSoundSensorHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);
	ch = (PhidgetSoundSensorHandle)phid;

	ret = EPHIDGET_OK;


	switch (phid->UCD->uid) {
	case PHIDCHUID_SND1000_SOUNDSENSOR_100:
		ch->dataInterval = 250;
		ch->maxDataInterval = 60000;
		ch->maxdB = 102;
		ch->maxSPLChangeTrigger = 102;
		ch->minDataInterval = 100;
		ch->noiseFloor = 34;
		ch->minSPLChangeTrigger = 0;
		ch->dB = PUNK_DBL;
		ch->dBA = PUNK_DBL;
		ch->dBC = PUNK_DBL;
		ch->octaves[0] = PUNK_DBL;
		ch->octaves[1] = PUNK_DBL;
		ch->octaves[2] = PUNK_DBL;
		ch->octaves[3] = PUNK_DBL;
		ch->octaves[4] = PUNK_DBL;
		ch->octaves[5] = PUNK_DBL;
		ch->octaves[6] = PUNK_DBL;
		ch->octaves[7] = PUNK_DBL;
		ch->octaves[8] = PUNK_DBL;
		ch->octaves[9] = PUNK_DBL;
		ch->SPLRange = SPL_RANGE_102dB;
		ch->SPLChangeTrigger = 0;
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}

	ch->lastdB = 0;

	return (ret);
}

static PhidgetReturnCode CCONV
_setDefaults(PhidgetChannelHandle phid) {
	PhidgetSoundSensorHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ch = (PhidgetSoundSensorHandle)phid;
	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_SND1000_SOUNDSENSOR_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETSPLRANGE, NULL, NULL, "%d", ch->SPLRange);
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
PhidgetSoundSensor_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetSoundSensor));
}

API_PRETURN
PhidgetSoundSensor_create(PhidgetSoundSensorHandle *phidp) {

	CHANNELCREATE_BODY(SoundSensor, PHIDCHCLASS_SOUNDSENSOR);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_delete(PhidgetSoundSensorHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}

API_PRETURN
PhidgetSoundSensor_setDataInterval(PhidgetSoundSensorHandle ch, uint32_t dataInterval) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETDATAINTERVAL, NULL, NULL, "%u",
	  dataInterval));
}

API_PRETURN
PhidgetSoundSensor_getDataInterval(PhidgetSoundSensorHandle ch, uint32_t *dataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*dataInterval = ch->dataInterval;
	if (ch->dataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getMinDataInterval(PhidgetSoundSensorHandle ch, uint32_t *minDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*minDataInterval = ch->minDataInterval;
	if (ch->minDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getMaxDataInterval(PhidgetSoundSensorHandle ch, uint32_t *maxDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*maxDataInterval = ch->maxDataInterval;
	if (ch->maxDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getdB(PhidgetSoundSensorHandle ch, double *dB) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dB);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*dB = ch->dB;
	if (ch->dB == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getMaxdB(PhidgetSoundSensorHandle ch, double *maxdB) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxdB);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*maxdB = ch->maxdB;
	if (ch->maxdB == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getdBA(PhidgetSoundSensorHandle ch, double *dBA) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dBA);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*dBA = ch->dBA;
	if (ch->dBA == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getdBC(PhidgetSoundSensorHandle ch, double *dBC) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dBC);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*dBC = ch->dBC;
	if (ch->dBC == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getNoiseFloor(PhidgetSoundSensorHandle ch, double *noiseFloor) {

	TESTPTR_PR(ch);
	TESTPTR_PR(noiseFloor);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*noiseFloor = ch->noiseFloor;
	if (ch->noiseFloor == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getOctaves(PhidgetSoundSensorHandle ch, double (*octaves)[10]) {

	TESTPTR_PR(ch);
	TESTPTR_PR(octaves);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	(*octaves)[0] = ch->octaves[0];
	if (ch->octaves[0] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[1] = ch->octaves[1];
	if (ch->octaves[1] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[2] = ch->octaves[2];
	if (ch->octaves[2] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[3] = ch->octaves[3];
	if (ch->octaves[3] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[4] = ch->octaves[4];
	if (ch->octaves[4] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[5] = ch->octaves[5];
	if (ch->octaves[5] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[6] = ch->octaves[6];
	if (ch->octaves[6] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[7] = ch->octaves[7];
	if (ch->octaves[7] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[8] = ch->octaves[8];
	if (ch->octaves[8] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	(*octaves)[9] = ch->octaves[9];
	if (ch->octaves[9] == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_setSPLChangeTrigger(PhidgetSoundSensorHandle ch, double SPLChangeTrigger) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETCHANGETRIGGER, NULL, NULL, "%g",
	  SPLChangeTrigger));
}

API_PRETURN
PhidgetSoundSensor_getSPLChangeTrigger(PhidgetSoundSensorHandle ch, double *SPLChangeTrigger) {

	TESTPTR_PR(ch);
	TESTPTR_PR(SPLChangeTrigger);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*SPLChangeTrigger = ch->SPLChangeTrigger;
	if (ch->SPLChangeTrigger == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getMinSPLChangeTrigger(PhidgetSoundSensorHandle ch, double *minSPLChangeTrigger) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minSPLChangeTrigger);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*minSPLChangeTrigger = ch->minSPLChangeTrigger;
	if (ch->minSPLChangeTrigger == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_getMaxSPLChangeTrigger(PhidgetSoundSensorHandle ch, double *maxSPLChangeTrigger) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxSPLChangeTrigger);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*maxSPLChangeTrigger = ch->maxSPLChangeTrigger;
	if (ch->maxSPLChangeTrigger == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_setSPLRange(PhidgetSoundSensorHandle ch, PhidgetSoundSensor_SPLRange SPLRange) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETSPLRANGE, NULL, NULL, "%d", SPLRange));
}

API_PRETURN
PhidgetSoundSensor_getSPLRange(PhidgetSoundSensorHandle ch, PhidgetSoundSensor_SPLRange *SPLRange) {

	TESTPTR_PR(ch);
	TESTPTR_PR(SPLRange);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);
	TESTATTACHED_PR(ch);

	*SPLRange = ch->SPLRange;
	if (ch->SPLRange == (PhidgetSoundSensor_SPLRange)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetSoundSensor_setOnSPLChangeHandler(PhidgetSoundSensorHandle ch,
  PhidgetSoundSensor_OnSPLChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_SOUNDSENSOR);

	ch->SPLChange = fptr;
	ch->SPLChangeCtx = ctx;

	return (EPHIDGET_OK);
}
