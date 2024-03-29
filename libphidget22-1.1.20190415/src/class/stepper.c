/* Generated: Tue Feb 02 2016 10:17:13 GMT-0700 (Mountain Standard Time) */
/* Will not be regenerated. */

#include "phidgetbase.h"
#include "class/stepper.gen.h"
#include "class/stepper.gen.c"

static void
PhidgetStepper_errorHandler(PhidgetChannelHandle phid, Phidget_ErrorEventCode code) {}

static PhidgetReturnCode CCONV
PhidgetStepper_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	return (_setStatus(phid, bp));
}

static PhidgetReturnCode CCONV
PhidgetStepper_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {
	return (_getStatus(phid, bp));
}

static PhidgetReturnCode CCONV
PhidgetStepper_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetStepperHandle ch;
	PhidgetReturnCode ret;

	ch = (PhidgetStepperHandle)phid;

	ret = _initAfterOpen(phid);
	if (ret != EPHIDGET_OK)
		return (ret);

	// Make sure these are always readable from the user
	//  We have sent the reset packet, so we know these are all 0, even if we haven't heard from firmware
	//  Some devices NEVER send back status if these aren't changed from 0, which violates POLA in the user API
	if (ch->position == PUNK_INT64)
		ch->position = 0;
	if (ch->velocity == PUNK_DBL)
		ch->velocity = 0;
	if (ch->isMoving == PUNK_BOOL)
		ch->isMoving = PFALSE;

	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
PhidgetStepper_setDefaults(PhidgetChannelHandle phid) {
	return (_setDefaults(phid));
}

static PhidgetReturnCode
PhidgetStepper_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetStepperHandle ch;
	PhidgetReturnCode res;

	ch = (PhidgetStepperHandle)phid;

	switch (bp->vpkt) {
	case BP_POSITIONCHANGE:
		ch->position = getBridgePacketInt64(bp, 0);
		FIRECH(ch, PositionChange, ((ch->position + ch->positionOffset) * ch->rescaleFactor));
		res = EPHIDGET_OK;
		break;

	case BP_VELOCITYCHANGE:
		ch->velocity = getBridgePacketDouble(bp, 0);
		FIRECH(ch, VelocityChange, (ch->velocity * ch->rescaleFactor));
		res = EPHIDGET_OK;
		break;

	case BP_SETCONTROLMODE:
		res = _bridgeInput(phid, bp);
		if (res == EPHIDGET_OK) {
			switch (ch->controlMode) {
			case CONTROL_MODE_STEP:
				ch->minVelocityLimit = 0;
				break;
			case CONTROL_MODE_RUN:
				ch->minVelocityLimit = -ch->maxVelocityLimit;
				break;
			}
		}
		break;

	case BP_SETHOLDINGCURRENTLIMIT:
		TESTRANGE_IOP(bp->iop, "%lf", getBridgePacketDouble(bp, 0), ch->minCurrentLimit, ch->maxCurrentLimit);
		res = _bridgeInput(phid, bp);
		break;

	case BP_STOPPED:
		ch->isMoving = PFALSE;
		res = _bridgeInput(phid, bp);
		break;

	case BP_SETENGAGED:
	case BP_SETTARGETPOSITION:
	case BP_SETVELOCITYLIMIT:
		res = _bridgeInput(phid, bp);
		if (res == EPHIDGET_OK) {
			if (ch->engaged == PTRUE && ch->velocityLimit != 0 && ch->position != ch->targetPosition)
				ch->isMoving = PTRUE;
		}
		break;

	default:
		res = _bridgeInput(phid, bp);
		break;
	}

	return (res);
}

static void
PhidgetStepper_fireInitialEvents(PhidgetChannelHandle phid) {
	_fireInitialEvents(phid);
}

