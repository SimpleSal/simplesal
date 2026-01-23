/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_pins_dcl.h      data type, prototype declarations:  interact with true/virtual pins.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_PINS_DCL_H
#define __SSIO_PINS_DCL_H

// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_PINS

// -------------------------------------------------------------------------------------------------
// A "pin" is an electrical connection between the mesa and the outside world.  Mesa attributes are
// far too complex for SimpleSal, beyond the ideas "receive the value the mesa perceives from the
// outside world's assertion" and "cause the pin to (try to) assert a value to the outside world".
// This complexity extends beyond the definition of input or output; electronic characteristics do
// not matter here at the SimpleSal level of control of reality and emulation of reality.  The key
// point is: the electronic characteristics can only be described by a vendor's official data sheet.
// -------------------------------------------------------------------------------------------------
#define     ssIO_PinNumber_None ((PinNumber_t) 0xFF)    // if there is a collision on your mesa, change.

typedef struct ssIO_Pin_Desc_s
{
    PinNumber_t mesaPinNumber;  // unique value for a physical or mapped Pin on this mesa and Host
    PinMode_t   mesaPinMode;    // attributes that are local to the mesa, as defined by the mesa
    Bits8_t     Value;          // Value of the Pin depends on the current MAC True/Virt state:
                                //   True: a read- and write-through cache of physical pin accesses
                                //   Virt: the value last stored into the memory emulating a pin
}   ssIO_Pin_Desc_t, *ssIO_pPin_Desc_t;

typedef ssIO_Pin_Desc_t     ssIO_Pin_t;
typedef ssIO_pPin_Desc_t    ssIO_pPin_t;
#define ssIO_pPinNull       ((ssIO_pPin_t) NULL)

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void        ssIO_Pin_default        (ssIO_pPin_t    pPin);
void        ssIO_Pin_undefine       (ssIO_pPin_t    pPin);

LogicalPinValue_t   ssIO_DigitalRead  (MAC_ID_t MAC_ID);

void                ssIO_DigitalWrite (MAC_ID_t MAC_ID, LogicalPinValue_t PinValue);

void                ssIO_PinMode      (MAC_ID_t MAC_ID, PinMode_t mesaPinMode);

#endif  // SSIO_OPTIN_PINS

#endif  // __SSIO_PINS_DCL_H

