/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_signals_dcl.h   data type, prototype declarations: true/virtual signaling devices.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_SIGNALS_DCL_H
#define __SSIO_SIGNALS_DCL_H

#ifdef  SSIO_OPTIN_SIGNALS

typedef     Bits8_t     SignalNumber_t;

typedef struct ssIO_Signal_Desc_s
{
    unsigned char   LogicalAddress;
    unsigned char   RegValue;
}   ssIO_Signal_Desc_t;

typedef ssIO_Signal_Desc_t    ssIO_Signal_t;
typedef ssIO_Signal_Desc_t   *ssIO_pSignaler_t;

#endif  // SSIO_OPTIN_SIGNALS

#endif  // __SSIO_SIGNALS_DCL_H

