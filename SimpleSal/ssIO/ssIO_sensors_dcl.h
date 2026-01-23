/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_sensors_dcl.h   data type, prototype declarations: true/virtual sensor devices.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_SENSORS_DCL_H
#define __SSIO_SENSORS_DCL_H

#ifdef  SSIO_OPTIN_SENSORS

typedef     Bits8_t     SensorNumber_t;

typedef struct ssIO_Sensor_Desc_s
{
    Bits8_t         LogicalAddress;
    Bits8_t         RegValue;
}   ssIO_Sensor_Desc_t;

typedef ssIO_Sensor_Desc_t    ssIO_Sensor_t;
typedef ssIO_Sensor_Desc_t   *ssIO_pSensor_t;

typedef struct virtual_IntController_s
{
    Bits8_t        Indication;
    Bits8_t        Enabled;
    Bits8_t        Acknowledge;

}   virtual_IntController_t;

typedef virtual_IntController_t    ssIO_VirtualInts_t;
typedef virtual_IntController_t   *ssIO_pVirtualInts_t;

#endif  // SSIO_OPTIN_SENSORS

#endif  // __SSIO_SENSORS_DCL_H

