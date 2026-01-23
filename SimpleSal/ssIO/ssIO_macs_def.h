/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssIO_macs_def.h      data and software definitions: SimpleSal Digital/Analog/Input/Output.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSIO_MACS_DEF_H
#define __SSIO_MACS_DEF_H

#ifdef  SSIO_OPTIN_ANY_MAC

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Noise about the software style and organization of ssIO with respect to the rest of SimpleSal:
//    The intention of ssIO is to be independent of the Time, Event, and Agency features of ssTEA.
//    Independence propagates out to the documentation extrusion style of ssTEA; all documents in
//      the form of explanatory notes for ssIO are contained in the ssIO files themselves.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// There exists two equivalent alternative views of any object: physical reality, virtual reality.
// The alternative views are identical in their public interface yet interchangeable in real time.
// This knowledge is managed by ssIO, as a MAC, for the ssIO user.  Wow. Cool.  What's the catch?
// Instead of "digitalRead (pin)", the call is "ssIO_digitalRead (MAC)"; a Pin has a MAC defined.
// This level of complexity, an interface layer, creates an opportunity: a Signal Algorithm Lab.
// -------------------------------------------------------------------------------------------------
// It is very common for chipmakers to publish Hardware Abstraction Layer software for all of the
// devices accessible within the chip, this is sometimes called the HAL.  A HAL will only consider
// reality only as defined for the physical chipset the same company paid some people to create.
// -------------------------------------------------------------------------------------------------
// In physical reality, signal carriers have a physical relationship with reality: Pin voltages may
// be driven high or low by external actors in conditions that are not known within the MAC itself.
// In physical reality a write of 0 then 1 to a pin and then a read of the pin may perceive 0 or 1.
// In virtual reality, the action taken by the electronics as memory accesses will be instantaneous.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Here is one key benefit of emulation: imagine a pin on the Mesa that will reset the Mesa; other
// pins are used by the App to blink LEDs.  The software developer has a single line of software
// that mistakenly refers to the reset pin while trying to blink an LED.  Change all the LED pin
// outputs to virtual, the same software runs everywhere and now can be developed without resets.
// The first clue is the change, a thought experiment leads to the 47 that should have been 37.
// -------------------------------------------------------------------------------------------------
// It's easy to generate output to a true or virtual sensor, e.g., 'printf ("hello world");'.
// Generation of input is the other key benefit of emulation: replacing a physical implementation of
// a signal carrier from another Mesa with software and memory on the local Mesa simplifies bringup.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Completely independent of the data in each element of the database data set, there must be an
// instantiation of the memory used to support the database and some method of managing the memory.
// -------------------------------------------------------------------------------------------------
typedef struct MAC_db_Definitions_s
{
    boolean         isDefined;
    ssIO_MAC_t      MAC;
}   MAC_db_Definitions_t, *pMAC_db_Definitions_t;

