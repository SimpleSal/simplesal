/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssA_fsms_def.h       data and software definitions:  ssTEA Api FSM and Run FSM software
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSA_FSMS_DEF_H
#define __SSA_FSMS_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\EvInfo Data.note]               State-change-based statistics; all FSMs
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\Mesa\ARM Function Pointers.note]         All ARM processors react to this bit
// -------------------------------------------------------------------------------------------------
#define ThumbBit (Bit0)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Statistics are interwoven into the software at key transitions in the Event API and Run FSMs.
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcStoppedToAgencying        = S("Stopped:Agencying   ");
ReadOnly pAsciiA_t  pcStoppedToPaused           = S("Stopped:Paused      ");
ReadOnly pAsciiA_t  pcAgencyingToStopped        = S("Agencying:Stopped   ");
ReadOnly pAsciiA_t  pcAgencyingToPaused         = S("Agencying:Paused    ");
ReadOnly pAsciiA_t  pcAgencyingToAgencying      = S("Agencying:Agencying ");
ReadOnly pAsciiA_t  pcPausedToStopped           = S("Paused:Stopped      ");
ReadOnly pAsciiA_t  pcPausedToAgencying         = S("Paused:Agencying    ");
ReadOnly pAsciiA_t  pcAgencyGranted             = S("Agency Granted      ");
ReadOnly pAsciiA_t  pcAutoResume                = S("Auto  Recurrence    ");
ReadOnly pAsciiA_t  pcAutoStop                  = S("Auto  Stop          ");
ReadOnly pAsciiA_t  pcAutoPause                 = S("Auto  Pause         ");
ReadOnly pAsciiA_t  pcSelfResume                = S("Self  Recurrence    ");
ReadOnly pAsciiA_t  pcSelfStop                  = S("Self  Stop          ");
ReadOnly pAsciiA_t  pcSelfPause                 = S("Self  Pause         ");

ReadOnly pAsciiA_t  pcMsg_AgApifsm_statsReq     = S("Agency Api FSM Stats request...");
ReadOnly pAsciiA_t  pcMsg_AgRunfsm_StatsReq     = S("Agency Run FSM Stats request...");
ReadOnly pAsciiA_t  pcOffToOnRunning            = S("Off:OnRunning       ");
ReadOnly pAsciiA_t  pcOffToOnReset              = S("Off:OnReset         ");
ReadOnly pAsciiA_t  pcOnRunningToOff            = S("OnRunning:Off       ");
ReadOnly pAsciiA_t  pcOnRunningToOnReset        = S("OnRunning:OnReset   ");
ReadOnly pAsciiA_t  pcOnResetToOff              = S("OnReset:Off         ");
ReadOnly pAsciiA_t  pcOnResetToOnRunning        = S("OnReset:OnRunning   ");

// -------------------------------------------------------------------------------------------------
// A normal result within the Api FSM or the Run FSM is a transition from one state to another.
// The reporting of information is always a good place to start when learning what an App does.
// The command processor within ssUI has view of normal also, within the user's view of ssUI events.
// -------------------------------------------------------------------------------------------------
// If every event's result creates an instance within a statistically distributed set of results,
// then an event result that is outside the understanding by the FSM of the distribution of results
// is called "nonnormal", because a result that is within the understood set is called "normal".
// The challenge of any computer-based logic is to comprehend both normal and nonnormal results.
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcNonNormalResult          = S("non-normal result   ");

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Statistics Issues.note]         state changes are tracked by statistics
// -------------------------------------------------------------------------------------------------
#ifdef SSA_OPTIN_STATS_APIFSM
#define SSA_STATS_APIFSM_Incr(xstat)         (++(pEvInfo->All_Stats.Apifsm.xstat))
#else   // not SSA_OPTIN_STATS_APIFSM
#define SSA_STATS_APIFSM_Incr(xstat)
#endif  // SSA_OPTIN_STATS_APIFSM

#ifdef SSA_OPTIN_STATS_RUNFSM
#define SSA_STATS_RUNFSM_EvEx_Incr(xstat)    (++(pEvInfo->All_Stats.Runfsm.EvEx.xstat))
#define SSA_STATS_RUNFSM_App_Incr(xstat)     (++(pEvInfo->All_Stats.Runfsm.App.xstat))
#else   // not SSA_OPTIN_STATS_RUNFSM
#define SSA_STATS_RUNFSM_EvEx_Incr(xstat)
#define SSA_STATS_RUNFSM_App_Incr(xstat)
#endif  // SSA_OPTIN_STATS_RUNFSM

