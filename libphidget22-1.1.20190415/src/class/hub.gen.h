#ifndef _HUB_H_
#define _HUB_H_

/* Generated by WriteClassHeaderVisitor: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

typedef struct _PhidgetHub *PhidgetHubHandle;

/* Methods */
API_PRETURN_HDR PhidgetHub_create(PhidgetHubHandle *ch);
API_PRETURN_HDR PhidgetHub_delete(PhidgetHubHandle *ch);
#ifdef INCLUDE_PRIVATE
API_PRETURN_HDR PhidgetHub_setADCCalibrationValues(PhidgetHubHandle ch, double voltageInputGain[6],
  double voltageRatioGain[6]);
#endif
#ifdef INCLUDE_PRIVATE
API_PRETURN_HDR PhidgetHub_setFirmwareUpgradeFlag(PhidgetHubHandle ch, int port, uint32_t timeout);
#endif
#ifdef INCLUDE_PRIVATE
API_PRETURN_HDR PhidgetHub_setPortMode(PhidgetHubHandle ch, int port, PhidgetHub_PortMode mode);
#endif
API_PRETURN_HDR PhidgetHub_setPortPower(PhidgetHubHandle ch, int port, int state);

/* Properties */

/* Events */

#endif /* _HUB_H_ */
