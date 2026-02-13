/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssA_agency_def.h     data and software definitions:  ssTEA subcomponent Agency
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSA_AGENCY_DEF_H
#define __SSA_AGENCY_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Statistics Issues.note]
// -------------------------------------------------------------------------------------------------
#ifdef SSA_OPTIN_STATS_APIFSM
#define SSA_STATS_APIFSM_Incr(xstat)         (++(pEvInfo->All_Stats.Apifsm.xstat))
#else   // not SSA_OPTIN_STATS_APIFSM
#define SSA_STATS_APIFSM_Incr(xstat)
#endif  // SSA_OPTIN_STATS_APIFSM

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\Software Developer Tricks.note]    these macros make software disappear
// -------------------------------------------------------------------------------------------------
#ifdef SSA_OPTIN_STATS_RUNFSM
#define SSA_STATS_RUNFSM_EvEx_Incr(xstat)    (++(pEvInfo->All_Stats.Runfsm.EvEx.xstat))
#define SSA_STATS_RUNFSM_App_Incr(xstat)     (++(pEvInfo->All_Stats.Runfsm.App.xstat))
#else   // not SSA_OPTIN_STATS_RUNFSM
#define SSA_STATS_RUNFSM_EvEx_Incr(xstat)
#define SSA_STATS_RUNFSM_App_Incr(xstat)
#endif  // SSA_OPTIN_STATS_RUNFSM

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssA_Initialize (void)
{
    ss_macSNR_t  signalReturn;

    msg_ssTEA_Path ();

    signalReturn = ssA_InitData_RunTime ();

    return (signalReturn);
}   // ssA_Initialize
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssA_InitData_RunTime (void)
{
    ss_macSNR_t  signalReturn = ss_macSNR_OK;
// bugbugbug is there really no initialization at run-time init of ssTEA????  Alvik may require
    msg_ssTEA_Path ();

    return (signalReturn);
}   // ssA_InitData_RunTime
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// the first instruction of User's loop should be to make CurrAgencyStart variables accurate. The
// math function assumes snapshot on entry to ssT Agency from the User loop mechanism; current value
// includes everything in loop that is a cost outside ssT (update Time variables, call to Agency Root).
// -------------------------------------------------------------------------------------------------
void ssA_EvAg_RootMath_Entry (void)
{
    ssTEA_Time_cts.us_RootStart_Curr = ssT_rawCt_usNow ();
    ssTEA_Time_cts.ms_LoopStart_Curr = ssT_rawCt_msNow (ssTEA_Time_cts.us_RootStart_Curr);
}   // ssA_EvAg_RootMath_Entry (void)
// -------------------------------------------------------------------------------------------------
// we are about to return Agency to User's loop, so all Time will be attributed to the User
// or the Host until Agency Root is re-entered.  This is Time from RootFinish to LoopFinish.
// Agency Root start/finish used here only to measure linearly, within one Agency Root execution.
// -------------------------------------------------------------------------------------------------
void ssA_EvAg_RootMath_Exit (TimeUnitsBig_t  Assumed_us)
{
    TimeUnitsBig_t  x;

    if (Assumed_us > 0)
    {
        ssTEA_Time_cts.us_RootFinish_Curr = ssTEA_Time_cts.us_RootStart_Curr + Assumed_us;
        ssTEA_Time_cts.us_Total_ssT += Assumed_us;
    }
    else
    {
        ssTEA_Time_cts.us_RootFinish_Curr = ssT_rawCt_usNow ();
        x = ssT_rawCt_SubPastFromNow (ssTEA_Time_cts.us_RootFinish_Curr,  ssTEA_Time_cts.us_RootStart_Curr);
        ssTEA_Time_cts.us_Total_ssT += x;
#ifdef SST_OPTIN_TIME_DEBUG_SST_SUM
        if (x > 1000)
        {
            ss_uiOp_emit_Hex_32bits (ssTEA_Time_cts.us_Total_ssT);
            ss_uiOp_emit_newline ();
        }
#endif  // SST_OPTIN_TIME_DEBUG_SST_SUM
    }
    ssTEA_Time_cts.us_SampleCt_ssT++;

    ssTEA_Time_cts.ms_RootFinish_Curr = ssT_rawCt_msNow (ssTEA_Time_cts.us_RootFinish_Curr);
}   // ssA_EvAg_RootMath_Exit
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
// -------------------------------------------------------------------------------------------------
// When "in" any state, knowledge about the context is inherent and available to the algorithm.
// This macro fills in all the fields for the service function and leaves only the state visible.
// The savings and economy from not typing, saving, editing allows for a super-long macro name.
// -------------------------------------------------------------------------------------------------
#define ssA_ChangeStateAgencyingTo(xto_state)  \
    ssA_RunfsmOp_state_Delta (pEvInfo, pApiSigNull, ssA_Runfsm_State_agencying, xto_state)

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
// -------------------------------------------------------------------------------------------------
void ssTEA_Agency_Root (TimeUnitsBig_t MicrosSinceLast, TimeUnitsBig_t MillisSinceLast)
{
    ssE_pEvInfo_t           pEvInfo;
    ssE_pAboutEv_t          ssE_pAboutEv;
    ssE_EvType_t            EvTypeFSM_state;        // an FSM runs, handling each Type in some order.
    ssT_pTime_t             pTime;
#ifdef SSA_OPTIN_AGENCY_DEBUG_ACTION
    pAsciiA_t               pEventDesc;
#endif // SSA_OPTIN_AGENCY_DEBUG_ACTION

    static TimeUnitsBig_t   MathSumMillis = 0;

    // ---------------------------------------------------------------------------------------------
    // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
    // ---------------------------------------------------------------------------------------------
    ssA_EvAg_RootMath_Entry ();

    if (ssTEA_control.Time_state != ssTEA_state_running)
    {
        ssA_EvAg_RootMath_Exit (0);
        return;
    }

    ssT_timeOp_Add_usToTimeCurr (MicrosSinceLast);

    if (ssTEA_control.Agency_state != ssTEA_state_running)
    {
        ssA_EvAg_RootMath_Exit (0);
        return;
    }

    // ---------------------------------------------------------------------------------------------
    // ssTEA's Agency Pace Period implementation is here; following this gate is the Agency Root FSM.
    // ---------------------------------------------------------------------------------------------
    if (ssTEA_control.Agency_pace != ssTEA_Agency_pace_planck)
    {
        // -----------------------------------------------------------------------------------------
        // MathSumMillis is a true count of milliseconds SINCE the last time MathSumMillis was 0.
        // Period pacing uses the MathSumMillis; planck pacing does not; the LED trick uses this.
        // this is an example of modifying the core of ssTEA for the benefit of the App.  Do it.
        // -----------------------------------------------------------------------------------------
        MathSumMillis += MillisSinceLast;
        if (MathSumMillis >= ssTEA_control.Agency_period)
        {
            MathSumMillis = MathSumMillis - ssTEA_control.Agency_period;     // accumulate slew
#ifdef SSA_OPTIN_SIGNAL_PERIOD
            // -------------------------------------------------------------------------------------
            if (mesa_LED_RO_subState_Is (mesa_RO_substate_pace))
            {
                mesa_FSM_LEDflash_Req ();
            }
#endif  // SSA_OPTIN_SIGNAL_PERIOD
        }   // completed a period of Time measured in milliseconds
        else
        {   // did not yet complete a period of Time, so therefore
            // -------------------------------------------------------------------------------------
            // it is too soon to run Agency within this region of Time between period boundaries.
            // -------------------------------------------------------------------------------------
            ssA_EvAg_RootMath_Exit (0);
            return;
        }
        if (OK_to_Blast_Ascii_Live ())   msg_ssTEA_Path ();     // only if feasible
    }   // if not planck speed
    else
    {
        // its ALWAYS time to grant some Agency but no Ascii output is possible
    }   // planck speed

    // ---------------------------------------------------------------------------------------------
    // The Time FSM is running, The Agency FSM is running, AND the agency pace period has completed.
    // ---------------------------------------------------------------------------------------------
    ssA_EvOp_Show_EvAg_Time ("***Loop:Agency Root->Init***", ssT_gpTime_Current);

    // ---------------------------------------------------------------------------------------------
    // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
    // [.\SimpleSal\ssDocs\ssTEA\Agency Events Defined.note]
    // ---------------------------------------------------------------------------------------------
    EvTypeFSM_state = ssE_EvType_Ireq;
    pEvInfo = ssE_pEvInfoNull;
    ssA_EvOp_Show_EvAg_Time ("***Init->Ireq***", ssT_pTimeNull);

    // ---------------------------------------------------------------------------------------------
    // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
    // ---------------------------------------------------------------------------------------------
    while (true)
    {
        while ((pEvInfo = ssA_EvAg_AgencyNext (pEvInfo, EvTypeFSM_state)) != ssE_pEvInfoNull)
        {
            // -------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\ssTEA\EvInfo Data.note]
            // -------------------------------------------------------------------------------------
            ssE_pAboutEv = pEvInfo->ssE_pAboutEv;
            ssA_EvOp_Show_EvAg_State (ssE_pAboutEv);

            // -------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
            // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
            // Please be aware that TruSoft Computing LLC believes it is silly to claim a patent on
            // easily-arrived-at computer software for universal problems by claiming "invention!".
            // But, all of the time involved in SimpleSal was spent to make this useful trick work.
            // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
            // -------------------------------------------------------------------------------------
            if (ssE_pAboutEv->Runfsm_EvType == ssE_EvType_Time)
            {
                // ---------------------------------------------------------------------------------
                // This Event has an Agency Api FSM On and Running, Run FSM state of Agencying.
                // ---------------------------------------------------------------------------------
                // Grant of Agency depends on OccursAt being less than or equal to the Current Time,
                // a more human way to say: a Pending Event has an OccursAt AFTER the Current Time.
                // ----------------------------------------------------------------------------------
                if (ssT_timeIf_P1afterP2 (&ssE_pAboutEv->OccursAt, ssT_gpTime_Current))
                {
                    ssA_EvOp_Show_EvAg_Time ("waiting", &ssE_pAboutEv->OccursAt);
                    continue;   // this event instance completely handled, find the next event
                }   // time-to-occur is greater than current time
            }   // if Ireq or data type event, runs no matter what if found agencying

            // ---------------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\Mesa\ARM Function Pointers.note]
            // This is not directly part of algorithm: keep an easy mistake from crashing the processor.
            // You are trusted to NOT explicitly create a 32-bit address of zero and set the Thumb Bits.
            // ---------------------------------------------------------------------------------------------
            if (ssE_pAboutEv->pEvFunc == pEvFuncNull)
            {
                ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_stopped);
                ssE_pAboutEv->AgencyResult = ssE_EvResult_notOK_pEvFunc;
                if (ssTEA_control.Show_Cause)
                {
                    ssA_EvOp_Show_EvAg_State (ssE_pAboutEv);
                    ssA_EvOp_Show_EvAg_Time ("pEvFunc cannot be NULL here", ssT_pTimeNull);
                }
                continue;   // this event instance completely handled, find the next event
            }

#ifdef SSA_OPTIN_AGENCY_DEBUG_ACTION
            pEventDesc = ss_pFlaw;          // compiler shut up, switch handles all possibilities
            switch (ssE_pAboutEv->Runfsm_EvType)
            {
                case ssE_EvType_Ireq   : pEventDesc = ss_pIreq;    break;
                case ssE_EvType_Data   : pEventDesc = ss_pData;    break;
                case ssE_EvType_Time   : pEventDesc = ss_pTime;    break;
                case ssE_EvType_flaw   : pEventDesc = ss_pFlaw;    break;
                // should we waste processor time handling bad eventType at this late state?  No.
            }   // switch on a trusted variable
            ss_uiOp_emit_qAsciiA ("granting Agency, to Event Type <");
            ss_uiOp_emit_pAsciiA (pEventDesc);
            ss_uiOp_emit_qAsciiA (">");
            ss_uiOp_emit_newline ();
#endif  // SSA_OPTIN_AGENCY_DEBUG_ACTION

            // -----------------------------------------------------------------------------------------
            // Dominion: Giving out a pointer to the official Current Time risks corruption.
            // -----------------------------------------------------------------------------------------
            // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
            // Please be aware that TruSoft Computing LLC believes it is silly to claim a patent on
            // easily-arrived-at computer software for universal problems by claiming "invention!".
            // But, all of the time involved in SimpleSal was spent to make this useful trick work.
            // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
            // -----------------------------------------------------------------------------------------
            // Use "a pointer to a function returning a result" to call the function and get a result.
            // Pass a valid Agency Descriptor, indicating this is within RunFSM, not Init validation.
            // -----------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\Mesa\ARM Function Pointers.note]
            // -----------------------------------------------------------------------------------------
            ssE_pAboutEv->AgencyResult = ssE_pAboutEv->pEvFunc (ssE_pAboutEv, ssT_gpTime_Current);

            // -------------------------------------------------------------------------------------
            // don't count it until it is completed
            // -------------------------------------------------------------------------------------
            SSA_STATS_RUNFSM_EvEx_Incr (AgencyGranted_ct);

#if SSA_OPTIN_SIGNAL_PERIOD
            // -------------------------------------------------------------------------------------
            // this is an example of modifying the core of ssTEA for the benefit of the App: because
            // blinking the LED has nothing to do with ssTEA.  Yet here is Agency Root blinking.
            // -------------------------------------------------------------------------------------
            if (mesa_LED_RO_subState_Is (mesa_RO_substate_agency))
            {
                mesa_FSM_LEDflash_Req ();
            }
#endif  // SSA_OPTIN_SIGNAL_PERIOD            bugbugbug

            // -------------------------------------------------------------------------------------
            // Any event occured at the time requested; the event returned a result about itself.
            // Any "_notOK_" result returned causes the event's agency to be stopped by design.
            // -------------------------------------------------------------------------------------
            if (InRange_AgencyResultFailures (ssE_pAboutEv->AgencyResult))
            {
                ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_stopped);
                SSA_STATS_RUNFSM_EvEx_Incr (AbbyNormal_ct);
                ssA_EvOp_Show_EvAg_Time ("Abby Normal result", &ssE_pAboutEv->OccursAt);
                ssA_EvAg_AgencyRefresh (ssE_pAboutEv);
                continue;   // this event instance completely handled, find the next event
            }

            // -------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
            // [.\SimpleSal\ssDocs\ssTEA\Agency Events Defined.note]
            // -------------------------------------------------------------------------------------
            if (ssE_pAboutEv->Runfsm_EvType != ssE_EvType_Time)
            {   // a non-Time event that just completed Agency is paused by design
                //    UNLESS the event has already asked to be self-paused or self-stopped.
                switch (ssE_pAboutEv->AgencyResult)
                {
                    case  ssE_EvResult_OK_pauseMe :
                        ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_paused);
                        SSA_STATS_RUNFSM_EvEx_Incr (Self_PauseAg_ct);
                        break;
                    case ssE_EvResult_OK_stopMe   :
                        // an Event that stopped itself needs to allow the Run FSM to be accurate
                        ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_stopped);
                        SSA_STATS_RUNFSM_EvEx_Incr (Self_StopAg_ct);
                        break;
                    case ssE_EvResult_OK_go       :
                        // an Event that did not stop or pause itself is paused by design.
                        // If you would like to change the design, change the TO state and the counter.
                    default                       :
                        // the "InRange" above catches any "notOK" result as abnormal; only _OK_ here.
                        // the definition of result allows the user to extend and catch here.
                        ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_paused);
                        SSA_STATS_RUNFSM_EvEx_Incr (Auto_PauseAg_ct);
                        break;
                }   // switch (result) of a non-Time Event

                ssA_EvAg_AgencyRefresh (ssE_pAboutEv);
                continue;  // while event found one to manage, it completed, while more
            }   // non-Time event

            // -------------------------------------------------------------------------------------
            // Only Time Events from here to exit
            // -------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
            // [.\SimpleSal\ssDocs\ssTEA\Agency Events Defined.note]
            // -------------------------------------------------------------------------------------
            switch (ssE_pAboutEv->AgencyResult)
            {
                case  ssE_EvResult_OK_pauseMe :
                    ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_paused);
                    SSA_STATS_RUNFSM_EvEx_Incr (Self_PauseAg_ct);
                    break;          // refresh and restart while ()
                case ssE_EvResult_OK_stopMe   :
                    // an Event that stopped itself needs to allow the Run FSM to be accurate
                    ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_stopped);
                    SSA_STATS_RUNFSM_EvEx_Incr (Self_StopAg_ct);
                    break;          // refresh and restart while ()
                case ssE_EvResult_OK_go       :
                    // an agencying Event that did not stop or pause itself is still agencying
                default                             :
                    // the "InRange" above catches any "notOK" result as abnormal; only _OK_ here.
                    // the definition of result allows the user to extend and catch here.
                    ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_agencying);
                    break;              // while event found one to manage, recurrence NOT complete
            }   // switch (result) of a Time event

            // ----------------------------------------------------------------------------------------
            // At this point: a Time event was agencying, did not change or ask for change to state.
            // The path following this is the implementation of ssTEA's stated rules and features...
            // ----------------------------------------------------------------------------------------
            if (ssE_pAboutEv->Runfsm_state != ssA_Runfsm_State_agencying)
            {
                ssA_EvAg_AgencyRefresh (ssE_pAboutEv);
                continue;
            }

            // -------------------------------------------------------------------------------------
            // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
            // -------------------------------------------------------------------------------------
            if (ssE_pAboutEv->recurs)
            {
                pTime = (ssE_pAboutEv->recursFromTrue) ? ssT_gpTime_Current : &ssE_pAboutEv->OccursAt;
                ssT_mathOp_P1getsP2plusP3 (&ssE_pAboutEv->OccursAt, pTime, &ssE_pAboutEv->Period);

#ifdef SSA_OPTIN_AGENCY_DEBUG_ACTION
                pEventDesc = S("recurring event new OccursAt");
#endif // SSA_OPTIN_AGENCY_DEBUG_ACTION
                SSA_STATS_RUNFSM_EvEx_Incr (Auto_RecurrenceAg_ct);
            }   // a recurring event occured; Period added to time current for new OccursAt
            else
            {   // a not-recurring Time event occured;
                // an Event is allowed to change its OccursAt value to any Time after Current Time
                // -----------------------------------------------------------------------------------------
                // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
                // -----------------------------------------------------------------------------------------
                if (ssT_timeIf_P1equalsORbeforeP2 (&ssE_pAboutEv->OccursAt, ssT_gpTime_Current))
                {
                    ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_stopped);
                    SSA_STATS_RUNFSM_EvEx_Incr (Auto_StopAg_ct);
                    ssA_EvOp_Show_EvAg_Time ("auto stop", &ssE_pAboutEv->OccursAt);
                    ssA_EvAg_AgencyRefresh (ssE_pAboutEv);
                    continue;   // this event instance completely handled, find the next event
                }   // the event is stopped by ssTEA not the FSM itself
                else
                {   // a not-recurring Time Event changed its OccursAt to a Time in the future
                    // -------------------------------------------------------------------------------------
                    // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
                    // -------------------------------------------------------------------------------------
                    if (!ssE_pAboutEv->OccursAt.Valid)
                    {
                        ssE_pAboutEv->AgencyResult = ssE_EvResult_notOK_time;
                        SSA_STATS_RUNFSM_EvEx_Incr (AbbyNormal_ct);
                        ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_stopped);
                        ssA_EvOp_Show_EvAg_Time ("Time math error", &ssE_pAboutEv->OccursAt);
                    }   // a bad Time was had by all
                    else
                    {
                        ssA_ChangeStateAgencyingTo (ssA_Runfsm_State_agencying);
                        SSA_STATS_RUNFSM_EvEx_Incr (Self_RecurrenceAg_ct);
                    }
                }   // a not-recurring Time Event changed its OccursAt to a Time in the future