// -------------------------------------------------------------------------------------------------
// These Agency API FSM functions perform critical changes to the state of the FSM given a signal.
// Any rules invented and implemented about allowable state changes are instantiated here.
// The User of ssA through the API changes the state of an Event through this service API.
// The ssA Root and Next FSMs change the state of an event through this service API.
// The statistics associated with state changes are collected here, as part of this service API.
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Statistics Issues.note]         state changes are tracked by statistics
// -------------------------------------------------------------------------------------------------
void ssA_ApifsmOp_state_Set (ssE_pEvInfo_t         pEvInfo,
                             ssTEA_pApiSig_t       pApiSig,
                             ssA_Apifsm_state_t    newApifsm_state)
{
    // a transition in state must be noted; some transitions are not interesting statistically.
    boolean stateDelta = true;

#ifdef SSA_OPTIN_STATS_APIFSM
    switch (pEvInfo->Apifsm_state)
    {
        case ssA_Apifsm_state_off         :
            switch (newApifsm_state)
            {   // off to off transition not interesting
                case ssA_Apifsm_state_on_running :
                    SSA_STATS_APIFSM_Incr (Off2OnRunning_ct);
                    break;
                case ssA_Apifsm_state_on_reset   :
                    SSA_STATS_APIFSM_Incr (Off2OnReset_ct);
                    break;
                case ssA_Apifsm_state_off        :
                case ssA_Apifsm_state_flaw       :
                    stateDelta = false;
                    break;
            }   // switch ssTEA's event desc new Event Api FSM state
            break;  //  switch ssTEA's event desc current Event Api FSM state: off
        case ssA_Apifsm_state_on_running  :
            switch (newApifsm_state)
            {   // running to running transition not interesting?
                case ssA_Apifsm_state_off        :
                    SSA_STATS_APIFSM_Incr (OnRunning2Off_ct);
                    break;
                case ssA_Apifsm_state_on_reset   :
                    SSA_STATS_APIFSM_Incr (OnRunning2OnReset_ct);
                    break;
                case ssA_Apifsm_state_on_running :
                case ssA_Apifsm_state_flaw       :
                    stateDelta = false;
                    break;
            }   // switch ssTEA's event desc new Event Api FSM state
            break;  //  switch ssTEA's event desc current Event Api FSM state: on_running

        case ssA_Apifsm_state_on_reset    :
            switch (newApifsm_state)
            {   // reset to reset transition not interesting?
                case ssA_Apifsm_state_off        :
                    SSA_STATS_APIFSM_Incr (OnReset2Off_ct);
                    break;
                case ssA_Apifsm_state_on_running :
                    SSA_STATS_APIFSM_Incr (OnReset2OnRunning_ct);
                    break;
                case ssA_Apifsm_state_on_reset   :
                case ssA_Apifsm_state_flaw       :
                    stateDelta = false;
                    break;
            }   // switch ssTEA's event desc new Event Api FSM state
            break;    //  switch ssTEA's event desc current Event Api FSM state: on_reset
        case ssA_Apifsm_state_flaw        :
            stateDelta = false;
            break;    //  switch ssTEA's event desc current Event Api FSM state
    }   // switch current state
#endif  // SSA_OPTIN_STATS_APIFSM

    if (stateDelta)
    {
        pEvInfo->Apifsm_state = newApifsm_state;

        // When a User decides an Event needs a state change, and uses the API to cause the
        // state change, there is an API signal context that can be updated with the new state.
        // When a service of ssTEA decides an Event has to be forced to a state (for example, halted
        // from agencying), there is no API signal context, so no state change signal occurs.
        if (pApiSig)
        {
            pApiSig->Apifsm_state = newApifsm_state;
        }
    }
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
}   // ssA_ApifsmOp_state_Set
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_state_Get (ssTEA_pApiSig_t pApiSig)
{
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_state_Get
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// These Agency Run FSM functions perform critical changes to the state of the FSM given a signal.
// Any rules invented and implemented about allowable state changes are instantiated here.
// The User of ssA through the API changes the state of an Event through this service API.
// The ssA Root and Next FSMs change the state of an event through this service API.
// The statistics associated with state changes are collected here, as part of this service API.
// -------------------------------------------------------------------------------------------------
void ssA_RunfsmOp_state_Set (ssE_pEvInfo_t       pEvInfo,
                             ssTEA_pApiSig_t     pApiSig,
                             ssA_Runfsm_state_t  newRunfsm_state)
{
    pEvInfo->ssE_pAboutEv->Runfsm_state = newRunfsm_state;

    // When a User decides an Event needs a state change, and uses the API to cause the
    // state change, there is an API signal context that can be updated with the new state.
    // When a service of ssTEA decides an Event has to be forced to a state (for example, halted
    // from agencying), there is no API signal context, so no state change signal occurs.
    if (pApiSig)
    {
        pApiSig->ssE_pAboutEv->Runfsm_state = newRunfsm_state;
    }
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
}   // ssA_RunfsmOp_state_Set
// -------------------------------------------------------------------------------------------------
// Runfsm statistics:
//  are maintained at the Event Info data structure level, at a cost per Event Info, when built in.
//  are based on Event Agency Run FSM state changes, at a cost per Event Info, when built in.
// HOWEVER, placing the cost in Event Info avoids placing the cost in every Event data structure.
// -------------------------------------------------------------------------------------------------
void ssA_RunfsmOp_state_Delta (ssE_pEvInfo_t       pEvInfo,
                               ssTEA_pApiSig_t     pApiSig,
                               ssA_Runfsm_state_t  oldRunfsm_state,
                               ssA_Runfsm_state_t  newRunfsm_state)
{
    // a transition in state must be noted; some transitions are not interesting statistically.
    boolean stateDelta = true;

#ifdef SSA_OPTIN_STATS_RUNFSM
    switch (oldRunfsm_state)
    {
        case ssA_Runfsm_State_stopped            :
            switch (newRunfsm_state)
            {   // stopped to stopped is not deemed interesting
                case ssA_Runfsm_State_agencying :
                    SSA_STATS_RUNFSM_App_Incr (StoppedToAgencying_ct);
                    break;
                case ssA_Runfsm_State_paused    :
                    SSA_STATS_RUNFSM_App_Incr (StoppedToPaused_ct);
                    break;
                case ssA_Runfsm_State_stopped   :
                case ssA_Runfsm_State_flaw      :
                    stateDelta = false;
                    break;
            }   // switch ssTEA's event desc Agency Run FSM state
            break;      // case stopped
        case ssA_Runfsm_State_agencying     :
            switch (newRunfsm_state)
            {   // any transition from agencying is interesting
                case ssA_Runfsm_State_stopped   :
                    SSA_STATS_RUNFSM_App_Incr (AgencyingToStopped_ct);
                    break;
                case ssA_Runfsm_State_paused    :
                    SSA_STATS_RUNFSM_App_Incr (AgencyingToPaused_ct);
                    break;
                case ssA_Runfsm_State_agencying :
                    SSA_STATS_RUNFSM_App_Incr (AgencyingToAgencying_ct);
                    break;
                case ssA_Runfsm_State_flaw      :
                    stateDelta = false;
                    break;
            }   // switch ssTEA's event desc Agency Run FSM
            break;  // case agencying
        case ssA_Runfsm_State_paused        :
            switch (newRunfsm_state)
            {   // paused to paused is not deemed interesting
                case ssA_Runfsm_State_stopped   :
                    SSA_STATS_RUNFSM_App_Incr (PausedToStopped_ct);
                    break;
                case ssA_Runfsm_State_agencying :
                    SSA_STATS_RUNFSM_App_Incr (PausedToAgencying_ct);
                    break;
                case ssA_Runfsm_State_paused    :
                case ssA_Runfsm_State_flaw      :
                    stateDelta = false;
                    break;
            }   // switch ssTEA's event desc Agency Run FSM
            break;
        case ssA_Runfsm_State_flaw          :
        default                             :
            stateDelta = false;
            break;
    }   // switch event desc Agency Run FSM
#endif  // SSA_OPTIN_STATS_RUNFSM

    if (stateDelta)
    {
        pEvInfo->ssE_pAboutEv->Runfsm_state = newRunfsm_state;
        // When a User decides an Event needs a state change, and uses the API to cause the
        // state change, there is an API signal context that can be updated with the new state.
        // When a service of ssTEA decides an Event has to be forced to a stopped state (halted
        // from agencying), there is no API signal context, so no state change signal occurs.
        if (pApiSig)
        {
            pApiSig->ssE_pAboutEv->Runfsm_state = newRunfsm_state;
        }
    }   // any delta
}   // ssA_RunfsmOp_state_Delta
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void  ssA_statOp_Apifsm_Zero (ssA_pApifsm_Stats_t pApifsm_stats)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_APIFSM
    pApifsm_stats->Off2OnRunning_ct =
     pApifsm_stats->Off2OnReset_ct =
      pApifsm_stats->OnRunning2Off_ct =
       pApifsm_stats->OnRunning2OnReset_ct =
        pApifsm_stats->OnReset2Off_ct =
         pApifsm_stats->OnReset2OnRunning_ct = 0;
#endif  // SSA_OPTIN_STATS_APIFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Apifsm_Zero
// -------------------------------------------------------------------------------------------------
void  ssA_statOp_Apifsm_P1getsP2 (ssA_pApifsm_Stats_t pP1, ssA_pApifsm_Stats_t pP2)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_APIFSM
    pP1->Off2OnRunning_ct        =  pP2->Off2OnRunning_ct;
    pP1->Off2OnReset_ct          =  pP2->Off2OnReset_ct;
    pP1->OnRunning2Off_ct        =  pP2->OnRunning2Off_ct;
    pP1->OnRunning2OnReset_ct    =  pP2->OnRunning2OnReset_ct;
    pP1->OnReset2Off_ct          =  pP2->OnReset2Off_ct;
    pP1->OnReset2OnRunning_ct    =  pP2->OnReset2OnRunning_ct;
#endif  // SSA_OPTIN_STATS_APIFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Apifsm_P1getsP2
// -------------------------------------------------------------------------------------------------
void  ssA_statOp_Apifsm_P1getsP1plusP2 (ssA_pApifsm_Stats_t pP1, ssA_pApifsm_Stats_t pP2)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_APIFSM
    pP1->Off2OnRunning_ct        +=  pP2->Off2OnRunning_ct;
    pP1->Off2OnReset_ct          +=  pP2->Off2OnReset_ct;
    pP1->OnRunning2Off_ct        +=  pP2->OnRunning2Off_ct;
    pP1->OnRunning2OnReset_ct    +=  pP2->OnRunning2OnReset_ct;
    pP1->OnReset2Off_ct          +=  pP2->OnReset2Off_ct;
    pP1->OnReset2OnRunning_ct    +=  pP2->OnReset2OnRunning_ct;
#endif  // SSA_OPTIN_STATS_APIFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Apifsm_P1getsP1plusP2

// -------------------------------------------------------------------------------------------------
void  ssA_statOp_Runfsm_Zero (ssA_pRunfsm_Stats_t pRunfsm_stats)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_RUNFSM

    pRunfsm_stats->App.StoppedToAgencying_ct =
     pRunfsm_stats->App.StoppedToPaused_ct =
      pRunfsm_stats->App.AgencyingToStopped_ct =
       pRunfsm_stats->App.AgencyingToPaused_ct =
        pRunfsm_stats->App.AgencyingToAgencying_ct =
         pRunfsm_stats->App.PausedToStopped_ct =
          pRunfsm_stats->App.PausedToAgencying_ct = 0;

    pRunfsm_stats->EvEx.AgencyGranted_ct =
     pRunfsm_stats->EvEx.AbbyNormal_ct =
      pRunfsm_stats->EvEx.Auto_RecurrenceAg_ct =
       pRunfsm_stats->EvEx.Auto_StopAg_ct =
        pRunfsm_stats->EvEx.Auto_PauseAg_ct =
         pRunfsm_stats->EvEx.Self_RecurrenceAg_ct =
          pRunfsm_stats->EvEx.Self_StopAg_ct =
           pRunfsm_stats->EvEx.Self_PauseAg_ct = 0;

#endif  // SSA_OPTIN_STATS_RUNFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Runfsm_Zero
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssA_statOp_Runfsm_P1getsP2 (ssA_pRunfsm_Stats_t pP1, ssA_pRunfsm_Stats_t pP2)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_RUNFSM
    pP1->App.StoppedToAgencying_ct   = pP2->App.StoppedToAgencying_ct;
    pP1->App.StoppedToPaused_ct      = pP2->App.StoppedToPaused_ct;
    pP1->App.AgencyingToStopped_ct   = pP2->App.AgencyingToStopped_ct;
    pP1->App.AgencyingToPaused_ct    = pP2->App.AgencyingToPaused_ct;
    pP1->App.AgencyingToAgencying_ct = pP2->App.AgencyingToAgencying_ct;
    pP1->App.PausedToStopped_ct      = pP2->App.PausedToStopped_ct;
    pP1->App.PausedToAgencying_ct    = pP2->App.PausedToAgencying_ct;

    pP1->EvEx.AgencyGranted_ct      = pP2->EvEx.AgencyGranted_ct;
    pP1->EvEx.AbbyNormal_ct         = pP2->EvEx.AbbyNormal_ct;
    pP1->EvEx.Auto_RecurrenceAg_ct  = pP2->EvEx.Auto_RecurrenceAg_ct;
    pP1->EvEx.Auto_StopAg_ct        = pP2->EvEx.Auto_StopAg_ct;
    pP1->EvEx.Auto_PauseAg_ct       = pP2->EvEx.Auto_PauseAg_ct;
    pP1->EvEx.Self_RecurrenceAg_ct  = pP2->EvEx.Self_RecurrenceAg_ct;
    pP1->EvEx.Self_StopAg_ct        = pP2->EvEx.Self_StopAg_ct;
    pP1->EvEx.Self_PauseAg_ct       = pP2->EvEx.Self_PauseAg_ct;
#endif  // SSA_OPTIN_STATS_RUNFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Runfsm_P1getsP2
// -------------------------------------------------------------------------------------------------
void  ssA_statOp_Runfsm_P1getsP1plusP2 (ssA_pRunfsm_Stats_t pP1, ssA_pRunfsm_Stats_t pP2)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_RUNFSM
    pP1->App.StoppedToAgencying_ct   += pP2->App.StoppedToAgencying_ct;
    pP1->App.StoppedToPaused_ct      += pP2->App.StoppedToPaused_ct;
    pP1->App.AgencyingToStopped_ct   += pP2->App.AgencyingToStopped_ct;
    pP1->App.AgencyingToPaused_ct    += pP2->App.AgencyingToPaused_ct;
    pP1->App.AgencyingToAgencying_ct += pP2->App.AgencyingToAgencying_ct;
    pP1->App.PausedToStopped_ct      += pP2->App.PausedToStopped_ct;
    pP1->App.PausedToAgencying_ct    += pP2->App.PausedToAgencying_ct;

    pP1->EvEx.AgencyGranted_ct      += pP2->EvEx.AgencyGranted_ct;
    pP1->EvEx.AbbyNormal_ct         += pP2->EvEx.AbbyNormal_ct;
    pP1->EvEx.Auto_RecurrenceAg_ct  += pP2->EvEx.Auto_RecurrenceAg_ct;
    pP1->EvEx.Auto_StopAg_ct        += pP2->EvEx.Auto_StopAg_ct;
    pP1->EvEx.Auto_PauseAg_ct       += pP2->EvEx.Auto_PauseAg_ct;
    pP1->EvEx.Self_RecurrenceAg_ct  += pP2->EvEx.Self_RecurrenceAg_ct;
    pP1->EvEx.Self_StopAg_ct        += pP2->EvEx.Self_StopAg_ct;
    pP1->EvEx.Self_PauseAg_ct       += pP2->EvEx.Self_PauseAg_ct;
#endif  // SSA_OPTIN_STATS_RUNFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Runfsm_P1getsP1plusP2
// -------------------------------------------------------------------------------------------------
void  ssA_statOp_All_Zero (ssA_pAll_Stats_t pAll_Stats)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_APIFSM
    ssA_statOp_Apifsm_Zero (&pAll_Stats->Apifsm);
#endif  // SSA_OPTIN_STATS_APIFSM
#ifdef SSA_OPTIN_STATS_RUNFSM
    ssA_statOp_Runfsm_Zero (&pAll_Stats->Runfsm);
#endif  // SSA_OPTIN_STATS_RUNFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_All_Zero
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// These service and utility functions do lots of outputing of information; a short name is tidier.
// -------------------------------------------------------------------------------------------------
#define newLine() ss_uiOp_emit_newline ()

// -------------------------------------------------------------------------------------------------
void ssA_statOp_Apifsm_Show (ssA_pApifsm_Stats_t pApifsm_stats)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_APIFSM

    ss_uiOp_emit_qAsciiA ("  ==  Shared view of Api FSM");
    newLine ();
    if (pApifsm_stats->Off2OnRunning_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcOffToOnRunning, pApifsm_stats->Off2OnRunning_ct);     newLine (); }
    if (pApifsm_stats->Off2OnReset_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcOffToOnReset, pApifsm_stats->Off2OnReset_ct);         newLine (); }
    if (pApifsm_stats->OnRunning2Off_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcOnRunningToOff, pApifsm_stats->OnRunning2Off_ct);     newLine (); }
    if (pApifsm_stats->OnRunning2OnReset_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcOnRunningToOnReset, pApifsm_stats->OnRunning2OnReset_ct); newLine (); }
    if (pApifsm_stats->OnReset2Off_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcOnResetToOff, pApifsm_stats->OnReset2Off_ct);         newLine (); }
    if (pApifsm_stats->OnReset2OnRunning_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcOnResetToOnRunning, pApifsm_stats->OnReset2OnRunning_ct); newLine (); }

