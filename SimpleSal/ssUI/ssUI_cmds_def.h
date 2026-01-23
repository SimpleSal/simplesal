/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_cmds_def.h        data and software definitions:  command string management and publication
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_CMDS_DEF_H
#define __SSUI_CMDS_DEF_H

// =================================================================================================

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]      stored input sequences: CMD arrays
// -------------------------------------------------------------------------------------------------
#ifdef SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI

// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsMENUS[] =
{
    "echo pcCmdsMENUS",
#ifdef SSUI_OPTIN_CMDS_MENUS
      "up",
//    &pcSS,
//    &pcTime,
//    &pcStats,
//    &pcAgency,
//    &pcPace,
//    &pcStats,
//    &pcMsgSet,
//    &pcLED,
//    "up",
//    &pcMath,
//    "up",
//    &pcEvApi,
    "evapi evapi0 access",
    "evapi evapi0 access on",
    "evapi evapi0 apifsm",
    "evapi evapi0 apifsm state",
    "evapi evapi0 apifsm stats",
    "evapi evapi0 apifsm on",
    "evapi evapi0 occurs",
    "evapi evapi0 recurs",
    "evapi evapi0 period",
    "evapi evapi0 apifsm state",
    "evapi evapi0 apifsm stats",
    "evapi evapi0 apifsm off",
    "evapi evapi0 apifsm state",
    "evapi evapi0 apifsm stats",
    "evapi evapi0 access off",
#endif
    "cmds off"
};  //

// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsMATH[] =
{
    "echo pcCmdsMATH",
#ifdef SSUI_OPTIN_CMDS_MATH
    "up", "up", "up", "up",
    "math",
    "timea=timeb+timec",
    "timea=timeb-timec",
    "timea=timec+timeb",
    "timea=timec-timeb",
#endif
    "cmds off"
};
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsMATHGOOD[] =
{
    "echo pcCmdsMATHGOOD",
    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
#ifdef SSUI_OPTIN_CMDS_MATHGOOD
    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
//    "math timee=1.2.3.4.5.6.7dy",
    "math timed=1ns",
    "math timea=timee+timed",
    "math timeb=timed-timee",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "math timed=1us",
    "math timea=timee+timed",
    "math timeb=timed-timee",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "math timed=1ms",
    "math timea=timee+timed",
    "math timeb=timed-timee",
    "math timea=timee+timed",
    "math timeb=timee-timed",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "math timed=1sc",
    "math timea=timee+timed",
    "math timeb=timee-timed",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "math timed=1mn",
    "math timea=timee+timed",
    "math timeb=timed-timee",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "math timed=1hr",
    "math timea=timee+timed",
    "math timeb=timed-timee",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

    "echo *&*#*&*#*&*#*&*#*&*#*&*#*&*#***",
    "math timed=1dy",
    "math timea=timee+timed",
    "math timeb=timed-timee",

    "math timef=0",
    "math timef=timef+timea",
    "math timef=timef-timea",
    "math timef=timef+timeb",
    "math timef=timef-timeb",
    "math timec=timef",

#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsEVENTS[] =
{
    "echo pcCmdsEVENTS",
#ifdef SSUI_OPTIN_CMDS_EVENTS
    "up", "up", "up", "up",
    "math timecurr = timebuilt",
    "evapi",
    "evapi0 apifsm on",
    "evapi0 occurs timecurr + 1.0mn",
    "evapi0 apifsm validate",
    "evapi0 apifsm run",
    "evapi0 runfsm start",
    "up",
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsTIMEOFDAY[] =
{
    "echo pcCmdsTIMEOFDAY",
#ifdef SSUI_OPTIN_CMDS_TIMEOFDAY
    "up", "up", "up", "up",
    "loop",
    "math timebigbang=1.2.3.4dy",
    "math timecurr",
    "math timeb=1.2.4.0dy",
    "math timecurr",
    "loop while timecurr < timeb ",
    "loop on",
    "loop off",
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsSSUICLOCK[] =
{
    "echo pcCmdsSSUICLOCK",
#ifdef SSUI_OPTIN_CMDS_SSUICLOCK
    "math timeb=1hr",
    "math timec=0",
    "loop while timec < timeb",
    "loop on",
    "math timecurr",
    "loop off",
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsTIMERLOOP[] =
{
    "echo pcCmdsTIMERLOOP",
#ifdef SSUI_OPTIN_CMDS_TIMERLOOP
    "math timea=1.23.59.59.999.999dy",
    "math timeb=8.0.0.0.0.0dy",
    "math timec=0dy",
    "loop while timec < timeb",
    "loop on",
    "math timed=timec",
    "math timec=timed+1.0.0.0.0.1dy",
    "loop off",

    "math timea=1.23.59.59.999.999dy",
    "math timeb=-4.0.0.0.0.0dy",
    "math timec=0dy",
    "loop while timec > timeb",
    "loop on",
    "math timed=timec",
    "math timec=timed-1.0.0.0.0.1dy",
    "loop off",

    "math timea=1.23.59.59.999.999dy",
    "math timeb=-1.0.0.0.0.0dy",
    "math timec=0dy",
    "loop while timec > timeb",
    "loop on",
    "math timed=timea",
    "timea=timed+timeb",
    "math timed=timec",
    "math timec=timed-1.0.1mn",
    "loop off",
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
//  THIS IS NOT AN ARRAY OF POINTERS, IT IS A #define OF AN ARRAY OF POINTERS
#ifdef SSUI_OPTIN_CMDS_CARRY
#define Carry10             \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",          \
    "timec=timeb+timea",    \
    "timeb=timec",
#endif
// -------------------------------------------------------------------------------------------------
//  THIS IS NOT AN ARRAY OF POINTERS, IT IS A #define OF AN ARRAY OF POINTERS
#ifdef SSUI_OPTIN_CMDS_BORROW
#define Borrow10            \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",          \
    "timec=timeb-timea",    \
    "timeb=timec",
#endif
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsBORROW[] =
{
    "echo pcCmdsBORROW",
#ifdef pcCmdsBORROW
    "up", "up", "up", "up",
    "math",
     Borrow10
#endif
    "cmds off"
};  //
pCmd_t pcCmdsCARRY[] =
{
    "echo pcCmdsCARRY",
#ifdef SSUI_OPTIN_CMDS_CARRY
    "up", "up", "up", "up",
    "math",
     Carry10
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsCMDZONES[] =
{
    "echo pcCmdsCMDZONES",
#ifdef SSUI_OPTIN_CMDS_CMDZONES
    "cmds", "up", "loop", "up", "math", "up", "evapi", "up",
    "cmds", "loop", "math", "evapi", "cmds", "up", "up", "up", "up", "up",
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsERRORS[] =
{
    "echo pcCmdsERRORS",
#ifdef SSUI_OPTIN_CMDS_ERRORS
    "9a", "61sc", "1577ms", "0dy", "25hr", "88mn", "1111ms", "2222us", "3333ns",
#endif
    "cmds off"
};  //
// -------------------------------------------------------------------------------------------------
pCmd_t pcCmdsPASSES[] =
{
    "echo pcCmdsPASSES",
#ifdef SSUI_OPTIN_CMDS_PASSES
    "up", "up", "up", "up",
    "math",
    "20", "59sc", "999ms", "10dy", "23hr", "59mn", "999ms", "999us", "1000ns",
    "30", "88sc", "66ms",  "33dy", "34hr", "60mn", "1sms", "1zss", "0ns",
    "40", "44sc", "45ms",  "46dy", "47hr", "48mn", "49ms", "41us", "42ns",
    "50", "50sc", "50ms",  "50dy", "50hr", "50mn", "50ms", "50us", "50ns",
    "timea=23.58.333.333m",
    "timeb=0.0.334.111m",
    "timec=timea+timeb",
    "timec=timeb+timea",
    "timec=timea+timea",
    "timec=timeb+timeb",
    "timed=timec",
    "timea=timeb",
    "timec=timed+timea",
    "up",
#endif
    "cmds off"
};  //

// -------------------------------------------------------------------------------------------------
ssUI_Token_t  cmdsTkn_menus      = { S("menus"),     5, NoBananas };
ssUI_Token_t  cmdsTkn_math       = { S("math"),      4, NoBananas };
ssUI_Token_t  cmdsTkn_mathgood   = { S("mathgood"),  8, NoBananas };
ssUI_Token_t  cmdsTkn_events     = { S("events"),    6, NoBananas };
ssUI_Token_t  cmdsTkn_timeofday  = { S("timeofday"), 9, NoBananas };
ssUI_Token_t  cmdsTkn_ssUIclock  = { S("ssUIclock"), 9, NoBananas };
ssUI_Token_t  cmdsTkn_loop       = { S("loop"),      4, NoBananas };
ssUI_Token_t  cmdsTkn_timerloop  = { S("timerloop"), 9, NoBananas };
ssUI_Token_t  cmdsTkn_borrow     = { S("borrow"),    6, NoBananas };
ssUI_Token_t  cmdsTkn_carry      = { S("carry"),     5, NoBananas };
ssUI_Token_t  cmdsTkn_cmdzones   = { S("cmdzones"),  8, NoBananas };
ssUI_Token_t  cmdsTkn_time       = { S("time"),      4, NoBananas };
ssUI_Token_t  cmdsTkn_errors     = { S("errors"),    6, NoBananas };
ssUI_Token_t  cmdsTkn_passes     = { S("passes"),    6, NoBananas };

#endif  // SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\SW Developer Tricks.note]   automatic generation of array maximum index
// -------------------------------------------------------------------------------------------------
// This instance of the SW developer trick generates a valid maximum index value for a CmdArrays.
// -------------------------------------------------------------------------------------------------
#define ComputepCharNArray_CT(Array)        (sizeof(Array) / sizeof(pCmdArray_t))
#define Compute_pCharNArray_MAX_I(Array)    (ComputepCharNArray_CT(Array)-1)

#ifndef SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP
#define SSUI_PLEASE_INCLUDE_THESE_APP_CMDS
#endif  // SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP

// -------------------------------------------------------------------------------------------------
CmdArrayLink_t  CmdArrayLinks[] =
{  // <test name to match>   <&(array of char const *)>  <(array of char const *) max index>
#ifdef SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP
    SSUI_PLEASE_INCLUDE_THESE_APP_CMDS
#endif  // SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP
#ifdef SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI
    {  &cmdsTkn_menus     ,  &pcCmdsMENUS[0]      ,      Compute_pCharNArray_MAX_I(pcCmdsMENUS    )  } ,
    {  &cmdsTkn_math      ,  &pcCmdsMATH[0]       ,      Compute_pCharNArray_MAX_I(pcCmdsMATH     )  } ,
    {  &cmdsTkn_mathgood  ,  &pcCmdsMATHGOOD[0]   ,      Compute_pCharNArray_MAX_I(pcCmdsMATHGOOD )  } ,
    {  &cmdsTkn_events    ,  &pcCmdsEVENTS[0]     ,      Compute_pCharNArray_MAX_I(pcCmdsEVENTS   )  } ,
    {  &cmdsTkn_timeofday ,  &pcCmdsTIMEOFDAY[0]  ,      Compute_pCharNArray_MAX_I(pcCmdsTIMEOFDAY)  } ,
    {  &cmdsTkn_ssUIclock ,  &pcCmdsSSUICLOCK[0]  ,      Compute_pCharNArray_MAX_I(pcCmdsSSUICLOCK)  } ,
    {  &cmdsTkn_timerloop ,  &pcCmdsTIMERLOOP[0]  ,      Compute_pCharNArray_MAX_I(pcCmdsTIMERLOOP)  } ,
    {  &cmdsTkn_borrow    ,  &pcCmdsBORROW[0]     ,      Compute_pCharNArray_MAX_I(pcCmdsBORROW   )  } ,
    {  &cmdsTkn_carry     ,  &pcCmdsCARRY[0]      ,      Compute_pCharNArray_MAX_I(pcCmdsCARRY    )  } ,
    {  &cmdsTkn_cmdzones  ,  &pcCmdsCMDZONES[0]   ,      Compute_pCharNArray_MAX_I(pcCmdsCMDZONES )  } ,
    {  &cmdsTkn_errors    ,  &pcCmdsERRORS[0]     ,      Compute_pCharNArray_MAX_I(pcCmdsERRORS   )  } ,
    {  &cmdsTkn_passes    ,  &pcCmdsPASSES[0]     ,      Compute_pCharNArray_MAX_I(pcCmdsPASSES   )  }
#endif      // SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI
};
// -------------------------------------------------------------------------------------------------
// These defined macros are referenced by initialization of the table field "how many pointers?".
// -------------------------------------------------------------------------------------------------
#define CmdArrayLinks_CT       (sizeof (CmdArrayLinks) / sizeof(CmdArrayLink_t))
#define CmdArrayLinks_MAX_I    ((int)((CmdArrayLinks_CT)-1))

// -------------------------------------------------------------------------------------------------
// Cmds Processing FSM: while ssUI "executes" a CMD array, this points to the CMD array's first CMD.
// -------------------------------------------------------------------------------------------------
static pCmdArrayLink_t  gpCmdArrayActive = pCmdArrayLinkNull;

// -------------------------------------------------------------------------------------------------
pAscii_t   ssUI_cmdsOp_nextCmdFromLinkActive (int from_pcCmd_i)
{
    if (gpCmdArrayActive == pCmdArrayLinkNull)
    {
#ifdef SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_qBanner (lfY, "next-in-CmdArray finds no Cmd array link active", lfY);
#endif  // SSUI_OPTIN_DEBUG_CMDS_I
        return (pAsciiNull);
    }

    // ---------------------------------------------------------------------------------------------
    // The link contains a value for the number of pointers in the array, pointers to CmdArrays.
    // The "stop" Cmd with a specific content will stop Cmd processing regardless of Array position.
    // ---------------------------------------------------------------------------------------------
    if (
        (from_pcCmd_i > gpCmdArrayActive->CmdArray_max_i)
          ||
        (ssUI_AaIf_pP1_eq_pP2 ((pAscii_t) "cmds off", (pAscii_t) gpCmdArrayActive->pCmdArray[from_pcCmd_i]))
       )
    {
#ifdef SSUI_OPTIN_DEBUG_CMDS_I
        ss_uiOp_emit_lbld_int ("cmds_i", from_pcCmd_i);
        ss_uiOp_emit_lbld_int ("max_i", gpCmdArrayActive->CmdArray_max_i);
        ss_uiOp_qBanner (lfY, "get-next-CmdArray finds end of array of Cmds", lfY);
#endif  // SSUI_OPTIN_DEBUG_CMDS_I
        return (pAsciiNull);
    }

    // pCmdArray is an array of pointers to variable-lengthed arrays of Ascii values.  The
    // variable-lengthed arrays are created using the C language syntax for defining a "string".
    // Return a value that is of the type ssUI uses (pointer to Ascii value)
    //      by asking the ssHL compiler to typecast ssHL old-style strings as the ssUI type.
    // This does not create any code, unless the host language implementation of "string"
    // gets in the way.  If there is a conversion needed, here is where it should occur.
    return ((pAscii_t) gpCmdArrayActive->pCmdArray[from_pcCmd_i]);

}   // ssUI_cmdsOp_nextCmdFromLinkActive
// -------------------------------------------------------------------------------------------------
int ssUI_cmdsOp_max_i (void)
{
    if (gpCmdArrayActive == pCmdArrayLinkNull)
    {
        return (0);
    }

    return (gpCmdArrayActive->CmdArray_max_i);
}   // ssUI_cmdsOp_max_i
// -------------------------------------------------------------------------------------------------
boolean ssUI_cmdsOp_IfFindName_SetLinkActive(pAsciiA_t pFindAscii)
{
    int lclCmd_i = 0;

    while (lclCmd_i <= CmdArrayLinks_MAX_I)
    {
        if (ssUI_tknIf_pP1_eq_pAa (CmdArrayLinks[lclCmd_i].pCmdArray_name_tkn, pFindAscii))
        {
            gpCmdArrayActive = &CmdArrayLinks[lclCmd_i];
            return (true);
        }
        else
        {
            lclCmd_i++;
        }
    }   // while
    gpCmdArrayActive = pCmdArrayLinkNull;
    return (false);
}   // ssUI_cmdsOp_IfFindName_SetLinkActive
// -------------------------------------------------------------------------------------------------
int ssUI_cmdsOp_Find_CmdFromHere (pAsciiA_t pFindAscii, int  from_pcCmd_i)
{
    int test_pcCmd_i = from_pcCmd_i;

    while (test_pcCmd_i <= gpCmdArrayActive->CmdArray_max_i)
    {
        if (
              ssUI_AaIf_pP1_eq_pP2
              (
                (pAscii_t) gpCmdArrayActive->pCmdArray[test_pcCmd_i],
                pFindAscii
              )
            )
        {
            break;  // out of while
        }
        else
        {
            test_pcCmd_i++;
        }
    }   // while
    return (test_pcCmd_i);
}   // ssUI_cmdsOp_Find_CmdFromHere
// -------------------------------------------------------------------------------------------------
int ssUI_cmdsOp_Find_LoopOff (int  from_pcCmd_i)
{
    return (ssUI_cmdsOp_Find_CmdFromHere (S("loop off"), from_pcCmd_i));

}   // ssUI_cmdsOp_Find_LoopOff
// -------------------------------------------------------------------------------------------------
void ssUI_cmdsOp_Show_AllCommands (void)
{
    int lclCmd_i = 0;

    while (lclCmd_i <= CmdArrayLinks_MAX_I)
    {
        ss_uiOp_emit_pAsciiA (CmdArrayLinks[lclCmd_i].pCmdArray_name_tkn->pAsciiA);
        ss_uiOp_emit_Space (ssUI_standard_fieldgap);
        lclCmd_i++;
        if ((lclCmd_i % 8) == 0)
        {
            ss_uiOp_emit_newline ();
        }
    }   // while
}   // ssUI_cmdsOp_Show_AllCommands

#endif  // __SSUI_CMDS_DEF_H

