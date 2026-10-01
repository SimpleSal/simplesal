/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_utils_dcl.h     data type, prototype declarations: basic utility routines
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_UTILS_DCL_H
#define __SSUI_UTILS_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

#define     ssUI_IntsAbsValue(a)            (((int)a < 0) ? (-((int)a)) : ((int)a))

boolean     ssUI_AaIf_pP1_eq_pP2            (pAsciiA_t pMaster, pAsciiA_t pCompareTo);
int         ssUI_AaOp_pP1_gets_pP2max       (pAsciiA_t pAsciiTo, pAsciiA_t pAsciiFrom, int max_i);
int         ssUI_AaOp_pP1_gets_pP2maxpadded (pAsciiA_t pAsciiTo, pAsciiA_t pAsciiFrom, int max_i);

// -------------------------------------------------------------------------------------------------
// this function copies an array of Ascii characters, adjusted to lowercase, returns pointer to Copy
// -------------------------------------------------------------------------------------------------
pAsciiA_t    ssUI_AaOp_Copy_gets_pP1_lc  (pAsciiA_t pAsciiA);

// -------------------------------------------------------------------------------------------------
// Given a pointer to an array of characters in the form ['0', 'x', 'N', 'N', '\0'] where N is a
// valid hexadecimal digit ('a' to 'f'). The value is evaluated in a lowercase version of the input.
// Return an 8-bit positive value for any input, set boolean indicating validity of value returned.
// The ToInt version makes sure the transition from an unsigned int 0-255 into a signed int 0-255.
// -------------------------------------------------------------------------------------------------
int     ssUI_Ascii_chg_hexDigitsToInt   (pAsciiA_t pHexAa, boolean *pValidHexNumber);

Bits8_t ssUI_Ascii_chg_hexDigitsToBits8 (pAsciiA_t pHexAa, boolean *pValidHexNumber);

#endif  // __SSUI_UTILS_DCL_H