#endif  // SSA_OPTIN_STATS_APIFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Apifsm_Show
// -------------------------------------------------------------------------------------------------
void   ssA_statOp_Runfsm_Show (ssA_pRunfsm_Stats_t pRunfsm_stats)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_RUNFSM

    ss_uiOp_emit_qAsciiA ("  ==  App view of Run FSM");
    newLine ();
    if (pRunfsm_stats->App.StoppedToAgencying_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcStoppedToAgencying, pRunfsm_stats->App.StoppedToAgencying_ct);       newLine (); }
    if (pRunfsm_stats->App.AgencyingToAgencying_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAgencyingToAgencying, pRunfsm_stats->App.AgencyingToAgencying_ct);   newLine (); }
    if (pRunfsm_stats->App.AgencyingToPaused_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAgencyingToPaused, pRunfsm_stats->App.AgencyingToPaused_ct);         newLine (); }
    if (pRunfsm_stats->App.PausedToAgencying_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcPausedToAgencying, pRunfsm_stats->App.PausedToAgencying_ct);         newLine (); }
    if (pRunfsm_stats->App.AgencyingToStopped_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAgencyingToStopped, pRunfsm_stats->App.AgencyingToStopped_ct);       newLine (); }
    if (pRunfsm_stats->App.StoppedToPaused_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcStoppedToPaused, pRunfsm_stats->App.StoppedToPaused_ct);             newLine (); }
    if (pRunfsm_stats->App.PausedToStopped_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcPausedToStopped, pRunfsm_stats->App.PausedToStopped_ct);             newLine (); }

    ss_uiOp_emit_qAsciiA ("  ==  ssTEA view of Run FSM");
    newLine ();
    if (pRunfsm_stats->EvEx.AgencyGranted_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAgencyGranted, pRunfsm_stats->EvEx.AgencyGranted_ct);     newLine (); }
    if (pRunfsm_stats->EvEx.AbbyNormal_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcNonNormalResult, pRunfsm_stats->EvEx.AbbyNormal_ct);      newLine (); }

    if (pRunfsm_stats->EvEx.Auto_PauseAg_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAutoPause, pRunfsm_stats->EvEx.Auto_PauseAg_ct);          newLine (); }
    if (pRunfsm_stats->EvEx.Self_PauseAg_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcSelfPause, pRunfsm_stats->EvEx.Self_PauseAg_ct);          newLine (); }

    if (pRunfsm_stats->EvEx.Auto_RecurrenceAg_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAutoResume, pRunfsm_stats->EvEx.Auto_RecurrenceAg_ct);    newLine (); }
    if (pRunfsm_stats->EvEx.Self_RecurrenceAg_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcSelfResume, pRunfsm_stats->EvEx.Self_RecurrenceAg_ct);    newLine (); }

    if (pRunfsm_stats->EvEx.Auto_StopAg_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcAutoStop, pRunfsm_stats->EvEx.Auto_StopAg_ct);            newLine (); }
    if (pRunfsm_stats->EvEx.Self_StopAg_ct != 0)
    {   ss_uiOp_emit_lbld_int (pcSelfStop, pRunfsm_stats->EvEx.Self_StopAg_ct);            newLine (); }

