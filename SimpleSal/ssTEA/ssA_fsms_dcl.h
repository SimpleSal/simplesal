/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssA_fsms_dcl.h       data type, prototype declarations: ssTEA Api FSM and Run FSM software
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSA_FSMS_DCL_H
#define __SSA_FSMS_DCL_H

// =================================================================================================

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The interfaces to the ssTEA Agency FSMs are implemented in these API functions, as called by ssA.
// -------------------------------------------------------------------------------------------------
ss_macSNR_t     ssA_Initialize (void);
ss_macSNR_t     ssA_InitData_RunTime (void);

// -------------------------------------------------------------------------------------------------
void    ssA_statOp_All_Zero              (ssA_pAll_Stats_t pAll_Stats);

void    ssA_statOp_Apifsm_Show           (ssA_pApifsm_Stats_t pApifsm_stats);
void    ssA_statOp_Apifsm_Zero           (ssA_pApifsm_Stats_t pApifsm_stats);
void    ssA_statOp_Apifsm_P1getsP2       (ssA_pApifsm_Stats_t pP1, ssA_pApifsm_Stats_t pP2);
void    ssA_statOp_Apifsm_P1getsP1plusP2 (ssA_pApifsm_Stats_t pP1, ssA_pApifsm_Stats_t pP2);

void    ssA_statOp_Runfsm_Show           (ssA_pRunfsm_Stats_t pRunfsm_stats);
void    ssA_statOp_Runfsm_Zero           (ssA_pRunfsm_Stats_t pRunfsm_stats);
void    ssA_statOp_Runfsm_P1getsP2       (ssA_pRunfsm_Stats_t pP1, ssA_pRunfsm_Stats_t pP2);
void    ssA_statOp_Runfsm_P1getsP1plusP2 (ssA_pRunfsm_Stats_t pP1, ssA_pRunfsm_Stats_t pP2);

// =================================================================================================
boolean         ssTEA_apiOp_Emit_Signal         (ssTEA_pApiSig_t pApiSig);
boolean         ssA_ApifsmOp_IsChannelValid     (ss_EvInfoChannel_t testChannel);

ss_apiSNR_t     ssA_ApifsmOp_validate_SigData   (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_validate_pEvFunc   (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_validate_completed (ssTEA_pApiSig_t pApiSig);

ss_apiSNR_t     ssA_ApifsmOp_agencyOn           (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_agencyRun          (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_agencyReset        (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_agencyOff          (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_state_Get          (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_ApifsmOp_stats_Get          (ssTEA_pApiSig_t pApiSig);

ss_apiSNR_t     ssA_RunfsmOp_start              (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_RunfsmOp_stop               (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_RunfsmOp_pause              (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_RunfsmOp_resume             (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_RunfsmOp_state_Get          (ssTEA_pApiSig_t pApiSig);
ss_apiSNR_t     ssA_RunfsmOp_stats_Get          (ssTEA_pApiSig_t pApiSig);

void ssA_ApifsmOp_state_Set (ssE_pEvInfo_t         pEvInfo,
                             ssTEA_pApiSig_t       pApiSig,
                             ssA_Apifsm_state_t    newApifsm_state);

void ssA_RunfsmOp_state_Set (ssE_pEvInfo_t         pEvInfo,
                             ssTEA_pApiSig_t       pApiSig,
                             ssA_Runfsm_state_t    newRunfsm_state);

void ssA_RunfsmOp_state_Delta (ssE_pEvInfo_t       pEvInfo,
                               ssTEA_pApiSig_t     pApiSig,
                               ssA_Runfsm_state_t  oldRunfsm_state,
                               ssA_Runfsm_state_t  newRunfsm_state);

#endif // __SSA_FSMS_DCL_H

