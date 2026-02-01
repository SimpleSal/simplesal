/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_api_dcl.h      data type, prototype declarations: API that implements ssTEA
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSTEA_API_DCL_H
#define __SSTEA_API_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// the collection of ssTEA FSMs all have an Init State.  Each is granted Agency, in turn, to start.
// -------------------------------------------------------------------------------------------------
ss_macSNR_t         ssTEA_InitState (void);

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// the two aspects to the ssT_Event software are: 1) the Event management API; 2) Event agency.
// said another way, there is an API side to events, and there is also an execution/run side.
// -------------------------------------------------------------------------------------------------
// The Event API is an Abstracted Programming Interface: an abstraction of events for the user.
// Event management references all aspects of ssTEA, providing access to features offered.
// Event Agency is the ssTEA mechanism that causes the Event to occur at the time requested.
// Inevitably, the ssTEA understanding will have to be mapped into the App's understanding.
// this will be true for anyone integrating ssTEA into an existing software application.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// API considerations for Events: parameters are provided to ssTEA that must exist and be accessible
// at the point in Time that the Event executes. This requires that data structures are allocated
// within the App where access is permitted; they describe an event managed by the User.
// Said more plainly: you cannot use a stack variable to allocate data the event will use later.
// -------------------------------------------------------------------------------------------------
// Bi-directional API: a data structure is used to communicate information in both directions.
//   Each interaction starts with a signal in one direction: something for you to receive.
//   That signal is respcOnded to with a signal in the other direction: results.
//   The signal carrier in both cases is the same data structure in memory, one for each Channel.
// ssTEA Apis are bi-directional signaling, it is possible and easy to write a uni-directional signal.
// that is in fact what all the ssUI message string outputs are doing: signaling the user.
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// calls to ssTEA to manage an event will use this data structure to communicate information.
// ssTEA makes use of the Event data type in the data structures used to manage events over Time.
// The user makes use of the Event data type in data structures used to manage events over Time.
// signalers relating to the FSMs Agency Api and Agency Run use the Event data structure, other
// Api services relating to statistics and state reporting use a void pointer to a data structure.
// Typecasting the assignment before calling and extracting the pointer in ssTEA is service-specific.
// -------------------------------------------------------------------------------------------------
typedef struct ssTEA_ApiSignalDesc_s
{
    ssA_Apifsm_state_t  Apifsm_state;   // the Agency Api FSM manages a Signal Carrier

    ss_ApiSigDir_e      sigDir;         // one API structure is used bidirectionally to communicate.
    ss_ApiSigValue_e    sigValue;       // Agency Api and Agency Run FSMs accept and emit signals.
    ss_macSNR_e         macSNR;         // the signal quality of this signal on this carrier
    ss_apiSNR_e         apiSNR;         // response from the API about params; an abstract signal.

    ssE_pAboutEv_t      ssE_pAboutEv;   // there must be an allocated structure somewhere to hold data

    void *              pApiSigData;    // optional data definition, by service: statistics report

    ss_EvInfoChannel_t  channel;        // mystical value mapping user event to ssE/ssA EvInfo data

}   ssTEA_ApiSignalDesc_t, *ssTEA_pApiSignalDesc_t;

typedef ssTEA_ApiSignalDesc_t   ssTEA_ApiSig_t;
typedef ssTEA_pApiSignalDesc_t  ssTEA_pApiSig_t;
#define pApiSigNull             ((ssTEA_pApiSig_t) NULL)

#define pApiSigDataNull         ((void *) NULL)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// In terms of memory needed to interact with ssTEA: a signal carrier and an event Agency structure.
// This structure may be used for ALLOCATION by an ssTEA API user: one per carrier, modifiable by all.
// Notice that all of the fields here are structures, not pointers to structures, memory is allocated.
// -------------------------------------------------------------------------------------------------
// Within the signal carrier data are pointers to the ssE_EventData for any event, allocated by App/ssE.
// To use the same signal carrier for all events, it is possible to change the pEventData value.
// This permits allocating Event Agency memory for each Event and allocating one signal carrier.
// -------------------------------------------------------------------------------------------------
typedef struct ssTEA_AppDataNeededPerEvent_s
{
    ssTEA_ApiSig_t              ssTEA_ApiSig;   // signal carrier for ssTEA API requests/responses

    ssT_Time_t                  TimeVar;        // may be used for mathOp purposes, not required

}   ssTEA_AppDataNeededPerEvent_t,  *ssTEA_pAppDataNeededPerEvent_t;

typedef ssTEA_AppDataNeededPerEvent_t   ssTEA_AppDataNeeded_t;
typedef ssTEA_pAppDataNeededPerEvent_t  ssTEA_pAppDataNeeded_t;
#define ssTEA_pAppDataNeededNull        ((ssTEA_pAppDataNeeded_t) NULL)

// =================================================================================================
pAscii_t    ssTEA_apiOp_Get_pApiMessage (ssTEA_pApiSig_t pApiSig);
void        ssTEA_apiOp_Show_SignalData (ssTEA_pApiSig_t pApiSig);

void        ssTEA_dbOp_Init_SigData (ssTEA_pApiSig_t pApiSig, boolean doDeepInit);
#define     DEEPINIT  (true)
#define     TIDYINIT  (false)

pAscii_t    ss_uiOp_Get_pName_ssState           (ssTEA_state_t state);
pAscii_t    ss_uiOp_Get_pName_ssAgency_Pace     (ssTEA_Agency_Pace_t Agency_Pace);

pAscii_t    ss_uiOp_Get_pName_Runfsm_state      (ssA_Runfsm_state_t Runfsm_state);
void        ss_uiOp_Show_RunfsmData             (ssE_pAboutEv_t ssE_pAboutEv);

pAscii_t    ss_uiOp_Get_pName_Apifsm_state      (ssA_Apifsm_state_t Apifsm_state);
void        ss_uiOp_Show_Apifsm_state           (ssTEA_pApiSig_t pApiSig, pAsciiA_t pApiSigName);
void        ss_uiOp_Show_Apifsm_result \
    (ssTEA_pApiSig_t pApiSig, ssA_Apifsm_state_t ExpectedState, pAsciiA_t pApiSigName);

void        ss_uiOp_Show_Runfsm_state  (ssTEA_pApiSig_t pApiSig, pAsciiA_t pApiSigName);
void        ss_uiOp_Show_Runfsm_result \
    (ssTEA_pApiSig_t pApiSig, ssA_Runfsm_state_t ExpectedState, pAsciiA_t pApiSigName);

pAsciiA_t   ss_uiOp_Get_pName_EvType            (ssE_EvType_t EvType);
void        ss_uiOp_Show_EventData              (ssE_pAboutEv_t ssE_pAboutEv);
void        ss_uiOp_Show_AgencyResult           (ssE_EvResult_t AgencyResult);

void        ss_uiOp_Show_ssTime_state    (void);
void        ss_uiOp_Show_ssTimeRatios    (void);

#endif  // __SSTEA_API_DCL_H

