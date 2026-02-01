/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_menucore_def.h  software and data definitions of command and parameter handlers for the UI
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_MENUCORE_DEF_H
#define __SSUI_MENUCORE_DEF_H

// -------------------------------------------------------------------------------------------------
// These static and global variables are used by FSMs in menus and cmdZones.
// -------------------------------------------------------------------------------------------------
// this is information the EvApi command FSM Computes, and then uses, as it processes parameters.
// this variable knows the EvApi API data structure in use (this command, across all parameters).
// -------------------------------------------------------------------------------------------------
ssUI_db_pEvApi_t   gThisCmds_pEvApi;

// -------------------------------------------------------------------------------------------------
// This supports an FSM: not Pending until it is Pending (asked once) then not Pending (asked twice).
// This require a repeat of a command that clears data (value is retained, used across commands).
// The transition in state [Pending->Not Pending] occurs after a pending Action is granted Agency.
// -------------------------------------------------------------------------------------------------
boolean     gClearPending = false;

// -------------------------------------------------------------------------------------------------
// The menu core FSM does not participate in resource sharing; it assumes it has ownership of Ascii.
// Resource sharing management (set/clear/show/etc) is supported by the menu core as an abstraction.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss    (int tkn_i, boolean moreInputTkns)
{
    if (!moreInputTkns)
    {
        ssUI_cmdZone_StackPush (ssUI_ss);
        return (eH_rHandled);
    }   // fully expressed

    return (eH_rDefer);
}   // H_ss
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_Time (int tkn_i, boolean moreInputTkns)
{
    if (!moreInputTkns)
    {
        ss_uiOp_Show_ssTime_state ();
        return (eH_rHandled);
    }
    return (eH_rDefer);
}   // H_ss_Time
// -------------------------------------------------------------------------------------------------
// Agency Root tracks the passage of Time while it is running; Agency requires Time to function.
// Time cannot be stopped while Agencying is running, but it can be paused/resumed to freeze Time.
// For example, The user of an application whose event is triggered by a Time occuring (3:00PM),
// can pause Time, change Current Time to 2:55PM, then resume Time.  It all depends on Time.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_TimeStart (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    switch (ssTEA_control.TimeState)
    {
        case ssTEA_state_stopped :
            break;
        case ssTEA_state_running :
            ss_uiOp_Show_ssTime_state ();
            ss_uiOp_pBanner (lfY, pcMsg_NotAllowed, lfY);
            ss_uiOp_pBanner (lfN, pcMsg_NoStartWhileRunning, lfY);
        case ssTEA_state_error   :
        case ssTEA_state_flaw    :
        default               :
            return (eH_rError);
    }   // switch

    ssTEA_control.TimeState = ssTEA_state_running;
    ss_uiOp_Show_ssTime_state ();
    return (eH_rHandled);
}   // H_ss_TimeStart
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_TimeStop (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    switch (ssTEA_control.Agency_state)
    {
        case ssTEA_state_stopped :
            break;
        case ssTEA_state_running :
            ss_uiOp_Show_ssAgency_state ();
            ss_uiOp_pBanner (lfY, pcMsg_NotAllowed, lfY);
            ss_uiOp_pBanner (lfN, pcMsg_NoTimeStopAgencyIsRunning, lfY);
        case ssTEA_state_error   :
        case ssTEA_state_flaw    :
        default               :
            return (eH_rError);
    }   // switch

    switch (ssTEA_control.TimeState)
    {
        case ssTEA_state_running :
            break;
        case ssTEA_state_stopped :
            ss_uiOp_Show_ssTime_state ();
            ss_uiOp_pBanner (lfY, pcMsg_NotAllowed, lfY);
            ss_uiOp_pBanner (lfN, pcMsg_NoStopWhileNotRunning, lfY);
        case ssTEA_state_error   :
        case ssTEA_state_flaw    :
        default               :
            return (eH_rError);
    }   // switch

    ssTEA_control.TimeState = ssTEA_state_stopped;
    ssTEA_control.Agency_state = ssTEA_state_stopped;
    ss_uiOp_Show_ssTime_state ();
    return (eH_rHandled);
}   // H_ss_TimeStop
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_TimeBigBang (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // bugbugbug action missing
    ss_uiOp_emit_qAsciiA ("<success>");
    ss_uiOp_emit_newline ();

    // repeat command to execute big bang
    return (eH_rHandled);
}   // H_ss_TimeBigBang
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_TimeStats (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    ss_uiOp_Show_ssTimeRatios ();
    return (eH_rHandled);
}   // H_ss_TimeStats
// -------------------------------------------------------------------------------------------------
// Agency Root uses the current state to know whether to grant Agency to ssTEA during the idle loop.
// If ssTEA is running, it has been given permission to manage time and events to implement FSM.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_Agency (int tkn_i, boolean moreInputTkns)
{
    if (!moreInputTkns)
    {
        ss_uiOp_Show_ssAgency_state ();
        return (eH_rHandled);
    }
    return (eH_rDefer);
}   // H_ss_Agency
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_AgencyStart (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (ssTEA_control.TimeState == ssTEA_state_stopped)
    {
        ss_uiOp_Show_ssTime_state ();
        ss_uiOp_pBanner (lfY, pcMsg_NotAllowed, lfY);
        ss_uiOp_pBanner (lfN, pcMsg_NoAgencyWithoutTime, lfY);
        return (eH_rError);
    }   // must have Time to Agency

    switch (ssTEA_control.Agency_state)
    {
        case ssTEA_state_stopped :
            break;
        case ssTEA_state_running :
            ss_uiOp_Show_ssAgency_state ();
            ss_uiOp_pBanner (lfY, pcMsg_NotAllowed, lfY);
            ss_uiOp_pBanner (lfN, pcMsg_NoStartWhileRunning, lfY);
        case ssTEA_state_error   :
        case ssTEA_state_flaw    :
        default               :
            return (eH_rError);
    }   // switch

    ssTEA_control.Agency_state = ssTEA_state_running;
    ss_uiOp_Show_ssAgency_state ();
    return (eH_rHandled);
}   // H_ss_AgencyStart
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_AgencyStop (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    switch (ssTEA_control.Agency_state)
    {
        case ssTEA_state_running :
            break;
        case ssTEA_state_stopped :
            ss_uiOp_Show_ssAgency_state ();
            ss_uiOp_pBanner (lfY, pcMsg_NotAllowed, lfY);
            ss_uiOp_pBanner (lfN, pcMsg_NoStopWhileNotRunning, lfY);
        case ssTEA_state_error   :
        case ssTEA_state_flaw    :
        default               :
            return (eH_rError);
    }   // switch

    ssTEA_control.Agency_state = ssTEA_state_stopped;
    ss_uiOp_Show_ssAgency_state ();
    return (eH_rHandled);
}   // H_ss_AgencyStop
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_AgencyPace (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    // menu ss agency pace          report pace
    // menu ss agency pace planck   set agency pace to planck
    // menu ss agency pace NNNNms   set agency pace to number of milliseconds
    if (!moreInputTkns)
    {
        ss_uiOp_Show_ssAgency_state ();
        return (eH_rHandled);
    }

    return (eH_rDefer);
}   // H_ss_AgencyPace
// -------------------------------------------------------------------------------------------------
// Pace period is expressed in milliseconds (ms).
// convert input Token TimeStamp into int variable
// rules: up to 4 digits, all integer
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_AgencyPeriod (int tkn_i, boolean moreInputTkns)
{
    int         sum      = 0;
    int         source_i = 0;
    pAsciiA_t   pCopy;

    msg_ssTEA_Path ();

    // the copy converts to lowercase into a buffer whose secret pointer we can use only here
    pCopy = ssUI_AaOp_Copy_gets_pP1_lc (gTokens[tkn_i].pAsciiA);
    if (ssUI_AaIf_pP1_eq_pP2 (pCopy, ss_pPlanck))
    {
        ssTEA_control.Agency_pace = ssTEA_Agency_Pace_planck;
        return (eH_rHandled);
    }

    while (gTokens[tkn_i].pAsciiA[source_i] != Ascii_NUL)
    {
        if (Ascii_isDigit (gTokens[tkn_i].pAsciiA[source_i]))
        {
            sum += Ascii_digit_toInt (gTokens[tkn_i].pAsciiA[source_i]);
        }
        else
        {
            ss_uiOp_pBanner (lfN, pcMsg_MillisRangeError_T1toT2, lfY);
            return (eH_rError);
        }
        source_i++;
        if (gTokens[tkn_i].pAsciiA[source_i] != Ascii_NUL)
        {
            sum *= 10;
        }
    }   // while

    if (sum == 0)
    {
        ss_uiOp_pBanner (lfN, pcMsg_MillisRangeError_T1toT2, lfY);
        return (eH_rError);
    }

    ssTEA_control.Agency_pace = ssTEA_Agency_Pace_period;
    ssTEA_control.Agency_period = sum;
    ss_uiOp_Show_ssAgency_state ();
    return (eH_rHandled);
}   // H_ss_AgencyPeriod
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_AgencyStats (int tkn_i, boolean moreInputTkns)
{
#ifdef  SSA_OPTIN_STATS_FSM
    ssA_All_Stats_t         AllEvs_All_Stats;
    ssA_All_Stats_t         ThisEv_All_Stats;
    ssUI_db_pEvApi_t        pEvApi;
    int                     EvApi_i;

    msg_ssTEA_Path ();

    ss_uiOp_Show_ssAgency_state ();

    ssA_statOp_All_Zero (&AllEvs_All_Stats);

    for (EvApi_i = 0; EvApi_i <= SSUI_EVAPI_ALLOC_MAX_I; EvApi_i++)
    {
        // -----------------------------------------------------------------------------------------
        // CHANGE pEvApi to refer to a specific EvApiData database entry or record.
        // -----------------------------------------------------------------------------------------
        pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (EvApi_i);
        if (!pEvApi->accessOn)
        {
            continue;
        }
        if (pEvApi->pssTEA_ApiSig->Apifsm_state == ssA_Apifsm_state_off)
        {
            continue;
        }

        ssA_statOp_All_Zero (&ThisEv_All_Stats);

#ifdef SSA_OPTIN_STATS_APIFSM
        ss_uiOp_emit_Dash (4);
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_qAsciiA ("    (nonzero stats only)");
        ss_uiOp_emit_newline ();

        // the API signal returns a pointer to the statistics maintained by ssTEA in ssTEA's data
        if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Stats))
        {
            if (pEvApi->pssTEA_ApiSig->pApiSigData != pApiSigDataNull)
            {
                ssA_statOp_Apifsm_P1getsP2
                    (
                        &ThisEv_All_Stats.Apifsm,
                        (ssA_pApifsm_Stats_t) pEvApi->pssTEA_ApiSig->pApiSigData
                    );
                ssA_statOp_Apifsm_Show (&ThisEv_All_Stats.Apifsm);
            }
            else
            {
                // This is a build choice, statistics must be included
                ss_uiOp_pBanner (lfY, pcMsg_FSMStats_ReqFailed, lfY);
            }
        }   // Api signal for ApiFsm stats
