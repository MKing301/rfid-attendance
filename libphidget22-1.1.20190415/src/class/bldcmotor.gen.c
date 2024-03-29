/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

static void CCONV PhidgetBLDCMotor_errorHandler(PhidgetChannelHandle ch, Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetBLDCMotor_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetBLDCMotor_setStatus(PhidgetChannelHandle phid, BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetBLDCMotor_getStatus(PhidgetChannelHandle phid, BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetBLDCMotor_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetBLDCMotor_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetBLDCMotor_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetBLDCMotor {
	struct _PhidgetChannel phid;
	int64_t positionOffset;
	double acceleration;
	double minAcceleration;
	double maxAcceleration;
	double brakingStrength;
	double minBrakingStrength;
	double maxBrakingStrength;
	uint32_t dataInterval;
	uint32_t minDataInterval;
	uint32_t maxDataInterval;
	int64_t position;
	int64_t minPosition;
	int64_t maxPosition;
	double rescaleFactor;
	double stallVelocity;
	double minStallVelocity;
	double maxStallVelocity;
	double targetBrakingStrength;
	double targetVelocity;
	double velocity;
	double minVelocity;
	double maxVelocity;
	PhidgetBLDCMotor_OnBrakingStrengthChangeCallback BrakingStrengthChange;
	void *BrakingStrengthChangeCtx;
	PhidgetBLDCMotor_OnPositionChangeCallback PositionChange;
	void *PositionChangeCtx;
	PhidgetBLDCMotor_OnVelocityUpdateCallback VelocityUpdate;
	void *VelocityUpdateCtx;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetBLDCMotorHandle ch;
	int version;

	ch = (PhidgetBLDCMotorHandle)phid;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 0) {
		logerr("%"PRIphid": bad version %d != 0", phid, version);
		return (EPHIDGET_BADVERSION);
	}

	ch->positionOffset = getBridgePacketInt64ByName(bp, "positionOffset");
	ch->acceleration = getBridgePacketDoubleByName(bp, "acceleration");
	ch->minAcceleration = getBridgePacketDoubleByName(bp, "minAcceleration");
	ch->maxAcceleration = getBridgePacketDoubleByName(bp, "maxAcceleration");
	ch->brakingStrength = getBridgePacketDoubleByName(bp, "brakingStrength");
	ch->minBrakingStrength = getBridgePacketDoubleByName(bp, "minBrakingStrength");
	ch->maxBrakingStrength = getBridgePacketDoubleByName(bp, "maxBrakingStrength");
	ch->dataInterval = getBridgePacketUInt32ByName(bp, "dataInterval");
	ch->minDataInterval = getBridgePacketUInt32ByName(bp, "minDataInterval");
	ch->maxDataInterval = getBridgePacketUInt32ByName(bp, "maxDataInterval");
	ch->position = getBridgePacketInt64ByName(bp, "position");
	ch->minPosition = getBridgePacketInt64ByName(bp, "minPosition");
	ch->maxPosition = getBridgePacketInt64ByName(bp, "maxPosition");
	ch->rescaleFactor = getBridgePacketDoubleByName(bp, "rescaleFactor");
	ch->stallVelocity = getBridgePacketDoubleByName(bp, "stallVelocity");
	ch->minStallVelocity = getBridgePacketDoubleByName(bp, "minStallVelocity");
	ch->maxStallVelocity = getBridgePacketDoubleByName(bp, "maxStallVelocity");
	ch->targetBrakingStrength = getBridgePacketDoubleByName(bp, "targetBrakingStrength");
	ch->targetVelocity = getBridgePacketDoubleByName(bp, "targetVelocity");
	ch->velocity = getBridgePacketDoubleByName(bp, "velocity");
	ch->minVelocity = getBridgePacketDoubleByName(bp, "minVelocity");
	ch->maxVelocity = getBridgePacketDoubleByName(bp, "maxVelocity");

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	PhidgetBLDCMotorHandle ch;

	ch = (PhidgetBLDCMotorHandle)phid;

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ",positionOffset=%l"
	  ",acceleration=%g"
	  ",minAcceleration=%g"
	  ",maxAcceleration=%g"
	  ",brakingStrength=%g"
	  ",minBrakingStrength=%g"
	  ",maxBrakingStrength=%g"
	  ",dataInterval=%u"
	  ",minDataInterval=%u"
	  ",maxDataInterval=%u"
	  ",position=%l"
	  ",minPosition=%l"
	  ",maxPosition=%l"
	  ",rescaleFactor=%g"
	  ",stallVelocity=%g"
	  ",minStallVelocity=%g"
	  ",maxStallVelocity=%g"
	  ",targetBrakingStrength=%g"
	  ",targetVelocity=%g"
	  ",velocity=%g"
	  ",minVelocity=%g"
	  ",maxVelocity=%g"
	  ,0 /* class version */
	  ,ch->positionOffset
	  ,ch->acceleration
	  ,ch->minAcceleration
	  ,ch->maxAcceleration
	  ,ch->brakingStrength
	  ,ch->minBrakingStrength
	  ,ch->maxBrakingStrength
	  ,ch->dataInterval
	  ,ch->minDataInterval
	  ,ch->maxDataInterval
	  ,ch->position
	  ,ch->minPosition
	  ,ch->maxPosition
	  ,ch->rescaleFactor
	  ,ch->stallVelocity
	  ,ch->minStallVelocity
	  ,ch->maxStallVelocity
	  ,ch->targetBrakingStrength
	  ,ch->targetVelocity
	  ,ch->velocity
	  ,ch->minVelocity
	  ,ch->maxVelocity
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetBLDCMotorHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetBLDCMotorHandle)phid;
	res = EPHIDGET_OK;

	switch (bp->vpkt) {
	case BP_SETACCELERATION:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minAcceleration,
		  ch->maxAcceleration);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->acceleration = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Acceleration");
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
	case BP_SETSTALLVELOCITY:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minStallVelocity,
		  ch->maxStallVelocity);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->stallVelocity = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "StallVelocity");
		break;
	case BP_SETBRAKINGDUTYCYCLE:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->targetBrakingStrength = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "TargetBrakingStrength");
		break;
	case BP_SETDUTYCYCLE:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->targetVelocity = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "TargetVelocity");
		break;
	case BP_BRAKINGSTRENGTHCHANGE:
		ch->brakingStrength = getBridgePacketDouble(bp, 0);
		FIRECH(ch, BrakingStrengthChange, ch->brakingStrength);
		break;
	case BP_DUTYCYCLECHANGE:
		ch->velocity = getBridgePacketDouble(bp, 0);
		FIRECH(ch, VelocityUpdate, ch->velocity);
		break;
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetBLDCMotorHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);
	ch = (PhidgetBLDCMotorHandle)phid;

	ret = EPHIDGET_OK;


	switch (phid->UCD->uid) {
	case PHIDCHUID_DCC1100_BLDCMOTOR_100:
		ch->dataInterval = 250;
		ch->minDataInterval = 100;
		ch->maxDataInterval = 60000;
		ch->acceleration = 1;
		ch->targetBrakingStrength = 0;
		ch->maxAcceleration = 100;
		ch->maxBrakingStrength = 1;
		ch->maxVelocity = 1;
		ch->maxPosition = 1000000000000000;
		ch->minVelocity = 0;
		ch->minAcceleration = 0.1;
		ch->minBrakingStrength = 0;
		ch->minPosition = -1000000000000000;
		ch->position = 0;
		ch->rescaleFactor = 1;
		ch->targetVelocity = PUNK_DBL;
		ch->velocity = PUNK_DBL;
		ch->brakingStrength = PUNK_DBL;
		ch->stallVelocity = 1000;
		ch->minStallVelocity = 0;
		ch->maxStallVelocity = 2000;
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}

	ch->positionOffset = 0;

	return (ret);
}