API_PRETURN
PhidgetStepper_getMaxPosition(PhidgetStepperHandle ch, double *maxPosition) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxPosition);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*maxPosition = (ch->maxPosition + ch->positionOffset) * ch->rescaleFactor;
	if (ch->maxPosition == PUNK_INT64)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getMinVelocityLimit(PhidgetStepperHandle ch, double *minVelocityLimit) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minVelocityLimit);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*minVelocityLimit = ch->minVelocityLimit * ch->rescaleFactor;
	if (ch->minVelocityLimit == (double)PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getMaxVelocityLimit(PhidgetStepperHandle ch, double *maxVelocityLimit) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxVelocityLimit);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*maxVelocityLimit = ch->maxVelocityLimit * ch->rescaleFactor;
	if (ch->maxVelocityLimit == PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getMaxAcceleration(PhidgetStepperHandle ch, double *maxAcceleration) {

	TESTPTR_PR(ch);
	TESTPTR_PR(maxAcceleration);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*maxAcceleration = ch->maxAcceleration * ch->rescaleFactor;
	if (ch->maxAcceleration == PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getMinPosition(PhidgetStepperHandle ch, double *minPosition) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minPosition);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*minPosition = (ch->minPosition + ch->positionOffset) * ch->rescaleFactor;
	if (ch->minPosition == PUNK_INT64)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getMinAcceleration(PhidgetStepperHandle ch, double *minAcceleration) {

	TESTPTR_PR(ch);
	TESTPTR_PR(minAcceleration);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*minAcceleration = ch->minAcceleration * ch->rescaleFactor;
	if (ch->minAcceleration == PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getPosition(PhidgetStepperHandle ch, double *position) {

	TESTPTR_PR(ch);
	TESTPTR_PR(position);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*position = (ch->position + ch->positionOffset) * ch->rescaleFactor;
	if (ch->position == PUNK_INT64)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_setTargetPosition(PhidgetStepperHandle ch, double targetPosition) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETTARGETPOSITION, NULL, NULL, "%l",
		roundl((targetPosition / ch->rescaleFactor) - ch->positionOffset)));
}


API_VRETURN
PhidgetStepper_setTargetPosition_async(PhidgetStepperHandle ch, double targetPosition,
	Phidget_AsyncCallback fptr, void *ctx) {
	PhidgetReturnCode res;

	if (ch == NULL) {
		if (fptr) fptr((PhidgetHandle)ch, ctx, EPHIDGET_INVALIDARG);
		return;
	}
	if (ch->phid.class != PHIDCHCLASS_STEPPER) {
		if (fptr) fptr((PhidgetHandle)ch, ctx, EPHIDGET_WRONGDEVICE);
		return;
	}
	if (!ISATTACHED(ch)) {
		if (fptr) fptr((PhidgetHandle)ch, ctx, EPHIDGET_NOTATTACHED);
		return;
	}

	res = bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETTARGETPOSITION, fptr, ctx, "%l", roundl((targetPosition / ch->rescaleFactor) - ch->positionOffset));
	if (res != EPHIDGET_OK && fptr != NULL)
		fptr((PhidgetHandle)ch, ctx, res);
}

API_PRETURN
PhidgetStepper_getTargetPosition(PhidgetStepperHandle ch, double *targetPosition) {

	TESTPTR_PR(ch);
	TESTPTR_PR(targetPosition);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*targetPosition = (ch->targetPosition + ch->positionOffset) * ch->rescaleFactor;
	if (ch->targetPosition == PUNK_INT64)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_setVelocityLimit(PhidgetStepperHandle ch, double velocityLimit) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETVELOCITYLIMIT, NULL, NULL, "%g",
		(velocityLimit / ch->rescaleFactor)));
}

API_PRETURN
PhidgetStepper_getVelocityLimit(PhidgetStepperHandle ch, double *velocityLimit) {

	TESTPTR_PR(ch);
	TESTPTR_PR(velocityLimit);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*velocityLimit = ch->velocityLimit * ch->rescaleFactor;
	if (ch->velocityLimit == PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_getVelocity(PhidgetStepperHandle ch, double *velocity) {

	TESTPTR_PR(ch);
	TESTPTR_PR(velocity);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*velocity = ch->velocity * ch->rescaleFactor;
	if (ch->velocity == PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_setAcceleration(PhidgetStepperHandle ch, double acceleration) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	return (bridgeSendToDevice((PhidgetChannelHandle)ch, BP_SETACCELERATION, NULL, NULL, "%g",
		(acceleration / ch->rescaleFactor)));
}

API_PRETURN
PhidgetStepper_getAcceleration(PhidgetStepperHandle ch, double *acceleration) {

	TESTPTR_PR(ch);
	TESTPTR_PR(acceleration);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	*acceleration = ch->acceleration * ch->rescaleFactor;
	if (ch->acceleration == PUNK_DBL)
		return (PHID_RETURN(EPHIDGET_UNKNOWNVAL));
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_setRescaleFactor(PhidgetStepperHandle ch, double rescaleFactor) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	if (rescaleFactor == 0)
		return (PHID_RETURN(EPHIDGET_INVALIDARG));
	ch->rescaleFactor = rescaleFactor;
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetStepper_addPositionOffset(PhidgetStepperHandle ch, double positionOffset) {

	TESTPTR_PR(ch);
	TESTCHANNELCLASS_PR(ch, PHIDCHCLASS_STEPPER);
	TESTATTACHED_PR(ch);

	ch->positionOffset += roundl(positionOffset / ch->rescaleFactor);
	return (EPHIDGET_OK);
}
