#ifndef _POWERGUARD_H_
#define _POWERGUARD_H_

/* Generated by WriteClassHeaderVisitor: Mon Apr 15 2019 15:38:10 GMT-0600 (MDT) */

typedef struct _PhidgetPowerGuard *PhidgetPowerGuardHandle;

/* Methods */
API_PRETURN_HDR PhidgetPowerGuard_create(PhidgetPowerGuardHandle *ch);
API_PRETURN_HDR PhidgetPowerGuard_delete(PhidgetPowerGuardHandle *ch);

/* Properties */
API_PRETURN_HDR PhidgetPowerGuard_setFanMode(PhidgetPowerGuardHandle ch, Phidget_FanMode fanMode);
API_PRETURN_HDR PhidgetPowerGuard_getFanMode(PhidgetPowerGuardHandle ch, Phidget_FanMode *fanMode);
API_PRETURN_HDR PhidgetPowerGuard_setOverVoltage(PhidgetPowerGuardHandle ch, double overVoltage);
API_PRETURN_HDR PhidgetPowerGuard_getOverVoltage(PhidgetPowerGuardHandle ch, double *overVoltage);
API_PRETURN_HDR PhidgetPowerGuard_getMinOverVoltage(PhidgetPowerGuardHandle ch, double *minOverVoltage);
API_PRETURN_HDR PhidgetPowerGuard_getMaxOverVoltage(PhidgetPowerGuardHandle ch, double *maxOverVoltage);
API_PRETURN_HDR PhidgetPowerGuard_setPowerEnabled(PhidgetPowerGuardHandle ch, int powerEnabled);
API_PRETURN_HDR PhidgetPowerGuard_getPowerEnabled(PhidgetPowerGuardHandle ch, int *powerEnabled);

/* Events */

#endif /* _POWERGUARD_H_ */