#else   // not SSA_OPTIN_STATS_APIFSM
        ss_uiOp_pBanner (lfY, pcMsg_ApiFSMStats_NotBuilt, lfY);
#endif  // SSA_OPTIN_STATS_APIFSM

#ifdef SSA_OPTIN_STATS_RUNFSM
        ss_uiOp_emit_Dash (4);
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_qAsciiA ("    (nonzero stats only)");
        ss_uiOp_emit_newline ();
        // the API returns a pointer to the statistics maintained by ssTEA in ssTEA's data
        if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Stats))
        {
            if (pEvApi->pssTEA_ApiSig->pApiSigData != pApiSigDataNull)
            {
                ssA_statOp_Runfsm_P1getsP2
                    (
                        &ThisEv_All_Stats.Runfsm,
                        (ssA_pRunfsm_Stats_t) pEvApi->pssTEA_ApiSig->pApiSigData
                    );
                ssA_statOp_Runfsm_Show (&ThisEv_All_Stats.Runfsm);
            }   // params pointer is non-NULL
            else
            {
                // This is a build choice, statistics must be included
                ss_uiOp_pBanner (lfY, pcMsg_FSMStats_ReqFailed, lfY);
            }
        }   // if Runfsm stats signal worked
#else   // not SSA_OPTIN_STATS_RUNFSM
         ss_uiOp_pBanner (lfY, pcMsg_RunFSMStats_NotBuilt, lfY);
#endif  // SSA_OPTIN_STATS_RUNFSM

        // NOTE that this function's function is to total the stats of all event APIs and show that.
        // If both signals worked and data was pointed to, add the individual to total.
        // Even if one or both of the signals failed and the data was unchanged from being zeroed, add.
#ifdef SSA_OPTIN_STATS_APIFSM
        ssA_statOp_Apifsm_P1getsP1plusP2 (&AllEvs_All_Stats.Apifsm, &ThisEv_All_Stats.Apifsm);
#endif  // SSA_OPTIN_STATS_APIFSM
#ifdef SSA_OPTIN_STATS_RUNFSM
        ssA_statOp_Runfsm_P1getsP1plusP2 (&AllEvs_All_Stats.Runfsm, &ThisEv_All_Stats.Runfsm);
#endif  // SSA_OPTIN_STATS_RUNFSM
    }   // for
    // present the totals computed in the same format that an individual event would see it formatted

#ifdef SSA_OPTIN_STATS_APIFSM
    // present the totals computed in the same format that an individual event would see it formatted
    ss_uiOp_qBanner  (lfY, "Statistics across all Api FSMs", lfY);
    ss_uiOp_emit_qAsciiA ("----  evapi<all>");
    ss_uiOp_emit_qAsciiA ("    (nonzero stats only)");
    ss_uiOp_emit_newline ();
    ssA_statOp_Apifsm_Show (&AllEvs_All_Stats.Apifsm);
#endif  // SSA_OPTIN_STATS_APIFSM

#ifdef SSA_OPTIN_STATS_RUNFSM
    ss_uiOp_qBanner  (lfY, "Statistics across all Run FSMs", lfY);
    ss_uiOp_emit_qAsciiA ("----  evapi<all>");
    ss_uiOp_emit_qAsciiA ("    (nonzero stats only)");
    ss_uiOp_emit_newline ();
    ssA_statOp_Runfsm_Show (&AllEvs_All_Stats.Runfsm);
#endif  // SSA_OPTIN_STATS_RUNFSM

#endif  // SSA_OPTIN_STATS_FSM
    return (eH_rHandled);
}   // H_ss_AgencyStats
// -------------------------------------------------------------------------------------------------
// Readers are encouraged to avoid the Messaging Signal Sensor Message ssUI subsystem, it's a mess.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_MsgSSM (int tkn_i, boolean moreInputTkns)
{
    boolean         IsNamedMsgSet;

    msg_ssTEA_Path ();

    IsNamedMsgSet = false;

    switch (tkn_i)
    {
        case ssUI_tkn2nd_i :

            if (!moreInputTkns)
            {
                break;                      // out of the switch, find nothing set, show help msg
            }   // if no more input follows command

            return (eH_rDefer);
        case ssUI_tkn3rd_i :
            // the token "any" in the 3rd token is used to indicate "set all fields to 'Any'".
            if (ssUI_tknIf_sP1_eq_pAA (gTknAny, gTokens[ssUI_tkn3rd_i].pAsciiA))
            {
                IsNamedMsgSet = true;
                break;                      // out of the switch, find everything set, do it
            }   // matched Any
            // the token "normal" in the 3rd token is used to indicate "set all to defaults".
            if (ssUI_tknIf_sP1_eq_pAA (gTknNormal, gTokens[ssUI_tkn3rd_i].pAsciiA))
            {
                IsNamedMsgSet = true;
                break;                      // out of the switch, find everything set, do it
            }   // matched normal
            // the token "normal" in the 3rd token is used to indicate "set all to none".
            if (ssUI_tknIf_sP1_eq_pAA (gTknNone, gTokens[ssUI_tkn3rd_i].pAsciiA))
            {
                IsNamedMsgSet = true;
                break;                      // out of the switch, find everything set, do it
            }   // matched none

            //  check for signalers/sensors/messages not being set correctly, given 5 tokens.
            if (!moreInputTkns)
            {   // no more input tokens
                if (!IsNamedMsgSet)
                {
                    return (eH_rError);         // must specify 3 fields if not named
                }
                else
                {
                    return (eH_rHandled);       // somebody matched a name and set all fields
                }
            }   // no more input tokens
            else
            {   // more input tokens
                if (IsNamedMsgSet)
                {
                    return (eH_rError);         // extra params after a name not allowed
                }
                else
                {
                    return (eH_rDefer);         // not named and there are more, defer for now
                }
            }   // more input tokens
            break;
        case ssUI_tkn4th_i :
            if (moreInputTkns)
            {
                return (eH_rDefer);
            }
            break;
        case ssUI_tkn5th_i :
            if (moreInputTkns)
            {
                return (eH_rDefer);
            }
            break;
        // this handler does not know to do with token 1, the parent command for this command parameter
        case ssUI_Cmd_tkn_i :
        default:
            return (eH_rError);
    }   // switch

    return (eH_rHandled);
}   // H_ss_MsgSSM
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Resource Ownership (RO) has a state (app or fsmDemo) and fsmDemo has substates (see definition).
// The LED and the Ascii resources use the same menu FSM for managing information about ownership.
// This set of menus is merely an editing mechanism for the data the Resource uses to decide to act.
// -------------------------------------------------------------------------------------------------
// The use of a global (but local to this FSM only) pointer variable is the FSM's memory-over-time.
// -------------------------------------------------------------------------------------------------
RO_state_t      *pRO_state;
RO_substate_t   *pRO_substate;
pAscii_t         pDesc;

