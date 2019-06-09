/*
 * This file is part of libphidget22
 *
 * Copyright 2015 Phidgets Inc <patrick@phidgets.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/>
 */

#ifndef __CPHIDGETANALOGDEVICE
#define __CPHIDGETANALOGDEVICE

typedef struct _PhidgetAnalogDevice *PhidgetAnalogDeviceHandle;
PhidgetReturnCode PhidgetAnalogDevice_create(PhidgetAnalogDeviceHandle *phid);

#define ANALOG_MAXOUTPUTS 4

struct _PhidgetAnalogDevice {
#undef devChannelCnts
#define devChannelCnts	phid.deviceInfo.UDD->channelCnts.analog
	PhidgetDevice phid;

	double voltage[ANALOG_MAXOUTPUTS];
	uint8_t enabled[ANALOG_MAXOUTPUTS];

} typedef PhidgetAnalogDeviceInfo;

#endif