#ifdef SSA_OPTIN_AGENCY_DEBUG_ACTION
                pEventDesc = S("one-time cascading event new OccursAt, agencying continues");
#endif // SSA_OPTIN_AGENCY_DEBUG_ACTION
            }   // a one-time event

            // -----------------------------------------------------------------------------------------
            // This event made it all the way through the granting and then re-granting of Agency.
            // -----------------------------------------------------------------------------------------
#ifdef SSA_OPTIN_AGENCY_DEBUG_ACTION
            ssA_EvOp_Show_EvAg_Time (pEventDesc, &ssE_pAboutEv->OccursAt);
#endif // SSA_OPTIN_AGENCY_DEBUG_ACTION
            ssA_EvAg_AgencyRefresh (ssE_pAboutEv);
            continue;   // this event instance completely handled, find the next event

        // software engineering note:   [don't bet anything precedes end-while, it may move later]
        }   // while event instances exist of this Event type, as provided by the database

        // -------------------------------------------------------------------------------------
        // [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]
        // -------------------------------------------------------------------------------------
        if (EvTypeFSM_state == ssE_EvType_Ireq)
        {   // move from Ireq state to Data state
            EvTypeFSM_state = ssE_EvType_Data;
            pEvInfo = ssE_pEvInfoNull;      // restart the loop by signaling: I do not know anything
            ssA_EvOp_Show_EvAg_Time ("***Ireq->Data***", ssT_pTimeNull);
        }
        else
        {
            if (EvTypeFSM_state == ssE_EvType_Data)
            {   // move from Data state to Time state
                EvTypeFSM_state = ssE_EvType_Time;
                pEvInfo = ssE_pEvInfoNull;  // restart the loop by signaling: I do not know anything
                ssA_EvOp_Show_EvAg_Time ("***Data->Time***", ssT_pTimeNull);
            }
            else
            {
                if (EvTypeFSM_state == ssE_EvType_Time)
                {   // all states have occured for this planck, change to state exit Agency Root
                    ssA_EvOp_Show_EvAg_Time ("***Time->Done***", ssT_pTimeNull);
                    break;          // out of while checking each type, the only exit from while (true)
                }
                else
                {   // an invalid Event Type, unlikely but leave a spot to add code to debug if occurs
                    ssA_EvOp_Show_EvAg_Time ("***Flaw->Halt***", ssT_pTimeNull);
                    break;
                }
            }
        }   // while event Types to process are available, the only exit is when Time events all handled
    }   // while (true) that is only exited by the terminating condition of the EvTypeFSM

    ssA_EvOp_Show_EvAg_Time ("***Loop:Agency Root->Done***", ssT_gpTime_Current);
    ssA_EvOp_Show_EvAg_Time (pAsciiNull, ssT_pTimeNull);

    // Tell exit math that the number of microseconds is not assumed and needs a calculation/lookup
    ssA_EvAg_RootMath_Exit (0);
}   // ssTEA_Agency_Root
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Agency Root Algorithm.note]           Use of Next by the Root Algorithm
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Agency Next Algorithm.note]      Root's requirements for Next Algorithm
// -------------------------------------------------------------------------------------------------
ssE_pEvInfo_t ssA_EvAg_AgencyNext (ssE_pEvInfo_t pEvInfo, ssE_EvType_t  eventType)
{
    static int working_lcl_i = 0;
    ssE_pEvInfo_t return_pEvInfo = ssE_pEvInfoNull;

    if (OK_to_Blast_Ascii_Live ())   msg_ssTEA_Path ();

    if ((pEvInfo == ssE_pEvInfoNull) || (working_lcl_i > SSE_EVINFO_MAX_I))
    {   // initialize the static value used by the algorithm to address/access the data structures
        working_lcl_i = 0;
    }   // if no knowledge from before, or, knowledge from last time is the search failed last time.

#ifdef SSA_OPTIN_AGENCY_DEBUG_ADDRESS
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("&ssE_EvInfos[0] ");
    ss_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_Hex_32bits ((unsigned long) &ssE_EvInfos[0]);
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_newline ();
#endif  // SSA_OPTIN_AGENCY_DEBUG_ADDRESS

    // ssHL-note: there is no initialization portion of the "for(;;)" statement; working_lcl_i retains
    // a value during the time the function is not running, and is live and correct on our return.
    for (; working_lcl_i <= SSE_EVINFO_MAX_I; working_lcl_i++)
    {
        pEvInfo = &ssE_EvInfos[working_lcl_i];

#ifdef SSA_OPTIN_AGENCY_DEBUG_ADDRESS
        ss_uiOp_emit_qAsciiA ("comparing event Info facts @[");
        ss_uiOp_emit_Hex_32bits ((unsigned long) pEvInfo);
        ss_uiOp_emit_qAsciiA ("]");
        ss_uiOp_emit_newline ();
#endif  // SSA_OPTIN_AGENCY_DEBUG_ADDRESS
        if (pEvInfo->Alloc.active)
        {
            if (pEvInfo->Apifsm_state == ssA_Apifsm_state_on_running)
            {
                if (pEvInfo->ssE_pAboutEv->Runfsm_EvType == eventType)
                {
                    if (pEvInfo->ssE_pAboutEv->Runfsm_state != ssA_Runfsm_State_agencying)
                    {
                        ssA_mShowActionNotTaken ("not agencying");
                        continue;   // with for loop, this event instance not a match to Run FSM state
                    }
                    else
                    {   // Active, Running, Event type matches requested, Agencying: got one!
#ifdef SSA_OPTIN_AGENCY_DEBUG_SEQUENCE
                        ss_uiOp_emit_qAsciiA ("R|");
                        ss_uiOp_emit_pAsciiA (ss_uiOp_Get_pName_EvType (pEvInfo->ssE_pAboutEv->Runfsm_EvType));
                        ss_uiOp_emit_qAsciiA ("|A");
                        ss_uiOp_emit_newline ();
#endif  // SSA_OPTIN_AGENCY_DEBUG_SEQUENCE
                        working_lcl_i++;                // start next search with this
                        return_pEvInfo = pEvInfo;       // the result of the search is this data
                        break;      // out of the for loop; to return a value through common exit
                    }
                }   // matching Event type
                else
                {
                    ssA_mShowActionNotTaken ("not Type");
                    continue;   // with for loop, this event instance not a match to type requested
                }
            }   // Api FSM On and Running
            else
            {
                ssA_mShowActionNotTaken ("not onRunning");
                continue;       // with for loop, this event instance is in Off or OnReset state
            }
        }   // Api FSM data allocated and accessible
        else
        {
            ssA_mShowActionNotTaken ("not active");
            continue;       // with for loop, this event instance is not in use by anyone
        }
    }   // go through the whole database looking for any event, look at every instance

    // "for" loop exits either due to finding a match, or, not finding a match: overloads pEvInfo.
    // The actual cost of a second variable is just another register, better than tricky logic.
    return (return_pEvInfo);
}   // ssA_EvAg_AgencyNext
// -------------------------------------------------------------------------------------------------
// Like AgencyNext, AgencyRefresh hides the details of the database from Agency Root.
// -------------------------------------------------------------------------------------------------
// Is an Event database change created by an Event moving from one Run FSM state to another? Maybe.
// Is there an Event database change created by an Event moving from current to future?  Probably.
//
// for example: a data base using a sorted linked list of Event descriptors, in OccursAt Time order.
// An agencying event, whether one-time or recurring, may change its own scheduled occurrence to a
// Time in the future.  Database may move the Event in the sort order when OccursAt changes.
// -------------------------------------------------------------------------------------------------
// Cannot run this code unless Event at ssE_pAboutEv WAS agencying, just reinforcing the key points:
//    the Agency Api FSM for this Event remains in the "On and running" state.
//    the Run FSM for this Event remains in a new state, but EvAg_Agency may react to the new state.
// -------------------------------------------------------------------------------------------------
// the function that performs the database action:     ssA_EvAg_AgencyRefresh (ssE_pAboutEv);
// The purpose of call to AgencyRefresh is: signal the database of changes, efficiently.
// Compare to this: API calls pausing, updating, resuming the Run FSM, all also signals.
// -------------------------------------------------------------------------------------------------
void ssA_EvAg_AgencyRefresh (ssE_pAboutEv_t ssE_pAboutEv)
{
    if (OK_to_Blast_Ascii_Live ())
    {
         msg_ssTEA_Path ();

#ifdef SSA_OPTIN_AGENCY_DEBUG
        if (ssE_pAboutEv->Runfsm_state == ssA_Runfsm_State_agencying)
        {
            ss_uiOp_emit_qAsciiA ("refreshed, waiting@");
            ss_uiOp_Show_Time (&ssE_pAboutEv->OccursAt, lfY);
        }
#endif // SSA_OPTIN_AGENCY_DEBUG
    }
}   // ssA_EvAg_AgencyRefresh
// =================================================================================================
#ifdef SSA_OPTIN_AGENCY_DEBUG
// -------------------------------------------------------------------------------------------------
// This function is assumed to only be called within ssTEA's Agency processing.
// Must prevent queued serial output interfering with operation of device.
// -------------------------------------------------------------------------------------------------
void do_ssA_EvOp_Show_EvAg_Time (pAsciiA_t pDesc, ssT_pTime_t  pTime)
{
    if (!OK_to_Blast_Ascii_Live ())   return;
    if (pDesc)
    {
        ss_uiOp_emit_pAsciiA (pDesc);
    }
    if (pTime)
    {
        ss_uiOp_emit_1 (Ascii_AtSign);
        ss_uiOp_Show_Time (pTime, lfN);
    }
    ss_uiOp_emit_newline ();         // whether any other information was displayed or not

}   // do_ssA_EvOp_Show_EvAg_Time
// -------------------------------------------------------------------------------------------------
void do_ssA_EvOp_Show_EvAg_EvFunc (pAsciiA_t pDesc, EvAg_pEvFunc_t pEvFunc)
{
    if (!OK_to_Blast_Ascii_Live ())  return;
    if (pDesc)
    {
        ss_uiOp_emit_lbld_hex (pDesc, (unsigned long) pEvFunc);
    }
    ss_uiOp_emit_newline ();         // whether any other information was displayed or not
}   // do_ssA_EvOp_Show_EvAg_EvFunc
// -------------------------------------------------------------------------------------------------
void do_ssA_EvOp_Show_EvAg_State (ssE_pAboutEv_t ssE_pAboutEv)
{
    if (!OK_to_Blast_Ascii_Live ())   return;
// bugbugbug  what can ssA tell the user about the event? ss_uiOp_Show_RunfsmData (ssE_pAboutEv);
}   // do_ssA_EvOp_Show_EvAg_State
#endif // SSA_OPTIN_AGENCY_DEBUG

#endif // __SSA_AGENCY_DEF_H


