/* Generated: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

#include "device/meshdongledevice.h"
static void CCONV PhidgetMeshDongle_errorHandler(PhidgetChannelHandle ch, Phidget_ErrorEventCode code);
static PhidgetReturnCode CCONV PhidgetMeshDongle_bridgeInput(PhidgetChannelHandle phid,
  BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetMeshDongle_setStatus(PhidgetChannelHandle phid, BridgePacket *bp);
static PhidgetReturnCode CCONV PhidgetMeshDongle_getStatus(PhidgetChannelHandle phid,
  BridgePacket **bp);
static PhidgetReturnCode CCONV PhidgetMeshDongle_initAfterOpen(PhidgetChannelHandle phid);
static PhidgetReturnCode CCONV PhidgetMeshDongle_setDefaults(PhidgetChannelHandle phid);
static void CCONV PhidgetMeshDongle_fireInitialEvents(PhidgetChannelHandle phid);

struct _PhidgetMeshDongle {
	struct _PhidgetChannel phid;
};

static PhidgetReturnCode CCONV
_setStatus(PhidgetChannelHandle phid, BridgePacket *bp) {
	int version;

	version = getBridgePacketUInt32ByName(bp, "_class_version_");
	if (version != 0) {
		logerr("%"PRIphid": bad version %d != 0", phid, version);
		return (EPHIDGET_BADVERSION);
	}


	return (EPHIDGET_OK);
}

static PhidgetReturnCode CCONV
_getStatus(PhidgetChannelHandle phid, BridgePacket **bp) {

	return (createBridgePacket(bp, 0, "_class_version_=%u"
	  ,0 /* class version */
	));
}

static PhidgetReturnCode CCONV
_bridgeInput(PhidgetChannelHandle phid, BridgePacket *bp) {
	PhidgetReturnCode res;

	res = EPHIDGET_OK;

	switch (bp->vpkt) {
	default:
		logerr("%"PRIphid": unsupported bridge packet:0x%x", phid, bp->vpkt);
		res = EPHIDGET_UNSUPPORTED;
	}

	return (res);
}

static PhidgetReturnCode CCONV
_initAfterOpen(PhidgetChannelHandle phid) {
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ret = EPHIDGET_OK;


	switch (phid->UCD->uid) {
	case PHIDCHUID_HUB0002_MESHDONGLE_100:
		break;
	default:
		MOS_PANIC("Unsupported Channel");
	}


	return (ret);
}

static PhidgetReturnCode CCONV
_setDefaults(PhidgetChannelHandle phid) {
	PhidgetReturnCode ret;

	TESTPTR(phid);

	ret = EPHIDGET_OK;

	switch (phid->UCD->uid) {
	case PHIDCHUID_HUB0002_MESHDONGLE_100:
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
PhidgetMeshDongle_free(PhidgetChannelHandle *ch) {

	mos_free(*ch, sizeof (struct _PhidgetMeshDongle));
}

API_PRETURN
PhidgetMeshDongle_create(PhidgetMeshDongleHandle *phidp) {

	CHANNELCREATE_BODY(MeshDongle, PHIDCHCLASS_MESHDONGLE);
	return (EPHIDGET_OK);
}

API_PRETURN
PhidgetMeshDongle_delete(PhidgetMeshDongleHandle *phidp) {

	return (Phidget_delete((PhidgetHandle *)phidp));
}