eHandlerResult_t  H_ss_RO (int tkn_i, boolean moreInputTkns)
{
    if (!moreInputTkns)
    {
        pRO_state = &mesa_LED_RO_state;
        pRO_substate = &mesa_LED_RO_substate;
        pDesc = pcLED;
        H_ss_ROs_Describe (tkn_i, moreInputTkns);

        pRO_state = &mesa_Ascii_RO_state;
        pRO_substate = &mesa_Ascii_RO_substate;
        pDesc = pcAscii;
        H_ss_ROs_Describe (tkn_i, moreInputTkns);

        pRO_state = &mesa_Matrix_RO_state;
        pRO_substate = &mesa_Matrix_RO_substate;
        pDesc = pcMatrix;
        H_ss_ROs_Describe (tkn_i, moreInputTkns);

        return (eH_rHandled);
    }

    pRO_state = pRO_stateNull;
    pRO_substate = pRO_substateNull;
    pDesc = pAsciiNull;
    return (eH_rDefer);
}   // H_ss_RO
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ss_ROs_LED (int tkn_i, boolean moreInputTkns)
{
    pRO_state = &mesa_LED_RO_state;
    pRO_substate = &mesa_LED_RO_substate;
    pDesc = pcLED;
    if (!moreInputTkns)
    {
        H_ss_ROs_Describe (tkn_i, moreInputTkns);
        return (eH_rHandled);
    }
    return (eH_rDefer);
}   // H_ss_ROs_LED
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ss_ROs_Ascii (int tkn_i, boolean moreInputTkns)
{
    pRO_state = &mesa_Ascii_RO_state;
    pRO_substate = &mesa_Ascii_RO_substate;
    pDesc = pcAscii;
    if (!moreInputTkns)
    {
        H_ss_ROs_Describe (tkn_i, moreInputTkns);
        return (eH_rHandled);
    }
    return (eH_rDefer);
}   // H_ss_ROs_Ascii
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ss_ROs_Matrix (int tkn_i, boolean moreInputTkns)
{
    pRO_state = &mesa_Matrix_RO_state;
    pRO_substate = &mesa_Matrix_RO_substate;
    pDesc = pcMatrix;
    if (!moreInputTkns)
    {
        H_ss_ROs_Describe (tkn_i, moreInputTkns);
        return (eH_rHandled);
    }
    return (eH_rDefer);
}   // H_ss_ROs_Matrix
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_ROs_Describe (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (*pRO_state == mesa_RO_state_App)
    {
        ss_uiOp_emit_qAsciiA ("App owns ");
        ss_uiOp_emit_pAsciiA (pDesc);
        ss_uiOp_emit_newline ();
        return (eH_rHandled);
    }
    ss_uiOp_emit_qAsciiA ("SimpleSal owns ");
    ss_uiOp_emit_pAsciiA (pDesc);
    ss_uiOp_emit_qAsciiA (", active substates [");

    if ((pRO_state == &mesa_LED_RO_state) || (pRO_state == &mesa_Ascii_RO_state))
    {
        if (*pRO_substate & mesa_RO_substate_delay)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcDelay);
        }   // not done until all have been considered
        if (*pRO_substate & mesa_RO_substate_classic)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcClassic);
        }   // not done until all have been considered
        if (*pRO_substate & mesa_RO_substate_pace)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcPace);
        }   // not done until all have been considered
        if (*pRO_substate & mesa_RO_substate_agency)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcAgency);
        }   // not done until all have been considered
    }   // LED and Ascii

    if (pRO_state == &mesa_Matrix_RO_state)
    {
        if (*pRO_substate & mesa_RO_substate_group0)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcGroup0);
        }   // not done until all have been considered
        if (*pRO_substate & mesa_RO_substate_group1)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcGroup1);
        }   // not done until all have been considered
        if (*pRO_substate & mesa_RO_substate_shape0)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcShape0);
        }   // not done until all have been considered
        if (*pRO_substate & mesa_RO_substate_shape1)
        {
            ss_uiOp_emit_lbld_AsciiA (pcNoLabelThanks, pcShape1);
        }   // not done until all have been considered
    }   // Resource LED has groups/shapes
    ss_uiOp_emit_qAsciiA ("]");
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // H_ss_ROs_Describe
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ss_ROs_Update (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcApp))
    {
        *pRO_state = mesa_RO_state_App;
        return (eH_rHandled);
    }   // done as soon as anything matches something
    if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcfsmDemo))
    {
        *pRO_state = mesa_RO_state_SimpleSal;
        return (eH_rHandled);
    }   // done as soon as anything matches something

    if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcNone))
    {
        *pRO_state = mesa_RO_state_SimpleSal;
        *pRO_substate = mesa_RO_substate_none;
        return (eH_rHandled);
    }   // done as soon as anything matches something
    if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcAll))
    {
        *pRO_state = mesa_RO_state_SimpleSal;
        *pRO_substate = mesa_RO_substate_all;
        return (eH_rHandled);
    }   // done as soon as anything matches something

    if ((pRO_state == &mesa_LED_RO_state) || (pRO_state == &mesa_Ascii_RO_state))
    {
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcDelay))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_delay) ?
                                *pRO_substate & ~mesa_RO_substate_delay :
                                *pRO_substate | mesa_RO_substate_delay;

            return (eH_rHandled);
        }   // done as soon as anything matches something
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcClassic))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_classic) ?
                                *pRO_substate & ~mesa_RO_substate_classic :
                                *pRO_substate | mesa_RO_substate_classic;
            return (eH_rHandled);
        }   // done as soon as anything matches something
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcPace))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_pace) ?
                                *pRO_substate & ~mesa_RO_substate_pace :
                                *pRO_substate | mesa_RO_substate_pace;
            return (eH_rHandled);
        }   // done as soon as anything matches something
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcAgency))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_agency) ?
                                *pRO_substate & ~mesa_RO_substate_agency :
                                *pRO_substate | mesa_RO_substate_agency;
            return (eH_rHandled);
        }   // done as soon as anything matches something
    }
    if (pRO_state == &mesa_Matrix_RO_state)
    {
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcGroup0))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_group0) ?
                                *pRO_substate & ~mesa_RO_substate_group0 :
                                *pRO_substate | mesa_RO_substate_group0;
            return (eH_rHandled);
        }   // done as soon as anything matches something
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcGroup1))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_group1) ?
                                *pRO_substate & ~mesa_RO_substate_group1 :
                                *pRO_substate | mesa_RO_substate_group1;
            return (eH_rHandled);
        }   // done as soon as anything matches something
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcShape0))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_shape0) ?
                                *pRO_substate & ~mesa_RO_substate_shape0 :
                                *pRO_substate | mesa_RO_substate_shape0;
            return (eH_rHandled);
        }   // done as soon as anything matches something
        if (ssUI_AaIf_pP1_eq_pP2 (gTokens[tkn_i].pAsciiA, pcShape1))
        {
            *pRO_state = mesa_RO_state_SimpleSal;
            *pRO_substate = (*pRO_substate & mesa_RO_substate_shape1) ?
                                *pRO_substate & ~mesa_RO_substate_shape1 :
                                *pRO_substate | mesa_RO_substate_shape1;
            return (eH_rHandled);
        }   // done as soon as anything matches something
    }   // Resource Matrix has groups/shapes

    return (eH_rError);
}   // H_ss_ROs_Update
// =================================================================================================
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_evapi (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        ssUI_cmdZone_StackPush (ssUI_evapi);
        return (eH_rHandled);
    }

    return (eH_rDefer);
}   // H_evapi
// -------------------------------------------------------------------------------------------------
// Exit conditions possible based on Entry condition.  Initial condition from evapi is EvApi_i_NONE.
//
//   entry 2nd token: unmatched evapi name or unmatched wildcard
//          more tokens or !more tokens: exit EvApi_i: EvApi_i_NONE, error result
//   entry 2nd token: matched evapi name
//          !more tokens: EvApi_VALID, show EvApi, handle
//          more tokens:  exit EvApi_i: EvApi_VALID, defer
//   entry 2nd token: "*" wildcard, tkn3 != "clear" (moretokens is true)
//          exit EvApi_i: EvApi_i_ALL, defer
//   entry 2nd token: "*" wildcard, tkn3 == "clear" (moretokens is true)
//          exit EvApi_i: EvApi_i_ALL, handled
//
// -------------------------------------------------------------------------------------------------
// This is an example of how FSMs require data that exists across time and agency, to maintain state.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_apivar (int tkn_i, boolean moreInputTkns)
{
    int                     EvApi_i;
    ssUI_db_pEvApi_t      pEvApi;

    msg_ssTEA_Path ();

    // check token: if not asterisk, must be in the list of valid names of APIs.
    if (ssUI_tknIf_P1eqP2 (&gTknOpAstrsk, &gTokens[ssUI_tkn2nd_i]))
    {   // not a valid EvApi name, but is a wildcard used to indicate "all found"
        gThisCmds_pEvApi = ssUI_db_pEvApiNull;
    }   // if wildcard
    else
    {   // not a wildcard
        // this LookupWith loop does not care what state the EvApi is in, just want to get to data.
        // a good practice with event-spanning variables is to be sure to set an initial value,
        //   and then never change them outside of a strictly controlled access mechanism.
        gThisCmds_pEvApi = ssUI_db_pEvApiNull;
        for (EvApi_i=0; EvApi_i <= SSUI_EVAPI_ALLOC_MAX_I; EvApi_i++)
        {
            // -----------------------------------------------------------------------------------------
            // CHANGE pEvApi to refer to a specific EvApiData database entry or record.
            // -----------------------------------------------------------------------------------------
            pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i(EvApi_i);

            if (
                 ssUI_tknIf_P1eqP2 (pEvApi->pEvApiNameTkn_ssDB,  &gTokens[ssUI_tkn2nd_i])
                  ||
                 ssUI_tknIf_P1eqP2 (pEvApi->pEvApiNameTkn_App,  &gTokens[ssUI_tkn2nd_i])
               )
            {   // a valid EvApi name, indicate "one found"
                gThisCmds_pEvApi = pEvApi;
                break;  // out of for
            }   // if
        }   // for

        if (gThisCmds_pEvApi == ssUI_db_pEvApiNull)
        {
            return (eH_rError);
        }
    }   // not a wildcard character

    if (!moreInputTkns)
    {       // form of input: evapi <valid API name>  or  evapi *   (without a '=' clear reqeust)
        if (gThisCmds_pEvApi != ssUI_db_pEvApiNull)
        {
            // Show function will show access On, and then, show all other data if access is On
            ssUI_dbOp_Show_OneEvApi (gThisCmds_pEvApi);
        }   // valid EvApi_i
        else
        {
            ssUI_dbOp_Show_AllEvApis ();
        }   // wildcard EvApi_i
        return (eH_rHandled);
    }   //

    return (eH_rDefer);
}   // H_ev_apivar
// -------------------------------------------------------------------------------------------------
// "apivar" set conditions checked here to allow wildcard in "evapi <2nd token> <3rd token>".
// Only can get here when <2nd token> is a valid EvApi_i or EvApi_i_ALL, and there are more tokens,
// and finally, when the token that is there is '='.  This is the only subcommand at this time.
// -------------------------------------------------------------------------------------------------
//  tokens expected in order are <evapi> [<* or EvApiName> [params]]
//
//     IF the 2nd token (1st parameter to evapi) is not present,
//          - ssUI will change the current section of the menu subtree tree before this parse level.
//
//     IF the 2nd token is '*'
//          -- there may be a 3rd token   (called the "subcommand")
//          if the 3rd token is NOT present
//              -- the data from all EVAPIs are displayed, if accessOn
//              return: Handled
//
//     IF the 2nd token is a valid EvAPi name
//          -- there may be a 3rd token   (called the "subcommand")
//          if the 3rd token is NOT present
//              -- the data from the EvApi with that name is displayed, if accessOn
//              return: Handled
//
//     IF the 2nd token is ('*' or a valid EvAPi name) AND there is a 3rd token, the subcommand:
//          - "apivar" validated the name and set the global EvApi this subcommand references.
//          IF the 3rd token, the subcommand, is '='
//              - the EvAPI(s) will be initialized (using a rule).
//              return: handled
//          IF the 3rd token, the subcommand, is not '='
//              - the subcommand that will be handled by handlers later in the menu database.
//              return: next command
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_clearEvApi (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // getting here is the result of H_ev_apivar decoding a wildcard or <valid name>,
    // followed by a Event Handler command meaning "clear EvApi to initial valid values".
    //    <H_evapi, H_ev_apivar, H_ev_clearEvApi>  matches with <evapi><*|name><clear>
    if (gClearPending)
    {
        gClearPending = false;
        if (gThisCmds_pEvApi != ssUI_db_pEvApiNull)
        {
            ss_uiOp_emit_qAsciiA ("clearing one Event Api");
            ssUI_dbOp_Init_OneEvApi (gThisCmds_pEvApi, TIDYINIT);
        }
        else
        {
            ss_uiOp_emit_qAsciiA ("clearing all Event Api(s)");
            ssUI_dbOp_Init_AllEvApis (TIDYINIT);
        }
    }   // clear pending, complete it, then fall through to the normal return
    else
    {
        gClearPending = true;
        ss_uiOp_emit_qAsciiA ("enter the same command now, again, to confirm: clear Event Api(s)");
    }
    return (eH_rHandled);
}   // H_ev_clearEvApi
// -------------------------------------------------------------------------------------------------
// after decoding the event API chosen (one or all), evapi handlers run a subcmd (for one or for each).
// ev_ subcommand handlers are given an API data pointer for each call, which allows one or all loops.
// -------------------------------------------------------------------------------------------------
typedef eHandlerResult_t (* ev_subcmd_handler_t) (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns);

