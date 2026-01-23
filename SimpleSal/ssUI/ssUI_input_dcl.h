/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_input_dcl.h     data type, prototype declarations: prompts, menus, Ascii device streams.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_INPUT_DCL_H
#define __SSUI_INPUT_DCL_H

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void        ssUI_Prompt (void);

boolean     ssUI_rootMain (boolean restart_ssUI);
boolean     ssUI_CmdsMain (void);
boolean     ssUI_LoopMain (void);
boolean     ssUI_MathMain (void);
boolean     ssUI_EvapiMain (void);
boolean     ssUI_ssMain (void);

boolean     ssUI_inOp_Line_FromSerial (pAsciiA_t pUserBuff, int userMax_i);
boolean     ssUI_inOp_Line_FromCmds (pAsciiA_t pUserBuff, int userBuff_MAX_I);

boolean     ssUI_inOp_Line_GetParseHandle (void);

#define THERES_NO_ACTION (false)                // nothing was output that requires a new prompt from UI
#define IM_THE_TOWN_CRIER (true)                // something was output that requires a new prompt

boolean     ssUI_BufferOnlyHas_ssui (pAsciiA_t pUserBuff);
boolean     ssUI_BufferOnlyHas_appui (pAsciiA_t pUserBuff);

boolean     ssUI_BufferStartsWith_echo (pAsciiA_t pUserBuff);
boolean     ssUI_BufferStartsWith_app (pAsciiA_t pUserBuff);

// =================================================================================================
int         ssUI_tknOp_ParseAllTokens (pAsciiA_t pInput);
int         ssUI_tknOp_Prepend_cmdZone (void);

void        ssUI_tknOp_SaveACopy_TermCondCmd (void);
void        ssUI_tknOp_Restore_TermCondCmd (void);

void        ssUI_tknOp_TokenizeP1 (ssUI_pToken_t pToken);
// bugbugbug using P1 as master, compare each Ascii character to the character in P2 array
boolean     ssUI_tknIf_P1eqP2 (ssUI_pToken_t pToken1, ssUI_pToken_t pToken2);

boolean     ssUI_tknIf_P1_eq_pAa (ssUI_pToken_t pToken, pAsciiA_t pAsciiA);

// bugbugbug the Ascii->Token compare, with Ascii as master, flips the order and keeps token as master (does it matter?)
#define     ssUI_tknIf_pP1_eq_pAa(cmpTo_p,pAsciiA)  (ssUI_tknIf_P1_eq_pAa (cmpTo_p, pAsciiA))
#define     ssUI_tknIf_pAscii_eq_pP2(pAsciiA,cmpTo_p)  (ssUI_tknIf_P1_eq_pAa (pAsciiA, cmpTo_p))

#define     ssUI_tknIf_sP1_eq_pAA(cmpTo_s,pAsciiA)  (ssUI_tknIf_P1_eq_pAa ((&cmpTo_s), pAsciiA))
#define     ssUI_tknIf_pAscii_eq_sP2(pAsciiA,cmpTo_s)  (ssUI_tknIf_P1_eq_pAa(pAsciiA, (&cmpTo_s)))

#define     ssUI_tknIf_iP1_eq_pAa(cmpTo_i,pAsciiA)  (ssUI_tknIf_P1_eq_pAa (&gTokens[(cmpTo_i)], pAsciiA))
#define     ssUI_tknIf_pAscii_eq_iP2(pAsciiA,cmpTo_i)  (ssUI_tknIf_P1_eq_pAa (&gTokens[(cmpTo_i)], pAsciiA))

#ifdef SSUI_OPTIN_DEBUG_TOKENS
void        ssUI_tknOp_Show_AllTkns (pAsciiA_t   pDescription);
#endif  // SSUI_OPTIN_DEBUG_TOKENS

#define ssUI_tknOp_sP1_gets_sP2(P1tkn_s,gP2tkn_s)    \
    { (P1tkn_s).pAsciiA = (gP2tkn_s).pAsciiA;  (P1tkn_s).valueCt = (gP2tkn_s).valueCt; }

#define ssUI_tknOp_iP1_gets_iP2(to_i,from_i)    \
    { ssUI_tknOp_sP1_gets_sP2 (gTokens[(to_i)], gTokens[(from_i)]); }

#define ssUI_tknOp_gP1_gets_iP2(gP1tkn,from_i)    \
    { ssUI_tknOp_sP1_gets_sP2 (gP2tkn, gTokens[(from_i)]); }

#define ssUI_tknOp_iP1_gets_gP2(to_i,gP2tkn)    \
    { ssUI_tknOp_sP1_gets_sP2 (gTokens[(to_i)], gP2tkn); }

// -------------------------------------------------------------------------------------------------
// Fancy token movement is used to let the user enter math operations such as "math x += y".
// -------------------------------------------------------------------------------------------------
void        ssUI_tknOp_MinusPlus_Prepend (Ascii_t thisValue, ssUI_pToken_t pToken);
int         ssUI_tknOp_MinusPlus_Fixup (int foundTokenCt);

// -------------------------------------------------------------------------------------------------
// inOp_Hold stops everything, says nothing, and does nothing: until a carriage return is entered.
// -------------------------------------------------------------------------------------------------
void        ssUI_inOp_Hold (void);

#endif  // __SSUI_INPUT_DCL_H