MAC_db_Definitions_t     MAC_db_Definitions[MAC_DATA_ALLOC];

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssIO_MAC_Initialize (void)
{
    ssIO_MAC_dbOp_Init ();

#ifdef MESA_OPTIN_LED_Matrix_Support
    LEDM_matrixSetup ();
    LEDM_matrixClearAll ();
#endif // MESA_OPTIN_LED_Matrix_Support
}   // ssIO_MAC_Initialize
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
MAC_ID_t ssIO_Build_MAC_ID (MACtype_t MACtype, int MACmedium, int MACnumber)
{
    MAC_ID_t    MAC_ID = 0;

    switch (MACtype)
    {
        case MACtype_Undefined  :
            break;
        case MACtype_Pin        :
        case MACtype_Reg        :
        case MACtype_fifo       :
        case MACtype_DMA        :
        case MACtype_Signal     :
        case MACtype_Sensor     :
        case MACtype_user1      :
        case MACtype_user2      :
            MAC_ID |= (MACtype    << MACtype_Shift)   & MACtype_Mask;
            MAC_ID |= (MACmedium  << MACmedium_Shift) & MACmedium_Mask;
            MAC_ID |= (MACnumber  << MACnumber_Shift) & MACnumber_Mask;
            break;
        case MACtype_flaw       :
            break;
        default                 :
            break;
    }
    return (MAC_ID);
}   // ssIO_Build_MAC_ID
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Programmatic Initialization.  Not a coherent description of a MAC but all values are valid.
// -------------------------------------------------------------------------------------------------
void    ssIO_MAC_dbOp_Clear (ssIO_pMAC_t pMAC)
{
    pMAC->MAC_ID        = 0;
    pMAC->TrueState     = ssIO_VirtNow;     // default state is virtual with no physical
    pMAC->MappedState   = ssIO_Direct;      // start out writing/reading a virtual value, no mapping
    pMAC->isDigital     = true;             // access should be digital 0/1 as input or output
    pMAC->InputAllowed  = false;            // until defined for use
    pMAC->OutputAllowed = false;            // until defined for use
    pMAC->MACtype       = MACtype_Undefined;
}   // ssIO_MAC_dbOp_Clear
// -------------------------------------------------------------------------------------------------
void    ssIO_MAC_dbOp_Init (void)
{
    int MAC_i;

    // each time initialization is performed, the database is cleared and made valid.
    for (MAC_i=0; MAC_i <= MAC_DATA_MAX_I; MAC_i++)
    {
        MAC_db_Definitions[MAC_i].isDefined = true;            // lie so that the Put will initialize
        ssIO_MAC_dbOp_Put (&MAC_db_Definitions[MAC_i].MAC);
    }   // for
}   // ssIO_MAC_dbOp_Init
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t   ssIO_MAC_dbOp_Get (void)
{
    int             MAC_i;
    ssIO_pMAC_t     pMAC = ssIO_pMACNull;

    for (MAC_i=0; MAC_i <= MAC_DATA_MAX_I; MAC_i++)
    {
        if (!MAC_db_Definitions[MAC_i].isDefined)
        {
            // when "Put" is performed, the database information is made valid, trust that is done.
            MAC_db_Definitions[MAC_i].isDefined = true;
            pMAC = &MAC_db_Definitions[MAC_i].MAC;
            break;
        }   // if not in use, allow use
    }   // for
    return (pMAC);
}   // ssIO_MAC_dbOp_Get
// -------------------------------------------------------------------------------------------------
void   ssIO_MAC_dbOp_Put (ssIO_pMAC_t pMAC)
{
    int                 MAC_i;

    for (MAC_i=0; MAC_i <= MAC_DATA_MAX_I; MAC_i++)
    {
        if (MAC_db_Definitions[MAC_i].isDefined)                   // let's have some rules for liars
        {
            if (&MAC_db_Definitions[MAC_i].MAC == pMAC)          // only put back valid pointers
            {
                // every time a "Put" is performed, the database information is made valid.
                ssIO_MAC_dbOp_Clear (&MAC_db_Definitions[MAC_i].MAC);
                MAC_db_Definitions[MAC_i].isDefined = false;
                break;      // out of for
            }   // if doesn't match a true address, can't be put back
        }   // if not in use, can't be put back
    }   // for
}   // ssIO_MAC_dbOp_Put
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t   ssIO_dbOp_pMAC_This_MAC_ID (MAC_ID_t MAC_ID)
{
    ssIO_pMAC_t     pMAC = ssIO_pMACNull;
    int             MAC_i;

#ifdef SSIO_OPTIN_DEBUG_PINS_DEF
    ss_uiOp_emit_qAsciiA ("Look up pMAC with MAC_ID <");
    ss_uiOp_emit_Hex_32bits (MAC_ID);
    ss_uiOp_emit_qAsciiA (">");
    ss_uiOp_emit_newline ();
#endif  // SSIO_OPTIN_DEBUG_PINS_DEF

    for (MAC_i=0; MAC_i <= MAC_DATA_MAX_I; MAC_i++)
    {
        if (MAC_db_Definitions[MAC_i].isDefined)
        {
            if (MAC_db_Definitions[MAC_i].MAC.MAC_ID == MAC_ID)
            {
                pMAC = &MAC_db_Definitions[MAC_i].MAC;
                break;          // out of for
            }   // matched ID
        }   // DB entry active
    }   // for
    return (pMAC);
}   // ssIO_dbOp_pMAC_This_MAC_ID
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// If the SSIO_OPTIN_XXX has not been #defined, the corresponding XXX file appears to be empty.
// -------------------------------------------------------------------------------------------------
#include "ssIO_pins_def.h"
#include "ssIO_regs_def.h"
#include "ssIO_fifos_def.h"
#include "ssIO_DMAs_def.h"
#include "ssIO_signals_def.h"
#include "ssIO_sensors_def.h"

