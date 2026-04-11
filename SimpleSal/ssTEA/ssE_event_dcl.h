/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssE_event_dcl.h      data type, prototype declarations: the implementation of Events by ssTEA
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSE_EVENT_DCL_H
#define __SSE_EVENT_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A Mesa has a processor which was constructed with knowledge of an entire vocabulary of words.
// Software says the word, the processor reacts in a deterministic and predictable way, every time.
// Software authors write stories using invented words that translate into words the processor uses.
// "compiler language terms" are specific words used to write the story the processor understands.
// Type definitions create a list of words; they do not create a story with a plot and happy ending.
// -------------------------------------------------------------------------------------------------
// Unless another note is named in this file, descriptions of these compiler language terms are in
// the note [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note].  Events are described there; this
// file is the compiler language declarations used to write the story with a plot and happy ending.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
typedef enum ssE_EvType_e
{
    ssE_EvType_Time,
    ssE_EvType_Data,
    ssE_EvType_Ireq,
    ssE_EvType_flaw
}   ssE_EvType_t;
// -------------------------------------------------------------------------------------------------
typedef enum ssE_EvResult_e
{
    ssE_EvResult_OK_go,
    ssE_EvResult_OK_stopMe,
    ssE_EvResult_OK_pauseMe,
    ssE_EvResult_notOK_halt,
    ssE_EvResult_notOK_data,
    ssE_EvResult_notOK_time,
    ssE_EvResult_notOK_pEvFunc,
    ssE_EvResult_notOK_state,
    ssE_EvResult_notOK_reset,
    ssE_EvResult_notOK_flaw,
    ssE_EvResult_notOK_API,
    ssE_EvResult_user1,
    ssE_EvResult_user2,
    ssE_EvResult_user3,
    ssE_EvResult_user4,
    ssE_EvResult_user5
} ssE_EvResult_t;

#define InAgencyResultSuccessRange(x)   ((x >= ssE_EvResult_OK_go) && (x <= ssE_EvResult_OK_pauseMe))
#define InRange_AgencyResultFailures(x) ((x >= ssE_EvResult_notOK_halt) && (x <= ssE_EvResult_notOK_flaw))

// =================================================================================================
// -------------------------------------------------------------------------------------------------
typedef enum ssA_Apifsm_state_e
{
    ssA_Apifsm_state_off,
    ssA_Apifsm_state_on_running,
    ssA_Apifsm_state_on_reset,
    ssA_Apifsm_state_flaw
}   ssA_Apifsm_state_t;
// -------------------------------------------------------------------------------------------------
typedef enum ssA_Runfsm_State_e
{
    ssA_Runfsm_State_stopped,
    ssA_Runfsm_State_paused,
    ssA_Runfsm_State_agencying,
    ssA_Runfsm_State_flaw
}   ssA_Runfsm_state_t;
// -------------------------------------------------------------------------------------------------
typedef pVoid_t                 ssE_pVoidEvParam1_t;
#define pVoidEvParam1Null   ((ssE_pVoidEvParam1_t) NULL)

typedef ssE_EvResult_t(* EvAg_pEvFunc_t) (ssE_pVoidEvParam1_t, ssT_pTime_t);

#define pEvFuncNull             ((EvAg_pEvFunc_t) NULL)

typedef pVoid_t                 EvEx_App_pEventData_t;
#define EvEx_App_pEventDataNull ((EvEx_App_pEventData_t) NULL)

typedef pVoid_t                 EvEx_pAgContext_t;
#define pAgContextNull          ((EvEx_pAgContext_t) NULL)


// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The App and ssTEA's Agency software know things About An Event.  There is one true description of
// each Event.  This description contains, and must contain, all the information about the Event.
// The difference between the App and ssTEA's Agency software is the method used to keep track of
// the About data for EACH Event.  Both have direct access to the data; they each have a pointer.
// A Trusted App refers to the same true data as ssTEA; an unTrusted App refers to its current copy.
// -------------------------------------------------------------------------------------------------
typedef struct ssE_AboutEvDesc_s
{
    ssA_Runfsm_state_t      Runfsm_state;       // the Agency Run FSM state for this event
    ssE_EvType_t            Runfsm_EvType;      // the event's type (time or data or interrupt)

    // valid values in these variables are required for a Time Event; ignored by other Event types

    ssT_Time_t              OccursAt;           // Time when this event is to occur
    ssT_Time_t              Period;             // period until event recurs after this occurence
    boolean                 recurs;             // [T: OccursAt += period | F: OccursAt untouched]
    boolean                 recursFromTrue;     // add period to [T: Time occured | F: Time Requested]

    EvAg_pEvFunc_t          pEvFunc;            // pointer to ssHL code to run when the event occurs
    ssE_EvResult_t          AgencyResult;       // as returned by the Event when last granted Agency

    EvEx_App_pEventData_t   App_pEventData;     // ssTEA has no definition; sees as pointer to void

    EvEx_pAgContext_t       pAgContext;         // App has no definition; promises to not reference

}   ssE_AboutEvDesc_t, *ssE_pAboutEvDesc_t;