// -------------------------------------------------------------------------------------------------
// The caller to RunThis_ev_subcmd must establish the identity of the EvMgr Api (direct or wildcard).
// This function is where the wild card processing of all commands and subcommands occurs.
// The exception is the parameters to occurs/period; the ones that are math operands and operators.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t RunThis_ev_subcmd (ev_subcmd_handler_t pEv_HandlerFunc, boolean moreInputTkns)
{
    int                 EvApi_i;
    ssUI_db_pEvApi_t pEvApi;
    eHandlerResult_t    FuncReturn;

    msg_ssTEA_Path ();

    gClearPending = false;

    // ---------------------------------------------------------------------------------------------
    // all subcommand handlers operate on the data structure they are given as a parameter.
    // ---------------------------------------------------------------------------------------------
    // when the condition (!EvApi_i_ALL && !EvApi_i_NONE): the specific EvApi is processed
    // ---------------------------------------------------------------------------------------------
    if (gThisCmds_pEvApi != ssUI_db_pEvApiNull)
    {

        FuncReturn = pEv_HandlerFunc (gThisCmds_pEvApi, moreInputTkns);
        if (FuncReturn == eH_rError)
        {
            return (FuncReturn);
        }   // if

        if (FuncReturn == eH_rDefer)
        {
            return (FuncReturn);
        }   // if

        return (FuncReturn);
    }   // if specific EvApi in process

    // ---------------------------------------------------------------------------------------------
    // Note return from the end of the "if" statement block means that only here for wildcard EvApi.
    // ---------------------------------------------------------------------------------------------
    for (EvApi_i=0; EvApi_i <= SSUI_EVAPI_ALLOC_MAX_I; EvApi_i++)
    {

        // -----------------------------------------------------------------------------------------
        // CHANGE pEvApi to refer to a specific EvApiData database entry or record.
        // -----------------------------------------------------------------------------------------
        pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (EvApi_i);

        // gThisCmds_pEvApi??   value now?
        FuncReturn = pEv_HandlerFunc (pEvApi, moreInputTkns);

        if (FuncReturn == eH_rError)
        {
            return (FuncReturn);
        }   // exact match to eH_rError returned

        if (FuncReturn == eH_rDefer)        // if handler defers for any API, will do for all.
        {
            return (FuncReturn);
        }   // exact match to eH_rError returned

        // keep going, don't return error code until final (bugbugbug)
    }   // for

    return (FuncReturn);
}   // RunThis_ev_subcmd
// -------------------------------------------------------------------------------------------------
// When the subcmd run, the EvApi_i value has been validated and identifies a valid EvApi.
// Access works different from other subcommands in that it must allow accessOn while AccessOff true.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    if (!moreInputTkns)
    {
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_Space (ssUI_standard_fieldgap);
        ss_uiOp_emit_lbld_boolean (pcAccess, pEvApi->accessOn);
        ss_uiOp_emit_lbld_boolean (S("locked"), pEvApi->locked);
        ss_uiOp_emit_newline ();
        return (eH_rHandled);
    }
    // subcommand with parameters takes no action, work within named and wildcard models
    return (eH_rDefer);
}   // ev_subcmd_access
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access_on (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here,
    // only clear the EvApi on the transition from Off->On; not Off->Off, On->On, or On->Off
    pEvApi->accessOn = true;

    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (pcAccess, pEvApi->accessOn);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_access_on
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access_off (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    if (pEvApi->accessOn)
    {
        pEvApi->accessOn = false;

        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_Space (ssUI_standard_fieldgap);
        ss_uiOp_emit_lbld_boolean (pcAccess, pEvApi->accessOn);
        ss_uiOp_emit_newline ();
    }
    return (eH_rHandled);
}   // ev_subcmd_access_off
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access_include (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    pEvApi->inWildcard = true;

    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (S("included"), pEvApi->inWildcard);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_access_include
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access_exclude (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    pEvApi->inWildcard = false;

    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (S("excluded"), pEvApi->inWildcard);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_access_exclude
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access_lock (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    pEvApi->locked = true;

    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (S("locked"), pEvApi->locked);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_access_lock
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_access_unlock (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    pEvApi->locked = false;

    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (S("locked"), pEvApi->locked);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_access_unlock
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    // actual purpose of this command handler and the parameters that got us here
    if (!moreInputTkns)
    {
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_Show_Apifsm_state (pApiSig, pApiSigName (pEvApi));
        ss_uiOp_emit_newline ();
        return (eH_rHandled);
    }

    // subcommand with parameters takes no action, work within named and wildcard models
    return (eH_rDefer);
}   // ev_subcmd_apifsm
// -------------------------------------------------------------------------------------------------
// When the Agency Api FSM state is not OFF, data has been allocated within ssTEA to support that event.
// The state of the Event may change from "not existing" to "existing" then back to "not existing".
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm_on (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_OnReq, lfY);
    if (pApiSig->Apifsm_state != ssA_Apifsm_state_off)
    {
        ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_ErrorApiNotOff, lfY);
        return (eH_rError);
    }

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    // ssE will write the ssE true data pointer into pEvApi->pssTEA_ApiSig->ssE_pAboutEv
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv *before* ssE allocates in Api FSM On : "), pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
// -- within ifdef/else/endif

#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

// -- within ifdef/else/endif
        // ssE will use the pointer in pEvApi->pssTEA_ApiSig->ssE_pAboutEv to communicate with the App
    pEvApi->pssTEA_ApiSig->ssE_pAboutEv = AboutEvDB_dbOp_Get ();      // pointer to our copy
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv Get *before* Api FSM On : "), pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG

#endif  // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    // actual purpose of this command handler and the parameters that got us here
    // This is a use of the shorthand for #defining names that are easier to use than ssTEA names
    if (!ssUI_apiSignalOp_Emit (pEvApi, Signal_ApiFsm_On))
    {
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
        // ssE owns the data and the pointer in the signal to the data
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
        // ssE will use the pointer in pEvApi->pssTEA_ApiSig->ssE_pAboutEv to communicate with the App
        // The Put routine returns a proper NULL pointer so the user of the DB doesn't have to know
        pEvApi->pssTEA_ApiSig->ssE_pAboutEv = AboutEvDB_dbOp_Put (pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
        return (eH_rError);
    }

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    // ssE wrote the pointer into pEvApi->pssTEA_ApiSig->ssE_pAboutEv
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    // ssE used the pointer in pEvApi->pssTEA_ApiSig->ssE_pAboutEv
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv after ApiFSM On : "), pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG

    // see the EvApi typedef. The reason the function pointer in the About Event is set from a
    // variable in the ssUI EvApi is that ssUI abstracts more than a typical App would need to.
    // This is a result of the design of ssUI supporting N Events inherently for the App to use,
    // the pointer must be initialized in the EvApi by the App before the Agency Api FSM is ON.
    pEvApi->pssTEA_ApiSig->ssE_pAboutEv->pEvFunc = pEvApi->pEvFuncToUse;

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_Show_Apifsm_result (pApiSig, ssA_Apifsm_state_on_reset, pApiSigName (pEvApi));
    }

    return (eH_rHandled);
}   // ev_subcmd_apifsm_on
// -------------------------------------------------------------------------------------------------
// When the Agency Api FSM state is OFF, data allocated within ssTEA is not available to an Event.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm_off (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_OffReq, lfY);
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_ErrorApiNotOn, lfY);
        return (eH_rError);
    }

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, Signal_ApiFsm_Off))
    {
        return (eH_rError);
    }

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
        // ssE owns the data and the pointer in the signal to the data
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
        // ssE will use the pointer in pEvApi->pssTEA_ApiSig->ssE_pAboutEv to communicate with the App
        // The Put routine returns a proper NULL pointer so the user of the DB doesn't have to know
        pEvApi->pssTEA_ApiSig->ssE_pAboutEv = AboutEvDB_dbOp_Put (pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    pApiSig->Apifsm_state = ssA_Apifsm_state_off;

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_Show_Apifsm_result (pApiSig, ssA_Apifsm_state_off, pApiSigName (pEvApi));
    }

    return (eH_rHandled);
}   // ev_subcmd_apifsm_off

// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm_run (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    ss_uiOp_pBanner (lfN, pcMsg_AgApifsm_RunReq, lfY);
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_ErrorApiNotOn, lfY);
        return (eH_rError);
    }

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Run))
    {
        return (eH_rError);
    }

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_Show_Apifsm_result (pApiSig, ssA_Apifsm_state_on_running, pApiSigName (pEvApi));
    }

    return (eH_rHandled);
}   // ev_subcmd_apifsm_run
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm_reset (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    ss_uiOp_pBanner (lfN, pcMsg_AgApifsm_ResetReq, lfY);
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_ErrorApiNotOn, lfY);
        return (eH_rError);
    }

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Reset))
    {
        return (eH_rError);
    }

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_Show_Apifsm_result (pApiSig, ssA_Apifsm_state_on_reset, pApiSigName (pEvApi));
    }
    return (eH_rHandled);
}   // ev_subcmd_apifsm_reset
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm_validate (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    // ---------------------------------------------------------------------------------------------
    // When an event runs, it is given a pointer to user data that ssTEA doesn't know or care about.
    // An event has a chance to establish fundamental starting data when the Function is validated,
    // which is a non-FSM way of saying that the "init" state is given Agency to do this task.
    // Which is a way to say when the Event pointed to is called, it may note the Null Event Agency
    // pointer parameter as a signal that the function was called to implement the "init" state.
    // ---------------------------------------------------------------------------------------------
    ss_uiOp_pBanner (lfN, pcMsg_AgApifsm_ValidateReq, lfY);
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_ErrorApiNotOn, lfY);
        return (eH_rError);
    }

    // actual purpose of this command handler and the parameters that got us here
    // ---------------------------------------------------------------------------------------------
    // the Api Validation includes granting Agency to the Event in order to execute Init state logic.
    // ---------------------------------------------------------------------------------------------
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Validate))
    {
        return (eH_rError);
    }

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_Show_Apifsm_result (pApiSig, pApiSig->Apifsm_state, pApiSigName (pEvApi));
    }

    return (eH_rHandled);
}   // ev_subcmd_apifsm_validate
// -------------------------------------------------------------------------------------------------
void    draw_label (pAsciiA_t pDescription, pAsciiA_t pEvApiName)
{
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_Space (2);
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pEvApiName);
    ss_uiOp_emit_Dash (25);
    ss_uiOp_emit_newline ();
    ss_uiOp_pBanner (lfN, pDescription, lfN);
    ss_uiOp_emit_Space (2);
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pEvApiName);
    ss_uiOp_emit_Dash (25);
    ss_uiOp_emit_newline ();
}   // draw_label

eHandlerResult_t ev_subcmd_apifsm_state (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    // Agency Api FSM state is always a valid point of information to access or display or act on.
    ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_StateReq, lfY);

    // actual purpose of this command handler and the parameters that got us here
    draw_label (pcMsg_CurrentViewData, pApiSigName (pEvApi));
    ss_uiOp_Show_Apifsm_state (pApiSig, pApiSigName (pEvApi));

    // The Api FSM state can transition on/off only through App actions;
    // While the Api FSM is not off the Api FSM data in ssTEA may be accessed.
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_State))
    {
        return (eH_rError);
    }
    draw_label (pcMsg_UpdatedViewData, pApiSigName (pEvApi));
    // The App must separately manage the knowledge of Api FSM state, ssTEA can't reference Off Apis
    ss_uiOp_Show_Apifsm_state (pApiSig, pApiSigName (pEvApi));
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (eH_rHandled);
}   // ev_subcmd_apifsm_state
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_apifsm_stats (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
#ifdef SSA_OPTIN_STATS_FSM
#ifdef SSA_OPTIN_STATS_APIFSM
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;
    ssA_pApifsm_Stats_t pStats;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    ss_uiOp_pBanner (lfN, pcMsg_AgApifsm_statsReq, lfY);
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        ss_uiOp_pBanner (lfY, pcMsg_AgApifsm_ErrorApiNotOn, lfY);
        return (eH_rError);
    }

    // actual purpose of this command handler and the parameters that got us here
    pApiSig->pApiSigData = pApiSigDataNull;
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Stats))
    {
        return (eH_rError);
    }
    if (pApiSig->pApiSigData != pApiSigDataNull)
    {
        ss_uiOp_emit_Dash (4);
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_newline ();
        pStats = (ssA_pApifsm_Stats_t) pApiSig->pApiSigData;
        ssA_statOp_Apifsm_Show (pStats);
    }
