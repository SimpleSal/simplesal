/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssE_event_def.h      data and software definitions: the implementation of Events by ssTEA
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSE_EVENT_DEF_H
#define __SSE_EVENT_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Event Database.note]    describes Event data seen as ssE manager's info
// -------------------------------------------------------------------------------------------------
#define SSE_EVINFO_ALLOC_CT     SSE_ACTIVE_EVENTS_CT_REQUESTED
#define SSE_EVINFO_MAX_I        (SSE_EVINFO_ALLOC_CT-1)

ssE_EvInfo_t  ssE_EvInfos[SSE_EVINFO_ALLOC_CT];

// =================================================================================================
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssE_Initialize (void)
{
    ss_macSNR_t  signalReturn;

    msg_ssTEA_Path ();

    signalReturn = ssE_InitData_RunTime (DEEPINIT);

    return (signalReturn);
}   // ssE_Initialize
// =================================================================================================
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssE_InitData_RunTime (boolean doDeepInit)
{
    ss_macSNR_t  signalReturn = ss_macSNR_OK;
    int          evInfo_i;

    msg_ssTEA_Path ();

    if (doDeepInit)
    {
        AboutEvDB_dbOp_Init_DB ();
    }

    for (evInfo_i=0; evInfo_i <= SSE_EVINFO_MAX_I; evInfo_i++)
    {
        ssE_dbOp_Init_One_EvInfo (&ssE_EvInfos[evInfo_i], doDeepInit);
    }

    return (signalReturn);
}   // ssE_InitData_RunTime
// =================================================================================================
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
// -------------------------------------------------------------------------------------------------
// This utility or debug function describes the current value of a pointer to a database structure.
// The idea of a database in combination with the idea of "trusted" creates one or two copies of a
// database that changes over time.  Some bugs are solved by showing the cards everybody holds.
// -------------------------------------------------------------------------------------------------
void ssE_EvDB_Show_ssE_pAboutEv (pAsciiA_t pDesc, ssE_pAboutEv_t ssE_pAboutEv)
{
    ss_uiOp_emit_pAsciiA (S(" **EvDB**: "));

    // ascii output of description if it is there, output value of pointer at any value
    if (pDesc)
    {
        ss_uiOp_emit_pAsciiA (pDesc);
        ss_uiOp_emit_1 (Ascii_AtSign);
        // ascii output of value of the pointer if it is there
        ss_uiOp_emit_Hex_32bits ((unsigned long) ssE_pAboutEv);
        ss_uiOp_emit_newline ();
    }
}   // ssE_EvDB_Show_ssE_pAboutEv
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note]        ssE declares what the interface will be
// [.\SimpleSal\ssDocs\ssTEA\Event Databases.note]          ssE or the App instantiates the database
// -------------------------------------------------------------------------------------------------
#ifdef SSE_ONEOF_ABOUTEVDB_ALLOC_BY_SSE

typedef struct DB_Allocation_s
{
    ssE_EvAlloc_t   Alloc;
    ssE_AboutEv_t   ssE_AboutEv;
}   DB_Allocation_t, *pDB_Allocation_t;

// Allocation amount is determined by Trusted (1 instance per Event) or !Trusted (2 instances)
DB_Allocation_t     DB_allocation[SSE_ONEOF_ABOUTEVDB_ALLOC_CT];

void    AboutEvDB_dbOp_Init_DB (void)
{
    int evData_db_i;

    // each time initialization is performed, the database is cleared and valid.
    // The index here spans the allocation for ssE (always) and possibly the App (!Trusted).
    for (evData_db_i = 0; evData_db_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; evData_db_i++)
    {
        DB_allocation[evData_db_i].Alloc.active = false;
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
        ss_uiOp_emit_1 (Ascii_EncapL);
        ss_uiOp_emit_Hex_2 (evData_db_i);
        ss_uiOp_emit_1 (Ascii_EncapR);
        ssE_EvDB_Show_ssE_pAboutEv (S("Event Data : ssE Init"), &DB_allocation[evData_db_i].ssE_AboutEv);
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
        ssE_dbOp_Init_One_AboutEv (&DB_allocation[evData_db_i].ssE_AboutEv, DEEPINIT);
    }   // for
}   // AboutEvDB_dbOp_Init_DB