// =================================================================================================
// -------------------------------------------------------------------------------------------------
#ifdef  SSIO_OPTIN_PINS
// -------------------------------------------------------------------------------------------------
// Create a coherent set of data describing a MAC: Digital/Analog/Input/Output coherent for a Pin.
// -------------------------------------------------------------------------------------------------
void  ssIO_MAC_Pin_Default (ssIO_pMAC_t pMAC, int MAC_IO_Ability)
{
    pMAC->MAC_ID        = 0;
    pMAC->TrueState     = ssIO_TrueNow;
    pMAC->MappedState   = ssIO_Direct;
    pMAC->InputAllowed  = ((MAC_IO_Ability & SSIO_MAC_IO_Ability_Input) != 0);
    pMAC->OutputAllowed = ((MAC_IO_Ability & SSIO_MAC_IO_Ability_Output) != 0);
    pMAC->isDigital     = true;
    pMAC->MACtype       = MACtype_Pin;
}   // ssIO_MAC_Pin_Default
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssIO_MAC_Pin_UseCheckWrite (ssIO_pMAC_t  pMAC, LogicalPinValue_t PinValue)
{
    return (true);
}   // ssIO_MAC_Pin_UseCheckWrite
// -------------------------------------------------------------------------------------------------
boolean ssIO_MAC_Pin_UseCheckRead (ssIO_pMAC_t  pMAC)
{
    return (true);
}   // ssIO_MAC_Pin_UseCheckRead
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Pin_UseCheckMode (ssIO_pMAC_t pMAC, PinMode_t mesaPinMode)
{
    return (true);
}   // ssIO_MAC_Pin_UseCheckMode
#endif  // SSIO_OPTIN_PINS
// =================================================================================================
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_REGS
// -------------------------------------------------------------------------------------------------
// Create a coherent set of data describing a MAC: Digital/Analog/Input/Output coherent for a Pin.
// -------------------------------------------------------------------------------------------------
void  ssIO_MAC_Reg_Default (ssIO_pMAC_t pMAC)
{
    pMAC->TrueState       = ssIO_TrueNow;
    pMAC->InputAllowed    = true;
    pMAC->OutputAllowed   = true;
    pMAC->isDigital  = true;
    pMAC->MACtype         = MACtype_Reg;
}   // ssIO_MAC_Reg_Default
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t  ssIO_MAC_Reg_pMAC (int RegID)
{
    ssIO_pMAC_t    pMAC = ssIO_pMACNull;
    return (pMAC);
}   // ssIO_MAC_Reg_pMAC
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Reg_UseCheckWrite (ssIO_pMAC_t  pMAC, ssIO_pReg_t pReg)
{
    return (true);
}   // ssIO_MAC_Reg_UseCheckWrite
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Reg_UseCheckRead (ssIO_pMAC_t  pMAC)
{
    return (true);
}   // ssIO_MAC_Reg_UseCheckRead
#endif  // SSIO_OPTIN_REGS

