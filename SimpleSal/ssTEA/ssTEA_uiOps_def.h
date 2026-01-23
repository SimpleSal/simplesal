/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_uiOps_def.h    data and software definitions: utilities supporting ssTEA services.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSTEA_UIOPS_DEF_H
#define __SSTEA_UIOPS_DEF_H

// =================================================================================================
ReadOnly pAscii_t    ss_pOff         = S("Off");
ReadOnly pAscii_t    ss_pOnRunning   = S("On, running");
ReadOnly pAscii_t    ss_pOnReset     = S("On, reset");
ReadOnly pAscii_t    ss_pPaused      = S("paused");
ReadOnly pAscii_t    ss_pStopped     = S("stopped");
ReadOnly pAscii_t    ss_pRunning     = S("running");
ReadOnly pAscii_t    ss_pAgencying   = S("agencying");
ReadOnly pAscii_t    ss_pFlaw        = S("flaw");
ReadOnly pAscii_t    ss_pError       = S("error");
ReadOnly pAscii_t    ss_pPace        = S("pace");
ReadOnly pAscii_t    ss_pPlanck      = S("planck");
ReadOnly pAscii_t    ss_pPeriod      = S("period");
ReadOnly pAscii_t    ss_pState       = S("state");
ReadOnly pAscii_t    ss_pTime        = S("time");
ReadOnly pAscii_t    ss_pData        = S("data");
ReadOnly pAscii_t    ss_pIreq        = S("ireq");
ReadOnly pAscii_t    ss_pEvFunc      = S("pEvFunc");
ReadOnly pAsciiA_t   pcHelpUnits     = S("TimeStamp selects Units for first Time Unit: '1.2mn' is 1 minute 2 seconds");

ReadOnly pAscii_t    ss_EventAgency_Announce      = S(" Event Agency    :");
ReadOnly pAscii_t    ss_EventAgencyBlank_Announce = S("                 :");

// -------------------------------------------------------------------------------------------------
// the relationship of names is dependent only on the #defines; use the #defines to reference names.
// -------------------------------------------------------------------------------------------------
ReadOnly pAscii_t  ss_pTimeUnitNames[] =
    {
        S("pk"), S("ns"), S("us"), S("ms"), S("sc"), S("mn"), S("hr"), S("dy"), S("  "), S("??")
    };
