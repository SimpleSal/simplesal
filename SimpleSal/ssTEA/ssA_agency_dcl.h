/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssA_agency_dcl.h     data type, prototype declarations: the Agency subsystem of ssTEA.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSA_AGENCY_DCL_H
#define __SSA_AGENCY_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// these are declarations of functions used or referred to only by 1) the Granter of Agency to the
// ssTEA_Agency_Root FSM in the App's loop function; or, 2) the Root/Next FSM functions themselves.
// -------------------------------------------------------------------------------------------------
void            ssTEA_Agency_Root (TimeUnitsBig_t MicrosSinceLast, TimeUnitsBig_t MillisSinceLast);

ssE_pEvInfo_t   ssA_EvAg_AgencyNext (ssE_pEvInfo_t pEvInfo, ssE_EvType_t eventType);
void            ssA_EvAg_AgencyRefresh (ssE_pAboutEv_t ssE_pAboutEv);

void            ssA_EvAg_RootMath_Entry (void);
void            ssA_EvAg_RootMath_Exit (TimeUnitsBig_t  Assumed_us);

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// ssTEA invokes these functions during initialization.
// -------------------------------------------------------------------------------------------------
ss_macSNR_t     ssA_Initialize (void);
ss_macSNR_t     ssA_InitData_RunTime (void);

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\Software Developer Tricks.note]    macros make function calls disappear
// -------------------------------------------------------------------------------------------------
// As the function call says, this is a useful function when an action is not taken for some reason.
// -------------------------------------------------------------------------------------------------
#ifdef SSA_OPTIN_AGENCY_DEBUG_INACTION
#define  ssA_mShowActionNotTaken(pAA) { ss_uiOp_emit_pAsciiA (S(pAA)); ss_uiOp_emit_newline (); }
#else   // not debug inaction
#define  ssA_mShowActionNotTaken(pAA)
#endif  // SSA_OPTIN_AGENCY_DEBUG_INACTION

#ifdef SSA_OPTIN_AGENCY_DEBUG
void            do_ssA_EvOp_Show_EvAg_Time   (pAsciiA_t pDesc, ssT_pTime_t  pTime);
void            do_ssA_EvOp_Show_EvAg_EvFunc (pAsciiA_t pDesc, EvAg_pEvFunc_t pEvFunc);
void            do_ssA_EvOp_Show_EvAg_State  (ssE_pAboutEv_t ssE_pAboutEv);

#define ssA_EvOp_Show_EvAg_Time(pAA,pTime)      do_ssA_EvOp_Show_EvAg_Time   (S(pAA), pTime)
#define ssA_EvOp_Show_EvAg_EvFunc(pAA,pEvFunc)  do_ssA_EvOp_Show_EvAg_EvFunc (S(pAA), pEvFunc)
#define ssA_EvOp_Show_EvAg_State(ssE_pAboutEv)  do_ssA_EvOp_Show_EvAg_State  (ssE_pAboutEv);
#else   // not SSA_OPTIN_AGENCY_DEBUG
#define ssA_EvOp_Show_EvAg_Time(pAA,pTime)
#define ssA_EvOp_Show_EvAg_EvFunc(pAA,pEvFunc)
#define ssA_EvOp_Show_EvAg_State(ssE_pAboutEv)
#endif // SSA_OPTIN_AGENCY_DEBUG

#endif // __SSA_AGENCY_DCL_H

