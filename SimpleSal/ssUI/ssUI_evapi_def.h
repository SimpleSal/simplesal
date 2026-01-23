/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_evapi_def.h     data and software definitions: Event API allocation and manipulation.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_EVAPI_DEF_H
#define __SSUI_EVAPI_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The ssUI_ application is implemented as a harness, from which ssTEA functionality may be exercised.
// ssTEA/E maintains Event Agency descriptors for some configurable number of simultaneously
// scheduled events.  That number is not related directly to the number of API descriptors and
// event descriptors that are used (needed/easiest/smoothest) in an App, ssUI tests the models.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Event descriptors are used to exchange information with the API, ssTEA/E services may write
// into the API data structures.  Event Agency data is always copied to an ssTEA/E data structure.
// Users enter strings that are names of eventApi data structures, this maps names to ssHL variables,
// the mapping is from the user saying "evapiN" resulting in a mapping to "evapi[N]", using LookupWiths.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Allocation of event and API data structures is the job of the App.  ssUI uses an array of
// instances, of a set of event API data structures, and a parallel set of time data structures.
// The model that ssUI follows for math operations requires each event to have an ssT time variable.
// Each Event API has a self-describing name string and a flag indicating someone thinks they own it.
// -------------------------------------------------------------------------------------------------
// The API model that ssT follows does not care whether the caller uses a unique Api parameter data
// structure for all interactions about an event, or, uses a single Api parameter data structure for
// all events.  The second model requires per-event information be transferred each use, however.
// -------------------------------------------------------------------------------------------------
// ssUI code manipulates data structures to manage events over time, the User supplies the storage.
// when a ssHL variable is "defined": the memory is allocated and the contents may be set at build time.
// -------------------------------------------------------------------------------------------------
// These variables are "allocated" because the memory is allocated but the contents are not defined.
// -------------------------------------------------------------------------------------------------
// what functionality is needed in order to allocate/deallocate sigApis using malloc/free?
// =================================================================================================
// define an array of event manager Data Descriptors, one for each event supported by ssDB for use.
// -------------------------------------------------------------------------------------------------
// This is one of two references to the global statically allocated array of data structures
// used to maintain the "database" of information about each event, from the user side.
// This reference allocates the space in user memory for the data structures.
// -------------------------------------------------------------------------------------------------
ssUI_db_EvApi_t     ssUI_db_EvApi_alloc[SSUI_EVAPI_ALLOC_CT];
ssTEA_ApiSig_t      ssUI_db_ApiSig_alloc[SSUI_EVAPI_ALLOC_CT];
// -------------------------------------------------------------------------------------------------
// Valid function pointers are used to make sure that invalid data does not cause a hard fault.
// -------------------------------------------------------------------------------------------------
// The ssHL software language problem of correlating _CT values for allocations and algorithms with
// actual data item allocations and references to actual functions and data structures during init.
// An arbitrary choice here to have 10 names in an array makes the maximum EVAPI_ALLOC_CT be 10.
// The alternative is much cleaner but is not known yet by a learner: don't refer to invalid data.
// -------------------------------------------------------------------------------------------------
EvAg_pEvFunc_t      ssUI_db_EvFunc_table[] =
{
    EvFunc0_Name, EvFunc1_Name, EvFunc2_Name, EvFunc3_Name, EvFunc4_Name,
    EvFunc5_Name, EvFunc6_Name, EvFunc7_Name, EvFunc8_Name, EvFunc9_Name
};

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// this is at the level of the Agency Api, after a signal transaction is ongoing (DoShow),
// or, as part of a dump from the database of all fields, without a context (Hide).
// -------------------------------------------------------------------------------------------------
void ssUI_apiSignalOp_Show_Data (ssUI_db_pEvApi_t pEvApi,
                                 boolean lfBefore,
                                 boolean lfAfter,
                                 boolean bShowSignalResults)
{
    ssTEA_pApiSig_t pApiSig;

    msg_ssTEA_Path ();

    // proceed knowing that the UI banner is actively signaling
    pApiSig = pEvApi->pssTEA_ApiSig;

    if (lfBefore == lfY)
    {
        ss_uiOp_emit_newline ();
    }
    // The carrier is always there, whether the signal is active or not.
    ss_uiOp_emit_pAsciiA (ssTEA_plbl_Carrier);
    ss_uiOp_emit_Space (ssUI_standard_fieldgap);
    if (pEvApi->pEvApiNameTkn_App)
    {
        ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_App, pEvApi->pEvApiNameTkn_App->pAsciiA);
    }   // user assigned a value
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNameLbl_ssDB, pEvApi->pEvApiNameTkn_ssDB->pAsciiA);
    if (ssTEA_control.Show_Cause)
    {
        ss_uiOp_emit_lbld_hex (ssTEA_plbl_Channel, (unsigned long) pApiSig->channel);
    }
    // the signal is active or not active, called here "Live"
    if (bShowSignalResults)
    {
        if (pApiSig->sigDir == ss_ApiSigDir_ToApi)
        {
            ss_uiOp_emit_Space (ssUI_standard_fieldgap);
            ss_uiOp_emit_pAsciiA (pcMsg_ArrowsReqDir);
        }
        else
        {
            ss_uiOp_emit_Space (ssUI_standard_fieldgap);
            ss_uiOp_emit_pAsciiA (pcMsg_ArrowsRespDir);
        }
    }
    ss_uiOp_emit_newline ();

    // Show_SignalCarrier repeats the signal value, not knowing what the caller has already stated.
    ssTEA_apiOp_Show_SignalData  (pApiSig);

    if (bShowSignalResults)
    {
        if (pApiSig->sigDir == ss_ApiSigDir_FromApi)
        {
            ss_uiOp_pBanner (lfN,
                                 ((pApiSig->macSNR == ss_macSNR_OK) ?
                                     pcMsg_Carrier_Worked : pcMsg_Carrier_Failed),
                                 lfN);

            if (pApiSig->macSNR == ss_macSNR_OK)
            {
                ss_uiOp_pBanner (lfN,
                                     ((pApiSig->apiSNR == ss_apiSNR_OK) ?
                                         pcMsg_apiSignalOp_Worked : pcMsg_apiSignalOp_Failed),
                                     lfN);
            }
            else
            {
                ss_uiOp_emit_qAsciiA ("no API signal possible.");
            }
            lfAfter = lfY;
        }   // the signal right now a response we received
    }

    if (lfAfter == lfY)
    {
        ss_uiOp_emit_newline ();
    }
}   // ssUI_apiSignalOp_Show_Data
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// this is at the level of the Agency Api, after a signal transaction has occured.
// -------------------------------------------------------------------------------------------------
void ssUI_apiSignalOp_Show_ApiError (ssUI_db_pEvApi_t pEvApi)
{
    msg_ssTEA_Path ();

    ss_uiOp_emit_qAsciiA (pcMsg_AgApiSignalError);
    ssUI_apiSignalOp_Show_Data (pEvApi, lfY, lfN, DoShowResults);
}   //  ssUI_apiSignalOp_Show_ApiError
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// ssFeature: This function and the ssTEA_Api function needs to be able to send a response as a signal,
// ssFeature: however, a response is only implemented in the API directly after the API request.
// ssFeature: An asynchronous response, or one generated at a later time, is not supported by ssTEA.
// ssFeature: When it is supported, it will be as a response asynchronously generated by ssTEA to
// ssFeature: a request previously made.  For example, the request "tell me when this happens".
// -------------------------------------------------------------------------------------------------
// While the channel is open (valid, live) the Agency Api FSM and the Agency Run FSM are active.
// Everything around an Event, as accessed through the API, is based on the channel set by ssTEA.
// Said the other way around, the channel value is used to quickly identify the ssTEA data to use.
// Users of the channel should never question or change the channel value, it is a secret of ssTEA.
// -------------------------------------------------------------------------------------------------
// called "ApiSigMsgValue" because it is assumed to be a valid signal value in a request context.
// -------------------------------------------------------------------------------------------------
boolean ssUI_apiSignalOp_Emit (ssUI_db_pEvApi_t pEvApi, ss_ApiSigMsgValue_t ApiSigMsgValue)
{
    msg_ssTEA_Path ();

    ssTEA_pApiSig_t pApiSig = pEvApi->pssTEA_ApiSig;

    // ---------------------------------------------------------------------------------------------
    // The App should use the Agency Api FSM state to judge the state of a channel, because
    // the signal and the signal carrier are specific and the details of channel state is specific.
    //  (the channel is zero only when state "Off" and is nonzero in every other Api FSM state)
    // The use of the Api FSM state is an abstraction for any MAC (signal/carrier) implementation.
    // ---------------------------------------------------------------------------------------------
    if (ApiSigMsgValue != ss_ApiSigMsgValue_AgApifsm_On)
    {
        // -----------------------------------------------------------------------------------------
        // The first action is open the channel's MAC; the last, when done, is to close it the MAC.
        // An open channel has a nonNull channel value, a closed channel's channel value is Null.
        // The Api signal, and the Api signal SNR, does not exist unless the MAC is functioning.
        // -----------------------------------------------------------------------------------------
        if (pApiSig->channel == pChannelNull)
        {
            pApiSig->sigDir     = ss_ApiSigDir_FromApi;
            pApiSig->sigValue   = ApiSigMsgValue;
            pApiSig->macSNR     = ss_macSNR_MACerror;
            return (false);
        }   // not an On Request, channel not open
    }   // allow only Api FSM On request if the Api FSM is not ON; if ON, allow any, including ON

    pApiSig->sigDir     = ss_ApiSigDir_ToApi;
    pApiSig->sigValue   = ApiSigMsgValue;
    pApiSig->macSNR     = ss_macSNR_flaw;      // force the API to set a valid value before return
    pApiSig->apiSNR     = ss_apiSNR_flaw;      // force the API to set a valid value before return

    // ssTEA will use the "ssm_ssTEA_" choices to display information; ssUI matches for a reason.
    if (ssTEA_control.Show_Signals)
    {
        ssUI_apiSignalOp_Show_Data (pEvApi, lfN, lfN, DoShowResults);
    }

    if (!ssTEA_apiOp_Emit_Signal (pApiSig))
    {
        // bugbugbug show api error regardless of output selected
        ssUI_apiSignalOp_Show_ApiError (pEvApi);

        // if the carrier signal was OK, it must have been an error in the API signal
        if (pApiSig->macSNR == ss_macSNR_OK)
        {
            ss_uiOp_pBanner (lfY, pcMsg_AgApiReqError, lfY);
        }
        return (false);
    }   // signal had error at signal or API level
    else
    {   // a successful request-response signal gets changed to a response before return by ApiSignal
        if (ssTEA_control.Show_Signals)
        {
            if (pApiSig->sigDir == ss_ApiSigDir_FromApi)
            {
                ssUI_apiSignalOp_Show_Data (pEvApi, lfN, lfN, DoShowResults);
            }
        }
        return (true);
    }
}   // ssUI_apiSignalOp_Emit
#undef lcl_ssm_ssTEA_Full
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// given an index into EvApis array, return a pointer to a ssTEA API data structure or return NULL.
// there are multiple purposes or reasons for doing this.
// 1) the events code of the user is referenced in this file, so must be included before this file.
//    that file's source code uses an declaration of this function to compile, in order to get
//    a pointer to an element in the array.  Otherwise Events code would have to reference the
//    allocated data array directly, which would require that the events code be included last.
// 2) SimpleSal and ssTEA abstract concepts are built entirely within the assumption that the
//    processor does not matter, but in fact the domination of ARM as a choice for many products
//    means that the best skill to have is the ability to learn enough details on one ARM at a time.
// -------------------------------------------------------------------------------------------------
//    The true most basic assumption is that SimpleSal and ssTEA are running on an ARM processor,
//    which of course causes the mesa to also contain all those devices built to work with ARM.
//    This includes the specification of "memory" on the ARM's Hal architecture code and data buses.
// -------------------------------------------------------------------------------------------------
//    probably true: ARM instructions and registers are 32-bits; ARM data pointers are 32-bits.
//      ARM registers are used in pointer instructions; e.g., read data from memory into registers.
//    The address of any memory location is described with a 32-bit number, computed at build time.
//    The 32-bit address is stored in memory, there is no "I am not an instruction" instruction,
//      so the 32-bit address can't be tucked into the pipeline content, other architectures may.
//    The value is stored in the program's image, near enough to use an ARM [register+offset] read.
//         That nearby location is in the code memory of the architecture, on the code memory bus.
//    The other reader of code memory is the processor instruction pipeline, complex/cheap helper
//         for the pipeline in order to run the ARM faster without ever finding an empty pipeline.
// -------------------------------------------------------------------------------------------------
ssUI_db_pEvApi_t  ssUI_dbOp_Lookup_UsingEvApi_i (int EvApi_i)
{
    // not a valid index? cannot calculate the address of an EvApi data structure
    if ((EvApi_i <= EvApi_i_NONE) || (EvApi_i >= EvApi_i_ALL))
    {
        return (ssUI_db_pEvApiNull);
    }

    return (&ssUI_db_EvApi_alloc[EvApi_i]);
}   // ssUI_dbOp_Lookup_UsingEvApi_i
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void ssUI_dbOp_Show_OneEvApi (ssUI_db_pEvApi_t pEvApi)
{
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("ssTEA API --------");
#ifdef SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES
    ssUI_dbOp_Show_EvApi_pointers (pEvApi);
#endif  // SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES
    ss_uiOp_emit_lbld_boolean (pcAccess, pEvApi->accessOn);

    if (!pEvApi->accessOn)
    {
        ss_uiOp_emit_newline ();
        return;
    }
    ss_uiOp_emit_lbld_boolean (S("wildcard"), pEvApi->inWildcard);
    ss_uiOp_emit_lbld_boolean (S("locked"), pEvApi->locked);
    ss_uiOp_emit_newline ();

#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    // bugbugbug need to get state from ssTEA
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    ssUI_apiSignalOp_Show_Data (pEvApi, lfN, lfN, DoHideResults);
    ss_uiOp_Show_Apifsm_state (pEvApi->pssTEA_ApiSig, pApiSigName (pEvApi));
    ss_uiOp_Show_RunfsmData (pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
    ss_uiOp_Show_EventData (pEvApi->pssTEA_ApiSig->ssE_pAboutEv);

    ss_uiOp_emit_qAsciiA (pcMsg_plbl_TimeVariable);
    ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
    ss_uiOp_emit_lbld_time ("value", pEvApi->pTimeVar);
    ss_uiOp_emit_lbld_AsciiA (pcEvApiNamelbl_Name, pEvApi->pTimeVarTkn->pAsciiA);
    ss_uiOp_emit_lbld_hex ("@", (unsigned long) pEvApi->pTimeVar);
    ss_uiOp_emit_newline ();

}   // ssUI_dbOp_Show_OneEvApi
// -------------------------------------------------------------------------------------------------
void ssUI_dbOp_Show_AllEvApis (void)
{
    int                 EvApi_i;
    ssUI_db_pEvApi_t    pEvApi;

    ss_uiOp_emit_newline ();
    ss_uiOp_emit_Dash (40);
    ss_uiOp_pBanner (lfY, pcMsg_CurrentViewData, lfY);
    ss_uiOp_emit_Dash (40);

    for (EvApi_i=0; EvApi_i <= SSUI_EVAPI_ALLOC_MAX_I; EvApi_i++)
    {
        pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (EvApi_i);
        if (pEvApi->accessOn)
        {
            Bld_ApiData_Debug_Access_func (pEvApi);
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_Dash (20);
            ssUI_dbOp_Show_OneEvApi (pEvApi);
        }
    }   // for each data structure in database
}   // ssUI_dbOp_Show_AllEvApis
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A Proper Event Api data structure contains two pointer values
//   1) pssTEA_ApiSig pointing to allocated memory that is actually the signal carrier or medium.
//   2) pTimeVar pointing to allocated memory that is a Time Variable ssUI makes available to Event.
// -------------------------------------------------------------------------------------------------
void  ssUI_dbOp_Init_OneEvApi (ssUI_db_pEvApi_t pEvApi, boolean doDeepInit)
{
    ssTEA_pApiSig_t     pApiSig     = pEvApi->pssTEA_ApiSig;

    pEvApi->accessOn    = true;
    pEvApi->inWildcard  = true;
    pEvApi->locked      = false;

    ssT_mathOp_P1getsZero (pEvApi->pTimeVar);

#ifdef MESA_OPTIN_DEBUG_MEMORYACCESS
    CTRICK_Prove_MemoryIsWritable ((unsigned long *) pApiSig, (unsigned char *) __func__);
#endif  // MESA_OPTIN_DEBUG_MEMORYACCESS

    pApiSig->sigDir         = ss_ApiSigDir_FromApi;
    pApiSig->sigValue       = ss_ApiSigMsgValue_flaw;
    pApiSig->macSNR         = ss_macSNR_flaw;
    pApiSig->apiSNR         = ss_apiSNR_flaw;
    pApiSig->channel        = pChannelNull;
    pApiSig->ssE_pAboutEv   = ssE_pAboutEvNull;
    pApiSig->pApiSigData    = pApiSigDataNull;

}   // ssUI_dbOp_Init_OneEvApi
// -------------------------------------------------------------------------------------------------
// some fields are auto-initialized at compile time, nested structures are difficult/avoided
//      pointers to data/functions in structures
//      basic variables such as state or a counter
// some fields are code-initialized at runtime
//      nested or 2nd-level structures requires code that must be run to create valid data
// some fields are used over time after initialized
//      algorithms require variables, in particular states and historical values
// -------------------------------------------------------------------------------------------------
// when doDeepInit is false, the data structures used to describe Events are initialized.
// when doDeepInit is true, the data structures used by ssUI to support Events are initialized,
//                              AND the data structures used to describe Events are initialized.
// "support" relates to the infrastructure of the data structures and their relationships, and
//      is primarily concerned with values in the ApiData data structure.
// "describe" relates to the Event and the signal carrier used to manage the Event through ssTEA.
// -------------------------------------------------------------------------------------------------
void ssUI_dbOp_Init_AllEvApis (boolean doDeepInit)
{
    int                 EvApi_i;
    ssUI_db_pEvApi_t    pEvApi;

    msg_ssTEA_Path ();

    // set all pointers to EvApi data structures
    for (EvApi_i=0; EvApi_i <= SSUI_EVAPI_ALLOC_MAX_I; EvApi_i++)
    {
        pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (EvApi_i);
        Bld_ApiData_Debug_Access_func (pEvApi);

#ifdef BLD_DEBUG_MEMORYACCESS
        // -----------------------------------------------------------------------------------------
        // [.\SimpleSal\ssDocs\swDev\SW Developer Tricks.note] perform memory read/write access test
        // -----------------------------------------------------------------------------------------
        CTRICK_Prove_MemoryIsWritable ((unsigned long *) pEvApi, (unsigned char *) __func__);
#endif  // BLD_DEBUG_MEMORYACCESS

        // config depending, ssDB Init may not allocate the ssE_AboutEv pointed to by the Signal.
        // "config depending" means "based on TRUSTED use of Event data structures".
        // ssTEA API signal carrier
        pEvApi->pssTEA_ApiSig = &ssUI_db_ApiSig_alloc[EvApi_i];
        ssUI_dbOp_Init_OneEvApi (pEvApi, doDeepInit);

        if (doDeepInit)
        {
            // These are all in support of ssUI's user interface describing events and FSMs
            // connect all pointed-to objects that create relationships such as names, Time vars.
            // ssUI display and API LookupWith
            pEvApi->pEvApiNameTkn_ssDB = &ssUI_db_tknEvApiNames_alloc[EvApi_i];
            // ssUI display and API LookupWith
            pEvApi->pEvApiNameTkn_App = pTokenNull;
            // for ssT mathOp purposes
            pEvApi->pTimeVar = &ssUI_db_timeVars_alloc[EvApi_i];
            // for ssUI mathOp purposes
            pEvApi->pTimeVarTkn = &ssUI_db_tknTimeVarNames_alloc[EvApi_i];
            // wait for ssE_AboutEv function assignment until AFTER Event data space is allocated
            pEvApi->pEvFuncToUse = ssUI_db_EvFunc_table[EvApi_i];
        }   // if doDeepInit
    }   // for each EvApi structure
}   // ssUI_dbOp_Init_AllEvApis
// -------------------------------------------------------------------------------------------------
// Given a string which is the name of a variable, return the name the user uses.
// -------------------------------------------------------------------------------------------------
pAsciiA_t ssUI_dbOp_Get_pName_Using_VarName (pAsciiA_t pVarName)
{
    int                 Var_i;
    pAsciiA_t           pCopy;

    // the copy converts to lowercase into a buffer whose secret pointer we can use only here
    pCopy = ssUI_AaOp_Copy_gets_pP1_lc (pVarName);

    for (Var_i=0; Var_i <= TimeVar_TUPLE_MAX_I; Var_i++)
    {
        // depend on the token name as length to compare, not copy's length
        if ( ssUI_AaIf_pP1_eq_pP2 (ssUI_TimeVars[Var_i].VarToken.pAsciiA, pCopy))
        {
            return (ssUI_TimeVars[Var_i].VarToken.pAsciiA);
        }
    }   // for each variable defined

    return (gTknTimeEphemeral.pAsciiA);
}   // ssUI_dbOp_Get_pName_Using_VarName

// -------------------------------------------------------------------------------------------------
// Given a string which is the name of a variable, return the ssHL-language name the program uses.
// If the string value doesn't match a variable, return ephemeral time variable, that will be used.
// -------------------------------------------------------------------------r------------------------
ssT_pTime_t ssUI_dbOp_Get_pTimeVar_Using_VarName (pAsciiA_t pVarName)
{
    int                 Var_i;
    pAsciiA_t           pCopy;

    // the copy converts to lowercase into a buffer whose secret pointer we can use only here
    pCopy = ssUI_AaOp_Copy_gets_pP1_lc (pVarName);

#ifdef SSUI_OPTIN_DEBUG_VARNAMES
    ss_uiOp_emit_lbld_AsciiA (S("looking for VarName", pCopy);
    ss_uiOp_emit_newline ();
#endif  // SSUI_OPTIN_DEBUG_VARNAMES

    for (Var_i=0; Var_i <= TimeVar_TUPLE_MAX_I; Var_i++)
    {
#ifdef SSUI_OPTIN_DEBUG_VARNAMES
        ss_uiOp_emit_lbld_AsciiA (S(" compare to"), ssUI_TimeVars[Var_i].VarToken.pAsciiA);
#endif  // SSUI_OPTIN_DEBUG_VARNAMES
        // depend on token name's length, not copy's length: Param1 is token name, Param2 is copy
        if ( ssUI_tknIf_sP1_eq_pAA (ssUI_TimeVars[Var_i].VarToken, pCopy) )
        {
#ifdef SSUI_OPTIN_DEBUG_VARNAMES
            ss_uiOp_emit_qAsciiA ("matched");
            ss_uiOp_emit_newline ();
#endif  // SSUI_OPTIN_DEBUG_VARNAMES
            return (ssUI_TimeVars[Var_i].pCodesName);
        }
#ifdef SSUI_OPTIN_DEBUG_VARNAMES
        ss_uiOp_emit_newline ();
#endif  // SSUI_OPTIN_DEBUG_VARNAMES
    }   // for each variable defined

    return (ssT_gpTime_Ephemeral);
}   // ssUI_dbOp_Get_pTimeVar_Using_VarName

// -------------------------------------------------------------------------------------------------
void ssUI_dbOp_Init_TimeVars (void)
{
    int     Var_i;

    for (Var_i=0; Var_i <= TimeVar_TUPLE_MAX_I; Var_i++)
    {
        ssT_mathOp_P1getsZero (ssUI_TimeVars[Var_i].pCodesName);
    }
}   // ssUI_dbOp_Init_TimeVars
// -------------------------------------------------------------------------------------------------
// Given a string which is the name of a variable, return the ssHL-language name the program uses.
// -------------------------------------------------------------------------------------------------
void ssUI_dbOp_Show_TimeVars (void)
{
    int                 Var_i;

    ss_uiOp_pBanner (lfN, pcMsg_VarnameHelp, lfY);

    for (Var_i=0; Var_i <= TimeVar_TUPLE_MAX_I; Var_i++)
    {   // timea or evapi0 or evtime0 or evtime5   as tokens triggers a newline to get all on the page
        if (
            (ssUI_tknIf_P1eqP2 (&ssUI_TimeVars[Var_i].VarToken, &gTknTimeA))
            ||
            (ssUI_tknIf_P1eqP2 (&ssUI_TimeVars[Var_i].VarToken, &ssUI_db_tknEvApiNames_alloc[0]))
            ||
            (ssUI_tknIf_P1eqP2 (&ssUI_TimeVars[Var_i].VarToken, &ssUI_db_tknTimeVarNames_alloc[0]))
            ||
            (ssUI_tknIf_P1eqP2 (&ssUI_TimeVars[Var_i].VarToken, &ssUI_db_tknTimeVarNames_alloc[5]))
           )
        {
            ss_uiOp_emit_newline ();
        }   // timea or evapi0 or evtime0    as tokens triggers a newline
        ss_uiOp_emit_pAsciiA  (ssUI_TimeVars[Var_i].VarToken.pAsciiA);
        ss_uiOp_emit_Space (4);
    }   // for each variable defined
}   // ssUI_dbOp_Show_TimeVars

#ifdef SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES
// -------------------------------------------------------------------------------------------------
void    ssUI_dbOp_Show_EvInfo_pointers (ssE_pEvInfo_t pEvInfo)
{
    ss_uiOp_emit_1 (Ascii_Larrow);
    ss_uiOp_emit_Hex_32bits (pEvInfo);
    ss_uiOp_emit_1 (Ascii_Period);
    if (pEvInfo)
    {
        ss_uiOp_emit_Hex_32bits (pEvInfo->ssE_pAboutEv);
    }
    ss_uiOp_emit_1 (Ascii_Rarrow);
}   // ssUI_dbOp_Show_EvInfo_pointers
// -------------------------------------------------------------------------------------------------
void    ssUI_dbOp_Show_EvApi_pointers (ssUI_db_pEvApi_t pEvApi)
{
    ss_uiOp_emit_1 (Ascii_Larrow);
    ss_uiOp_emit_Hex_32bits (pEvApi);
    ss_uiOp_emit_1 (Ascii_Period);
    if (pEvApi)
    {
        ss_uiOp_emit_Hex_32bits (pEvApi->pssTEA_ApiSig);
    }
    ss_uiOp_emit_1 (Ascii_Period);
    if (pEvApi)
    {
        if (pEvApi->pssTEA_ApiSig)
        {
            ss_uiOp_emit_Hex_32bits (pEvApi->pssTEA_ApiSig->ssE_pAboutEv);
        }
    }
    ss_uiOp_emit_1 (Ascii_Rarrow);
}   // ssUI_dbOp_Show_EvApi_pointers
#endif  // SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// placeholders for assignment to event function pointer field before User is allowed to overwrite.
// -------------------------------------------------------------------------------------------------
void AppTemplate_EvAnnounceAnyAgency (Ascii_t AnIdentifier)
{
    ss_uiOp_emit_qAsciiA ("running empty User Event");
    ss_uiOp_emit_1 (Ascii_EncapL);
    ss_uiOp_emit_1 (AnIdentifier);
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_newline ();
}   // AppTemplate_EvAnnounceAnyAgency
// -------------------------------------------------------------------------------------------------
void AppTemplate_EvAnnounceThisAgency (ssE_pAboutEvVoid_t param1)
{
    if (param1 == ssE_pAboutEvNull)
    {
        ss_uiOp_emit_qAsciiA ("init Agency ");
    }
    else
    {
        ss_uiOp_emit_qAsciiA ("not init Agency");
    }
    ss_uiOp_emit_newline ();
}   // AppTemplate_EvAnnounceAnyAgency
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t AppTemplate_EvFunc0 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
{
    AppTemplate_EvAnnounceAnyAgency (Ascii_0);
    AppTemplate_EvAnnounceThisAgency (param1);
    return(ssE_EvResult_OK_go);
}   // AppTemplate_EvFunc0

ssE_EvResult_t AppTemplate_EvFunc1 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_1); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc1
ssE_EvResult_t AppTemplate_EvFunc2 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_2); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc2
ssE_EvResult_t AppTemplate_EvFunc3 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_3); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc3
ssE_EvResult_t AppTemplate_EvFunc4 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_4); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc4
ssE_EvResult_t AppTemplate_EvFunc5 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_5); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc5
ssE_EvResult_t AppTemplate_EvFunc6 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_6); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc6
ssE_EvResult_t AppTemplate_EvFunc7 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_7); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc7
ssE_EvResult_t AppTemplate_EvFunc8 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_8); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc8
ssE_EvResult_t AppTemplate_EvFunc9 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
    {  AppTemplate_EvAnnounceAnyAgency (Ascii_9); AppTemplate_EvAnnounceThisAgency (param1); return(ssE_EvResult_OK_go);  }   // AppTemplate_EvFunc9

#endif  // __SSUI_EVAPI_DEF_H

