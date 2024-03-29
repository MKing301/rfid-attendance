/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

static void CCONV PhidgetMotorPositionController_errorHandler(PhidgetChannelHandle ch,
  Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetMotorPositionController_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetMotorPositionController_setStatus(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetMotorPositionController_getStatus(PhidgetChannelHandle phid,
  BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetMotorPositionController_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetMotorPositionController_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetMotorPositionController_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetMotorPositionController {
	struct _PhidgetChannel phid;
	int64_t positionOffset;
	double acceleration;
	double minAcceleration;
	double maxAcceleration;
	double currentLimit;
	double minCurrentLimit;
	double maxCurrentLimit;
	double currentRegulatorGain;
	double minCurrentRegulatorGain;
	double maxCurrentRegulatorGain;
	uint32_t dataInterval;
	uint32_t minDataInterval;
	uint32_t maxDataInterval;
	uint32_t deadBand;
	double dutyCycle;
	int engaged;
	Phidget_FanMode fanMode;
	Phidget_EncoderIOMode IOMode;
	double kd;
	double ki;
	double kp;
	int64_t position;
	int64_t minPosition;
	int64_t maxPosition;
	double rescaleFactor;
	double stallVelocity;
	double minStallVelocity;
	double maxStallVelocity;
	int64_t targetPosition;
	double velocityLimit;
	double minVelocityLimit;
	double maxVelocityLimit;
	PhidgetMotorPositionController_OnDutyCycleUpdateCallback DutyCycleUpdate;
	void *DutyCycleUpdateCtx;
	PhidgetMotorPositionController_OnPositionChangeCallback PositionChange;
	void *PositionChangeCtx;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetMotorPositionControllerHandle ch;
	int version;

	ch = (PhidgetMotorPositionControllerHandle)phid;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 0) {
		logerr("%"PRIphid": bad version %d != 0", phid, version);
		return (EPHIDGET_BADVERSION);
	}

	ch->positionOffset = getBridgePacketInt64ByName(bp, "positionOffset");
	ch->acceleration = getBridgePacketDoubleByName(bp, "acceleration");
	ch->minAcceleration = getBridgePacketDoubleByName(bp, "minAcceleration");
	ch->maxAcceleration = getBridgePacketDoubleByName(bp, "maxAcceleration");
	ch->currentLimit = getBridgePacketDoubleByName(bp, "currentLimit");
	ch->minCurrentLimit = getBridgePacketDoubleByName(bp, "minCurrentLimit");
	ch->maxCurrentLimit = getBridgePacketDoubleByName(bp, "maxCurrentLimit");
	ch->currentRegulatorGain = getBridgePacketDoubleByName(bp, "currentRegulatorGain");
	ch->minCurrentRegulatorGain = getBridgePacketDoubleByName(bp, "minCurrentRegulatorGain");
	ch->maxCurrentRegulatorGain = getBridgePacketDoubleByName(bp, "maxCurrentRegulatorGain");
	ch->dataInterval = getBridgePacketUInt32ByName(bp, "dataInterval");
	ch->minDataInterval = getBridgePacketUInt32ByName(bp, "minDataInterval");
	ch->maxDataInterval = getBridgePacketUInt32ByName(bp, "maxDataInterval");
	ch->deadBand = getBridgePacketUInt32ByName(bp, "deadBand");
	ch->dutyCycle = getBridgePacketDoubleByName(bp, "dutyCycle");
	ch->engaged = getBridgePacketInt32ByName(bp, "engaged");
	ch->fanMode = getBridgePacketInt32ByName(bp, "fanMode");
	ch->IOMode = getBridgePacketInt32ByName(bp, "IOMode");
	ch->kd = getBridgePacketDoubleByName(bp, "kd");
	ch->ki = getBridgePacketDoubleByName(bp, "ki");
	ch->kp = getBridgePacketDoubleByName(bp, "kp");
	ch->position = getBridgePacketInt64ByName(bp, "position");
	ch->minPosition = getBridgePacketInt64ByName(bp, "minPosition");
	ch->maxPosition = getBridgePacketInt64ByName(bp, "maxPosition");
	ch->rescaleFactor = getBridgePacketDoubleByName(bp, "rescaleFactor");
	ch->stallVelocity = getBridgePacketDoubleByName(bp, "stallVelocity");
	ch->minStallVelocity = getBridgePacketDoubleByName(bp, "minStallVelocity");
	ch->maxStallVelocity = getBridgePacketDoubleByName(bp, "maxStallVelocity");
	ch->targetPosition = getBridgePacketInt64ByName(bp, "targetPosition");
	ch->velocityLimit = getBridgePacketDoubleByName(bp, "velocityLimit");
	ch->minVelocityLimit = getBridgePacketDoubleByName(bp, "minVelocityLimit");
	ch->maxVelocityLimit = getBridgePacketDoubleByName(bp, "maxVelocityLimit");

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	PhidgetMotorPositionControllerHandle ch;

	ch = (PhidgetMotorPositionControllerHandle)phid;

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ",positionOffset=%l"
	  ",acceleration=%g"
	  ",minAcceleration=%g"
	  ",maxAcceleration=%g"
	  ",currentLimit=%g"
	  ",minCurrentLimit=%g"
	  ",maxCurrentLimit=%g"
	  ",currentRegulatorGain=%g"
	  ",minCurrentRegulatorGain=%g"
	  ",maxCurrentRegulatorGain=%g"
	  ",dataInterval=%u"
	  ",minDataInterval=%u"
	  ",maxDataInterval=%u"
	  ",deadBand=%u"
	  ",dutyCycle=%g"
	  ",engaged=%d"
	  ",fanMode=%d"
	  ",IOMode=%d"
	  ",kd=%g"
	  ",ki=%g"
	  ",kp=%g"
	  ",position=%l"
	  ",minPosition=%l"
	  ",maxPosition=%l"
	  ",rescaleFactor=%g"
	  ",stallVelocity=%g"
	  ",minStallVelocity=%g"
	  ",maxStallVelocity=%g"
	  ",targetPosition=%l"
	  ",velocityLimit=%g"
	  ",minVelocityLimit=%g"
	  ",maxVelocityLimit=%g"
	  ,0 /* class version */
	  ,ch->positionOffset
	  ,ch->acceleration
	  ,ch->minAcceleration
	  ,ch->maxAcceleration
	  ,ch->currentLimit
	  ,ch->minCurrentLimit
	  ,ch->maxCurrentLimit
	  ,ch->currentRegulatorGain
	  ,ch->minCurrentRegulatorGain
	  ,ch->maxCurrentRegulatorGain
	  ,ch->dataInterval
	  ,ch->minDataInterval
	  ,ch->maxDataInterval
	  ,ch->deadBand
	  ,ch->dutyCycle
	  ,ch->engaged
	  ,ch->fanMode
	  ,ch->IOMode
	  ,ch->kd
	  ,ch->ki
	  ,ch->kp
	  ,ch->position
	  ,ch->minPosition
	  ,ch->maxPosition
	  ,ch->rescaleFactor
	  ,ch->stallVelocity
	  ,ch->minStallVelocity
	  ,ch->maxStallVelocity
	  ,ch->targetPosition
	  ,ch->velocityLimit
	  ,ch->minVelocityLimit
	  ,ch->maxVelocityLimit
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetMotorPositionControllerHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetMotorPositionControllerHandle)phid;
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
	case BP_SETCURRENTLIMIT:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minCurrentLimit,
		  ch->maxCurrentLimit);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->currentLimit = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "CurrentLimit");
		break;
	case BP_SETCURRENTREGULATORGAIN:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minCurrentRegulatorGain,
		  ch->maxCurrentRegulatorGain);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->currentRegulatorGain = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "CurrentRegulatorGain");
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
	case BP_SETDEADBAND:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->deadBand = getBridgePacketUInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "DeadBand");
		break;
	case BP_SETENGAGED:
		TESTBOOL_IOP(bp->iop, getBridgePacketInt32(bp, 0));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->engaged = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Engaged");
		break;
	case BP_SETFANMODE:
		if (!supportedFanMode(phid, (Phidget_FanMode)getBridgePacketInt32(bp, 0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified FanMode is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->fanMode = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "FanMode");
		break;
	case BP_SETIOMODE:
		if (!supportedEncoderIOMode(phid, (Phidget_EncoderIOMode)getBridgePacketInt32(bp, 0)))
			return (MOS_ERROR(bp->iop, EPHIDGET_INVALIDARG,
			  "Specified EncoderIOMode is unsupported by this device."));
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->IOMode = getBridgePacketInt32(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "IOMode");
		break;
	case BP_SETKD:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->kd = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Kd");
		break;
	case BP_SETKI:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->ki = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Ki");
		break;
	case BP_SETKP:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->kp = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "Kp");
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
	case BP_SETTARGETPOSITION:
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->targetPosition = getBridgePacketInt64(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "TargetPosition");
		break;
	case BP_SETDUTYCYCLE:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minVelocityLimit,
		  ch->maxVelocityLimit);
		res = DEVBRIDGEINPUT(phid, bp);
		if (res != EPHIDGET_OK) {
			break;
		}
		ch->velocityLimit = getBridgePacketDouble(bp, 0);
		if (bridgePacketIsFromNet(bp))
			FIRE_PROPERTYCHANGE(ch, "VelocityLimit");
		break;
	case BP_DUTYCYCLECHANGE:
		ch->dutyCycle = getBridgePacketDouble(bp, 0);
		FIRECH(ch, DutyCycleUpdate, ch->dutyCycle);
		break;
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetMotorPositionControllerHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);
	ch = (PhidgetMotorPositionControllerHandle)phid;

	ret = EPHIDGET_OK;


	switch (phid->UCD->uid) {
	case PHIDCHUID_DCC1000_MOTORPOSITIONCONTROLLER_200:
		ch->dataInterval = 100;
		ch->minDataInterval = 20;
		ch->maxDataInterval = 60000;
		ch->currentLimit = 2;
		ch->maxCurrentLimit = 25;
		ch->minCurrentLimit = 2;
		ch->currentRegulatorGain = 10;
		ch->dutyCycle = PUNK_DBL;
		ch->engaged = 0;
		ch->maxCurrentRegulatorGain = 100;
		ch->minCurrentRegulatorGain = 1;
		ch->velocityLimit = 20000;
		ch->minVelocityLimit = 0;
		ch->maxVelocityLimit = 250000;
		ch->maxAcceleration = 10000000;
		ch->minAcceleration = 0.1;
		ch->acceleration = 10000;
		ch->targetPosition = 0;
		ch->position = PUNK_INT64;
		ch->maxPosition = 1000000000000000;
		ch->minPosition = -1000000000000000;
		ch->rescaleFactor = 1;
		ch->fanMode = FAN_MODE_AUTO;
		ch->deadBand = 0;
		ch->kp = 2000;
		ch->kd = 25000;
		ch->ki = 5;
		ch->IOMode = ENCODER_IO_MODE_PUSH_PULL;
		break;
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
		ch->dataInterval = 100;
		ch->minDataInterval = 20;
		ch->maxDataInterval = 60000;
		ch->currentLimit = 1;
		ch->maxCurrentLimit = 3.5;
		ch->minCurrentLimit = 0;
		ch->dutyCycle = PUNK_DBL;
		ch->engaged = 0;
		ch->velocityLimit = 20000;
		ch->minVelocityLimit = 0;
		ch->maxVelocityLimit = 250000;
		ch->maxAcceleration = 10000000;
		ch->minAcceleration = 0.1;
		ch->acceleration = 10000;
		ch->targetPosition = 0;
		ch->position = PUNK_INT64;
		ch->maxPosition = 1000000000000000;
		ch->minPosition = -1000000000000000;
		ch->rescaleFactor = 1;
		ch->deadBand = 0;
		ch->kp = 2000;
		ch->kd = 25000;
		ch->ki = 5;
		break;
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
		ch->dataInterval = 100;
		ch->minDataInterval = 20;
		ch->maxDataInterval = 60000;
		ch->currentLimit = 1;
		ch->maxCurrentLimit = 4;
		ch->minCurrentLimit = 0;
		ch->dutyCycle = PUNK_DBL;
		ch->engaged = 0;
		ch->velocityLimit = 20000;
		ch->minVelocityLimit = 0;
		ch->maxVelocityLimit = 250000;
		ch->maxAcceleration = 10000000;
		ch->minAcceleration = 0.1;
		ch->acceleration = 10000;
		ch->targetPosition = 0;
		ch->position = PUNK_INT64;
		ch->maxPosition = 1000000000000000;
		ch->minPosition = -1000000000000000;
		ch->rescaleFactor = 1;
		ch->deadBand = 0;
		ch->kp = 2000;
		ch->kd = 25000;
		ch->ki = 5;
		break;
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		ch->dataInterval = 100;
		ch->minDataInterval = 20;
		ch->maxDataInterval = 60000;
		ch->dutyCycle = PUNK_DBL;
		ch->engaged = 0;
		ch->velocityLimit = 1000;
		ch->minVelocityLimit = 0;
		ch->maxVelocityLimit = 10000;
		ch->maxAcceleration = 100000;
		ch->minAcceleration = 0.1;
		ch->acceleration = 100;
		ch->targetPosition = 0;
		ch->position = PUNK_INT64;
		ch->maxPosition = 1000000000000000;
		ch->minPosition = -1000000000000000;
		ch->rescaleFactor = 1;
		ch->deadBand = 0;
		ch->kp = 20000;
		ch->kd = 40000;
		ch->ki = 2;
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
	PhidgetMotorPositionControllerHandle ch;
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ch = (PhidgetMotorPositionControllerHandle)phid;
	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_DCC1000_MOTORPOSITIONCONTROLLER_200:
		break;
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCURRENTLIMIT, NULL, NULL, "%g", ch->currentLimit);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETENGAGED, NULL, NULL, "%d", ch->engaged);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETDUTYCYCLE, NULL, NULL, "%g", ch->velocityLimit);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETACCELERATION, NULL, NULL, "%g", ch->acceleration);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETTARGETPOSITION, NULL, NULL, "%l", ch->targetPosition);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETDEADBAND, NULL, NULL, "%u", ch->deadBand);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKP, NULL, NULL, "%g", ch->kp);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKD, NULL, NULL, "%g", ch->kd);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKI, NULL, NULL, "%g", ch->ki);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETCURRENTLIMIT, NULL, NULL, "%g", ch->currentLimit);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETENGAGED, NULL, NULL, "%d", ch->engaged);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETDUTYCYCLE, NULL, NULL, "%g", ch->velocityLimit);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETACCELERATION, NULL, NULL, "%g", ch->acceleration);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETTARGETPOSITION, NULL, NULL, "%l", ch->targetPosition);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETDEADBAND, NULL, NULL, "%u", ch->deadBand);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKP, NULL, NULL, "%g", ch->kp);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKD, NULL, NULL, "%g", ch->kd);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKI, NULL, NULL, "%g", ch->ki);
		if (ret != EPHIDGET_OK) {
			break;
		}
		break;
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		ret = bridgeSendToDevice(phid, BP_SETDATAINTERVAL, NULL, NULL, "%u", ch->dataInterval);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETENGAGED, NULL, NULL, "%d", ch->engaged);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETDUTYCYCLE, NULL, NULL, "%g", ch->velocityLimit);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETACCELERATION, NULL, NULL, "%g", ch->acceleration);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETTARGETPOSITION, NULL, NULL, "%l", ch->targetPosition);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETDEADBAND, NULL, NULL, "%u", ch->deadBand);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKP, NULL, NULL, "%g", ch->kp);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKD, NULL, NULL, "%g", ch->kd);
		if (ret != EPHIDGET_OK) {
			break;
		}
		ret = bridgeSendToDevice(phid, BP_SETKI, NULL, NULL, "%g", ch->ki);
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
	PhidgetMotorPositionControllerHandle ch;

	ch = (PhidgetMotorPositionControllerHandle)phid;

	if(ch->dutyCycle != PUNK_DBL)
		FIRECH(ch, DutyCycleUpdate, ch->dutyCycle);

}