// -------------------------------------------------------------------------------------------------
ssE_pAboutEv_t   AboutEvDB_dbOp_Get (void)
{
    int             evData_db_i;
    ssE_pAboutEv_t  ssE_pAboutEv = ssE_pAboutEvNull;

    // The index here spans the allocation for ssE (always) and possibly the App (!Trusted).
    for (evData_db_i=0; evData_db_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; evData_db_i++)
    {
        if (!DB_allocation[evData_db_i].Alloc.active)
        {
            DB_allocation[evData_db_i].Alloc.active = true;
            ssE_pAboutEv = &DB_allocation[evData_db_i].ssE_AboutEv;
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
            ssE_EvDB_Show_ssE_pAboutEv (S("Event Data : Gave Out From Free"), ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
            break;
        }   // if not in use, allocate it
    }   // for
    return (ssE_pAboutEv);
}   // AboutEvDB_dbOp_Get

// -------------------------------------------------------------------------------------------------
// find: "ssE_AboutEv Allocation Rules"     : trusted or not, ssE allocates ssE_AboutEv during Init.
ssE_pAboutEv_t AboutEvDB_dbOp_Put (ssE_pAboutEv_t ssE_pAboutEv)
{
    int                 evData_db_i;

    // The index here spans the allocation for ssE (always) and possibly the App (!Trusted).
    for (evData_db_i=0; evData_db_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; evData_db_i++)
    {
        if (DB_allocation[evData_db_i].Alloc.active)                  // let's have some rules
        {
            if (&DB_allocation[evData_db_i].ssE_AboutEv == ssE_pAboutEv)    // before allowing just anyone
            {
                DB_allocation[evData_db_i].Alloc.active = false;      // with a pointer to init ssE memory
                ssE_dbOp_Init_One_AboutEv (ssE_pAboutEv, TIDYINIT);
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
                ssE_EvDB_Show_ssE_pAboutEv (S("Event Data : Returned to Free"), ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
                return (ssE_pAboutEvNull);
            }   // if doesn't match a true address, can't be put back
        }   // if not in use, can't be put back
    }   // for

    // couldn't put it back, better to not just pretend we did put it back
    return (ssE_pAboutEv);
}   // AboutEvDB_dbOp_Put
#endif  // SSE_ONEOF_ABOUTEVDB_ALLOC_BY_SSE

// -------------------------------------------------------------------------------------------------
// P1getsP2 does not refer to AgContext; similarly, the App should never modify or use AgContext.
// -------------------------------------------------------------------------------------------------
// This function does not modify the destination pAgContext field, as it is used to track validity
// by a process that is above the level of understanding that an ssE_AboutEv FSM knows about itself.
// This function is outside the "allocation by app/ssE?" debate; it is a service provided by ssE.
// -------------------------------------------------------------------------------------------------
void ssE_dbOp_P1getsP2 (ssE_pAboutEv_t pP1, ssE_pAboutEv_t pP2)
{
    pP1->Runfsm_state = pP2->Runfsm_state;
    pP1->Runfsm_EvType = pP2->Runfsm_EvType;
    ssT_mathOp_P1getsP2 (&pP1->OccursAt, &pP2->OccursAt);
    pP1->recurs = pP2->recurs;
    pP1->recursFromTrue = pP2->recursFromTrue;
    ssT_mathOp_P1getsP2 (&pP1->Period, &pP2->Period);
    pP1->pEvFunc = pP2->pEvFunc;
    pP1->App_pEventData = pP2->App_pEventData;
}   // ssE_dbOp_P1getsP2
// -------------------------------------------------------------------------------------------------
// This function assigns valid values to all of the fields in an Agency data structure.
// -------------------------------------------------------------------------------------------------
void ssE_dbOp_Init_One_AboutEv (ssE_pAboutEv_t ssE_pAboutEv, boolean doDeepInit)
{
     msg_ssTEA_Path ();

#ifdef MESA_OPTIN_DEBUG_MEMORYACCESS
    CTRICK_Prove_MemoryIsWritable ((unsigned long *) ssE_pAboutEv, (unsigned char *) __func__);
#endif  // MESA_OPTIN_DEBUG_MEMORYACCESS

    // in the Runfsm state machine, set the initial conditions.
    ssE_dbOp_Init_RunfsmData (ssE_pAboutEv);

    ssE_pAboutEv->Runfsm_EvType = ssE_EvType_Time;
    ssE_pAboutEv->AgencyResult = ssE_EvResult_OK_go;
    ssE_pAboutEv->recurs = false;
    ssE_pAboutEv->recursFromTrue = false;
    ssT_mathOp_P1getsZero (&ssE_pAboutEv->OccursAt);
    ssT_mathOp_P1getsZero (&ssE_pAboutEv->Period);
    if (doDeepInit)
    {
        ssE_pAboutEv->pEvFunc = pEvFuncNull;
    }
}   // ssE_dbOp_Init_One_AboutEv
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssE_dbOp_Init_One_EvInfo (ssE_pEvInfo_t pEvInfo, boolean doDeepInit)
{
     msg_ssTEA_Path ();

#ifdef MESA_OPTIN_DEBUG_MEMORYACCESS
    CTRICK_Prove_MemoryIsWritable ((unsigned long *) pEvInfo, (unsigned char *) __func__);
#endif  // MESA_OPTIN_DEBUG_MEMORYACCESS
    pEvInfo->MagicNumber = MakeMagicNumber (pEvInfo);

    ssE_dbOp_Init_ApifsmData (pEvInfo);

    pEvInfo->Alloc.active = false;

#ifdef SSA_OPTIN_STATS_FSM
    ssA_statOp_All_Zero (&pEvInfo->All_Stats);
#endif  // SSA_OPTIN_STATS_FSM

    // find: "ssE_AboutEv Allocation Rules"     : trusted or not, ssE allocates ssE_AboutEv during Init.
    // ssE ALWAYS consumes allocated Event data space for the "true Event Data", managed by EvInfo.
    // The implication of this is that the Event data space "gotten" by ssE is never "put back".
    // TRUSTED means the ssE pointer to critical data is given to the App when the Api FSM On occurs.
    //   and not TRUSTED means that the application must allocate an ssE_AboutEv for the API signal,
    // and API services must copy the results from the EvInfo ssE_AboutEv to the Api Signal ssE_AboutEv.
    if (doDeepInit)
    {
        pEvInfo->ssE_pAboutEv = AboutEvDB_dbOp_Get ();
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
        ssE_EvDB_Show_ssE_pAboutEv (S("ssE pEvInfo->ssE_pAboutEv during initialization : "), pEvInfo->ssE_pAboutEv);
        ss_uiOp_emit_newline ();
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
    }
}   // ssE_dbOp_Init_One_EvInfo
// -------------------------------------------------------------------------------------------------
ssE_pEvInfo_t  ssE_dbOp_Get_pEvInfo (void)
{
    int             evInfo_i;

    for (evInfo_i=0; evInfo_i <= SSE_EVINFO_MAX_I; evInfo_i++)
    {
        if (!ssE_EvInfos[evInfo_i].Alloc.active)
        {   // make active at the general level
            ssE_EvInfos[evInfo_i].Alloc.active = true;
            // return the address to someone who may want to keep track of it
            return (&ssE_EvInfos[evInfo_i]);
        }   // if not in use
    }   // for
    return (ssE_pEvInfoNull);
}   // ssE_dbOp_Get_pEvInfo
// -------------------------------------------------------------------------------------------------
void  ssE_dbOp_Put_pEvInfo (ssE_pEvInfo_t pEvInfo)
{   // make inactive at the general level
    pEvInfo->Alloc.active = false;
}   // ssE_dbOp_Put_pEvInfo

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// implement the conditions that are stated as the "init" state for the FSM, before it is activated.
// -------------------------------------------------------------------------------------------------
void ssE_dbOp_Init_ApifsmData (ssE_pEvInfo_t pEvInfo)
{
    msg_ssTEA_Path ();

    pEvInfo->Apifsm_state = ssA_Apifsm_state_off;
} // ssE_dbOp_Init_ApifsmData
// -------------------------------------------------------------------------------------------------
// implement the conditions that are stated as the "init" state for the FSM, before it is activated.
// -------------------------------------------------------------------------------------------------
void ssE_dbOp_Init_RunfsmData (ssE_pAboutEv_t ssE_pAboutEv)
{
    msg_ssTEA_Path ();

    ssE_pAboutEv->Runfsm_state  = ssA_Runfsm_State_stopped;
} // ssE_dbOp_Init_RunfsmData

#endif  // __SSE_EVENT_DEF_H