static PhidgetReturnCode CCONV
_setDefaults(PhidgetChannelHandle phid) {
	PhidgetBLDCMotorHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ch = (PhidgetBLDCMotorHandle)phid;
	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_DCC1100_BLDCMOTOR_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETACCELERATION, NULL, NULL, "%g", ch->acceleration);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETBRAKINGDUTYCYCLE, NULL, NULL, "%g",
		  ch->targetBrakingStrength);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETSTALLVELOCITY, NULL, NULL, "%g", ch->stallVelocity);
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
	PhidgetBLDCMotorHandle ch;

	ch = (PhidgetBLDCMotorHandle)phid;

	if(ch->brakingStrength != PUNK_DBL)
		FIRECH(ch, BrakingStrengthChange, ch->brakingStrength);
	if(ch->velocity != PUNK_DBL)
		FIRECH(ch, VelocityUpdate, ch->velocity);

}

static void CCONV
PhidgetBLDCMotor_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetBLDCMotor));
}

API_PRETURN
PhidgetBLDCMotor_create(PhidgetBLDCMotorHandle *phidp) {

	CHANNELCREATE_BODY(BLDCMotor, PHIDCHCLASS_BLDCMOTOR);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_delete(PhidgetBLDCMotorHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}

API_PRETURN
PhidgetBLDCMotor_setAcceleration(PhidgetBLDCMotorHandle ch, double acceleration) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETACCELERATION, NULL, NULL, "%g",
	  acceleration));
}