#endif  // SSA_OPTIN_STATS_RUNFSM
#endif  // SSA_OPTIN_STATS_FSM
}   // ssA_statOp_Runfsm_Show

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\ssTEA API As Signal Carrier.note]   Authenticate Channel's Magic Number
// -------------------------------------------------------------------------------------------------
boolean  ssA_ApifsmOp_IsChannelValid (ss_EvInfoChannel_t testChannel)
{
    int             evInfo_i;
    ssE_pEvInfo_t   pEvInfo;

    for (evInfo_i=0; evInfo_i <= SSE_EVINFO_MAX_I; evInfo_i++)
    {
        pEvInfo = &ssE_EvInfos[evInfo_i];
        if (testChannel == (ss_EvInfoChannel_t) pEvInfo)
        {
            // make a hidden check on a hidden value to be sure the Channel is valid
            if (~testChannel == (ss_EvInfoChannel_t) pEvInfo->MagicNumber)
            {
                // at the general level, this EvDesc content can be trusted, as a proper EvDesc.
                return (true);
            }
            // while it is a valid pointer to a structure, the data in the structure is invalid
            return (false);
        }   // if address matched a structure
    }   // check each structure

    return (false);
}   //  ssA_ApifsmOp_IsChannelValid
// =================================================================================================
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_validate_SigData (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
    ssE_pAboutEv_t      pApisAboutEvData;

    msg_ssTEA_Path ();

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\Software Developer Tricks.note]   perform memory read/write access test
// -------------------------------------------------------------------------------------------------
#ifdef MESA_OPTIN_DEBUG_MEMORYACCESS
    CTRICK_Prove_MemoryIsWritable ((unsigned long *) pApiSig, (unsigned char *) __func__);