// =================================================================================================
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_FIFOS
// -------------------------------------------------------------------------------------------------
// Create a coherent set of data describing a MAC: Digital/Analog/Input/Output coherent for a Pin.
// -------------------------------------------------------------------------------------------------
void  ssIO_MAC_Fifo_Default (ssIO_pMAC_t pMAC)
{
    pMAC->TrueState       = ssIO_TrueNow;
    pMAC->InputAllowed    = true;
    pMAC->OutputAllowed   = true;
    pMAC->isDigital  = true;
    pMAC->MACtype         = MACtype_fifo;
}   // ssIO_MAC_Fifo_Default
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t  ssIO_MAC_Fifo_This_fifoID (int FifoID)
{
    ssIO_pMAC_t    pMAC = ssIO_pMACNull;
    return (pMAC);
}   // ssIO_MAC_Fifo_This_fifoID
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Fifo_UseCheckWrite (ssIO_pMAC_t  pMAC, ssIO_pFifo_t pFifo)
{
    return (true);
}   // ssIO_MAC_Fifo_UseCheckWrite
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Fifo_UseCheckRead (ssIO_pMAC_t  pMAC)
{
    return (true);
}   // ssIO_MAC_Fifo_UseCheckRead
#endif  // SSIO_OPTIN_FIFOS
// =================================================================================================
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_DMAS
// -------------------------------------------------------------------------------------------------
// Create a coherent set of data describing a MAC: Digital/Analog/Input/Output coherent for a Pin.
// -------------------------------------------------------------------------------------------------
void  ssIO_MAC_DMA_Default (ssIO_pMAC_t pMAC)
{
    pMAC->TrueState     = ssIO_VirtNow;
    pMAC->InputAllowed  = true;
    pMAC->OutputAllowed = true;
    pMAC->isDigital     = true;
    pMAC->MACtype       = MACtype_DMA;
}   // ssIO_MAC__DMA_Default
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t  ssIO_MAC_DMA_pMAC (DMANumber_t DMANumber)
{
    ssIO_pMAC_t    pMAC = ssIO_pMACNull;
    return (pMAC);
}   // ssIO_MAC_DMA_pMAC
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_DMA_UseCheckWrite (ssIO_pMAC_t  pMAC, ssIO_DMA_pEngine_t pDMA)
{
    return (true);
}   // ssIO_MAC_DMA_UseCheckWrite
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_DMA_UseCheckRead (ssIO_pMAC_t  pMAC)
{
    return (true);
}   // ssIO_MAC_DMA_UseCheckRead
#endif  // SSIO_OPTIN_DMAS
// =================================================================================================
// -------------------------------------------------------------------------------------------------
#ifdef SSIO_OPTIN_SIGNALS
ssIO_pMAC_t  ssIO_MAC_Signal_pMAC (int SignalerID)
{
    ssIO_pMAC_t    pMAC = ssIO_pMACNull;
    return (pMAC);
}   // ssIO_MAC_Signal_pMAC
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Signal_UseCheckWrite (ssIO_pMAC_t  pMAC, ssIO_pSignaler_t pSignaler)
{
    return (true);
}   // ssIO_MAC_Signal_UseCheckWrite
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Signal_UseCheckRead (ssIO_pMAC_t  pMAC)
{
    return (true);
}   // ssIO_MAC_Signal_UseCheckRead
#endif  // SSIO_OPTIN_SIGNALS
// =================================================================================================
#ifdef SSIO_OPTIN_SENSORS
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t        ssIO_MAC_Sensor_pMAC (int SensorID)
{
    ssIO_pMAC_t    pMAC = ssIO_pMACNull;
    return (pMAC);
}   // ssIO_MAC_Sensor_pMAC
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Sensor_UseCheckWrite (ssIO_pMAC_t  pMAC, ssIO_pSensor_t pSensor)
{
    return (true);
}   // ssIO_MAC_Sensor_UseCheckWrite
// -------------------------------------------------------------------------------------------------
boolean  ssIO_MAC_Sensor_UseCheckRead (ssIO_pMAC_t  pMAC)
{
    return (true);
}   // ssIO_MAC_Sensor_UseCheckRead
#endif  // SSIO_OPTIN_SENSORS

// -------------------------------------------------------------------------------------------------
#define DumpBoolean(pAAlabel,isTrue)                \
    mesa_uiOp_emit_pAsciiA (S(pAAlabel));           \
    mesa_uiOp_emit_1 (Ascii_EncapL);                \
    mesa_uiOp_emit_1 (isTrue ? Ascii__t : Ascii_f); \
    mesa_uiOp_emit_1 (Ascii_EncapR);                \
    mesa_uiOp_emit_1 (Ascii_Space);                 \
    mesa_uiOp_emit_1 (Ascii_Space);

