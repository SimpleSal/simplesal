/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_api_def.h      data and software definitions: top-level interactions with ssTEA Api.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSTEA_API_DEF_H
#define __SSTEA_API_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This software source file describes the interface for interactions between an App and ssTEA.
// ssTEA co-exists with the App but as with any software, ssTEA has a language based on words.
// The software assumes the reader has full knowledge of the words and the language; the notes
// associated explain the words and language in a natural human language (American English); the
// fsmDemo SimpleSal App used to test ssTEA contains complete references to all of the interface.
// -------------------------------------------------------------------------------------------------
pAscii_t    ssTEA_plbl_StateGood    = S("  result: <as expected>");
pAscii_t    ssTEA_plbl_StateFail    = S("  result: <not expected>");

pAscii_t    ssTEA_plbl_AgRunfsm     = S(" Agency Run FSM  :");
pAscii_t    ssTEA_plbl_AgApifsm     = S(" Agency Api FSM  :");
pAscii_t    ssTEA_plbl_AppApiName   = S(" App");
pAscii_t    ssTEA_plbl_Signal       = S("  -- signal --   :");
pAscii_t    ssTEA_plbl_Carrier      = S("  -- carrier --  :");
pAscii_t    ssTEA_plbl_Channel      = S("  -- channel --  :");