typedef ssE_AboutEvDesc_t     ssE_AboutEv_t;
typedef ssE_pAboutEvDesc_t    ssE_pAboutEv_t;

#define ssE_pAboutEvNull       ((ssE_pAboutEv_t) NULL)

typedef pVoid_t                 ssE_pAboutEvVoid_t;
#define ssE_pAboutEvVoid       ((ssE_pAboutEvVoid_t) ssE_pAboutEvNull)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Using a specific method of allocating event descriptors, per-event data is maintained by ssTEA.
// Any method supports the tracking of active/inactive at the general level as referenced by ssTEA.
// The easy method allocates all data structures at compile time, and then manages them at run time.
// -------------------------------------------------------------------------------------------------
#define METHOD_EASY
// #define METHOD_MADEUP

typedef struct ssE_EventAlloc_t
{
    boolean                 active;
#ifdef METHOD_EASY
#endif
#ifdef METHOD_MADEUP
// An arbitrarily complex solution could be implemented, separate but alongside the Event Data itself
#endif
}   ssE_EventAlloc_t, *ssE_pEventAlloc_t;

typedef ssE_EventAlloc_t   ssE_EvAlloc_t;
typedef ssE_pEventAlloc_t  ssE_pEvAlloc_t;

#define pEventAllocNull     ((ssE_pEventAlloc_t) NULL)
// -------------------------------------------------------------------------------------------------
typedef struct ssE_EventInfoDesc_s
{
    ssA_Apifsm_state_t      Apifsm_state;   // Agency Api FSM treats an event as a resource

    ssE_pAboutEv_t          ssE_pAboutEv;   // this is a pointer to the allocated Event's AboutEvent data

    ssE_EventAlloc_t        Alloc;          // this is allocation method for EventInfoDesc data

    void *                  MagicNumber;    // used during the channel verification process

#ifdef SSA_OPTIN_STATS_FSM
    ssA_All_Stats_t         All_Stats;      // maintained for this event independent of others
#endif  // SSA_OPTIN_STATS_FSM

    boolean                 AgenciedThisPlanck; // while On and running, EvInfo accessed by AgencyNext

}   ssE_EventInfoDesc_t, *ssE_pEventInfoDesc_t;

typedef         ssE_EventInfoDesc_t    ssE_EvInfo_t;
typedef         ssE_pEventInfoDesc_t   ssE_pEvInfo_t;

#define         ssE_pEvInfoNull    ((ssE_pEvInfo_t) NULL)

// -------------------------------------------------------------------------------------------------
ss_macSNR_t     ssE_Initialize          (void);
ss_macSNR_t     ssE_InitData_RunTime     (boolean doDeepInit);

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note]        ssE declares what the interface will be
// [.\SimpleSal\ssDocs\ssTEA\Event Databases.note]          ssE or the App instantiates the database
// -------------------------------------------------------------------------------------------------
void            AboutEvDB_dbOp_Init_DB (void);
ssE_pAboutEv_t  AboutEvDB_dbOp_Get (void);
ssE_pAboutEv_t  AboutEvDB_dbOp_Put (ssE_pAboutEv_t pReleaseThis);

#define         MakeMagicNumber(value) ((pVoid_t) ~((unsigned long) value))

// -------------------------------------------------------------------------------------------------
void            ssE_dbOp_Init_One_AboutEv (ssE_pAboutEv_t ssE_pAboutEv, boolean doDeepInit);
void            ssE_dbOp_Init_EvInfo (ssE_pEvInfo_t pEvInfo);
void            ssE_dbOp_Init_One_EvInfo (ssE_pEvInfo_t pEvInfo, boolean doDeepInit);

ssE_pEvInfo_t   ssE_dbOp_Get_pEvInfo (void);
void            ssE_dbOp_Put_pEvInfo (ssE_pEvInfo_t pEvInfo);

void            ssE_dbOp_Init_ApifsmData (ssE_pEvInfo_t pEvInfo);
void            ssE_dbOp_Init_RunfsmData (ssE_pAboutEv_t ssE_pAboutEv);

pAsciiA_t       ssE_EvType_pDesc (ssE_EvType_t EvType);

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
void ssE_EvDB_Show_ssE_pAboutEv (pAsciiA_t pDesc, ssE_pAboutEv_t ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG

#endif  // __SSE_EVENT_DCL_H