#define PK      (0)
#define NS      (1)
#define US      (2)
#define MS      (3)
#define SC      (4)
#define MN      (5)
#define HR      (6)
#define DY      (7)
#define NONE    (8)
#define FLAW    (9)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
int    ss_uiOp_Count_Aa (pAscii_t  pAsciiA)
{
    int length = 0;
    while (*pAsciiA)
    {
        length++;
        pAsciiA++;
    }
    // found the NUL byte, how many did we count?
    return (length);
}   // ss_uiOp_Count_AscArr
// -------------------------------------------------------------------------------------------------
void   ss_uiOp_Duplicate_Aa (pAsciiA_t pCopyAa, pAsciiA_t pSourceAa)
{
    while (*pSourceAa)
    {
        *pCopyAa = *pSourceAa;
        pSourceAa++;
        pCopyAa++;
    }
    // the function says it will copy an AsciiA, which is 0-N bytes before a NUL byte.
    *pCopyAa = Ascii_NUL;
}   // ss_uiOp_Duplicate_Aa
// -------------------------------------------------------------------------------------------------
// compare two Ascii Arrays: 0-N sequential bytes in memory must match through the NUL byte.
// When one array contains a subset of the other, the compare fails with the NUL byte of the subset.
// -------------------------------------------------------------------------------------------------
boolean  ss_uiOp_Compare_AaToAa (pAsciiA_t pRootAa, pAsciiA_t pCompareToAa)
{
    while (true)
    {
        // if *Root NUL and *CompareTo NUL  :
        //      != comparison fails, NUL matches below and returns true
        // if *Root NUL and *CompareTo !NUL :
        //      != comparison succeeds, the Root has reached the end of values to compare, false
        // if *Root !NUL and *CompareTo NUL :
        //      != comparison succeeds, the CompareTo has reached the end of values to compare, false
        // if *Root !NUL and *CompareTo !NUL:
        //      != comparison succeeds/fails based on values, if EQ and not NUL, continue comparison
        if (*pRootAa != *pCompareToAa)
        {
            break;
        }
        if (*pRootAa == Ascii_NUL)
        {
            return (true);
        }
        pRootAa++;
        pCompareToAa++;
    }   // while
    return (false);
}   // ss_uiOp_Compare_AaToAa
// -------------------------------------------------------------------------------------------------
// user of this function may or may not limit the value passed in; don't allow a crazy index.
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_ByteAsBits (Bits8_t thisByte, int numBits, boolean MSBtoLSBorder)
{
    Bits8_t          bitMask;

    bitMask = (MSBtoLSBorder) ? Bit7 : Bit0;
    for (numBits = ((numBits > 8) ? 8 : numBits); numBits > 0; numBits--)
    {
        ss_uiOp_emit_1 ((thisByte & bitMask) ? Ascii_1 : Ascii_0);
        if (MSBtoLSBorder)  bitMask >>= 1;
        else                bitMask <<= 1;
    }
}   // ss_uiOp_emit_ByteAsBits
// -------------------------------------------------------------------------------------------------
// user of this function are responsible for only requesting the display of Hex digit as Ascii.
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Hex_1 (Bits8_t thisByte)
{
    ss_uiOp_emit_1 (mesa_uiOp_give_4bitsAsHex (thisByte & 0xF));
}   // ss_uiOp_emit_Hex_1
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Hex_2 (Bits8_t thisByte)
{
    ss_uiOp_emit_Hex_1 ((thisByte >> 4) & 0xF);
    ss_uiOp_emit_Hex_1 ((thisByte >> 0) & 0xF);
}   // ss_uiOp_emit_Hex_2
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Hex_999 (unsigned long this32bits)
{
    ss_uiOp_emit_Hex_1 ((this32bits >> 8) & 0xF);
    ss_uiOp_emit_Hex_1 ((this32bits >> 4) & 0xF);
    ss_uiOp_emit_Hex_1 ((this32bits >> 0) & 0xF);
}   // ss_uiOp_emit_Hex_999
// -------------------------------------------------------------------------------------------------
void CTRICK_Show_Hex_32bits (unsigned long this32bits)
{
    ss_uiOp_emit_Hex_2 ((this32bits >> 24) & 0xFF);
    ss_uiOp_emit_Hex_2 ((this32bits >> 16) & 0xFF);
    ss_uiOp_emit_Hex_2 ((this32bits >>  8) & 0xFF);
    ss_uiOp_emit_Hex_2 ((this32bits >>  0) & 0xFF);
}   // CTRICK_Show_Hex_32bits
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_ThisRecursively (int numberOfTimes, Ascii_t ThisAscii)
{
    if (numberOfTimes == 0)
    {
        return;                         // terminating condition for a recursive solution
    }
    ss_uiOp_emit_1 (ThisAscii);         // must be at least one, terminating condition checks
    ss_uiOp_emit_ThisRecursively ((numberOfTimes-1), ThisAscii);
}   // ss_uiOp_emit_ThisRecursively
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Dash (int numberOfTimes)
{
    ss_uiOp_emit_ThisRecursively (numberOfTimes, Ascii_Dash);
}   // ss_uiOp_emit_Dash
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Period (int numberOfTimes)
{
    ss_uiOp_emit_ThisRecursively (numberOfTimes, Ascii_Period);
}   // ss_uiOp_emit_Period
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Zero (int numberOfTimes)
{
    ss_uiOp_emit_ThisRecursively (numberOfTimes, Ascii_0);
}   // ss_uiOp_emit_Zero
// -------------------------------------------------------------------------------------------------
void ss_uiOp_emit_Space (int numberOfTimes)
{
    ss_uiOp_emit_ThisRecursively (numberOfTimes, Ascii_Space);
}   // ss_uiOp_emit_Space

#endif  // __SSTEA_UIOPS_DEF_H