#endif  // MESA_OPTIN_DEBUG_MEMORYACCESS

    // ---------------------------------------------------------------------------------------------
    // the ssTEA API bases knowledge of activation of Channel based on the channel's value.
    // ---------------------------------------------------------------------------------------------
    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    switch (pEvInfo->Apifsm_state)
    {
        case ssA_Apifsm_state_off             :
        case ssA_Apifsm_state_on_reset        :
            break;      // normal
        // -----------------------------------------------------------------------------------------
        // choice: user cannot validate Agency Api FSM referring to an On_running or On_reset Api.
        // -----------------------------------------------------------------------------------------
        case ssA_Apifsm_state_on_running      :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Apifsm_state_flaw            :
        default                                 :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // ---------------------------------------------------------------------------------------------
    // Channel valid, so use for pEvInfo.  Besides critical allocation of an Event Agency structure,
    // not much really in ssTEA structure to validate, Agency has many specific valid combinations.
    // ---------------------------------------------------------------------------------------------
    // Validate the data in the Api Signal Event data structure; if trusted, is ssE's ssE_AboutEv.
    // ---------------------------------------------------------------------------------------------
    pApisAboutEvData = pApiSig->ssE_pAboutEv;
    if (pApisAboutEvData == ssE_pAboutEvNull)
    {
        return (pApiSig->apiSNR = ss_apiSNR_noEvAgencyData);
    }

    if ( (pApisAboutEvData->Runfsm_EvType != ssE_EvType_Time) &&
         (pApisAboutEvData->Runfsm_EvType != ssE_EvType_Data) &&
         (pApisAboutEvData->Runfsm_EvType != ssE_EvType_Ireq) )
    {
        return (pApiSig->apiSNR = ss_apiSNR_dataBad_Type);
    }

    // ---------------------------------------------------------------------------------------------
    // if Time Event, an operative value that can't ever be zero is OccursAt.
    // If recurring Time Event, an operative value that can't be zero is Period.
    //
    // To cause a recurring event to immediately get agency once its run FSM is started,
    // and then have ssTEA set the new OccursAt time based on the period (start periodic):
    // A Next-planck OccursAt Time is never zero (by rule, the index field is never zero),
    // so an OccursAt set to Next-planck will start the periodic event from that moment in Time.
    // ---------------------------------------------------------------------------------------------
    if (pApisAboutEvData->Runfsm_EvType == ssE_EvType_Time)
    {
        // -----------------------------------------------------------------------------------------
        // a valid Time for OccursAt is Valid, nonzero, Positive; assumed to be in the Future.
        // -----------------------------------------------------------------------------------------
        if (
            (ssT_timeIf_P1hasTUerror (&pApisAboutEvData->OccursAt) != ss_TimeUnit_none)
            ||
            (ssT_timeif_P1equalsZero (&pApisAboutEvData->OccursAt))
            ||
            (!pApisAboutEvData->OccursAt.Positive)
           )
        {
            return (pApiSig->apiSNR = ss_apiSNR_dataBad_Time);
        }

        if (pApisAboutEvData->recurs)
        {
            // -------------------------------------------------------------------------------------
            // After the Eventcompletes, the Period value is added to one of:
            //  the original requested Time for Agency being granted (!recursFromTrue),
            // or,
            //  the Actual Time Agency was granted (recursFromTrue).
            // -------------------------------------------------------------------------------------
            // a valid Time for Period is Valid, nonzero, Positive; not much else limiting value.
            // -------------------------------------------------------------------------------------
            if (
                (ssT_timeIf_P1hasTUerror (&pApisAboutEvData->Period) != ss_TimeUnit_none)
                ||
                (ssT_timeif_P1equalsZero (&pApisAboutEvData->Period))
                ||
                (!pApisAboutEvData->Period.Positive)
               )
            {
                return (pApiSig->apiSNR = ss_apiSNR_dataBad_Time);
            }
        }   // recurring Time Event
    }   // Time Event

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_validate_SigData
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_validate_pEvFunc (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
    ssE_pAboutEv_t      pApisAboutEvData;
    unsigned long       u32_a, u32_b;

    msg_ssTEA_Path ();

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_emit_qAsciiA ("pEvFunc validation");
        ss_uiOp_emit_newline ();
    }

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        if (ssTEA_control.Show_Cause)
        {
            ss_uiOp_emit_qAsciiA ("bad channel in validate pEvFunc");
            ss_uiOp_emit_newline ();
        }
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;

    // Validation may be called from Agency Api FSM Off or Agency Api FSM On state.
    // when called in the Off state, the function cannot be called because there is no context yet.
    if (pEvInfo->Apifsm_state == ssA_Apifsm_state_off)
    {
        if (ssTEA_control.Show_Cause)
        {
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_qAsciiA ("function validation requires the Agency Api FSM to be in the On state.");
            ss_uiOp_emit_qAsciiA ("Not an error; but until Run FSM is On_reset or On_running, no Run FSM exists.");
            ss_uiOp_emit_newline ();
        }
        return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
    }

    pApisAboutEvData = pApiSig->ssE_pAboutEv;
    if (pApisAboutEvData->pEvFunc == pEvFuncNull)
    {
        return (pApiSig->apiSNR = ss_apiSNR_dataBad_pEvFunc);
    }

    u32_a = (unsigned long) &ssA_ApifsmOp_validate_pEvFunc;
    u32_a &= 0xFFFF0000;
    u32_b = (unsigned long) pApisAboutEvData->pEvFunc;
    u32_b &= 0xFFFF0000;

    if (u32_a != u32_b)
    {
        if (ssTEA_control.Show_Cause)
        {
            ss_uiOp_emit_qAsciiA ("warning: (upper 16 bits of function pointer) != (upper 16 bits ssTEA function address)");
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_lbld_hex ("ssTEA...validateFunc", u32_a);
            ss_uiOp_emit_lbld_hex (ss_pEvFunc, u32_b);
            ss_uiOp_emit_newline ();
        }   // wanna hear theories
    }   // code for ssTEA is not in the same chunk of memory as the code for the user event

    u32_b = (unsigned long) pApisAboutEvData->pEvFunc;
    // determine whether the least signficant 2 bits are nonzero
    if ((u32_b & (Bit0|Bit1)) != 0)
    {
        if (ssTEA_control.Show_Notes)
        {
            ss_uiOp_emit_qAsciiA ("note: ARM code instructions are always at a 4-byte boundary (Thumb excluded).");
            ss_uiOp_emit_newline ();
            if (u32_b & (ThumbBit))
            {
                ss_uiOp_emit_qAsciiA ("    : the least significant bit(s) in '&code as pointer' may point to Thumb code.");
                ss_uiOp_emit_newline ();
            }
            ss_uiOp_emit_lbld_hex (ss_pEvFunc, u32_b);
            ss_uiOp_emit_qAsciiA ("    : unless flames are coming out the back, this is not important.");
            ss_uiOp_emit_newline ();
        }   // I wanna hear about theories
    }   // least significant two bits in a 32-bit aligned address were nonzero

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("granting Init State Agency to Event, with a NULL Event Agency");
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("   pointer and a pointer to the Current Time...");
        ss_uiOp_emit_newline ();
        // -----------------------------------------------------------------------------------------
        // what does it look like when the Event Function "EvFunc" instantiation of the Event
        // software (by calling the host language function with proper paramters) crashes everything?
        // When a pointer value is not valid, or, when the Event software causes a processor fault.
        // -----------------------------------------------------------------------------------------
    }   // explain everything

    // ---------------------------------------------------------------------------------------------
    // use the pointer to function returning a result to call the function and get the result.
    // pass the parameters expected, with Null indicating this is within ApiFSM not RunFSM.
    // ---------------------------------------------------------------------------------------------
    // The Initialization state is comprised of code that establishes a valid set of conditions,
    // while in the "ApiFSM On, Reset" state, such that the transition into the "ApiFSM On, Running"
    // state, and therefore inclusion in Agency considerations by ssA, finds a proper "RunFSM" state.
    // ---------------------------------------------------------------------------------------------
    pApisAboutEvData->AgencyResult = pApisAboutEvData->pEvFunc (ssE_pAboutEvVoid, ssT_gpTime_Current);

    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    if (ssTEA_control.Show_Cause)
    {
        // -----------------------------------------------------------------------------------------
        // What has actually happened when this message is visible to the User?  The Event function
        // returned without crashing the processor, these messages were sent, the ssTEA and Host OS
        // functions have returned Agency to the Host OS, and the serial port has been given Agency.
        // The Event has satisfied the Golden Rule of Programming: Don't Cause A Hard Fault.
        // -----------------------------------------------------------------------------------------
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("and, we're back! Thank you, Doc. Doc Severinsen, everyone!");
        ss_uiOp_emit_newline ();
    }

    if (pApisAboutEvData->AgencyResult != ssE_EvResult_OK_go)
    {
        pApiSig->apiSNR = ss_apiSNR_EvExResult;
    }

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_validate_pEvFunc
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_validate_completed (ssTEA_pApiSig_t pApiSig)
{
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    // NOTE order here is copying API signal ssE_AboutEv (P2) to ssE true data ssE_AboutEv (P1)
    ssE_dbOp_P1getsP2 (pEvInfo->ssE_pAboutEv, pApiSig->ssE_pAboutEv); // make ssE ssE_AboutEv equal validated
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_validate_completed
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// each fsmOp "receives" a signal that an application has sent to ssTEA about an event API or event.
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_agencyOn     (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    // the ssTEA API bases knowledge of activation of Channel being pChannelNull or not,
    // the only API service where a pChannelNull Channel is acceptable is AgencyOn.
    if (pApiSig->channel == pChannelNull)
    {
        // the secret Channel we ask the user to supply is the address of our event descriptor
        pEvInfo = ssE_dbOp_Get_pEvInfo ();
        if (pEvInfo == ssE_pEvInfoNull)
        {
            return (pApiSig->apiSNR = ss_apiSNR_resourceMemory);
        }
        pApiSig->channel = (ss_EvInfoChannel_t) pEvInfo;
        // Apifsm state is init state (AgencyOff?) as implemented by EvDesc init and get
    }   // pEvInfo is not pChannelNull anymore

    // if the IsValid check ever returns false for a Channel we just EvInfo_Got above, trouble.
    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    switch (pEvInfo->Apifsm_state)
    {
        case ssA_Apifsm_state_off             :
            break;      // normal
        // choice: user cannot signal Agency On Api FSM to an Api currently in On_*  state
        case ssA_Apifsm_state_on_running      :
        case ssA_Apifsm_state_on_reset        :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Apifsm_state_flaw            :
        default                                 :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch

    // find: "ssE_AboutEv Allocation Rules"     : when App IS trusted, ssE pointer copy during Api FSM On
    // In all cases, ssTEA ssE_AboutEv allocated by run-time init, when the single database initializes.
    // If SSE_APP_IS_NOT_TRUSTED, the ssE_pAboutEv carrying the Signal to Api On is the App's ssE_pAboutEv,
    //   Values in "the App's Event Agency copy" should not be written except to copy ssE's value
    // if SSE_OPTIN_ABOUTEVDB_TRUSTED_APP, the ssE_pAboutEv is not valid until this Api On request completes
    //   The value in "the Event Agency" single instance is always live and changeable by anyone.
    // Therefore:
    //    if SSE_OPTIN_ABOUTEVDB_TRUSTED_APP,
    //       pApiSig->ssE_pAboutEv (App's ssE_AboutEv) == pEvInfo->ssE_pAboutEv (ssE's ssE_AboutEv)
    // The App safely uses the connection by refering to the ssE_pAboutEv ONLY AFTER the Api FSM Open.
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    pApiSig->ssE_pAboutEv = pEvInfo->ssE_pAboutEv;  // here is where ssE's pointer is given to App!!
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ssE assigns ssE_pAboutEv value to ssE core data : "), pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
// -- ifdef/else/endif

#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

// -- ifdef/else/endif
    // pApiSig->ssE_pAboutEv may be allocated before or after the return from Agency On request, BUT
    // SOMEBODY has to own the ssE_AboutEv the App is referring to when the App is not trusted by ssTEA.
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ssE does not modify ssE_pAboutEv when Api FSM On occurs : "), pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    ssA_ApifsmOp_state_Set (pEvInfo, pApiSig, ssA_Apifsm_state_on_reset);
    ssA_RunfsmOp_state_Set (pEvInfo, pApiSig, ssA_Runfsm_State_stopped);

    // ---------------------------------------------------------------------------------------------
    // [.\SimpleSal\ssDocs\ssTEA\ssTEA API As Signal Carrier.note]     Creating a valid Magic Number
    // ---------------------------------------------------------------------------------------------
    pApiSig->channel = (ss_EvInfoChannel_t) pEvInfo;
    pEvInfo->MagicNumber = MakeMagicNumber (pEvInfo);

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order correct, copy=ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_agencyOn
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_agencyRun   (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    switch (pEvInfo->Apifsm_state)
    {
        case ssA_Apifsm_state_on_reset        :
            break;      // normal
        case ssA_Apifsm_state_off             :
        // choice: user cannot signal Agency Run to an Api currently in On and Running state
        case ssA_Apifsm_state_on_running      :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Apifsm_state_flaw            :
        default                                 :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch

    ssA_ApifsmOp_state_Set (pEvInfo, pApiSig, ssA_Apifsm_state_on_running);
    ssA_RunfsmOp_state_Set (pEvInfo, pApiSig, ssA_Runfsm_State_stopped);

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_agencyRun
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_agencyReset   (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    switch (pEvInfo->Apifsm_state)
    {
        case ssA_Apifsm_state_on_running      :
            break;      // normal
        case ssA_Apifsm_state_off             :
        // choice: user cannot signal Agency Reset Api FSM to an Api currently in On and Reset state
        case ssA_Apifsm_state_on_reset        :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Apifsm_state_flaw            :
        default                                 :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // set new state
    ssA_ApifsmOp_state_Set (pEvInfo, pApiSig, ssA_Apifsm_state_on_reset);
    ssA_RunfsmOp_state_Set (pEvInfo, pApiSig, ssA_Runfsm_State_stopped);

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_agencyReset
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_agencyOff   (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    switch (pEvInfo->Apifsm_state)
    {
        case ssA_Apifsm_state_on_running      :
        case ssA_Apifsm_state_on_reset        :
            break;      // normal
        // choice: user cannot signal Agency Off Api FSM to an Api currently in Off state
        case ssA_Apifsm_state_off             :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Apifsm_state_flaw            :
        default                                 :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // set new state, don't really need to set pEvInfo (releasing soon) but use canonical methods
    ssA_ApifsmOp_state_Set (pEvInfo, pApiSig, ssA_Apifsm_state_off);
    ssA_RunfsmOp_state_Set (pEvInfo, pApiSig, ssA_Runfsm_State_stopped);

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    // the ssTEA Event Agency data structure for an Event managed by ssTEA is permanently
    // allocated and remains pointed to (and consumed) by the pEvInfo being shut down now.
    ssE_dbOp_Put_pEvInfo (pEvInfo);

    // the ssTEA API bases knowledge of activation of Channel being pChannelNull or not.
    // here Agency data is being deallocated, make sure Channel is no longer a valid value.
    pApiSig->channel = pChannelNull;

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_agencyOff
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_ApifsmOp_stats_Get (ssTEA_pApiSig_t pApiSig)
{
#ifdef SSA_OPTIN_STATS_APIFSM
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
#endif  // SSA_OPTIN_STATS_APIFSM
    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
#ifdef SSA_OPTIN_STATS_APIFSM
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    pApiSig->pApiSigData = (pVoid_t) &pEvInfo->All_Stats.Apifsm;
#else   // not SSA_OPTIN_STATS_APIFSM
    pApiSig->pApiSigData = (pVoid_t) pApiSigDataNull;
#endif  // SSA_OPTIN_STATS_APIFSM

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_ApifsmOp_stats_Get
// =================================================================================================
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_RunfsmOp_start (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;

    switch (pEvInfo->ssE_pAboutEv->Runfsm_state)
    {
        // choice: user can only signal Run FSM Agencying to a stopped Run FSM
        case ssA_Runfsm_State_stopped    :
            break;
        case ssA_Runfsm_State_paused     :
        case ssA_Runfsm_State_agencying  :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Runfsm_State_flaw       :
        default:
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // The Api OccursAt has a valid value, as determined by the completion of the validation FSM.
    // The first occurrence of an Event will occur at the time indicated; a one-time Event is done.
    // for both recurring Events or cascaded Events, the next occurrence is calculated at the Time.
    ssT_mathOp_P1getsP2 (&pEvInfo->ssE_pAboutEv->OccursAt, &pApiSig->ssE_pAboutEv->OccursAt);

    // note that the ssTEA database knows the time of next occurrence, but user does not.
    // design choice: copy ssTEA's true version into the user's API data structure Event
    // bugbugbug: when agency structures collapsed to one, need to detect copy A->A crunch.
    ssT_mathOp_P1getsP2 (&pApiSig->ssE_pAboutEv->OccursAt, &pEvInfo->ssE_pAboutEv->OccursAt);

    ssA_RunfsmOp_state_Delta (pEvInfo, pApiSig, pEvInfo->ssE_pAboutEv->Runfsm_state, ssA_Runfsm_State_agencying);
    pApiSig->channel = (ss_EvInfoChannel_t) pEvInfo;

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_RunfsmOp_start
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_RunfsmOp_stop         (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;

    switch (pEvInfo->ssE_pAboutEv->Runfsm_state)
    {
        case ssA_Runfsm_State_agencying  :
        case ssA_Runfsm_State_paused     :
            break;
        // choice: user cannot signal Run FSM Stop to a stopped event
        case ssA_Runfsm_State_stopped    :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Runfsm_State_flaw       :
        default:
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // set new state
    ssA_RunfsmOp_state_Delta (pEvInfo, pApiSig, pEvInfo->ssE_pAboutEv->Runfsm_state, ssA_Runfsm_State_stopped);
    pApiSig->channel = (ss_EvInfoChannel_t) pEvInfo;

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_RunfsmOp_Stop
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_RunfsmOp_pause        (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;

    switch (pEvInfo->ssE_pAboutEv->Runfsm_state)
    {
        case ssA_Runfsm_State_stopped    :
        case ssA_Runfsm_State_agencying  :
            break;
        // choice: user cannot signal Run FSM Pause to a paused event
        case ssA_Runfsm_State_paused     :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Runfsm_State_flaw       :
        default                           :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // set new state
    ssA_RunfsmOp_state_Delta (pEvInfo, pApiSig, pEvInfo->ssE_pAboutEv->Runfsm_state, ssA_Runfsm_State_paused);
    pApiSig->channel = (ss_EvInfoChannel_t) pEvInfo;

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_RunfsmOp_Pause
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_RunfsmOp_resume       (ssTEA_pApiSig_t pApiSig)
{
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;

    switch (pEvInfo->ssE_pAboutEv->Runfsm_state)
    {
        // choice: user can only signal Run FSM Resume to a paused FSM
        case ssA_Runfsm_State_paused     :
            break;
        case ssA_Runfsm_State_stopped    :
        case ssA_Runfsm_State_agencying  :
            return (pApiSig->apiSNR = ss_apiSNR_stateWrong);
        case ssA_Runfsm_State_flaw       :
        default                           :
            return (pApiSig->apiSNR = ss_apiSNR_stateInvalid);
    }   // switch current state

    // set new state
    ssA_RunfsmOp_state_Delta (pEvInfo, pApiSig, pEvInfo->ssE_pAboutEv->Runfsm_state, ssA_Runfsm_State_agencying);
    pApiSig->channel = (ss_EvInfoChannel_t) pEvInfo;

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_RunfsmOp_Resume
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_RunfsmOp_state_Get   (ssTEA_pApiSig_t pApiSig)
{
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not Trusted
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    ssE_dbOp_P1getsP2 (pApiSig->ssE_pAboutEv, pEvInfo->ssE_pAboutEv);   // order is correct, user copy gets ssE
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_RunfsmOp_state_Get
// -------------------------------------------------------------------------------------------------
ss_apiSNR_t ssA_RunfsmOp_stats_Get   (ssTEA_pApiSig_t pApiSig)
{
#ifdef  SSA_OPTIN_STATS_RUNFSM
    ssE_pEvInfo_t       pEvInfo = ssE_pEvInfoNull;
#endif  // SSA_OPTIN_STATS_APIFSM

    msg_ssTEA_Path ();

    if (!ssA_ApifsmOp_IsChannelValid (pApiSig->channel))
    {
        return (pApiSig->apiSNR = ss_apiSNR_databad_Channel);
    }

#ifdef SSA_OPTIN_STATS_RUNFSM
    // Channel valid so use for pEvInfo
    pEvInfo = (ssE_pEvInfo_t) pApiSig->channel;
    pApiSig->pApiSigData = (pVoid_t) &pEvInfo->All_Stats.Runfsm;
#else   // not SSA_OPTIN_STATS_RUNFSM
    pApiSig->pApiSigData = (pVoid_t) pApiSigDataNull;
#endif  // SSA_OPTIN_STATS_RUNFSM

    return (pApiSig->apiSNR = ss_apiSNR_OK);
} // ssA_RunfsmOp_stats_Get

#endif // __SSA_FSMS_DEF_H