#endif  // SSA_OPTIN_STATS_APIFSM
#endif  // SSA_OPTIN_STATS_FSM
    return (eH_rHandled);
}   // ev_subcmd_apifsm_stats
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    // actual purpose of this command handler and the parameters that got us here
    if (!moreInputTkns)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        // bugbugbug
        ss_uiOp_emit_newline ();
        return (eH_rHandled);
    }   // no more tokens

    // subcommand with parameters takes no action, work within named and wildcard models
    return (eH_rDefer);
}   // ev_subcmd_runfsm
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm_start (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }
    ss_uiOp_pBanner (lfN, pcMsg_AgRunfsm_StartReq, lfY);

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Start))
    {
        return (eH_rError);
    }

    ss_uiOp_Show_Runfsm_result (pApiSig, ssA_Runfsm_State_agencying, pApiSigName (pEvApi));

    return (eH_rHandled);
}   // ev_subcmd_runfsm_start
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm_stop (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }
    ss_uiOp_pBanner (lfN, pcMsg_AgRunfsm_StopReq, lfY);

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Stop))
    {
        return (eH_rError);
    }

    ss_uiOp_Show_Runfsm_result (pApiSig, ssA_Runfsm_State_stopped, pApiSigName (pEvApi));
    return (eH_rHandled);
}   // ev_subcmd_runfsm_stop
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm_pause (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }
    ss_uiOp_pBanner (lfN, pcMsg_AgRunfsm_PauseReq, lfY);

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Pause))
    {
        return (eH_rError);
    }

    ss_uiOp_Show_Runfsm_result (pApiSig, ssA_Runfsm_State_paused, pApiSigName (pEvApi));
    return (eH_rHandled);
}   // ev_subcmd_runfsm_pause
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm_resume (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }
    ss_uiOp_pBanner (lfN, pcMsg_AgRunfsm_ResumeReq, lfY);

    // actual purpose of this command handler and the parameters that got us here
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Resume))
    {
        return (eH_rError);
    }

    ss_uiOp_Show_Runfsm_result (pApiSig, ssA_Runfsm_State_agencying, pApiSigName (pEvApi));

    return (eH_rHandled);
}   // ev_subcmd_runfsm_resume
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm_state (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }
    // "not off" state is always a valid point of information to access or display or act on
    ss_uiOp_pBanner (lfY, pcMsg_AgRunfsm_StateReq, lfY);

    draw_label (pcMsg_CurrentViewData, pApiSigName (pEvApi));
    ss_uiOp_Show_Runfsm_state (pApiSig, pApiSigName (pEvApi));
    ss_uiOp_Show_EventData (pApiSig->ssE_pAboutEv);

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    pApiSig->pApiSigData = pApiSigDataNull;
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_State))
    {
        pApiSig->pApiSigData = pApiSigDataNull;
        return (eH_rError);
    }

    draw_label (pcMsg_UpdatedViewData, pApiSigName (pEvApi));
    ss_uiOp_Show_Runfsm_state (pApiSig, pApiSigName (pEvApi));
    ss_uiOp_Show_EventData (pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    return (eH_rHandled);
}   // ev_subcmd_runfsm_state
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_runfsm_stats (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
#ifdef  SSA_OPTIN_STATS_FSM
#ifdef  SSA_OPTIN_STATS_RUNFSM
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;
    ssA_pRunfsm_Stats_t pStats;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    if (pApiSig->Apifsm_state == ssA_Apifsm_state_off)
    {
        return (eH_rHandled);
    }
    ss_uiOp_pBanner (lfN, pcMsg_AgRunfsm_StatsReq, lfY);

    // actual purpose of this command handler and the parameters that got us here
    pApiSig->pApiSigData = pApiSigDataNull;
    if (!ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Stats))
    {
        ss_uiOp_qBanner  (lfY, "signal failed requesting Runfsm stats update", lfY);
        return (eH_rError);
    }

    if (pApiSig->pApiSigData != pApiSigDataNull)
    {
        ss_uiOp_emit_Dash (4);
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_newline ();
        pStats = (ssA_pRunfsm_Stats_t) pApiSig->pApiSigData;
        ssA_statOp_Runfsm_Show (pStats);
    }
#endif  // SSA_OPTIN_STATS_RUNFSM
#endif  // SSA_OPTIN_STATS_FSM
    return (eH_rHandled);
}   // ev_subcmd_runfsm_stats
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_occurs (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    if (!moreInputTkns)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_qAsciiA ("Occurs@");
        ss_uiOp_Show_Time (&pApiSig->ssE_pAboutEv->OccursAt, lfY);
        return (eH_rHandled);
    }

    // actual purpose of this command handler and the parameters that got us here
    // This sets up the Lvalue for the Time operand and Time Math when next token parsed.
    gpLive_MO->pLvalue = pEvApi->pTimeVar;
    if ( ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (pApiSigName (pEvApi)) )
    {
        // if we get here and there are more input tokens: has to be a time expression or time math.
        // subsequent parameters (oper1, oper2) don't know the EvApi_i in process, they refer to Live_MO.
        if (!moreInputTkns)
        {
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
            ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);

            return (eH_rHandled);
        }   // valid variable name and no more tokens, handled
        else
        {   // more tokens, but we have reset math operands, and operation
            return (eH_rDefer);
        }   // didn't handle this token in that we are not completely done with all tokens
    }
    else
    {
        // name not found?
        return (eH_rError);
    }
}   // ev_subcmd_occurs
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_occurs_oper1 (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    eHandlerResult_t    result;
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    result = H_math_oper1 (ssUI_tkn4th_i, moreInputTkns);

    if (result == eH_rHandled)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("math result (Lvalue=<operand1>) is copied to OccursAt");
#ifdef SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES
        ssUI_dbOp_Show_EvApi_pointers (pEvApi);
#endif  // SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES
        ssT_mathOp_P1getsP2
        (
            &pApiSig->ssE_pAboutEv->OccursAt,
            gpLive_MO->pLvalue
        );
    }   // math_oper1 handled

    return (result);
}   // ev_subcmd_occurs_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_op_math (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    eHandlerResult_t         result;

    msg_ssTEA_Path ();

    // the math operation for the Event operators is the same as the "math" command
    result = H_math_op_math (ssUI_tkn5th_i, moreInputTkns);

    return (result);
}   // H_ev_subcmd_op_math
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_occurs_oper2 (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    eHandlerResult_t    result;
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();
    result = H_math_op_math (ssUI_tkn6th_i, moreInputTkns);

    if (result == eH_rHandled)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("math result (Lvalue=<operand1><operator><operand2>) is copied to 'occursAt''");

        ssT_mathOp_P1getsP2
        (
            &pApiSig->ssE_pAboutEv->OccursAt,
            gpLive_MO->pLvalue
        );
    }
    return (result);
}   // H_ev_subcmd_occurs_oper2
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_period (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }

    if (!moreInputTkns)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_qAsciiA (pcPeriod);
        ss_uiOp_Show_Time (&pApiSig->ssE_pAboutEv->Period, lfY);
        return (eH_rHandled);
    }

    // actual purpose of this command handler and the parameters that got us here
    // This sets up the Lvalue for the Time operand and Time Math when next token parsed.
    gpLive_MO->pLvalue = pEvApi->pTimeVar;
    if ( ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (pApiSigName (pEvApi)) )
    {
        // if we get here and there are more input tokens: has to be a time expression or time math.
        // subsequent parameters (oper1, oper2) don't know the EvApi_i in process, they refer to Live_MO.
        if (!moreInputTkns)
        {
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
            ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);

            return (eH_rHandled);
        }   // valid variable name and no more tokens, handled
        else
        {   // more tokens, but we have reset math operands, and operation
            return (eH_rDefer);
        }   // didn't handle this token in that we are not completely done with all tokens
    }
    else
    {
        // name not found?
        return (eH_rError);
    }
}   // ev_subcmd_period
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_period_oper1 (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    eHandlerResult_t    result;
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    result = H_math_oper1 (ssUI_tkn4th_i, moreInputTkns);

    if (result == eH_rHandled)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("math result (Lvalue=<operand1>) is copied to Period");
        ssT_mathOp_P1getsP2
          (
            &pApiSig->ssE_pAboutEv->Period,
            gpLive_MO->pLvalue
          );
    }
    return (result);
}   // ev_subcmd_period_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_period_oper2 (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    eHandlerResult_t    result;
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    result = H_math_oper2 (ssUI_tkn6th_i, moreInputTkns);

    if (result == eH_rHandled)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("math result (Lvalue=<operand1><operator><operand2>) is copied to 'Period''");

        ssT_mathOp_P1getsP2
        (
            &pApiSig->ssE_pAboutEv->Period,
            gpLive_MO->pLvalue
        );
    }
    return (result);
}   // H_ev_subcmd_period_oper2
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_recurs (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    if (!pEvApi->accessOn)
    {
        return (eH_rHandled);
    }
    // actual purpose of this command handler and the parameters that got us here
    if (!moreInputTkns)
    {
        // show recurs value this EvApi
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
        ss_uiOp_emit_lbld_boolean (pcRecurs, pApiSig->ssE_pAboutEv->recurs);
        ss_uiOp_emit_newline ();
        return (eH_rHandled);
    }

    return (eH_rDefer);
}   // ev_subcmd_recurs
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_recurs_on (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    pApiSig->ssE_pAboutEv->recurs = true;
    pApiSig->ssE_pAboutEv->recursFromTrue = true;             // bugbugbug  same thing as precision

    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (pcRecurs, pApiSig->ssE_pAboutEv->recurs);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_recurs_on
// -------------------------------------------------------------------------------------------------
eHandlerResult_t ev_subcmd_recurs_off (ssUI_db_pEvApi_t pEvApi, boolean moreInputTkns)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    msg_ssTEA_Path ();

    // actual purpose of this command handler and the parameters that got us here
    pApiSig->ssE_pAboutEv->recurs = false;
    pApiSig->ssE_pAboutEv->recursFromTrue = false;            // bugbugbug  same thing as precision

    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_EvApi, pApiSigName (pEvApi));
    ss_uiOp_emit_lbld_boolean (pcRecurs, pApiSig->ssE_pAboutEv->recurs);
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // ev_subcmd_recurs_off
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_access (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access, moreInputTkns));
}   // H_ev_access
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_access_on   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access_on, moreInputTkns));
}   // H_ev_access_on
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_access_off  (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access_off, moreInputTkns));
}   // H_ev_access_off
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_access_include   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access_include, moreInputTkns));
}   // H_ev_access_include
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_access_exclude   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access_exclude, moreInputTkns));
}   // H_ev_access_exclude
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_access_lock   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access_lock, moreInputTkns));
}   // H_ev_access_lock
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_access_unlock   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_access_unlock, moreInputTkns));
}   // H_ev_access_unlock
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_Apifsm (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm, moreInputTkns));
}   // H_ev_Apifsm
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_on   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_on, moreInputTkns));
}   // H_ev_Apifsm_on
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_off  (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_off, moreInputTkns));
}   // H_ev_Apifsm_off
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_run  (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_run, moreInputTkns));
}   // H_ev_Apifsm_run
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_reset (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_reset, moreInputTkns));
}   // H_ev_Apifsm_reset
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_validate (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_validate, moreInputTkns));
}   // H_ev_Apifsm_validate
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_state (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_state, moreInputTkns));
}   // H_ev_Apifsm_state
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Apifsm_stats (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_apifsm_stats, moreInputTkns));
}   // H_ev_Apifsm_stats
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_Runfsm (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm, moreInputTkns));
}   // H_ev_Runfsm
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_Runfsm_start (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm_start, moreInputTkns));
}   // H_ev_Runfsm_start
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_Runfsm_stop (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm_stop, moreInputTkns));
}   // H_ev_Runfsm_stop
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_Runfsm_pause (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm_pause, moreInputTkns));
}   // H_ev_Runfsm_pause
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_Runfsm_resume (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm_resume, moreInputTkns));
}   // H_ev_Runfsm_resume
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Runfsm_state (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm_state, moreInputTkns));
}   // H_ev_Runfsm_state
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_Runfsm_stats (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_runfsm_stats, moreInputTkns));
}   // H_ev_Runfsm_stats
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_recurs (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_recurs, moreInputTkns));
}   // H_ev_recurs
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_recurs_on   (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_recurs_on, moreInputTkns));
}   // H_ev_recurs_on
// -------------------------------------------------------------------------------------------------
eHandlerResult_t H_ev_recurs_off  (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_recurs_off, moreInputTkns));
}   // H_ev_recurs_off
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_occurs (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_occurs, moreInputTkns));
}   // H_ev_occurs
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_occurs_oper1 (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_occurs_oper1, moreInputTkns));
}   // H_ev_occurs_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_occurs_oper2 (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_occurs_oper2, moreInputTkns));
}   // H_ev_occurs_oper2
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_period (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_period, moreInputTkns));
}   // H_ev_period
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_period_oper1 (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_period_oper1, moreInputTkns));
}   // H_ev_period_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_period_oper2 (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_period_oper2, moreInputTkns));
}   // H_ev_period_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_ev_op_math (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();
    return (RunThis_ev_subcmd (&ev_subcmd_op_math, moreInputTkns));
}   // H_ev_op_math
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Loop While and Until commands perform the commands between loop ON and loop OFF, until the
// condition occurs that will terminate the loop.  A Condition is a comparison between two Time
// values, such as "Time A < TimeB" or "TimeCurrent >= 10.30hr"; the condition is true or false,
// and when evaluated the values used in the calculation are the values AT THE TIME of evaluation.
// -------------------------------------------------------------------------------------------------
// The time of evaluation is when the "loop on" command is encountered in the command stream.
// The current values in the gToken array (tokenized input string) are pointing to "loop" and "on".
// All evaluation for math operations and execution of math operations happens only during a command.
// -------------------------------------------------------------------------------------------------
// problem: the math operations depend on the tokens from the loop while/until command.
// solution: the loop while/until command copies the (tokenized) input string, for use at this time.
// -------------------------------------------------------------------------------------------------
pAsciiA_t get_CompOp_pDesc (ssUI_math_CompOps_t CompareOperOrResult)
{
    pAsciiA_t           pDesc;

    switch (CompareOperOrResult)
    {
        case eComp_isMath       : pDesc = pcCompIsMath;     break;
        case eComp_isAfter      : pDesc = pcCompIsAfter;    break;
        case eComp_isEQorAfter  : pDesc = pcCompIsEQAfter;  break;
        case eComp_isEQ         : pDesc = pcCompIsEQ;       break;
        case eComp_isNEQ        : pDesc = pcCompIsNEQ;      break;
        case eComp_isBefore     : pDesc = pcCompIsBefore;   break;
        case eComp_isEQorBefore : pDesc = pcCompIsEQBefore; break;
        default                 : pDesc = pcFlaw;           break;
    }  // switch compare result
    return (pDesc);
}   // get_CompOp_pDesc;