static void CCONV
PhidgetMotorPositionController_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetMotorPositionController));
}

API_PRETURN
PhidgetMotorPositionController_create(PhidgetMotorPositionControllerHandle *phidp) {

	CHANNELCREATE_BODY(MotorPositionController, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_delete(PhidgetMotorPositionControllerHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}

API_PRETURN
PhidgetMotorPositionController_setCurrentLimit(PhidgetMotorPositionControllerHandle ch,
  double currentLimit) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETCURRENTLIMIT, NULL, NULL, "%g",
	  currentLimit));
}

API_PRETURN
PhidgetMotorPositionController_getCurrentLimit(PhidgetMotorPositionControllerHandle ch,
  double *currentLimit) {

	TESTPTR_PR(ch);
	TESTPTR_PR(currentLimit);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*currentLimit = ch->currentLimit;
	if (ch->currentLimit == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getMinCurrentLimit(PhidgetMotorPositionControllerHandle ch,
  double *minCurrentLimit) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minCurrentLimit);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*minCurrentLimit = ch->minCurrentLimit;
	if (ch->minCurrentLimit == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getMaxCurrentLimit(PhidgetMotorPositionControllerHandle ch,
  double *maxCurrentLimit) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxCurrentLimit);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*maxCurrentLimit = ch->maxCurrentLimit;
	if (ch->maxCurrentLimit == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setCurrentRegulatorGain(PhidgetMotorPositionControllerHandle ch,
  double currentRegulatorGain) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETCURRENTREGULATORGAIN, NULL, NULL, "%g",
	  currentRegulatorGain));
}

