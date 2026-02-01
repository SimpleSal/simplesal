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

// assumes the 'c' in question has been validated as a valid (0-9,a-f) hex number all in lowercase
#define     ssUI_Ascii_chg_hexDigitLcase_ToInt(c) \
                (Ascii_isDigit(c) ? Ascii_digit_toInt(c) : ((c - Ascii_a) + 10))

#define     ssUI_IntsAbsValue(a)            (((int)a < 0) ? (-((int)a)) : ((int)a))

boolean     ssUI_AaIf_pP1_eq_pP2            (pAsciiA_t pMaster, pAsciiA_t pCompareTo);
int         ssUI_AaOp_pP1_gets_pP2max       (pAsciiA_t pAsciiTo, pAsciiA_t pAsciiFrom, int max_i);
int         ssUI_AaOp_pP1_gets_pP2maxpadded (pAsciiA_t pAsciiTo, pAsciiA_t pAsciiFrom, int max_i);

// -------------------------------------------------------------------------------------------------
// this function copies an array of Ascii characters, adjusted to lowercase, returns pointer to Copy
// -------------------------------------------------------------------------------------------------
pAsciiA_t    ssUI_AaOp_Copy_gets_pP1_lc  (pAsciiA_t pAsciiA);

#endif  // __SSUI_UTILS_DCL_H