void ssIO_MAC_dbOp_dumpAll (void)
{
    ssIO_pMAC_t    pMAC = ssIO_pMACNull;
    int             MAC_i;
    boolean         empties_output = false;

    mesa_uiOp_emit_newline ();
    mesa_uiOp_emit_qAsciiA ("dump of all MACs   [db] - not defined; [db]def - defined");
    mesa_uiOp_emit_newline ();

    for (MAC_i=0; MAC_i <= MAC_DATA_MAX_I; MAC_i++)
    {
        if (MAC_db_Definitions[MAC_i].isDefined)
        {
            pMAC = &MAC_db_Definitions[MAC_i].MAC;
            if (empties_output)            mesa_uiOp_emit_newline ();

            mesa_uiOp_emit_1 (Ascii_squareLBracket);
            mesa_uiOp_emit_Int_99 (MAC_i);
            mesa_uiOp_emit_1 (Ascii_squareRBracket);
            mesa_uiOp_emit_1 (Ascii_Space);

            mesa_uiOp_emit_1 (Ascii_EncapL);
            switch (pMAC->MACtype)
            {
                case MACtype_Undefined :   mesa_uiOp_emit_qAsciiA ("Undefined");   break;
                case MACtype_Pin       :   mesa_uiOp_emit_qAsciiA ("Pin      ");   break;
                case MACtype_Reg       :   mesa_uiOp_emit_qAsciiA ("Reg      ");   break;
                case MACtype_fifo      :   mesa_uiOp_emit_qAsciiA ("fifo     ");   break;
                case MACtype_DMA       :   mesa_uiOp_emit_qAsciiA ("DMA      ");   break;
                case MACtype_Signal    :   mesa_uiOp_emit_qAsciiA ("Signal   ");   break;
                case MACtype_Sensor    :   mesa_uiOp_emit_qAsciiA ("Sensor   ");   break;
                case MACtype_user1     :   mesa_uiOp_emit_qAsciiA ("user1    ");   break;
                case MACtype_user2     :   mesa_uiOp_emit_qAsciiA ("user2    ");   break;
                case MACtype_flaw      :   mesa_uiOp_emit_qAsciiA ("flaw     ");   break;
            }   // switch MACtype
            mesa_uiOp_emit_1 (Ascii_EncapR);
            mesa_uiOp_emit_1 (Ascii_Space);
            mesa_uiOp_emit_1 (Ascii_Space);

            mesa_uiOp_emit_qAsciiA ("MAC_ID");
            mesa_uiOp_emit_1 (Ascii_EncapL);
            mesa_uiOp_emit_Hex_32bits (pMAC->MAC_ID);
            mesa_uiOp_emit_1 (Ascii_EncapR);
            mesa_uiOp_emit_1 (Ascii_Space);
            mesa_uiOp_emit_1 (Ascii_Space);
            mesa_uiOp_emit_qAsciiA ("value");
            mesa_uiOp_emit_1 (Ascii_EncapL);
            mesa_uiOp_emit_1 ((pMAC->u.Pin.Value == mesaPin_VALUE_LOW) ? Ascii_0 : Ascii_1);
            mesa_uiOp_emit_1 (Ascii_EncapR);
            mesa_uiOp_emit_1 (Ascii_Space);
            mesa_uiOp_emit_1 (Ascii_Space);
            DumpBoolean ("TrueState", (pMAC->TrueState == ssIO_TrueNow));
            if (pMAC->MappedState == ssIO_Mapped)
                { DumpBoolean ("Mapped", true); }
            else
                { DumpBoolean ("Direct", true); }

            DumpBoolean ("Digital",   pMAC->isDigital);
            DumpBoolean ("Input OK",  pMAC->InputAllowed);
            DumpBoolean ("Output OK", pMAC->OutputAllowed);
            mesa_uiOp_emit_newline ();
            empties_output = false;
        }
        else
        {
            empties_output = true;
            mesa_uiOp_emit_1 (Ascii_squareLBracket);
            mesa_uiOp_emit_Int_99 (MAC_i);
            mesa_uiOp_emit_1 (Ascii_squareRBracket);
        }
    }   // for
    mesa_uiOp_emit_newline ();
}   // ssIO_MAC_dbOp_dumpAll

#endif  //  SSIO_OPTIN_ANY_MAC

#endif  // __SSIO_MACS_DEF_H

