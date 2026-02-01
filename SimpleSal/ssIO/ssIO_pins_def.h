/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_pins_def.h      data and software definitions: interact with true/virtual pins.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_PINS_DEF_H
#define __SSIO_PINS_DEF_H

#ifdef  SSIO_OPTIN_PINS

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Noise about the software style and organization of ssIO with respect to the rest of SimpleSal:
//    The intention of ssIO is to be independent of the Time, Event, and Agency features of ssTEA.
//    Independence propagates out to the documentation extrusion style of ssTEA; all documents in
//      the form of explanatory notes for ssIO are contained in the ssIO files themselves.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void    ssIO_Pin_undefine (ssIO_pPin_t    pPin)
{
    ssIO_Pin_default (pPin);
}   //  ssIO_Pin_undefine

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_DEBUG_PINS_MODE

#define MAC_ID_none (99)

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void ShowPinMode (pAsciiA_t pDesc, int MAC_ID, PinNumber_t mesaPinNumber, int mesaPinMode)
{
    mesa_uiOp_emit_pAsciiA (pDesc);

    if (MAC_ID != MAC_ID_none)
    {
        mesa_uiOp_emit_qAsciiA (" for MrB<");
        mesa_uiOp_emit_Int_9 (MAC_ID);
        mesa_uiOp_emit_qAsciiA (">");
    }
    mesa_uiOp_emit_qAsciiA (" for Pin <");
    mesa_uiOp_emit_Int_9 (mesaPinNumber);
    mesa_uiOp_emit_qAsciiA ("> to <");
    switch (mesaPinMode)
    {
        case mesaPinMode_INPUT_NOPULLUP :   mesa_uiOp_emit_qAsciiA ("input, no pullup");    break;
        case mesaPinMode_INPUT_PULLEDUP :   mesa_uiOp_emit_qAsciiA ("input, pulled up");    break;
        case mesaPinMode_OUTPUT         :   mesa_uiOp_emit_qAsciiA ("output");              break;
        default                         :   mesa_uiOp_emit_qAsciiA ("pin mode error");      break;
    }   // switch
    mesa_uiOp_emit_qAsciiA (">");
    mesa_uiOp_emit_newline ();
}   // ShowPinMode
#else   // not SSIO_OPTIN_DEBUG_PINS_MODE
// make the references to ShowPinMode resolve to nothing
#define ShowPinMode(p,ID,P,M)
#endif  // SSIO_OPTIN_DEBUG_PINS_MODE

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssIO_PinMode (MAC_ID_t MAC_ID, PinMode_t mesaPinMode)
{
    ssIO_pMAC_t     pMAC;

    pMAC = ssIO_dbOp_pMAC_This_MAC_ID (MAC_ID);
    if (pMAC == ssIO_pMACNull)
    {
        ShowPinMode (S("pMAC not found, setting Pin Mode directly"), MAC_ID_none,
              (((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift),  mesaPinMode);
        // if not a pin defined in ssIO by a pMAC_t, directly set via HostOS
        pinMode ((((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift), mesaPinMode);
        return;
    }   // no pMAC

#ifdef  SSIO_OPTIN_CHECK_ACCESS
    // virtualization of Pins is supported, virtualized Pin exists, compare action to ability
    if (!ssIO_MAC_Pin_UseCheckMode (pMAC, mesaPinMode))
    {
        ShowPinMode (S("PinMode Use Check failure"), MAC_ID_none,
            (((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift), mesaPinMode);
        return;
    }
#endif  //  SSIO_OPTIN_CHECK_ACCESS

    pMAC->u.Pin.mesaPinNumber = (((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift);
    pMAC->u.Pin.mesaPinMode = mesaPinMode;

    if (pMAC->TrueState == ssIO_TrueNow)
    {
        if (pMAC->MappedState == ssIO_Direct)
        {
            pinMode (pMAC->u.Pin.mesaPinNumber, pMAC->u.Pin.mesaPinMode);
            ShowPinMode (S("set PinMode true direct "), MAC_ID,
                    pMAC->u.Pin.mesaPinNumber, pMAC->u.Pin.mesaPinMode);
        }
        else
        {   // must be ssIO_mapped which displays the shape assigned to the mapped PinNumber
#ifdef MESA_OPTIN_LED_Matrix_Support
            ShowPinMode (S("set PinMode true mapped"), MAC_ID,
                    pMAC->u.Pin.mesaPinNumber, pMAC->u.Pin.mesaPinMode);
#else // not MESA_OPTIN_LED_Matrix_Support
            mesa_uiOp_qAsciiA ("mode not set, true mapped pin mode requires LEDM");
            mesa_uiOp_newline ();
#endif // MESA_OPTIN_LED_Matrix_Support
        }   // true mapped
    }   // true
    else
    {   // must be ssIO_VirtNow, which has no mode action similar to true GPIO configuration, right?
        ShowPinMode (S("set PinMode virtual direct "),  MAC_ID,
                pMAC->u.Pin.mesaPinNumber, pMAC->u.Pin.mesaPinMode);
    }
}   // ssIO_PinMode
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_DEBUG_PINS_WRITE
void ShowPinWrite (pAsciiA_t pDesc, PinNumber_t mesaPinNumber, LogicalPinValue_t PinValue)
{
    mesa_uiOp_emit_qAsciiA (pDesc);
    mesa_uiOp_emit_qAsciiA (" <");
    mesa_uiOp_emit_Hex_32bits (mesaPinNumber);
    mesa_uiOp_emit_qAsciiA ("> with value <");
    mesa_uiOp_emit_Int_9 ((Bits8_t) PinValue);
    mesa_uiOp_emit_qAsciiA (">");
    mesa_uiOp_emit_newline ();
}   // ShowPinWrite
#else   // not SSIO_OPTIN_DEBUG_PINS_WRITE
#define ShowPinWrite(p,ID,V)
#endif  // SSIO_OPTIN_DEBUG_PINS_WRITE
// -------------------------------------------------------------------------------------------------
void  ssIO_DigitalWrite (MAC_ID_t MAC_ID, LogicalPinValue_t PinValue)
{
    ssIO_pMAC_t     pMAC;
    PinNumber_t     mesaPinNumber;

    ShowPinWrite (S("digital write to Pin "),
            (((PinNumber_t) MAC_ID & MACnumber_Mask) >> MACnumber_Shift), PinValue);

    // if not found in virtualizations, ID is true mesa Pin #.
    pMAC = ssIO_dbOp_pMAC_This_MAC_ID (MAC_ID);
    if (pMAC == ssIO_pMACNull)
    {
        ShowPinWrite (S("  true direct not MAC configured "),
            (((PinNumber_t) MAC_ID & MACnumber_Mask) >> MACnumber_Shift), PinValue);
        // if not a pin defined in ssIO by a pMAC_t, directly set via HostOS.
        // Important to note that the operation occurs without checking any values for validity.
        digitalWrite ((((PinNumber_t) MAC_ID & MACnumber_Mask) >> MACnumber_Shift), PinValue);
        return;
    }

    // ---------------------------------------------------------------------------------------------
    // virtualization supported, virtualized Pin exists, action in ability, clarify the target.
    // ---------------------------------------------------------------------------------------------
    MAC_ID = (pMAC->MAC_ID & MrB_Mask ) >> MrB_Shift;
    mesaPinNumber = (((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift);
    pMAC->u.Pin.Value = PinValue;

#ifdef  SSIO_OPTIN_CHECK_ACCESS
    // virtualization of Pins is supported, virtualized Pin exists, compare action to ability
    if (!ssIO_MAC_Pin_UseCheckWrite (pMAC, PinValue))
    {
        ShowPinWrite (S("write check error "), mesaPinNumber, PinValue);
        return;
    }
#endif  //  SSIO_OPTIN_CHECK_ACCESS

    // ---------------------------------------------------------------------------------------------
    // By design, MAC Pins have a virtual value that caches the physical value on read/write.
    // This is the sequence: virtual or physical, set cached to value; if direct, set physical.
    // ---------------------------------------------------------------------------------------------
    if (pMAC->TrueState != ssIO_VirtNow)
    {
#ifdef MESA_OPTIN_LED_Matrix_Support
        if (pMAC->MappedState == ssIO_Mapped)
        {
            ShowPinWrite (S("mapped"), mesaPinNumber, PinValue);
            LEDM_DigitalWrite_MrB (MAC_ID, mesaPinNumber, PinValue);
        }   // mapped
        else
#endif // MESA_OPTIN_LED_Matrix_Support
        {
            ShowPinWrite (S("direct"), mesaPinNumber, PinValue);
            digitalWrite (pMAC->u.Pin.mesaPinNumber, PinValue);
        }
    }
    else    // must be PhysNow
    {
        ShowPinWrite     (S("direct"), mesaPinNumber, PinValue);
        digitalWrite (pMAC->u.Pin.mesaPinNumber, PinValue);
    }
}   // ssIO_DigitalWrite

// -------------------------------------------------------------------------------------------------
// virtualization supported, virtualized Pin exists, action in ability, perform the action.
// The assumption is that reading the value of the pin does not change the value of the Pin.
// This is true of registers often, but not always.  The Pin is a physical measure of electricity.
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_DEBUG_PINS_READ
void ShowPinRead (pAsciiA_t pDesc, PinNumber_t mesaPinNumber, LogicalPinValue_t PinValue)
{
    mesa_uiOp_emit_qAsciiA (pDesc);
    mesa_uiOp_emit_qAsciiA ("did read from <");
    mesa_uiOp_emit_1 (mesa_uiOp_give_4bitsAsHex ((mesaPinNumber >>  4) & 0xF));
    mesa_uiOp_emit_1 (mesa_uiOp_give_4bitsAsHex ((mesaPinNumber      ) & 0xF));
    mesa_uiOp_emit_qAsciiA ("> value <");
    mesa_uiOp_emit_Int_99 ((Bits8_t) PinValue);
    mesa_uiOp_emit_qAsciiA (">");
    mesa_uiOp_emit_newline ();
}   // ShowPinRead
#else   // not SSIO_OPTIN_DEBUG_PINS_READ
#define ShowPinRead(p,ID,V)
#endif  // SSIO_OPTIN_DEBUG_PINS_READ
// -------------------------------------------------------------------------------------------------
LogicalPinValue_t   ssIO_DigitalRead (MAC_ID_t MAC_ID)
{
    ssIO_pMAC_t         pMAC;
    LogicalPinValue_t   PinValue = 0;

    // virtualization of Pins is supported: if not found in virtualizations database, use mesa.
    pMAC = ssIO_dbOp_pMAC_This_MAC_ID (MAC_ID);
    if (pMAC == ssIO_pMACNull)
    {
        ShowPinRead (S("  true direct not MAC configured "),
            (((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift), PinValue);
        // if not a pin defined in ssIO by a pMAC_t, directly return HostOS value returned
        return (digitalRead ((((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift)));
    }   // no MAC definition

#ifdef  SSIO_OPTIN_CHECK_ACCESS
    // ---------------------------------------------------------------------------------------------
    // virtualization supported, virtualized Pin exists, action in ability, clarify the target.
    // ---------------------------------------------------------------------------------------------
    // virtualization of Pins is supported, virtualized Pin exists, compare action to ability
    if (!ssIO_MAC_Pin_UseCheckRead (pMAC))
    {
        ShowPinRead (S("Read Check Error"),
            (((PinNumber_t) pMAC->MAC_ID & MACnumber_Mask) >> MACnumber_Shift), PinValue);
        return (PinValue);
    }
#endif  //  SSIO_OPTIN_CHECK_ACCESS

    // virtualization supported, virtualized Pin exists, action in ability, perform the action.
    // by design, MAC Pins have a virtual value that caches the physical value on read/write;
    // This is the sequence (if physical, get physical to cache; virtual or physical, give cached).
    if (pMAC->TrueState == ssIO_VirtNow)
    {
        PinValue = pMAC->u.Pin.Value;
        ShowPinRead (S("virt direct "), pMAC->u.Pin.mesaPinNumber, PinValue);
    }   // virtual copies the current value, without changing the current value
    else
    {   // true either direct or mapped
        if (pMAC->MappedState == ssIO_Mapped)
        {   // physical mapped by convention acts like virtual unless the device supports bit reads
            PinValue = pMAC->u.Pin.Value;
            ShowPinRead (S("true mapped"), pMAC->u.Pin.mesaPinNumber, PinValue);
        }
        else
        {   // physical unmapped reads new current value, changes the copy of value to new, returns new.
            PinValue = digitalRead (pMAC->u.Pin.mesaPinNumber);
            pMAC->u.Pin.Value = PinValue;
            ShowPinRead (S("true direct "), pMAC->u.Pin.mesaPinNumber, PinValue);
        }
    }   // true/mapped

    return (PinValue);
}   // ssIO_DigitalRead

#endif  // SSIO_OPTIN_PINS

#endif  // __SSIO_PINS_DEF_H