API_PRETURN
PhidgetMotorPositionController_getCurrentRegulatorGain(PhidgetMotorPositionControllerHandle ch,
  double *currentRegulatorGain) {

	TESTPTR_PR(ch);
	TESTPTR_PR(currentRegulatorGain);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*currentRegulatorGain = ch->currentRegulatorGain;
	if (ch->currentRegulatorGain == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getMinCurrentRegulatorGain(PhidgetMotorPositionControllerHandle ch,
  double *minCurrentRegulatorGain) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minCurrentRegulatorGain);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*minCurrentRegulatorGain = ch->minCurrentRegulatorGain;
	if (ch->minCurrentRegulatorGain == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getMaxCurrentRegulatorGain(PhidgetMotorPositionControllerHandle ch,
  double *maxCurrentRegulatorGain) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxCurrentRegulatorGain);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*maxCurrentRegulatorGain = ch->maxCurrentRegulatorGain;
	if (ch->maxCurrentRegulatorGain == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setDataInterval(PhidgetMotorPositionControllerHandle ch,
  uint32_t dataInterval) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETDATAINTERVAL, NULL, NULL, "%u",
	  dataInterval));
}

API_PRETURN
PhidgetMotorPositionController_getDataInterval(PhidgetMotorPositionControllerHandle ch,
  uint32_t *dataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*dataInterval = ch->dataInterval;
	if (ch->dataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getMinDataInterval(PhidgetMotorPositionControllerHandle ch,
  uint32_t *minDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*minDataInterval = ch->minDataInterval;
	if (ch->minDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getMaxDataInterval(PhidgetMotorPositionControllerHandle ch,
  uint32_t *maxDataInterval) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxDataInterval);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*maxDataInterval = ch->maxDataInterval;
	if (ch->maxDataInterval == (uint32_t)PUNK_UINT32)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getDutyCycle(PhidgetMotorPositionControllerHandle ch,
  double *dutyCycle) {

	TESTPTR_PR(ch);
	TESTPTR_PR(dutyCycle);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*dutyCycle = ch->dutyCycle;
	if (ch->dutyCycle == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setEngaged(PhidgetMotorPositionControllerHandle ch, int engaged) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETENGAGED, NULL, NULL, "%d", engaged));
}

API_PRETURN
PhidgetMotorPositionController_getEngaged(PhidgetMotorPositionControllerHandle ch, int *engaged) {

	TESTPTR_PR(ch);
	TESTPTR_PR(engaged);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*engaged = ch->engaged;
	if (ch->engaged == (int)PUNK_BOOL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setFanMode(PhidgetMotorPositionControllerHandle ch,
  Phidget_FanMode fanMode) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETFANMODE, NULL, NULL, "%d", fanMode));
}

API_PRETURN
PhidgetMotorPositionController_getFanMode(PhidgetMotorPositionControllerHandle ch,
  Phidget_FanMode *fanMode) {

	TESTPTR_PR(ch);
	TESTPTR_PR(fanMode);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*fanMode = ch->fanMode;
	if (ch->fanMode == (Phidget_FanMode)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setIOMode(PhidgetMotorPositionControllerHandle ch,
  Phidget_EncoderIOMode IOMode) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETIOMODE, NULL, NULL, "%d", IOMode));
}