API_PRETURN
PhidgetBLDCMotor_getAcceleration(PhidgetBLDCMotorHandle ch, double *acceleration) {

	TESTPTR_PR(ch);
	TESTPTR_PR(acceleration);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*acceleration = ch->acceleration;
	if (ch->acceleration == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMinAcceleration(PhidgetBLDCMotorHandle ch, double *minAcceleration) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minAcceleration);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*minAcceleration = ch->minAcceleration;
	if (ch->minAcceleration == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMaxAcceleration(PhidgetBLDCMotorHandle ch, double *maxAcceleration) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxAcceleration);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*maxAcceleration = ch->maxAcceleration;
	if (ch->maxAcceleration == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getBrakingStrength(PhidgetBLDCMotorHandle ch, double *brakingStrength) {

	TESTPTR_PR(ch);
	TESTPTR_PR(brakingStrength);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*brakingStrength = ch->brakingStrength;
	if (ch->brakingStrength == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMinBrakingStrength(PhidgetBLDCMotorHandle ch, double *minBrakingStrength) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minBrakingStrength);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*minBrakingStrength = ch->minBrakingStrength;
	if (ch->minBrakingStrength == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMaxBrakingStrength(PhidgetBLDCMotorHandle ch, double *maxBrakingStrength) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxBrakingStrength);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*maxBrakingStrength = ch->maxBrakingStrength;
	if (ch->maxBrakingStrength == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_setDataInterval(PhidgetBLDCMotorHandle ch, uint32_t dataInterval) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETDATAINTERVAL, NULL, NULL, "%u",
	  dataInterval));
}

API_PRETURN
PhidgetBLDCMotor_getDataInterval(PhidgetBLDCMotorHandle ch, uint32_t *dataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*dataInterval = ch->dataInterval;
	if (ch->dataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMinDataInterval(PhidgetBLDCMotorHandle ch, uint32_t *minDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*minDataInterval = ch->minDataInterval;
	if (ch->minDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMaxDataInterval(PhidgetBLDCMotorHandle ch, uint32_t *maxDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*maxDataInterval = ch->maxDataInterval;
	if (ch->maxDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getRescaleFactor(PhidgetBLDCMotorHandle ch, double *rescaleFactor) {

	TESTPTR_PR(ch);
	TESTPTR_PR(rescaleFactor);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*rescaleFactor = ch->rescaleFactor;
	if (ch->rescaleFactor == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_setTargetBrakingStrength(PhidgetBLDCMotorHandle ch, double targetBrakingStrength) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETBRAKINGDUTYCYCLE, NULL, NULL, "%g",
	  targetBrakingStrength));
}

API_PRETURN
PhidgetBLDCMotor_getTargetBrakingStrength(PhidgetBLDCMotorHandle ch, double *targetBrakingStrength) {

	TESTPTR_PR(ch);
	TESTPTR_PR(targetBrakingStrength);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*targetBrakingStrength = ch->targetBrakingStrength;
	if (ch->targetBrakingStrength == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_setTargetVelocity(PhidgetBLDCMotorHandle ch, double targetVelocity) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETDUTYCYCLE, NULL, NULL, "%g",
	  targetVelocity));
}

API_PRETURN
PhidgetBLDCMotor_getTargetVelocity(PhidgetBLDCMotorHandle ch, double *targetVelocity) {

	TESTPTR_PR(ch);
	TESTPTR_PR(targetVelocity);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*targetVelocity = ch->targetVelocity;
	if (ch->targetVelocity == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getVelocity(PhidgetBLDCMotorHandle ch, double *velocity) {

	TESTPTR_PR(ch);
	TESTPTR_PR(velocity);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*velocity = ch->velocity;
	if (ch->velocity == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMinVelocity(PhidgetBLDCMotorHandle ch, double *minVelocity) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minVelocity);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*minVelocity = ch->minVelocity;
	if (ch->minVelocity == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_getMaxVelocity(PhidgetBLDCMotorHandle ch, double *maxVelocity) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxVelocity);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);
	TESTATTACHED_PR(ch);

	*maxVelocity = ch->maxVelocity;
	if (ch->maxVelocity == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_setOnBrakingStrengthChangeHandler(PhidgetBLDCMotorHandle ch,
  PhidgetBLDCMotor_OnBrakingStrengthChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);

	ch->BrakingStrengthChange = fptr;
	ch->BrakingStrengthChangeCtx = ctx;

	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_setOnPositionChangeHandler(PhidgetBLDCMotorHandle ch,
  PhidgetBLDCMotor_OnPositionChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);

	ch->PositionChange = fptr;
	ch->PositionChangeCtx = ctx;

	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetBLDCMotor_setOnVelocityUpdateHandler(PhidgetBLDCMotorHandle ch,
  PhidgetBLDCMotor_OnVelocityUpdateCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_BLDCMOTOR);

	ch->VelocityUpdate = fptr;
	ch->VelocityUpdateCtx = ctx;

	return (EPHIDGET_OK);
}