#define TERMCOND_DEBUG
#ifdef TERMCOND_DEBUG
#define TermCondError_Announce(xstr) ss_uiOp_qBanner  (lfN, xstr, lfY)
#else
#define TermCondError_Announce(xstr)
#endif
// -------------------------------------------------------------------------------------------------

boolean H_loop_IsThisTermCondition (void)
{
    eHandlerResult_t    result;
    boolean             isTermCond;

    // the gTokens array will be pointing to a modified copy of the original loop command:
    //      "loop" "while|until" "=" "operand1" "operator" "operand2"
    // This makes "while" or "until" into an Lvalue as required and assumed by math operations.
    ssUI_tknOp_Restore_TermCondCmd ();
    isTermCond = true;                      // design choice to return true if error found below
    result = eH_rError;                     // result if the token is not while or until
    if (ssUI_tknIf_P1eqP2 (&gTknWhile, &gTokens[ssUI_tkn2nd_i]))
    {
        result = H_loop_while (ssUI_tkn2nd_i, true);
    }
    else
    {
        if (ssUI_tknIf_P1eqP2 (&gTknUntil, &gTokens[ssUI_tkn2nd_i]))
        {
            result = H_loop_until (ssUI_tkn2nd_i, true);
        }
    }
    if (result == eH_rError)
        { TermCondError_Announce ("loop error token 2"); return (isTermCond); }
    result = H_loop_assign (ssUI_tkn3rd_i, true);
    if (result == eH_rError)
        { TermCondError_Announce ("loop error token 3"); return (isTermCond); }
    result = H_loop_oper1 (ssUI_tkn4th_i, true);
    if (result == eH_rError)
        { TermCondError_Announce ("loop error token 4"); return (isTermCond); }
    result = H_loop_op_cmp (ssUI_tkn5th_i, true);
    if (result == eH_rError)
        { TermCondError_Announce ("loop error token 5"); return (isTermCond); }
    result = H_loop_oper2 (ssUI_tkn6th_i, false);
    if (result == eH_rError)
        { TermCondError_Announce ("loop error token 6"); return (isTermCond); }

    // now perform the evaluation of the expression created as a result of the handlers all running,
    ss_uiOp_emit_1 (Ascii_Lparen);
    ss_uiOp_emit_pAsciiA (get_CompOp_pDesc (gpLive_MO->CompareOper));
    ss_uiOp_emit_1 (Ascii_Qmark);
    ss_uiOp_emit_1 (Ascii_Colon);
    ss_uiOp_emit_qAsciiA (get_CompOp_pDesc (gpLive_MO->compResult));
    ss_uiOp_emit_1 (Ascii_Rparen);

    // while: when expression resolves to false (EQ request and NEQ result), exit loop
    // while: when expression resolves to true (NEQ request and NEQ result), continue loop
    isTermCond = false;
    switch (gpLive_MO->CompareOper)
    {
        case eComp_isMath    :
            break;          // result isMath not possible as comparison, complete switch cases
        case eComp_isAfter    :
            switch (gpLive_MO->compResult)
            {
                case eComp_isEQorBefore : isTermCond = true;  break;
                default                 : break;
            }  // switch compare result
            break;
        case eComp_isEQorAfter  :
            switch (gpLive_MO->compResult)
            {
                case eComp_isBefore     : isTermCond = true;  break;
                default                 : break;
            }  // switch compare result
            break;
        case eComp_isEQ    :
            switch (gpLive_MO->compResult)
            {
                case eComp_isNEQ        : isTermCond = true;  break;
                default                 : break;
            }  // switch compare result
            break;
        case eComp_isNEQ   :
            switch (gpLive_MO->compResult)
            {
                case eComp_isEQ         : isTermCond = true;  break;
                default                 : break;
            }  // switch compare result
            break;
        case eComp_isBefore    :
            switch (gpLive_MO->compResult)
            {
                case eComp_isEQorAfter  : isTermCond = true;  break;
                default                 : break;
            }  // switch compare result
            break;
        case eComp_isEQorBefore  :
            switch (gpLive_MO->compResult)
            {
                case eComp_isAfter      : isTermCond = true;  break;
                default                 : break;
            }  // switch compare result
            break;
    }   // switch on operator

    ss_uiOp_emit_lbld_boolean (S("terminating condition?"), isTermCond);
    ss_uiOp_emit_newline ();
    return (isTermCond);
}   // H_loop_IsThisTermCondition
// =================================================================================================
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_up (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    ssUI_cmdZone_StackPop ();

    return (eH_rHandled);
}   // H_up
// =================================================================================================