API_PRETURN
PhidgetMotorPositionController_getIOMode(PhidgetMotorPositionControllerHandle ch,
  Phidget_EncoderIOMode *IOMode) {

	TESTPTR_PR(ch);
	TESTPTR_PR(IOMode);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	switch (ch->phid.UCD->uid) {
	case PHIDCHUID_DCC1001_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1002_MOTORPOSITIONCONTROLLER_100:
	case PHIDCHUID_DCC1100_MOTORPOSITIONCONTROLLER_100:
		return (PHID_RETURN(EPHIDGET_UNSUPPORTED));
	default:
		break;
	}

	*IOMode = ch->IOMode;
	if (ch->IOMode == (Phidget_EncoderIOMode)PUNK_ENUM)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setKd(PhidgetMotorPositionControllerHandle ch, double kd) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETKD, NULL, NULL, "%g", kd));
}

API_PRETURN
PhidgetMotorPositionController_getKd(PhidgetMotorPositionControllerHandle ch, double *kd) {

	TESTPTR_PR(ch);
	TESTPTR_PR(kd);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*kd = ch->kd;
	if (ch->kd == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setKi(PhidgetMotorPositionControllerHandle ch, double ki) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETKI, NULL, NULL, "%g", ki));
}

API_PRETURN
PhidgetMotorPositionController_getKi(PhidgetMotorPositionControllerHandle ch, double *ki) {

	TESTPTR_PR(ch);
	TESTPTR_PR(ki);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*ki = ch->ki;
	if (ch->ki == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setKp(PhidgetMotorPositionControllerHandle ch, double kp) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETKP, NULL, NULL, "%g", kp));
}

API_PRETURN
PhidgetMotorPositionController_getKp(PhidgetMotorPositionControllerHandle ch, double *kp) {

	TESTPTR_PR(ch);
	TESTPTR_PR(kp);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*kp = ch->kp;
	if (ch->kp == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_getRescaleFactor(PhidgetMotorPositionControllerHandle ch,
  double *rescaleFactor) {

	TESTPTR_PR(ch);
	TESTPTR_PR(rescaleFactor);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);
	TESTATTACHED_PR(ch);

	*rescaleFactor = ch->rescaleFactor;
	if (ch->rescaleFactor == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setOnDutyCycleUpdateHandler(PhidgetMotorPositionControllerHandle ch,
  PhidgetMotorPositionController_OnDutyCycleUpdateCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);

	ch->DutyCycleUpdate = fptr;
	ch->DutyCycleUpdateCtx = ctx;

	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMotorPositionController_setOnPositionChangeHandler(PhidgetMotorPositionControllerHandle ch,
  PhidgetMotorPositionController_OnPositionChangeCallback fptr, void *ctx) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_MOTORPOSITIONCONTROLLER);

	ch->PositionChange = fptr;
	ch->PositionChangeCtx = ctx;

	return (EPHIDGET_OK);
}
