/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_evapi_dcl.h     data and software definitions: Event API data definition.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_EVAPI_DCL_H
#define __SSUI_EVAPI_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// this Range construct defines a minimum value of 1 for ssUI's EvApi allocation: one per event.
// The number of Events requested is established by the App before including any Event software.
// -------------------------------------------------------------------------------------------------
#define SSUI_EVAPI_ALLOC_CT      (SSE_ACTIVE_EVENTS_CT_REQUESTED)
#define SSUI_EVAPI_ALLOC_MAX_I   (SSUI_EVAPI_ALLOC_CT-1)

typedef enum ssUI_EvApi_Range_e
{
    EvApi_i_NONE = (-1),
    EvApi_i_ONE = (0),
    EvApi_i_ALL = (SSUI_EVAPI_ALLOC_CT)
} ssUI_EvApi_Range_t;

// =================================================================================================

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Each Event managed through the Api must have a signal mechanism to communicate with the API.
// That mechanism is sssA_ApiSig: the data signal carrier for commands and their parameters.
// Within the sssApi signal data is the Event Agency descriptor structure allocation for the event.
// Therefore, a single data structure that must be allocated for each Event:  sssA_ApiSig_t.
// If you want to minimize data storage requirements it is feasible to use a single data structure
// and multiplex (share) between all Events, but code will have to do that and code can't store data.
// -------------------------------------------------------------------------------------------------
// An Event runs asynchronously to the application, how is data communicated between the two?
// Variables that always exist (whether the event is running or not) must be allocated and referenced.
// The EventData data structure allows for a Per-Event data allocation, instantiates the variables
// in non-stack memory for use at any Time, and gets the "volatile" attribute assigned at the roots.
// -------------------------------------------------------------------------------------------------
typedef struct ssUI_db_EventApiDataDesc_s
{
    boolean                 accessOn;           // ssUI API manager: manage/alloc/dealloc database
    boolean                 inWildcard;         // ssUI API manager: include/exclude in '*' search
    boolean                 locked;             // ssUI API manager: block changes to database

    ssUI_pToken_t           pEvApiNameTkn_ssDB; // for ssUI display and API LookupWith
    ssUI_pToken_t           pEvApiNameTkn_App;  // for ssUI display and API LookupWith

    ssTEA_pApiSig_t         pssTEA_ApiSig;      // signal carrier for ssTEA API requests/responses

    ssT_pTime_t             pTimeVar;           // for ssT mathOp purposes
    ssUI_pToken_t           pTimeVarTkn;        // for ssUI mathOp purposes

    App_EventData_t         App_EventData;      // ALLOCATES App variable, MUST refer to App's typedef

    EvAg_pEvFunc_t          pEvFuncToUse;       // ssUI abstracts more than a typical App would need

}   ssUI_db_EventApiDataDesc_t,  *ssUI_db_pEventApiDataDesc_t;

typedef ssUI_db_EventApiDataDesc_t      ssUI_db_EvApi_t;
typedef ssUI_db_pEventApiDataDesc_t     ssUI_db_pEvApi_t;
#define ssUI_db_pEvApiNull              ((ssUI_db_pEvApi_t) NULL)

// =================================================================================================
boolean ssUI_apiSignalOp_Emit (ssUI_db_pEvApi_t pEvApi, ss_ApiSigMsgValue_t ApiSigMsgValue);
void    ssUI_apiSignalOp_Show_Data (ssUI_db_pEvApi_t pEvApi, boolean lfBefore, boolean lfAfter, boolean bShowResults);
#define     DoHideResults     false
#define     DoShowResults     (!DoHideResults)
void    ssUI_apiSignalOp_Show_ApiError (ssUI_db_pEvApi_t pEvApi);

// This is used widely by ssUI when displaying information about an Event and/or its carrier signal.
#define pApiSigName(xpEvApiData)                            \
        ((xpEvApiData->pEvApiNameTkn_App) ?                 \
                xpEvApiData->pEvApiNameTkn_App->pAsciiA :   \
                xpEvApiData->pEvApiNameTkn_ssDB->pAsciiA)

void            ssUI_dbOp_Init_AllEvApis   (boolean doDeepInit);
void            ssUI_dbOp_Init_OneEvApi    (ssUI_db_pEvApi_t pEvApi, boolean doDeepInit);

void            ssUI_dbOp_Show_AllEvApis   (void);
void            ssUI_dbOp_Show_OneEvApi    (ssUI_db_pEvApi_t pEvApi);

ssUI_db_pEvApi_t  ssUI_dbOp_Lookup_UsingEvApi_i (int EvApi_i);

// -------------------------------------------------------------------------------------------------
pAsciiA_t       ssUI_dbOp_Get_pName_Using_VarName   (pAsciiA_t pAsciiA);
ssT_pTime_t     ssUI_dbOp_Get_pTimeVar_Using_VarName (pAsciiA_t pAsciiA);

void            ssUI_dbOp_Init_TimeVars     (void);
void            ssUI_dbOp_Show_TimeVars     (void);

#ifdef SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES
void            ssUI_dbOp_Show_EvApi_pointers (ssUI_db_pEvApi_t pEvApi);
#endif  // SSUI_OPTIN_DEBUG_SHOW_PTR_VALUES

// -------------------------------------------------------------------------------------------------
// The EvApi data set is allocated one-per-Event.  Each Event needs an Event Function.  Template
// definitions may be taken over in place or change the "Name" before the table builds.
// The baggage of the functions is carried because the table needs to point to a Function.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t  AppTemplate_EvFunc0 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc1 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc2 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc3 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc4 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc5 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc6 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc7 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc8 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t  AppTemplate_EvFunc9 (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);

#define EvFunc0_Name    AppTemplate_EvFunc0
#define EvFunc1_Name    AppTemplate_EvFunc1
#define EvFunc2_Name    AppTemplate_EvFunc2
#define EvFunc3_Name    AppTemplate_EvFunc3
#define EvFunc4_Name    AppTemplate_EvFunc4
#define EvFunc5_Name    AppTemplate_EvFunc5
#define EvFunc6_Name    AppTemplate_EvFunc6
#define EvFunc7_Name    AppTemplate_EvFunc7
#define EvFunc8_Name    AppTemplate_EvFunc8
#define EvFunc9_Name    AppTemplate_EvFunc9

#endif  // __SSUI_EVAPI_DCL_H