// -------------------------------------------------------------------------------------------------
// the collection of ssTEA FSMs all have an Init State.  Each is granted Agency, in turn, to start.
// -------------------------------------------------------------------------------------------------
ss_macSNR_t  ssTEA_InitState (void)
{
    ss_macSNR_t  signalReturn;

    msg_ssTEA_Path ();

    signalReturn = ssT_Initialize ();
    if (signalReturn == ss_macSNR_OK)
    {
        signalReturn = ssE_Initialize ();
        if (signalReturn == ss_macSNR_OK)
        {
            signalReturn = ssA_Initialize ();
        }
    }   // OK signal

    ssTEA_Agency_Root (0, 0);                   // Init State for Agency FSM allowed to occur

    return (signalReturn);
}   // ssTEA_InitState

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A bidirectional, request-response, signal-sensor model is used by the ssTEA APIs.  App signals
// are sensed by the API when encoded as required by the API, ssTEA processes the signal, and sends
// a signal in response through the API to the App.  The API is the signal carrier.
// The model must support the idea that in the future ssTEA will signal the App similarly.
//
// If any error occurs at any level (e.g., signal carrier, ssTEA API), this function returns false;
//    indicating to the caller that something was received and data describes what failed.  This
//    protocol responds whether or not the signal was encoded well enough to receive, some do not.
//
// ssTEA may receive a signal that is a request:
//      then send a response now via the current signal carrier, or,
//      then send a response now via "some" signal carrier, or,
//      then send a response later via "some" signal carrier, or,
//      then send a response at any time only in an error condition via "some" signal carrier, or,
//
// ssTEA may send a signal that does not expect a response, the signal protocol will handle that.
// ssTEA may receive a signal that is a response, react, and then take no further signal actions.
// -------------------------------------------------------------------------------------------------
// The bidirectional, request-response, signal-reply model used by the APIs does not expect
// to "receive a response" from the App and process it via this switch and function. Stated above.
// -------------------------------------------------------------------------------------------------
// Will ssTEA ever want to asynchronously and unidirectionally send signals?  For example,
// an App could sign up to receive resource error signals from ssTEA: call this function, please.
// It will require that ssTEA has an API signaling data structure to pass to this function.
// The receiver of the response will know how to correlate the signal to expected responses.
// -------------------------------------------------------------------------------------------------
// note that the signal data structure may contain data that was physically moved between N chips
// or computers.  For the purposes of ssTEA signaling, Memory is the carrier.  The model used to
// assign values to the signal is that the caller defines the contents, rather than having a list
// of parameters that this function conveniently copies into the data structure.
// -------------------------------------------------------------------------------------------------
boolean ssTEA_apiOp_Emit_Signal (ssTEA_pApiSig_t pApiSig)
{
    msg_ssTEA_Path ();

    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    switch (pApiSig->sigDir)
    {
        case ss_ApiSigDir_ToApi :
        {
            switch (pApiSig->sigValue)
            {
                case ss_ApiSigMsgValue_AgApifsm_On              :
                    pApiSig->apiSNR = ssA_ApifsmOp_agencyOn (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgApifsm_Run             :
                    pApiSig->apiSNR = ssA_ApifsmOp_agencyRun (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgApifsm_Reset           :
                    pApiSig->apiSNR = ssA_ApifsmOp_agencyReset (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgApifsm_Off             :
                    pApiSig->apiSNR = ssA_ApifsmOp_agencyOff (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgApifsm_Validate        :
                    pApiSig->apiSNR = ssA_ApifsmOp_validate_SigData (pApiSig);
                    if (pApiSig->apiSNR == ss_apiSNR_OK)
                    {
                        pApiSig->apiSNR = ssA_ApifsmOp_validate_pEvFunc (pApiSig);
                        if (pApiSig->apiSNR == ss_apiSNR_OK)
                        {
                            pApiSig->apiSNR = ssA_ApifsmOp_validate_completed (pApiSig);
                        }
                    }   // state good, data good, check function
                    // when validate returns NOT_OK, this case finishes with a NOT OK apiSNR value
                    break;
                case ss_ApiSigMsgValue_AgApifsm_State           :
                    pApiSig->apiSNR = ssA_ApifsmOp_state_Get (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgApifsm_Stats           :
                    pApiSig->apiSNR = ssA_ApifsmOp_stats_Get (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgRunfsm_Start           :
                    pApiSig->apiSNR = ssA_RunfsmOp_start (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgRunfsm_Pause           :
                    pApiSig->apiSNR = ssA_RunfsmOp_pause (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgRunfsm_Resume          :
                    pApiSig->apiSNR = ssA_RunfsmOp_resume (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgRunfsm_Stop            :
                    pApiSig->apiSNR = ssA_RunfsmOp_stop (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgRunfsm_State           :
                    pApiSig->apiSNR = ssA_RunfsmOp_state_Get (pApiSig);
                    break;
                case ss_ApiSigMsgValue_AgRunfsm_Stats           :
                    pApiSig->apiSNR = ssA_RunfsmOp_stats_Get (pApiSig);
                    break;
                case ss_ApiSigMsgValue_flaw                     :
                default                                 :
                    pApiSig->apiSNR = ss_apiSNR_signalInvalid;
                    ss_uiOp_emit_qAsciiA ("   switch on signal request received: no matching case");
                    break;
            }   // switch value
            break;          // go on to formulate the response to the service request
        }   // case request

        case ss_ApiSigDir_FromApi :
        {   // case response
            switch (pApiSig->sigValue)
            {
                case ss_ApiSigMsgValue_Yacked   :
                    break;
                case ss_ApiSigMsgValue_Nacked   :
                    break;
                case ss_ApiSigMsgValue_flaw        :
                default                    :
                    pApiSig->macSNR = ss_macSNR_signalInvalid;
                    pApiSig->apiSNR = ss_apiSNR_signalInvalid;
                    ss_uiOp_emit_qAsciiA ("   switch on signal response received: flaw or no matching case");
                    break;
            }   // switch value
            return (false);     // note no response to receiving a response
        }   // case response

        case ss_ApiSigDir_flaw :
        default :
            pApiSig->macSNR = ss_macSNR_signalInvalid;
            pApiSig->apiSNR = ss_apiSNR_signalInvalid;
            ss_uiOp_emit_qAsciiA ("   switch on signal direction: flaw or no matching case");
            break;
    }   // switch type

    // If a request was received and handled, the signal must have been valid, API error or not.
    // every requested service that encountered an error or problem has assigned a value to apiSNR.
    if (pApiSig->sigDir == ss_ApiSigDir_ToApi)
    {
        pApiSig->sigDir = ss_ApiSigDir_FromApi;
        pApiSig->macSNR = ss_macSNR_OK;
        pApiSig->sigValue = (pApiSig->apiSNR == ss_apiSNR_OK) ? ss_ApiSigMsgValue_Yacked : ss_ApiSigMsgValue_Nacked;
    }   // request received, generate response

    return ((pApiSig->macSNR == ss_macSNR_OK) && (pApiSig->apiSNR == ss_apiSNR_OK));
}   // ssTEA_apiOp_Emit_Signal
// -------------------------------------------------------------------------------------------------
// This function assigns valid values to all of the fields in an EvApi parameters data structure,
// in preparation for issuing Api FSM signals to get the active Agency Api FSM to run/off/on/reset.
// The Agency Api FSM's initial state is Off.  Start with a request to turn the Agency Api FSM on.
// -------------------------------------------------------------------------------------------------
void ssTEA_dbOp_Init_SigData (ssTEA_pApiSig_t pApiSig, boolean doDeepInit)
{
    msg_ssTEA_Path ();

}   // ssTEA_dbOp_Init_SigData
// -------------------------------------------------------------------------------------------------
// This ssTEA display function only uses the basic "display this string" and bracketing helpers.
// -------------------------------------------------------------------------------------------------
pAscii_t ssTEA_apiOp_Get_pApiMessage (ssTEA_pApiSig_t pApiSig)
{
    pAscii_t    pLabel;

    switch (pApiSig->sigValue)
    {
        case ss_ApiSigMsgValue_AgApifsm_On       :  pLabel = S("agApiFSM on    ");   break;
        case ss_ApiSigMsgValue_AgApifsm_Run      :  pLabel = S("agApiFSM run   ");   break;
        case ss_ApiSigMsgValue_AgApifsm_Reset    :  pLabel = S("agApiFSM reset ");   break;
        case ss_ApiSigMsgValue_AgApifsm_Off      :  pLabel = S("agApiFSM off   ");   break;
        case ss_ApiSigMsgValue_AgApifsm_Validate :  pLabel = S("agApiFSM valid8");   break;
        case ss_ApiSigMsgValue_AgApifsm_State    :  pLabel = S("agApiFSM state ");   break;
        case ss_ApiSigMsgValue_AgApifsm_Stats    :  pLabel = S("agApiFSM stats ");   break;

        case ss_ApiSigMsgValue_AgRunfsm_Start    :  pLabel = S("agRunFSM start ");   break;
        case ss_ApiSigMsgValue_AgRunfsm_Pause    :  pLabel = S("agRunFSM pause ");   break;
        case ss_ApiSigMsgValue_AgRunfsm_Resume   :  pLabel = S("agRunFSM resume");   break;
        case ss_ApiSigMsgValue_AgRunfsm_Stop     :  pLabel = S("agRunFSM stop  ");   break;
        case ss_ApiSigMsgValue_AgRunfsm_State    :  pLabel = S("agRunFSM state ");   break;
        case ss_ApiSigMsgValue_AgRunfsm_Stats    :  pLabel = S("agRunFSM stats ");   break;

        case ss_ApiSigMsgValue_Yacked            :  pLabel = S("Y ack          ");   break;
        case ss_ApiSigMsgValue_Nacked            :  pLabel = S("N ack          ");   break;
        case ss_ApiSigMsgValue_flaw              :  pLabel = ss_pFlaw;               break;
        default                                  :  pLabel = ss_pError;              break;
    }   // switch
    return (pLabel);
}   // ssTEA_apiOp_Get_pApiMessage
// -------------------------------------------------------------------------------------------------
// This ssTEA display function only uses the basic "display this string" and bracketing helpers.
// -------------------------------------------------------------------------------------------------
void ssTEA_apiOp_Show_SignalData (ssTEA_pApiSig_t pApiSig)
{
    pAscii_t        pLabel;

    msg_ssTEA_Path ();

    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_emit_pAsciiA (ssTEA_plbl_Signal);
        ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
        ss_uiOp_emit_lbld_AsciiA (S("message"), ssTEA_apiOp_Get_pApiMessage (pApiSig));

        if (ssTEA_control.Show_Cause)
        {
            switch (pApiSig->sigDir)
            {
                case ss_ApiSigDir_ToApi             :  pLabel = S("req");               break;
                case ss_ApiSigDir_FromApi           :  pLabel = S("rsp");               break;
                case ss_ApiSigDir_flaw              :  pLabel = ss_pFlaw;               break;
                default                             :  pLabel = ss_pError;              break;
            }
            ss_uiOp_emit_lbld_AsciiA (S("dir"), pLabel);
        }   // full

        if (ssTEA_control.Show_Cause)
        {
            switch (pApiSig->macSNR)
            {
                case ss_macSNR_OK                  :  pLabel = S("good");              break;
                case ss_macSNR_signalInvalid       :  pLabel = S("!OK ");              break;
                case ss_macSNR_MACerror            :  pLabel = S("!MAC");              break;
                case ss_macSNR_notOK_reason1       :  pLabel = S("!OK1");              break;
                case ss_macSNR_notOK_reason2       :  pLabel = S("!OK2");              break;
                case ss_macSNR_flaw                :  pLabel = ss_pFlaw;               break;
                default                            :  pLabel = ss_pError;              break;
            }
            ss_uiOp_emit_lbld_AsciiA (S("macSNR"), pLabel);

            switch (pApiSig->apiSNR)
            {
                case ss_apiSNR_OK                  :  pLabel = S("all is true     ");  break;
                case ss_apiSNR_dataBad_Type        :  pLabel = S("data:bad type   ");  break;
                case ss_apiSNR_dataBad_Time        :  pLabel = S("data:bad Time   ");  break;
                case ss_apiSNR_dataBad_pEvFunc     :  pLabel = S("data:bad pEvFunc");  break;
                case ss_apiSNR_dataBad_Incomplete  :  pLabel = S("data:incomplete ");  break;
                case ss_apiSNR_databad_Channel     :  pLabel = S("data:bad channel");  break;
                case ss_apiSNR_stateWrong          :  pLabel = S("state wrong     ");  break;
                case ss_apiSNR_stateInvalid        :  pLabel = S("state invalid   ");  break;
                case ss_apiSNR_signalInvalid       :  pLabel = S("signal invalid  ");  break;
                case ss_apiSNR_resourceMemory      :  pLabel = S("Memory Error    ");  break;
                case ss_apiSNR_resourceTime        :  pLabel = S("Time Error      ");  break;
                case ss_apiSNR_EvExResult          :  pLabel = S("Event Exec Error");  break;
                case ss_apiSNR_noEvAgencyData      :  pLabel = S("Event Data Alloc");  break;
                case ss_apiSNR_flaw                :  pLabel = ss_pFlaw;               break;
                default                            :  pLabel = ss_pError;              break;
            }
            ss_uiOp_emit_lbld_AsciiA (S("apiSNR"), pLabel);
        }   // describe it all
        ss_uiOp_emit_newline ();
    }   // at least the message, if not it all
} // ssTEA_apiOp_Show_SignalData
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
pAscii_t ss_uiOp_Get_pName_ssState (ssTEA_state_t state)
{
    switch (state)
    {
        case ssTEA_state_stopped    :  return (ss_pStopped);
        case ssTEA_state_running    :  return (ss_pRunning);
        case ssTEA_state_error      :  return (ss_pError);
        case ssTEA_state_flaw       :
        default                     :  return (ss_pFlaw);
    }   // switch state
}   // ss_uiOp_Get_pName_ssState
// -------------------------------------------------------------------------------------------------
pAscii_t ss_uiOp_Get_pName_ssAgency_Pace (ssTEA_Agency_Pace_t Agency_pace)
{
    switch (Agency_pace)
    {
        case ssTEA_Agency_Pace_planck    :  return (ss_pPlanck);
        case ssTEA_Agency_Pace_period    :  return (ss_pPeriod);
        case ssTEA_Agency_Pace_error     :  return (ss_pError);
        case ssTEA_Agency_Pace_flaw      :
        default                         :  return (ss_pFlaw);
    }   // switch pace
    return (ss_pFlaw);
}   // ss_uiOp_Get_pName_ssAgency_Pace
// -------------------------------------------------------------------------------------------------
pAscii_t  ss_uiOp_Get_pName_Apifsm_state (ssA_Apifsm_state_t Apifsm_state)
{
    switch (Apifsm_state)
    {
        case ssA_Apifsm_state_off         : return (ss_pOff);
        case ssA_Apifsm_state_on_running  : return (ss_pOnRunning);
        case ssA_Apifsm_state_on_reset    : return (ss_pOnReset);
        case ssA_Apifsm_state_flaw        :
        default                             :  return (ss_pFlaw);
    }   // switch event desc API state
    return (ss_pFlaw);
}   // ss_uiOp_Get_pName_Apifsm_state
// -------------------------------------------------------------------------------------------------
pAscii_t  ss_uiOp_Get_pName_Runfsm_state (ssA_Runfsm_state_t Runfsm_state)
{
    switch (Runfsm_state)
    {
        case ssA_Runfsm_State_paused        : return (ss_pPaused);
        case ssA_Runfsm_State_stopped       : return (ss_pStopped);
        case ssA_Runfsm_State_agencying     : return (ss_pAgencying);
        case ssA_Apifsm_state_flaw        :
        default                             : return (ss_pFlaw);
    }   // switch
    return (ss_pFlaw);
}   // ss_uiOp_Get_pName_Runfsm_state
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_ssAgency_state  (void)
{
    ss_uiOp_qBanner (lfY, "Agency Root FSM: ", lfN);
    ss_uiOp_emit_lbld_AsciiA (ss_pState, ss_uiOp_Get_pName_ssState (ssTEA_control.Agency_state));
    ss_uiOp_emit_lbld_AsciiA (ss_pPace, ss_uiOp_Get_pName_ssAgency_Pace (ssTEA_control.Agency_pace));
    if (ssTEA_control.Agency_pace == ssTEA_Agency_Pace_period)
    {
        ss_uiOp_emit_lbld_int (ss_pPeriod, ssTEA_control.Agency_period);
    }
    ss_uiOp_qBanner (lfY, "@Time: ", lfN);
    ss_uiOp_Show_Time (ssT_gpTime_Current, lfY);
}   // ss_uiOp_Show_ssAgency_state
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_ssTime_state (void)
{
    ss_uiOp_qBanner (lfY, "Time FSM: ", lfN);
    ss_uiOp_emit_lbld_AsciiA (ss_pState, ss_uiOp_Get_pName_ssState (ssTEA_control.TimeState));
    ss_uiOp_qBanner (lfY, "@Time:", lfN);
    ss_uiOp_Show_Time (ssT_gpTime_Current, lfY);
}   // ss_uiOp_Show_ssTime_state
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_ssTimeRatios (void)
{
    // there are a lot of variables with similar names thrown around in here, separate these locals.
    // Only "last register read" is static to make the data accessible across multiple calls.
    static  TimeUnitsBig_t  lcl_us_lastMesaRegisterRead = 0;
            TimeUnitsBig_t  lcl_us_currMesaRegisterRead = 0;
            TimeUnitsBig_t  lcl_us_ssTimeThisPeriod;
            TimeUnitsBig_t  lcl_us_total_of_totals;

    ss_uiOp_Show_ssTime_state ();

    if (ssTEA_Time_cts.us_SampleCt_Host == 0)  ssTEA_Time_cts.us_SampleCt_Host++;
    if (ssTEA_Time_cts.us_SampleCt_Loop == 0)  ssTEA_Time_cts.us_SampleCt_Loop++;
    if (ssTEA_Time_cts.us_SampleCt_ssT == 0)   ssTEA_Time_cts.us_SampleCt_ssT++;

    ss_uiOp_qBanner (lfN, "Time analysis since the last 'ss time stats' (us):", lfY);
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_int ("  Host Time   (outside loop)       ",
                            (ssTEA_Time_cts.us_Total_Host));
    ss_uiOp_emit_lbld_int ("  average this period ",
                            (ssTEA_Time_cts.us_Total_Host / ssTEA_Time_cts.us_SampleCt_Host));
    ss_uiOp_emit_newline ();

    ss_uiOp_emit_lbld_int ("  App Time    (inside loop)        ",
                            (ssTEA_Time_cts.us_Total_Loop - ssTEA_Time_cts.us_Total_ssT));
    ss_uiOp_emit_lbld_int ("                      ",
        ((ssTEA_Time_cts.us_Total_Loop - ssTEA_Time_cts.us_Total_ssT) / ssTEA_Time_cts.us_SampleCt_Loop));
    ss_uiOp_emit_newline ();

    ss_uiOp_emit_lbld_int ("  ssTEA Time  (inside loop)        ",
                            (ssTEA_Time_cts.us_Total_ssT));
    ss_uiOp_emit_lbld_int ("                      ",
                            (ssTEA_Time_cts.us_Total_ssT / ssTEA_Time_cts.us_SampleCt_ssT));
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_Space (36);
    ss_uiOp_emit_Dash (11);
    ss_uiOp_emit_newline ();

    lcl_us_total_of_totals = ssTEA_Time_cts.us_Total_Host + ssTEA_Time_cts.us_Total_Loop;
    ss_uiOp_emit_lbld_int ("  accumulated Time           total ", lcl_us_total_of_totals);
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_newline ();

    lcl_us_currMesaRegisterRead = ssT_rawCt_usNow ();
    lcl_us_ssTimeThisPeriod = ssT_rawCt_SubPastFromNow (lcl_us_currMesaRegisterRead, lcl_us_lastMesaRegisterRead);
    lcl_us_lastMesaRegisterRead = lcl_us_currMesaRegisterRead;

    ss_uiOp_emit_lbld_int ("  'reported by mesa' us counter    ", lcl_us_ssTimeThisPeriod);
    ss_uiOp_emit_newline ();

    if (lcl_us_ssTimeThisPeriod > lcl_us_total_of_totals)
    {
        ss_uiOp_emit_lbld_int ("  Time accumulation delta  -under- ",
               ssT_rawCt_SubPastFromNow (lcl_us_ssTimeThisPeriod, lcl_us_total_of_totals));
    }
    else
    {
        ss_uiOp_emit_lbld_int ("  Time accumulation delta  -over-  ",
               ssT_rawCt_SubPastFromNow (lcl_us_total_of_totals, lcl_us_ssTimeThisPeriod));
    }
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_int (
                         "  Seconds since the last 'Time Stats', based on reported Time (sc):",
                         (lcl_us_ssTimeThisPeriod / USECS_PER_SEC)
                        );
    ss_uiOp_emit_newline ();

    // a read of the register clears the accumulated values (not in mesa) to the value 0
    ssTEA_Time_cts.us_Total_Host   = 0;
    ssTEA_Time_cts.us_SampleCt_Host = 0;
    ssTEA_Time_cts.us_Total_Loop    = 0;
    ssTEA_Time_cts.us_SampleCt_Loop  = 0;
    ssTEA_Time_cts.us_Total_ssT    = 0;
    ssTEA_Time_cts.us_SampleCt_ssT = 0;
}   // ss_uiOp_Show_ssTimeRatios
// -------------------------------------------------------------------------------------------------
// this is at the level of the Agency Api FSM, after a signal transaction has occured.
// The state expected is the state of this event's Agency Api FSM.
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_Apifsm_result (ssTEA_pApiSig_t pApiSig, ssA_Apifsm_state_t ExpectedState, pAsciiA_t pApiSigName)
{
    pAsciiA_t   pName_Apifsm_state = ss_uiOp_Get_pName_Apifsm_state (pApiSig->Apifsm_state);

    ss_uiOp_emit_pAsciiA (ssTEA_plbl_AgApifsm);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
    ss_uiOp_emit_lbld_AsciiA (ssTEA_plbl_AppApiName, pApiSigName);
    ss_uiOp_emit_lbld_AsciiA (ss_pState, pName_Apifsm_state);

    ss_uiOp_pBanner
      (
        lfN,
        ((pApiSig->Apifsm_state == ExpectedState) ?
            ssTEA_plbl_StateGood : ssTEA_plbl_StateFail),
        lfY
      );
}   //  ss_uiOp_Show_Apifsm_result
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_Apifsm_state (ssTEA_pApiSig_t pApiSig, pAsciiA_t pApiSigName)
{
    ss_uiOp_emit_pAsciiA (ssTEA_plbl_AgApifsm);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
    ss_uiOp_emit_lbld_AsciiA (ss_pState, ss_uiOp_Get_pName_Apifsm_state (pApiSig->Apifsm_state));
    ss_uiOp_emit_newline ();
}   // ss_uiOp_Show_Apifsm_state
// -------------------------------------------------------------------------------------------------
// this is at the level of the Agency Run FSM, after a signal transaction has occured.
// The state expected is the state of this event's Agency Run FSM.
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_Runfsm_result (ssTEA_pApiSig_t pApiSig, ssA_Runfsm_state_t ExpectedState, pAsciiA_t pApiSigName)
{
    ss_uiOp_emit_pAsciiA (ssTEA_plbl_AgRunfsm);
    ss_uiOp_emit_lbld_AsciiA (ssTEA_plbl_AppApiName, pApiSigName);
    ss_uiOp_emit_lbld_AsciiA (ss_pState, ss_uiOp_Get_pName_Runfsm_state (pApiSig->ssE_pAboutEv->Runfsm_state));
    ss_uiOp_emit_qAsciiA ("  result of Agency Run FSM signal");
    ss_uiOp_emit_newline ();

    ss_uiOp_pBanner
      (
        lfN,
        ((pApiSig->ssE_pAboutEv->Runfsm_state == ExpectedState) ?
            ssTEA_plbl_StateGood : ssTEA_plbl_StateFail),
        lfY
      );
}   //  ss_uiOp_Show_Runfsm_result
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_Runfsm_state (ssTEA_pApiSig_t pApiSig, pAsciiA_t pApiSigName)
{
    ss_uiOp_emit_pAsciiA (ssTEA_plbl_AgRunfsm);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
    ss_uiOp_emit_lbld_AsciiA (ssTEA_plbl_AppApiName, pApiSigName);
    ss_uiOp_emit_lbld_AsciiA (ss_pState, ss_uiOp_Get_pName_Runfsm_state (pApiSig->ssE_pAboutEv->Runfsm_state));
    ss_uiOp_emit_newline ();
}   // ss_uiOp_Show_Runfsm_state
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_AgencyResult (ssE_EvResult_t AgencyResult)
{
    pAscii_t        pLabel;

    msg_ssTEA_Path ();

    pLabel = pAsciiANull;
    switch (AgencyResult)
    {
        case ssE_EvResult_OK_go             :  pLabel = S("normal      ");  break;
        case ssE_EvResult_OK_stopMe         :  pLabel = S("stop self   ");  break;
        case ssE_EvResult_OK_pauseMe        :  pLabel = S("pause self  ");  break;
        case ssE_EvResult_notOK_halt        :  pLabel = S("stop halt  !");  break;
        case ssE_EvResult_notOK_data        :  pLabel = S("data error !");  break;
        case ssE_EvResult_notOK_time        :  pLabel = S("time error !");  break;
        case ssE_EvResult_notOK_pEvFunc     :  pLabel = S("func error !");  break;
        case ssE_EvResult_notOK_state       :  pLabel = S("state error!");  break;
        case ssE_EvResult_notOK_reset       :  pLabel = S("reset      !");  break;
        case ssE_EvResult_notOK_flaw        :  pLabel = S("flaw       !");  break;
        default                             :  pLabel = S("userdef    !");  break;
    }   // switch
    ss_uiOp_emit_lbld_AsciiA (S("result"), pLabel);
}   // ss_uiOp_Show_AgencyResult
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_EventData (ssE_pAboutEv_t ssE_pAboutEv)
{
    pAscii_t        pLabel;

    msg_ssTEA_Path ();

    ss_uiOp_emit_pAsciiA (ss_EventAgency_Announce);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);

    if (ssE_pAboutEv == ssE_pAboutEvNull)
    {
        ss_uiOp_emit_qAsciiA ("not allocated");
        ss_uiOp_emit_newline ();
        return;
    }
// #define SHOW_EVAGENCY_ADDRESS
#ifdef SHOW_EVAGENCY_ADDRESS
    ss_uiOp_emit_lbld_hex ("@", (unsigned long) ssE_pAboutEv);
#endif  // SHOW_EVAGENCY_ADDRESS

    switch (ssE_pAboutEv->Runfsm_EvType)
    {
        case ssE_EvType_Time     :   pLabel = ss_pTime;    break;
        case ssE_EvType_Data     :   pLabel = ss_pData;    break;
        case ssE_EvType_Ireq     :   pLabel = ss_pIreq;    break;
        case ssE_EvType_flaw     :   pLabel = ss_pFlaw;    break;
        default                  :   pLabel = ss_pError;   break;
    }   // switch
    ss_uiOp_emit_lbld_AsciiA (S("EvType"), pLabel);

    switch (ssE_pAboutEv->Runfsm_EvType)
    {
        case ssE_EvType_Time     :
            ss_uiOp_emit_qAsciiA ("Occurs@");
            ss_uiOp_emit_1 (Ascii_Larrow);
            ss_uiOp_Show_Time (&ssE_pAboutEv->OccursAt, lfN);
            ss_uiOp_emit_1 (Ascii_Rarrow);      // bugbugbug?
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_newline ();

            ss_uiOp_emit_pAsciiA (ss_EventAgencyBlank_Announce);
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_qAsciiA ("recurs");
            ss_uiOp_emit_1 (Ascii_Larrow);
            ss_uiOp_emit_1 ((ssE_pAboutEv->recurs) ? Ascii_y : Ascii_n);
            ss_uiOp_emit_1 (Ascii_Rarrow);
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_lbld_time ("    period", &ssE_pAboutEv->Period);
            ss_uiOp_emit_lbld_hex (ss_pEvFunc, (unsigned long) ssE_pAboutEv->pEvFunc);
            ss_uiOp_emit_newline ();
            break;
        case ssE_EvType_Data     :
            ss_uiOp_emit_qAsciiA ("Occurs@<next pace period>");
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_lbld_hex (ss_pEvFunc, (unsigned long) ssE_pAboutEv->pEvFunc);
            ss_uiOp_emit_newline ();
            break;
        case ssE_EvType_Ireq     :
            ss_uiOp_emit_lbld_hex (ss_pEvFunc, (unsigned long) ssE_pAboutEv->pEvFunc);
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_qAsciiA ("Occurs@<this pace period>");
            ss_uiOp_emit_newline ();
            break;
        case ssE_EvType_flaw           :
        default                     :
            ss_uiOp_emit_qAsciiA ("flaw: signal type");
            ss_uiOp_emit_newline ();
            break;
    }   // switch
}   // ss_uiOp_Show_EventData
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_RunfsmData (ssE_pAboutEv_t ssE_pAboutEv)
{
    msg_ssTEA_Path ();

    ss_uiOp_emit_pAsciiA (ssTEA_plbl_AgRunfsm);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);

    if (ssE_pAboutEv == ssE_pAboutEvNull)
    {
        ss_uiOp_emit_qAsciiA ("not allocated");
        ss_uiOp_emit_newline ();
        return;
    }
    ss_uiOp_emit_lbld_AsciiA (ss_pState, ss_uiOp_Get_pName_Runfsm_state (ssE_pAboutEv->Runfsm_state));
    ss_uiOp_Show_AgencyResult (ssE_pAboutEv->AgencyResult);
    ss_uiOp_emit_newline ();
}   // ss_uiOp_Show_RunfsmData
// -------------------------------------------------------------------------------------------------
pAsciiA_t ss_uiOp_Get_pName_EvType (ssE_EvType_t EvType)
{
    switch (EvType)
    {
        case ssE_EvType_Time    : return (ss_pTime);
        case ssE_EvType_Data    : return (ss_pData);
        case ssE_EvType_Ireq    : return (ss_pIreq);
        default                 : return (S("flaw"));
    }   // switch
}   // ss_uiOp_Get_pName_EvType
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void ss_uiOp_pBanner (boolean lfBefore, pAsciiA_t pAsciiA, boolean lfAfter)
{
    if (lfBefore)
    {
        ss_uiOp_emit_newline ();
    }
    if (pAsciiA)
    {
        while (*pAsciiA != Ascii_NUL)
        {
            ss_uiOp_emit_1 (*pAsciiA);
            pAsciiA++;
        }   // while non-null characters
    }
    if (lfAfter)
    {
        ss_uiOp_emit_newline ();
    }
}   // ss_uiOp_pBanner
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_lbld_AsciiA (pAsciiA_t pLabel, pAsciiA_t pValue)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    ss_uiOp_emit_1 (Ascii_EncapL);
    ss_uiOp_emit_pAsciiA (pValue);
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_lbld_AsciiA
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_lbld_boolean (pAsciiA_t pLabel, boolean IsItTrue)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    ss_uiOp_emit_1 (Ascii_EncapL);
    ss_uiOp_emit_1 ((IsItTrue) ? Ascii__t : Ascii_f);
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_lbld_boolean
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_lbld_hex (pAsciiA_t pLabel, unsigned long aValue)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    ss_uiOp_emit_1 (Ascii_EncapL);
    if (aValue == 0)
    {
        ss_uiOp_emit_Hex_32bits (aValue);
        ss_uiOp_emit_1 (Ascii_EncapR);
        ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
        return;
    }

    if (aValue > 0xF)
    {
        if (aValue > 0xFF)
        {
            if (aValue > 999)
            {
                ss_uiOp_emit_Hex_32bits (aValue);
            }
            else
            {
                ss_uiOp_emit_Hex_999 (aValue);
            }
        }
        else
        {
            ss_uiOp_emit_Hex_2 (aValue);
        }
    }
    else
    {
        ss_uiOp_emit_Hex_1 (aValue);
    }
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_lbld_hex
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_lbld_hex8 (pAsciiA_t pLabel, Bits8_t aValue)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    ss_uiOp_emit_1 (Ascii_EncapL);
    ss_uiOp_emit_1 (Ascii_0);
    ss_uiOp_emit_1 ('x');
    ss_uiOp_emit_Hex_2 (aValue);
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_lbld_hex8
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_lbld_time (pAsciiA_t pLabel, ssT_pTime_t pTime)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    ss_uiOp_emit_1 (Ascii_EncapL);
    ss_uiOp_Show_Time (pTime, lfN);
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_lbld_time
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_lbld_int (pAsciiA_t pLabel, unsigned long aValue)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    ss_uiOp_emit_1 (Ascii_EncapL);
    if (aValue == 0)
    {
        ss_uiOp_emit_Zero (9);
    }
    else
    {
        if (aValue <= 999)
        {   // 1-999
            ss_uiOp_emit_Zero (6);
        }
        else
        {
            if (aValue <= 999999)
            {   // 1000-999999
                ss_uiOp_emit_Zero (3);
            }
        }   //
        ss_uiOp_emit_Int_999 (aValue);
    }   // not zero
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_lbld_int
// -------------------------------------------------------------------------------------------------
void ss_uiOp_do_emit_plot_int (pAsciiA_t pLabel, unsigned long aValue)
{
    ss_uiOp_emit_pAsciiA (pLabel);
    if (aValue == 0)
    {
        ss_uiOp_emit_Zero (9);
    }
    else
    {
        if (aValue <= 999)
        {   // 1-999
            ss_uiOp_emit_Zero (6);
        }
        else
        {
            if (aValue <= 999999)
            {   // 1000-999999
                ss_uiOp_emit_Zero (3);
            }
        }   //
        ss_uiOp_emit_Int_999 (aValue);
    }   // not zero
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
}   // ss_uiOp_do_emit_plot_int

#endif  // __SSTEA_API_DEF_H