// =================================================================================================
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_cmds (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        ssUI_cmdZone_StackPush (ssUI_cmds);
        return (eH_rHandled);
    }   // just cmdZone name

    return (eH_rDefer);
}   // H_cmds
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_cmds_run (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("cmds available:");
        ss_uiOp_emit_newline ();
        ssUI_cmdsOp_Show_AllCommands ();
        return (eH_rHandled);
    }

    return (eH_rDefer);
}   // H_cmds_run
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_cmds_runName (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (moreInputTkns)
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("only one cmds sequence at a time, please");
        ss_uiOp_emit_newline ();
        return (eH_rError);
    }

    if (ssUI_cmdsOp_IfFindName_SetLinkActive (gTokens[tkn_i].pAsciiA))
    {
        ssUI_control.Cmds_i = 0;
        ssUI_control.CmdsAreRunning = true;

        ss_uiOp_emit_newline ();
        ss_uiOp_emit_Dash (40);
        ss_uiOp_emit_qAsciiA ("commands starting");
        ss_uiOp_emit_newline ();
        return (eH_rHandled);
    }
    return (eH_rError);
}   // H_cmds_runName
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_cmds_halt (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    ssUI_control.Cmds_i = 0;
    ssUI_control.CmdsAreRunning = false;

    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("commands halted");
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // H_cmds_halt
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void H_help_Show_Topics (void)
{
    ss_uiOp_emit_qAsciiA ("use 'help topics' to see the list of available <topic-names>.");
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("use 'help <topic-name>' to get specific information about <topic-name>.");
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("For example,");
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("    'help topics'   : list of topics, such as '[commands | buildOptions]");
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("    'help commands' : all commands; 'help commands evapi' : evapi subset.");
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("    'help buildOptions' : report configuration, such as 'trusted'.");
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("The list of topics is not limited to the list of command zones in ssUI.");
    ss_uiOp_emit_newline ();
}   // H_help_Show_Topics
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_help (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (moreInputTkns)
    {
        return (eH_rDefer);
    }

    H_help_Show_Topics ();
    return (eH_rHandled);
}   // H_help
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_help_Show_ByTopic (int tkn_i, boolean moreInputTkns)
{
    int     topic_i;
    pAsciiA_t           pCopy;

    msg_ssTEA_Path ();

    // the copy converts to lowercase into a buffer whose secret pointer we can use only here
    pCopy = ssUI_AaOp_Copy_gets_pP1_lc (gTokens[tkn_i].pAsciiA);

    if (ssUI_tknIf_sP1_eq_pAA (gTknHelpTopics, pCopy))
    {
        if (!moreInputTkns)
        {
            ss_uiOp_emit_qAsciiA ("<topic> choices (use without <>): ");
            for (topic_i=0; topic_i <= TOPIC_MAX_I; topic_i++)
            {
                ss_uiOp_emit_pAsciiA (pcHelpTopicNames[topic_i]);
                if (topic_i < TOPIC_MAX_I)
                {
                    ss_uiOp_emit_qAsciiA (" | ");
                }
            }
            return (eH_rHandled);
        }   // just "tokens"
        else
        {
            ss_uiOp_emit_qAsciiA ("<topics> has no parameter(s): ");
            return (eH_rError);
        }   // "tokens blah"
    }   // "tokens"

    if (ssUI_tknIf_sP1_eq_pAA (gTknHelpCommands, pCopy))
    {
        if (!moreInputTkns)
        {
            ssUI_menuOp_Show_Help_CmdsParams (pcOpAstrsk);
            return (eH_rHandled);
        }
        else
        {
            ssUI_menuOp_Show_Help_CmdsParams (gTokens[tkn_i+1].pAsciiA);
            return (eH_rHandled);
        }
    }   // "commands"

    if ( ssUI_tknIf_sP1_eq_pAA (gTknHelpParams, pCopy) )
    {
        if (!moreInputTkns)
        {
            ssUI_menuOp_Show_Help_CmdsParamHints (pcOpAstrsk);
        }
        else
        {
            ssUI_menuOp_Show_Help_CmdsParamHints (gTokens[tkn_i+1].pAsciiA);
        }
        return (eH_rHandled);
    }   // "params"

    if (ssUI_tknIf_sP1_eq_pAA (gTknHelpTimeVars, pCopy))
    {
        ssUI_dbOp_Show_TimeVars ();
        ss_uiOp_Show_TimeLegend ();
        return (eH_rHandled);
    }   // "timevars"

    ss_uiOp_emit_qAsciiA ("requested help topic not found in help database");
    ss_uiOp_emit_newline ();
    return (eH_rHandled);
}   // H_help_Show_ByTopic
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_help_Show_ByCommand (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

// bugbugbug something missing here

    return (eH_rHandled);
}   // H_help_Show_ByCommand
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The math parsing Handler FSM accepts any values for each token, using wildcards in command table.
// Each parameter has a specific handler in the table for that specific token, for that parameter.
// The typical cmdZone FSMs have the handler run only when the matching token selects the handler.
// Unlike matching-token FSMs, when the unique handler is called: 1) a token exists in that position,
// and 2) the value can be anything, because it has not been filtered by a matching token.
// -------------------------------------------------------------------------------------------------
// The math operation "x +=" and "x -=" are allowed in the token array; tokens are rearranged into a
// canonical form "x = x +" and "x = x -" when encountered, before parsing proceeds on the result.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        ssUI_cmdZone_StackPush (ssUI_math);
        return (eH_rHandled);
    }   // fully expressed

    return (eH_rDefer);
}   // H_math
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math_Lvalue (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    gpLive_MO->pLvalue = ssT_pTimeNull;      // I don't know what the current Lvalue is
    if ( ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (gTokens[tkn_i].pAsciiA) )
    {
        // if we get here and there are more input tokens: has to be a time expression or time math.
        // subsequent parameters (oper1, oper2) don't know the EvApi_i in process, they refer to Live_MO.
        if (!moreInputTkns)
        {
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
            ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);

            return (eH_rHandled);
        }   // valid variable name and no more tokens, handled
        else
        {   // more tokens, but we have reset math operands, and cleared the math operation data
            return (eH_rDefer);
        }   // didn't handle this token in that we are not completely done with all tokens
    }
    else
    {
        // name not found?
        return (eH_rError);
    }
}   // H_math_Lvalue
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math_op_assign (int tkn_i, boolean moreInputTkns)
{
    boolean PlusEqual;
    boolean MinusEqual;

    msg_ssTEA_Path ();

    if (!gpLive_MO->pLvalue || !moreInputTkns)
    {
        return (eH_rError);
    }

    PlusEqual = MinusEqual = false;
    if ( ssUI_tknIf_P1eqP2 (&gTknOpPlusEqual, &gTokens[tkn_i]) )
    {
        PlusEqual = true;
    }
    else
    {
        if ( ssUI_tknIf_P1eqP2 (&gTknOpMinusEqual, &gTokens[tkn_i]) )
        {
            MinusEqual = true;
        }
    }

    // either one or the other may be true, but of course not both
    if (PlusEqual || MinusEqual)
    {
        // convert "math Lvalue += Operand" to "math Lvalue = Lvalue + Operand".
        ssUI_tknOp_iP1_gets_iP2 (ssUI_tkn6th_i, ssUI_tkn4th_i);
        if (PlusEqual)
        {
            ssUI_tknOp_iP1_gets_gP2 (ssUI_tkn5th_i, gTknOpPlus);
        }
        else
        {
            ssUI_tknOp_iP1_gets_gP2 (ssUI_tkn5th_i, gTknOpMinus);
        }
        gToken_Ct++;
        ssUI_tknOp_iP1_gets_iP2 (ssUI_tkn4th_i, ssUI_tkn2nd_i);
        ssUI_tknOp_iP1_gets_gP2 (ssUI_tkn3rd_i, gTknOpAssign);
        gToken_Ct++;
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("<assign reorder>"));
#endif  // SSUI_OPTIN_DEBUG_TOKENS
    }   // -= or += form of assignment

    return (eH_rDefer);
}   // H_math_op_assign
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math_oper1 (int tkn_i, boolean moreInputTkns)
{
    gpLive_MO->pOperand1 = ssUI_dbOp_Get_pTimeVar_Using_VarName (gTokens[tkn_i].pAsciiA);

    msg_ssTEA_Path ();

    if (gpLive_MO->pOperand1 == ssT_gpTime_Ephemeral)
    {
        if ( !(ssT_stampOp_AsciiToBinary (gTokens[tkn_i].pAsciiA, gpLive_MO->pOperand1)) )
        {
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_qAsciiA ("Operand1 TimeStamp must be well-formed");
            return (eH_rError);
        }
    }

    if (gpLive_MO->pOperand1)
    {
        gpLive_MO->pOperand1_Name = ssUI_dbOp_Get_pName_Using_VarName (gTokens[tkn_i].pAsciiA);
        if (!moreInputTkns)
        {
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
            ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);
            return (eH_rHandled);
        }   // valid variable name and no more tokens, handled
        else
        {
            return (eH_rDefer);
        }   // didn't handle this token in that we are not completely done with all tokens
    }   // valid variable names
    else
    {   // not a variable, not a TimeStamp, error
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("No variable named <");
        ss_uiOp_emit_pAsciiA (gTokens[tkn_i].pAsciiA);
        ss_uiOp_emit_qAsciiA (">, or, it is an invalid TimeStamp");
        return (eH_rError);
    }
}   // H_math_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math_op_math (int tkn_i, boolean moreInputTkns)
{
    boolean match;

    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        return (eH_rError);
    }

    match = (gTokens[tkn_i].pAsciiA[0] == Ascii_Minus);
    if (match)
    {
        gpLive_MO->IsSubtractOp = true;
    }   // matched Ascii_Minus
    else
    {
        match = (gTokens[tkn_i].pAsciiA[0] == Ascii_Plus);
        if (match)
        {
            gpLive_MO->IsSubtractOp = false;
        }
        else
        {
            gpLive_MO->IsSubtractOp = false;
            return (eH_rError);
        }   // valid variable name and no more tokens, handled
    }   // matched Ascii_Minus

    // we know there are more tokens
    return (eH_rDefer);
}   // H_math_op_math
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math_oper2 (int tkn_i, boolean moreInputTkns)
{
    gpLive_MO->pOperand2 = ssUI_dbOp_Get_pTimeVar_Using_VarName (gTokens[tkn_i].pAsciiA);

    msg_ssTEA_Path ();

    if (gpLive_MO->pOperand2 == ssT_gpTime_Ephemeral)
    {
        if ( !( ssT_stampOp_AsciiToBinary (gTokens[tkn_i].pAsciiA, gpLive_MO->pOperand2)) )
        {
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_qAsciiA ("Operand2 TimeStamp must be well-formed");
            return (eH_rError);
        }
    }   // operand2 is ephemeral in TimeStamp form

    if (gpLive_MO->pOperand2)
    {
        gpLive_MO->pOperand2_Name = ssUI_dbOp_Get_pName_Using_VarName (gTokens[tkn_i].pAsciiA);

        ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
        ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
        ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);
        return (eH_rHandled);
    }   // valid variable names
    else
    {   // not a variable, not a TimeStamp, error
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_lbld_AsciiA (S("No Time variable named "), gTokens[tkn_i].pAsciiA);
        return (eH_rError);
    }   //
}   // H_math_oper2
// -------------------------------------------------------------------------------------------------
// This handler decodes comparison operations that can be used during "loop while" and "loop until".
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_math_op_cmp (int tkn_i, boolean moreInputTkns)
{
    boolean match = true;

    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        return (eH_rError);
    }

    switch (gTokens[tkn_i].pAsciiA[0])
    {
        case Ascii_LT       :
            // "A <= B" asks the question: "is A before or equal to B?"
            // "A < B" asks the question: "is A before B?"
            if (gTokens[tkn_i].pAsciiA[1] == Ascii_EQ)
            {   // matched '<='
                if (gTokens[tkn_i].pAsciiA[2] == Ascii_NUL)
                {
                    gpLive_MO->CompareOper = eComp_isEQorBefore;
                }
                else
                {
                    match = false;
                }
            }
            else
            {
                if (gTokens[tkn_i].pAsciiA[1] == Ascii_NUL)
                {   // matched '<'
                    gpLive_MO->CompareOper = eComp_isBefore;
                }
                else
                {
                    match = false;
                }
            }
            break;      // switch Array[0]
        case Ascii_GT       :
            // "A >= B" asks the question: "is A before or equal to B?"
            // "A > B" asks the question: "is A after B?"
            if (gTokens[tkn_i].pAsciiA[1] == Ascii_EQ)
            {   // matched '>='
                if (gTokens[tkn_i].pAsciiA[2] == Ascii_NUL)
                {
                    gpLive_MO->CompareOper = eComp_isEQorAfter;
                }
                else
                {
                    match = false;
                }
            }
            else
            {
                if (gTokens[tkn_i].pAsciiA[1] == Ascii_NUL)
                {   // matched '>'
                    gpLive_MO->CompareOper = eComp_isAfter;
                }
                else
                {
                    match = false;
                }
            }
            break;      // switch Array[0]
        case Ascii_EQ :
            // "A == B" asks the question: "is A equal to B?"
            if (gTokens[tkn_i].pAsciiA[1] == Ascii_EQ)
            {   // matched '=='
                if (gTokens[tkn_i].pAsciiA[2] == Ascii_NUL)
                {
                    gpLive_MO->CompareOper = eComp_isEQ;
                }
                else
                {
                    match = false;
                }
            }
            else
            {
                 match = false;
            }
            break;      // switch Array[0]
        case Ascii_Epoint :
            // "A != B" asks the question: "is A not equal to B?"
            if (gTokens[tkn_i].pAsciiA[1] == Ascii_EQ)
            {   // matched '!='
                if (gTokens[tkn_i].pAsciiA[2] == Ascii_NUL)
                {
                    gpLive_MO->CompareOper = eComp_isNEQ;
                }
                else
                {
                    match = false;
                }
            }
            else
            {
                 match = false;
            }
            break;      // switch Array[0]
        default:
            match = false;
            break;      // switch Array[0]
    }   // switch Array[0]

    // we know there are more tokens
    if (!match)
        return (eH_rError);
    else
    {
        return (eH_rDefer);
    }
}   // H_math_op_cmp
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        ssUI_cmdZone_StackPush (ssUI_loop);
        return (eH_rHandled);
    }

    if (!ssUI_control.CmdsAreRunning)      // command line input has no array active, can't use
    {
        ss_uiOp_emit_qAsciiA ("loop commands are only usable while performing a compiled-in loop");
        ss_uiOp_emit_newline ();
        return (eH_rError);
    }

    return (eH_rDefer);
}   // H_loop
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_on (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (moreInputTkns)
    {
        ss_uiOp_emit_qAsciiA ("extra params");
        ss_uiOp_emit_newline ();
        return (eH_rError);
    }

    ssUI_control.LoopOff_i = 0;             // point this at the "cmds off", next CMDs string is run next loop

    // the terminating condition is stated in the "loop while/until" and then evaluated each time
    // "loop on" is encountered.  The body of the loop starts where the "loop on" command is found.
    // When the command processor in an active loop condition finds "loop off", it returns to the
    // "loop on" command (recalled by ssUI_control.LoopOn_i): change ssUI_control.Cmds_i, continue.
    if (!H_loop_IsThisTermCondition ())
    {
        ssUI_control.LoopIsActive = true;       // matched "on" while off
        ssUI_control.LoopOn_i = ssUI_control.Cmds_i;
#ifdef SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_emit_1 (Ascii_Lparen);
        ss_uiOp_emit_Int_999 (ssUI_control.Cmds_i);
        ss_uiOp_emit_1 (Ascii_Rparen);
#endif  // SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_emit_qAsciiA (" loop body found");
    }
    else
    {
        if (ssUI_control.LoopOff_i == 0)
        {
            ssUI_control.LoopOff_i = ssUI_cmdsOp_Find_LoopOff (ssUI_control.Cmds_i);
        }
        if (ssUI_control.LoopOff_i == gpCmdArrayActive->CmdArray_max_i)
        {
            ss_uiOp_emit_qAsciiA (" loop body not found");
            return (eH_rError);
        }
        ssUI_control.Cmds_i = ssUI_control.LoopOff_i + 1;       // if not max, must be there
#ifdef SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_emit_1 (Ascii_Lparen);
        ss_uiOp_emit_Int_999 (ssUI_control.Cmds_i);
        ss_uiOp_emit_1 (Ascii_Rparen);
#endif  // SSUI_OPTIN_DEBUG_CMDS_I
    }

    return (eH_rHandled);
}   // H_loop_on
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_assign (int tkn_i, boolean moreInputTkns)
{
    eHandlerResult_t         result;

    msg_ssTEA_Path ();

    result = H_math_op_assign (tkn_i, moreInputTkns);

    return (result);
}   // H_loop_assign
// -------------------------------------------------------------------------------------------------
// this step in the loop command processing converts the user tokens into a complete command.
// The resulting modified token list is processed as if the User had entered it.
// This handler is invoked on the 3rd token, following "loop while|until",
// an assignment operator is inserted into the array after the token while/until.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_order (int tkn_i, boolean moreInputTkns)
{

    msg_ssTEA_Path ();

    if (tkn_i != ssUI_tkn3rd_i)
    {
        return (eH_rError);
    }
    // it is possible that the current 3rd, 4th and 5th token slots are not occupied, a Null pointer.
    // This sequence simplies shifts around the token information, subsequent parsing will find errors.
    ssUI_tknOp_iP1_gets_iP2 (ssUI_tkn6th_i, ssUI_tkn5th_i);
    ssUI_tknOp_iP1_gets_iP2 (ssUI_tkn5th_i, ssUI_tkn4th_i);
    ssUI_tknOp_iP1_gets_iP2 (ssUI_tkn4th_i, ssUI_tkn3rd_i);
    ssUI_tknOp_iP1_gets_gP2 (ssUI_tkn3rd_i, gTknOpAssign);
    gToken_Ct++;

    // On reaching the "loop off" command, the loop command processor evaluates the condition
    // specified in the original command (captured here for re-examination as loop progresses).
    ssUI_tknOp_SaveACopy_TermCondCmd ();

    return (H_loop_assign (tkn_i, moreInputTkns));
}   // H_loop_order
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_oper1 (int tkn_i, boolean moreInputTkns)
{
    eHandlerResult_t         result;

    msg_ssTEA_Path ();

    result = H_math_oper1 (tkn_i, moreInputTkns);

    return (result);
}   // H_loop_oper1
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_op_cmp (int tkn_i, boolean moreInputTkns)
{
    eHandlerResult_t         result;

    msg_ssTEA_Path ();

    result = H_math_op_cmp (tkn_i, moreInputTkns);

    return (result);
}   // H_loop_op_cmp
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_oper2 (int tkn_i, boolean moreInputTkns)
{
    eHandlerResult_t         result;

    msg_ssTEA_Path ();

    result = H_math_oper2 (tkn_i, moreInputTkns);

    return (result);
}   // H_loop_oper2
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_while (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        // display while as Lvalue   bugbugbug
        return (eH_rHandled);
    }

    gpLive_MO->pLvalue = ssT_pTimeNull;      // I don't know what the current Lvalue is
    if ( ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (gTokens[tkn_i].pAsciiA) )
    {
#ifdef SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_emit_1 (Ascii_Lparen);
        ss_uiOp_emit_Int_999 (ssUI_control.Cmds_i);
        ss_uiOp_emit_1 (Ascii_Rparen);
        ss_uiOp_emit_qAsciiA (" loop while");
#endif  // SSUI_OPTIN_DEBUG_CMDS_I
        // if we get here and there are more input tokens: has to be a time expression or time math.
        // subsequent parameters (oper1, oper2) don't know the EvApi_i in process, they refer to Live_MO.
        if (!moreInputTkns)
        {
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
            ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);

            return (eH_rHandled);
        }   // valid variable name and no more tokens, handled
        else
        {   // more tokens, but we have reset math operands, and operation
            return (eH_rDefer);
        }   // didn't handle this token in that we are not completely done with all tokens
    }
    else
    {
        // name not found?
        return (eH_rError);
    }
}   // H_loop_while
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_until (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (!moreInputTkns)
    {
        // display until as Lvalue   bugbugbug
        return (eH_rHandled);
    }

    gpLive_MO->pLvalue = ssT_pTimeNull;      // I don't know what the current Lvalue is
    if ( ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (gTokens[tkn_i].pAsciiA) )
    {
        // we only get here when there are more input tokens: has to be a time expression or time math.
        // subsequent parameters (oper1, oper2) don't know the EvApi_i in process, they refer to Live_MO.
#ifdef SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_emit_1 (Ascii_Lparen);
        ss_uiOp_emit_Int_999 (ssUI_control.Cmds_i);
        ss_uiOp_emit_1 (Ascii_Rparen);
        ss_uiOp_emit_qAsciiA (" loop until");
#endif  // SSUI_OPTIN_DEBUG_CMDS_I
        // if we get here and there are more input tokens: has to be a time expression or time math.
        // subsequent parameters (oper1 and oper2) don't know the EvApi_i in process, set the global.
        if (!moreInputTkns)
        {
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, SHOW_PREOP);
            ssUI_mathOp_Make_TimeMathWork (gpLive_MO);
            ssUI_mathOp_Show_AllMathTerms (gpLive_MO, !SHOW_PREOP);

            return (eH_rHandled);
        }   // valid variable name and no more tokens, handled
        else
        {   // more tokens, but we have reset math operands, and operation
            return (eH_rDefer);
        }   // didn't handle this token in that we are not completely done with all tokens
    }
    else
    {
        // name not found?
        return (eH_rError);
    }
}   // H_loop_until
// -------------------------------------------------------------------------------------------------
// "loop off" has complex logic at the point of occurrence: the while or until expression is evaluated.
// a proper MathOp data structure requires copying live parsed tokens for use later, such as Lvalue name.
// This FSM model has the occurrence of this state driving the math FSM using
// stored values.  Values changed between loop on/off can be evaluated as the "are we done" expression.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t  H_loop_off (int tkn_i, boolean moreInputTkns)
{
    msg_ssTEA_Path ();

    if (moreInputTkns)
    {
        return (eH_rError);
    }

    ssUI_control.LoopIsActive = false;                  // matched "off" while on
    // ssUI_control.LoopOff_i element is the one at current value
    ssUI_control.LoopOff_i = ssUI_control.Cmds_i;
    // cause incremented value of ssUI_control.Cmds_i to find ssUI_control.LoopOn_i element
    // bad style to create a dependency assumption to share
    ssUI_control.Cmds_i = ssUI_control.LoopOn_i-1;

#ifdef SSUI_OPTIN_DEBUG_CMDS_I
    ss_uiOp_emit_1 (Ascii_Lparen);
    ss_uiOp_emit_Int_999 (ssUI_control.Cmds_i);
    ss_uiOp_emit_1 (Ascii_Rparen);
#endif  // SSUI_OPTIN_DEBUG_CMDS_I

    ss_uiOp_emit_qAsciiA (" loop continues");

    return (eH_rHandled);
}   // H_loop_off

#endif  // __SSUI_MENUCORE_DEF_H

