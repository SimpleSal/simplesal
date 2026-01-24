/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_utils_def.h     software definitons of basic utility routines
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_UTILS_DEF_H
#define __SSUI_UTILS_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// returns the number of bytes (0-max_i) that were copied, if stopped due to limit, returns i>max_i
int     ssUI_AaOp_pP1_gets_pP2max (pAsciiA_t pAsciiA_To, pAsciiA_t pAsciiA_From, int max_i)
{
    int i = 0;

    if ( (pAsciiA_To == pAsciiANull) || (pAsciiA_From == pAsciiANull) || (max_i == 0) )
    {
        return (i);
    }

    // non-canonical loop of i: stop with one byte left, for NUL value required by data structure
    while (*pAsciiA_From && (i <= max_i))
    {
        *pAsciiA_To = *pAsciiA_From;
        i++;
        pAsciiA_From++;
        pAsciiA_To++;
    }   // while
    // truncate the result whether just hit actual NUL in source, or i reached max_i
    *pAsciiA_To = Ascii_NUL;

    // terminated loop due to nul byte, or due to max_i being exceeded.  tell user
    return (i);
}   // ssUI_AaOp_pP1_gets_pP2max
// -------------------------------------------------------------------------------------------------
boolean ssUI_AaIf_pP1_eq_pP2 (pAsciiA_t pAsciiMaster, pAsciiA_t pCompareTo)
{
    if ((pAsciiMaster == pAsciiANull) || (pCompareTo == pAsciiANull))
    {
        return (false);
    }
    if (pAsciiMaster == pCompareTo)
    {
        return (true);
    }
    return (ss_uiOp_Compare_AaToAa (pAsciiMaster, pCompareTo));
}   // ssUI_AaIf_pP1_eq_pP2
// -------------------------------------------------------------------------------------------------
// shift any Ascii uppercase characters to the Ascii lowercase character that matches.  Return a
// pointer to the lowercase version of the characters; a pointer to a statically allocated buffer.
// -------------------------------------------------------------------------------------------------
pAsciiA_t  ssUI_AaOp_Copy_gets_pP1_lc (pAsciiA_t pAsciiA)
{
    // this variable always exist, accessible by name within the function, by pointer elsewhere.
    // the pointer returned points to the lowercase copied, until the next call occurs to lowercase.
    static AsciiA_t      copy[SSUI_BUFFER_ALLOC];
    int                 i;

    // need a copy because we want to compare lowercased versions for all variable names
    i = 0;
    while (*pAsciiA)
    {
        if (i == SSUI_BUFFER_MAX_I)       // what happens if MAX_I == 0?  possible, so check.
        {
            break;      // leaving [i] pointing to the last position in the array
        }
        copy[i] = Ascii_toLower (*pAsciiA);
        i++;
        pAsciiA++;
    }
    copy[i] = Ascii_NUL;

    // return a proper pointer to a statically allocated array with the lowercased characters
    return (&copy[0]);
}   // ssUI_AaOp_Copy_gets_pP1_lc
// -------------------------------------------------------------------------------------------------
// prepare for tokenization by seperating math operators from operands with a gauranteed blank.
// This small FSM copies anything not a math operation, and inserts blanks before and after math ops.
// -------------------------------------------------------------------------------------------------
int     ssUI_AaOp_pP1_gets_pP2maxpadded (pAsciiA_t pAsciiA_To, pAsciiA_t pAsciiA_From, int max_i)
{
    int         i = 0;
    boolean     inMathOp = false;

    if ( (pAsciiA_To == pAsciiANull) || (pAsciiA_From == pAsciiANull) || (max_i == 0) )
    {
        return (i);
    }

    // non-canonical loop of i: stop with one byte left, for NUL value required by data structure
    while (*pAsciiA_From && (i <= max_i))
    {
        if (!inMathOp)
        {
            if (ssUI_Ascii_isMathSymbol (*pAsciiA_From))
            {
                inMathOp = true;
                *pAsciiA_To = Ascii_Space;
                pAsciiA_To++;
                i++;
                *pAsciiA_To = *pAsciiA_From;
                pAsciiA_To++;
                i++;
            }
            else
            {
                *pAsciiA_To = *pAsciiA_From;
                pAsciiA_To++;
                i++;
            }
        }   // not in math op
        else
        {   // in math op.
            // This parser only allows (or allows only) 1-letter or 2-letter math operations
            if (ssUI_Ascii_isMathSymbol (*pAsciiA_From))
            {
                *pAsciiA_To = *pAsciiA_From;
                pAsciiA_To++;
                i++;
                *pAsciiA_To = Ascii_Space;
                pAsciiA_To++;
                i++;
            }
            else
            {
                *pAsciiA_To = Ascii_Space;
                pAsciiA_To++;
                i++;
                *pAsciiA_To = *pAsciiA_From;
                pAsciiA_To++;
                i++;
            }
            inMathOp = false;
        }   // in math operation, always terminated at most 2 characters

        pAsciiA_From++;
    }   // while
    // truncate the result whether just hit actual NUL in source, or i reached max_i
    *pAsciiA_To = Ascii_NUL;

    // terminated loop due to nul byte, or due to max_i being exceeded.  tell user, may be new length
    return (i);
}   // ssUI_AaOp_pP1_gets_pP2maxpadded

// -------------------------------------------------------------------------------------------------
Bits8_t ssUI_Ascii_chg_hexDigitsToBits8 (pAsciiA_t pHexAa, boolean *pValidHex8Bits)
{
    pAsciiA_t pCopy;

    *pValidHex8Bits = false;

    // the copy converts to lowercase into a buffer whose secret pointer we can use only here
    pCopy = ssUI_AaOp_Copy_gets_pP1_lc (pHexAa);

    if ((pCopy[0] != Ascii_0) || (pCopy[1] != 'x'))
    {
        return (0);
    }   // require hex specification

    if (
        (Ascii_isHexDigit_LC (pCopy[2]))       // Ascii value folded to lowercase
       &&
        (Ascii_isHexDigit_LC (pCopy[3]))       // Ascii value folded to lowercase
       &&
        (pCopy[4] == Ascii_NUL)
       )
    {   // we know we have <LC hexdigit><LC hexdigit><Ascii_NUL>, we know it will convert to hex.
        *pValidHex8Bits = true;
        return          // "0x24": 0x20 + 0x4; as a number with 8 bits
              (
                 ((ssUI_Ascii_chg_hexDigitLcase_ToInt (pCopy[2])) << 4)
                |   // bit-wise OR
                  (ssUI_Ascii_chg_hexDigitLcase_ToInt (pCopy[3])) // << 0 is inferred
              );
    }

    // did not pass the rules of the "if" above, Valid is still false.
    return (0);
}   // ssUI_Ascii_chg_hexDigitsToBits8

#endif  // __SSUI_UTILS_DEF_H

