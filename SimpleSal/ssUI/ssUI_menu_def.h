/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_menu_def.h        data and software definitions:  command and parameter handlers for the UI
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_MENU_DEF_H
#define __SSUI_MENU_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// "ssUI_menu_dcl.h" and "ssUI_menu_def.h" contain the table-based data for menu handling.
// Work-in-Process handlers can be started and developed here for an easier file experience.
// "ssUI_menucore_*.h" was used for stable and probably working software that doesn't change.
// The user will have included "ssUI_menu_dcl.h" and "ssUI_menucore_dcl.h") to get here.
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The commands and parameters of the UI are handled by what ssTEA calls FSMs; this FSM uses a table.
// The FSM is run through an array for all of the strings in all of the positions possibly entered.
//   In the array, the first column contains a "command", the remaining columns contain "parameters".
// When a string is matched, a function is run to handle that string, in the same position in an
// array of functions.  Each handles the FSM state described by the sequence of strings, so far.
//
// The underlying assumption is that any text-based UI cannot have duplicate or identical sequences
// of tokens that have different meanings, once fully comprehended. Each combination is unique, of
// any strings used.  The "yes" Handler for "light on yes" is not the same as for "light blue yes".
// A combination of command+parameter(s) is consumed by an FSM, one token at a time, until handled.
//
// States are mutually exclusive, because each "current state" exists at a row:column position in
// the table.  If the handler for that state is running, preceding states have occured and deferred.
// The FSM designer chooses when processing actions occur as progress is made through the tokens.
//    Each column handler may defer to the next column's handler, or, defer to the NEXT COMMAND's
//    hander using information accrued by the FSM up that point (see Event Api subcmd Handlers).
//
// If tokens <a><b><c> are entered, and they match the <a> command with parameters <b> and <c>,
// the handler FSM is in "state <a><b><c>".  There is one place "state <a><b><c>" is handled, that
// place can be encapsulated in handlers for <a>, for <b>, for <c>, or, a handler for <a><b><c>.
// Groups of rows, multiple commands or parameter variations, probably share the same FSM.
//
// Models possible for handling a row of tokens:
// 2) write one handler for each state (row:column), called as matching strings are found
// 2) write one handler for each row, calls by column # pushes the FSM into Handler using column #.
// 3) write one handler to handle all rows and states in rows (call an existing parser you wrote).
//      You could use a switch statement of command strings; for each command parameter a switch.
//      You would then remember that switch requires an ordinal value.  Humans are string-based.
//
// a command line token that matches the string for the handler will cause the handler to run.
// commands with three unique parameters (off/on/reset) can make a handler for each is matched.
// a "wild card" string will run the handler for any string, which will then be interpreted by
// the handler in the context in which it is found.  This allows a wider range of strings to be
// handled without the need to create rows in the CmdParam/Help/Params arrays.
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// column index range is defined by this count of columns, must match rows/columns in all arrays.
// note that changing the number of tokens allowed globally will change the number of columns here.
// -------------------------------------------------------------------------------------------------
#define CMD_TOKEN_CT    SSUI_TOKENS_ALLOC
#define CMD_TOKEN_MAX_I (CMD_TOKEN_CT-1)

// -------------------------------------------------------------------------------------------------
// This is only to create a demonstration of a basic handler that can be used as a reference.
// The command Handler table entry with no matching string should contain pHNull, not &unAssigned.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t unAssignedHandler (int tkn_i, boolean moreInputTkns)
{
    // this expressions says if "a valid token index and more tokens, defer to the next handler";
    //    else-if "a valid token index and not more tokens, completed"; else-if "bad index, Error".
    return ((tkn_i <= CMD_TOKEN_MAX_I) ? ((moreInputTkns) ? eH_rDefer : eH_rHandled) : (eH_rError));
}   // unAssignedHandler

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The command processor bases execution of a CmdParamFunc based on the presence of a series of
// tokens, as found in this table.  A match for a token in an a input value makes CmdParamFunc run.
// -------------------------------------------------------------------------------------------------
// The number of rows in the ssUI_CmdParamWords is determined by the initialization sequence defined.
// CmdParamHints and CmdParamFuncs have a row count (CMD_CT), a computed value from CmdParamWords.
// To add commands: add strings to compare to input, duplicate as hints, add handlers for each.
// The ssHL compiler will warn if there are too many initializers in Hints/Funcs, but not for too few.
// -------------------------------------------------------------------------------------------------
pAsciiA_t  ssUI_CmdParamWords[][CMD_TOKEN_CT] =
{
//    tkn1st_i       tkn2nd_i      tkn3rd_i      tkn4th_i      tkn5th_i      tkn6th_i
    { pcUp,          pAAN,         pAAN,         pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcTime,       pcStart,      pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcTime,       pcStop,       pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcTime,       pcBigBang,    pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcTime,       pcStats,      pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcAgency,     pcPace,       pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcSS,          pcAgency,     pcStart,      pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcAgency,     pcStop,       pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcAgency,     pcStats,      pAAN,         pAAN,         pAAN       } ,
    { pcSS,          pcMsgSet,     pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk,   pAAN       } ,
    { pcSS,          pcRO,         pcLED,        pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcSS,          pcRO,         pcAscii,      pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcSS,          pcRO,         pcMatrix,     pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcMath,        pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk } ,
    { pcLoop,        pcOn,         pAAN,         pAAN,         pAAN,         pAAN       } ,
    { pcLoop,        pcOff,        pAAN,         pAAN,         pAAN,         pAAN       } ,
    { pcLoop,        pcWhile,      pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk } ,
    { pcLoop,        pcUntil,      pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk } ,
    { pcEvApi,       pcOpAstrsk,   pcClear,      pAAN,         pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcAccess,     pcOn,         pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcAccess,     pcOff,        pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcAccess,     pcInclude,    pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcAccess,     pcExclude,    pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcAccess,     pcLock,       pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcAccess,     pcUnlock,     pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcOn,         pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcOff,        pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcRun,        pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcReset,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcValidate,   pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcState,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcApifsm,     pcStats,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRunfsm,     pcStart,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRunfsm,     pcStop,       pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRunfsm,     pcPause,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRunfsm,     pcResume,     pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRunfsm,     pcState,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRunfsm,     pcStats,      pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcOccurs,     pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk } ,
    { pcEvApi,       pcOpAstrsk,   pcPeriod,     pcOpAstrsk,   pcOpAstrsk,   pcOpAstrsk } ,
    { pcEvApi,       pcOpAstrsk,   pcRecurs,     pcOn,         pAAN,         pAAN       } ,
    { pcEvApi,       pcOpAstrsk,   pcRecurs,     pcOff,        pAAN,         pAAN       } ,
    { pcHelp,        pcOpAstrsk,   pcOpAstrsk,   pAAN,         pAAN,         pAAN       } ,
    { pcCmds,        pcRun,        pcOpAstrsk,   pAAN,         pAAN,         pAAN       } ,
    { pcCmds,        pcHalt,       pAAN,         pAAN,         pAAN,         pAAN       }
};
// don't like the complex array notation? create a type that is an array of CMD_TOKEN_CT values.
#define CMD_CT      (ComputeArray_CT (ssUI_CmdParamWords, (sizeof (pAsciiA_t)*CMD_TOKEN_CT)))
#define CMD_MAX_I   (CMD_CT-1)

// =================================================================================================
pAsciiA_t  ssUI_CmdParamHints[CMD_CT][CMD_TOKEN_CT] =
{
//    tkn1st_i     tkn2nd_i             tkn3rd_i        tkn4th_i        tkn5th_i       tkn6th_i
    { pcUp,        pAAN,                pAAN,           pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcTime,              pcStart,        pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcTime,              pcStop,         pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcTime,              pcBigBang,      pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcTime,              pcStats,        pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcAgency,            pcPace,   pcHelpPacePeriod,     pAAN,          pAAN  } ,
    { pcSS,        pcAgency,            pcStart,        pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcAgency,            pcStop,         pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcAgency,            pcStats,        pAAN,           pAAN,          pAAN  } ,
    { pcSS,        pcMsgSet,   pcHelpSetSignal,    pcHelpSetSensor,     pcHelpSetMsg,  pAAN  } ,
    { pcSS,        pcRO,                pcLED,        pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcSS,        pcRO,                pcAscii,      pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcSS,        pcRO,                pcMatrix,     pcOpAstrsk,   pAAN,         pAAN       } ,
    { pcMath,      pcHelpLvalue,  pcOpAssign,  pcHelpOper1,   pcHelpMathOp,   pcHelpOper2    } ,
    { pcLoop,      pcOn,                pAAN,           pAAN,           pAAN,          pAAN  } ,
    { pcLoop,      pcOff,               pAAN,           pAAN,           pAAN,          pAAN  } ,
    { pcLoop,      pcWhile,     pcOpAssign,    pcHelpOper1,   pcHelpCmpOp,    pcHelpOper2    } ,
    { pcLoop,      pcUntil,     pcOpAssign,    pcHelpOper1,   pcHelpCmpOp,    pcHelpOper2    } ,
    { pcEvApi,     pcHelpEvApiN,        pcClear,        pAAN,           pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcAccess,       pcOn,           pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcAccess,       pcOff,          pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcAccess,       pcInclude,      pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcAccess,       pcExclude,      pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcAccess,       pcLock,         pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcAccess,       pcUnlock,       pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcOn,           pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcOff,          pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcRun,          pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcReset,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcValidate,     pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcState,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcApifsm,       pcStats,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRunfsm,       pcStart,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRunfsm,       pcStop,         pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRunfsm,       pcPause,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRunfsm,       pcResume,       pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRunfsm,       pcState,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRunfsm,       pcStats,        pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,    pcOccurs,    pcHelpOper1,   pcHelpMathOp,   pcHelpOper2  } ,
    { pcEvApi,     pcHelpEvApiN,    pcPeriod,    pcHelpOper1,   pcHelpMathOp,   pcHelpOper2  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRecurs,       pcOn,           pAAN,          pAAN  } ,
    { pcEvApi,     pcHelpEvApiN,        pcRecurs,       pcOff,          pAAN,          pAAN  } ,
    { pcHelp,      pcHelpTopics,        pcHelpCommands, pAAN,           pAAN,          pAAN  } ,
    { pcCmds,      pcRun,               pcHelpCmdsRun,  pAAN,           pAAN,          pAAN  } ,
    { pcCmds,      pcHalt,              pAAN,           pAAN,           pAAN,          pAAN  }
};
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// definition of table placed after handler definitions, to avoid excessive forward declarations.
// -------------------------------------------------------------------------------------------------
// The search mechanism matches input strings to strings in the ssUI_CmdAnd table.  When a string
// is matched, the handler in the table is called.  See that table for rules about duplications.
// A Handler may examine the parameter and
//      return DEFER, to allow later (as expected) handlers to run.
//      return NextCmd to allow wildcards to be viewed by multiple parameter handlers.
// -------------------------------------------------------------------------------------------------
pHandler_t   ssUI_CmdParamFuncs[CMD_CT][CMD_TOKEN_CT] =
{
//menu up
    { &H_up,   pHNull,           pHNull,                pHNull,              pHNull,        pHNull  } ,
//menu ss
//menu ss time
//menu ss time [start | stop]
//menu ss time bigbang
//menu ss time units <TimeStamp>       // TimeStamp value 1.1.1.1.1.1dy is default (all fields)
//menu ss time stats
    { &H_ss,   &H_ss_Time,       &H_ss_TimeStart,       pHNull,              pHNull,        pHNull } ,
    { &H_ss,   &H_ss_Time,       &H_ss_TimeStop,        pHNull,              pHNull,        pHNull } ,
    { &H_ss,   &H_ss_Time,       &H_ss_TimeBigBang,     pHNull,              pHNull,        pHNull } ,
    { &H_ss,   &H_ss_Time,       &H_ss_TimeStats,       pHNull,              pHNull,        pHNull } ,
//menu ss agency
//menu ss agency pace [<planck> | <ms count t1->t2>]
//menu ss agency [start | stop]
//menu ss agency stats
    { &H_ss,   &H_ss_Agency,     &H_ss_AgencyPace,      &H_ss_AgencyPeriod,  pHNull,        pHNull } ,
    { &H_ss,   &H_ss_Agency,     &H_ss_AgencyStart,     pHNull,              pHNull,        pHNull } ,
    { &H_ss,   &H_ss_Agency,     &H_ss_AgencyStop,      pHNull,              pHNull,        pHNull } ,
    { &H_ss,   &H_ss_Agency,     &H_ss_AgencyStats,     pHNull,              pHNull,        pHNull } ,
//menu ss msgset <8-bit field or '='>  <8-bit field or '='>  <8-bit field or '='>
    { &H_ss,   &H_ss_MsgSSM,     &H_ss_MsgSSM,          &H_ss_MsgSSM,        &H_ss_MsgSSM,  pHNull } ,
//menu ss ro
//menu ss ro led    [app | delay | classic | pace | agency]
//menu ss ro ascii  [app | delay | classic | pace | agency]
//menu ss ro matrix [group0 group1 shape0 shape1]
    { &H_ss,   &H_ss_RO,         &H_ss_ROs_LED,         &H_ss_ROs_Update,    pHNull,        pHNull } ,
    { &H_ss,   &H_ss_RO,         &H_ss_ROs_Ascii,       &H_ss_ROs_Update,    pHNull,        pHNull } ,
    { &H_ss,   &H_ss_RO,         &H_ss_ROs_Matrix,      &H_ss_ROs_Update,    pHNull,        pHNull } ,
// menu--------------------------------------------------------------------------------------------
//menu math
//menu math <time>
//menu math <time> = <time>
//menu math <time> = <time> + <time>
//menu math <time> = <time> - <time>
//menu math <time> += <time>
//menu math <time> -= <time>
    { &H_math, &H_math_Lvalue, &H_math_op_assign, &H_math_oper1, &H_math_op_math, &H_math_oper2    } ,
// menu--------------------------------------------------------------------------------------------
//menu loop on
    { &H_loop,    &H_loop_on,       pHNull,             pHNull,              pHNull,         pHNull } ,
//menu loop off
    { &H_loop,    &H_loop_off,      pHNull,             pHNull,              pHNull,         pHNull } ,
//menu loop while <Time>
//menu loop while <Time><compareOp><Time>
    { &H_loop,    &H_loop_while,    &H_loop_order,    &H_loop_oper1, &H_loop_op_cmp, &H_loop_oper2 } ,
//menu loop until <Time>
//menu loop until <Time><compareOp><Time>
    { &H_loop,    &H_loop_until,    &H_loop_order,    &H_loop_oper1, &H_loop_op_cmp, &H_loop_oper2 } ,
// menu--------------------------------------------------------------------------------------------
//menu evapi
//menu evapi *
//menu evapi * =
//menu evapi <eventApiVar>
    { &H_evapi,   &H_ev_apivar,     &H_ev_clearEvApi,   pHNull,                 pHNull,     pHNull } ,
//menu evapi <eventApiVar> access [on | off | include | exclude | lock | unlock]
    { &H_evapi,   &H_ev_apivar,     &H_ev_access,       &H_ev_access_on,        pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_access,       &H_ev_access_off,       pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_access,       &H_ev_access_include,   pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_access,       &H_ev_access_exclude,   pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_access,       &H_ev_access_lock,      pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_access,       &H_ev_access_unlock,    pHNull,     pHNull } ,
//menu evapi <eventApiVar> apifsm [on | off | run | reset | validate | state | stats]
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_on,        pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_off,       pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_run,       pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_reset,     pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_validate,  pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_state,     pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Apifsm,       &H_ev_Apifsm_stats,     pHNull,     pHNull } ,
//menu evapi <eventApiVar> runfsm [ start | stop | pause | resume | state | stats ]
    { &H_evapi,   &H_ev_apivar,     &H_ev_Runfsm,       &H_ev_Runfsm_start,     pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Runfsm,       &H_ev_Runfsm_stop,      pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Runfsm,       &H_ev_Runfsm_pause,     pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Runfsm,       &H_ev_Runfsm_resume,    pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Runfsm,       &H_ev_Runfsm_state,     pHNull,     pHNull } ,
    { &H_evapi,   &H_ev_apivar,     &H_ev_Runfsm,       &H_ev_Runfsm_stats,     pHNull,     pHNull } ,
//menu evapi <eventApiVar> occurs <time>
//menu evapi <eventApiVar> occurs <time> + <time>
//menu evapi <eventApiVar> occurs <time> - <time>
    { &H_evapi, &H_ev_apivar, &H_ev_occurs, &H_ev_occurs_oper1, &H_ev_op_math, &H_ev_occurs_oper2  } ,
//menu evapi <eventApiVar> period <time>
//menu evapi <eventApiVar> period <time> + <time>
//menu evapi <eventApiVar> period <time> - <time>
    { &H_evapi, &H_ev_apivar, &H_ev_period, &H_ev_period_oper1, &H_ev_op_math,  &H_ev_period_oper2 } ,
//menu evapi <eventApiVar> recurs on
    { &H_evapi, &H_ev_apivar,       &H_ev_recurs,       &H_ev_recurs_on,        pHNull,     pHNull } ,
//menu evapi <eventApiVar> recurs off
    { &H_evapi, &H_ev_apivar,       &H_ev_recurs,       &H_ev_recurs_off,       pHNull,     pHNull } ,
// menu--------------------------------------------------------------------------------------------
//menu help
//menu help *
    { &H_help, &H_help_Show_ByTopic, &H_help_Show_ByCommand,   pHNull,          pHNull,     pHNull } ,
// menu--------------------------------------------------------------------------------------------
//menu cmds run <name>
//menu cmds halt
    { &H_cmds,    &H_cmds_run,      &H_cmds_runName,   pHNull,                 pHNull,      pHNull } ,
    { &H_cmds,    &H_cmds_halt,     pHNull,            pHNull,                 pHNull,      pHNull } ,
// menu------------------------------------------------------------ -----------------------------
#ifdef NEED_A_PLACEHOLDER
    { pHNull,     pHNull,           pHNull,            pHNull,                 pHNull,      pHNull }
#endif  // NEED_A_PLACEHOLDER
};
// -------------------------------------------------------------------------------------------------
#ifdef SSUI_OPTIN_DEBUG_CMDS_CHANDLER
void    CmdHandlerDebug (pAsciiA_t pCheck, int x_Cmd_i, int x_tkn_i, pAsciiA_t pOutcome)
{
    ss_uiOp_emit_lbld_AsciiA (S("compare "), gTokens[x_tkn_i].pAsciiA);
    ss_uiOp_emit_qAsciiA (" to [row ");
    ss_uiOp_emit_Int_99 (x_Cmd_i);
    ss_uiOp_emit_qAsciiA (", column ");
    ss_uiOp_emit_Int_99 (x_tkn_i);
    ss_uiOp_emit_lbld_AsciiA (S("] which is "), pCheck);
    ss_uiOp_emit_lbld_AsciiA (S("   outcome is "), pOutcome);
    ss_uiOp_emit_newline ();
}   // CmdHandlerDebug
#endif  // SSUI_OPTIN_DEBUG_CMDS_CHANDLER
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// bugbugbug describe command handler FSM in note
// -------------------------------------------------------------------------------------------------
#define     ThisCmdTkn  ssUI_CmdParamWords[Cmd_i][ssUI_Cmd_tkn_i]

eCmdFsmResult_t ssUI_menuOp_CmdHandlerFSM (void)
{
    int                 Cmd_i;
    int                 tkn_i = ssUI_Cmd_tkn_i;
    boolean             done;
    eHandlerResult_t    HandlerResult;

    eCmdFsmResult_t     CmdFsmResult = eCmdFsm_rNoMatch;

    // ---------------------------------------------------------------------------------------------
    // does the command received, the value in the first token, match a value in the table?
    //  The question asked really is: is the table Ascii value equal to the input token value?
    // The comparison of one Ascii value to another uses Cmd_i (varying in for loop), and
    // the address computation to get a pointer out of "CmdParamsWords[Cmd_i][0]", and then
    // get the Ascii value at that location pointed to in memory using "pointer[0]".
    // ---------------------------------------------------------------------------------------------
    // By definition, all commands in a CmdZone have the same command array and value in array[0].
    // By definition, N CmdZones that share the first letter will be filtered more cleanly below.
    // ---------------------------------------------------------------------------------------------
    done = false;
    for (Cmd_i=0; ((!done) && (Cmd_i <= CMD_MAX_I)); Cmd_i++)
    {
        // This does not push the words through the FSM row match, this just filters nonmatches.
        if (ssUI_CmdParamWords[Cmd_i][ssUI_Cmd_tkn_i][0] != gTokens[ssUI_Cmd_tkn_i].pAsciiA[0])
        {
#ifdef SSUI_OPTIN_DEBUG_CMDS_CHANDLER
            CmdHandlerDebug (ssUI_CmdParamWords[Cmd_i][ssUI_Cmd_tkn_i], Cmd_i, ssUI_Cmd_tkn_i, S("skip"));
#endif  // SSUI_OPTIN_DEBUG_CMDS_CHANDLER
            continue;      // for each command, skip if the command does not match this row
        }

        // -----------------------------------------------------------------------------------------
        // Need to push each token through the state machine, in the form of handlers per each word.
        // Important to remember the command itself is re-evaluated after having only matched @[0].
        // -----------------------------------------------------------------------------------------
        done = false;
        for (tkn_i = ssUI_Cmd_tkn_i; ((!done) && (tkn_i <= CMD_TOKEN_MAX_I)); tkn_i++)
        {
            // -------------------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
            if (gTokens[tkn_i].pAsciiA == pAsciiANull)
            {
                CmdFsmResult = eCmdFsm_rUnresolved;
                break;          // for loop
            }   // exhausted input tokens without handled or error return

            // -------------------------------------------------------------------------------------
            // if the filter in CmdParamWords is "allow any", the handler will test and act on value.
            // if not allow any, the handler has a specific value that when matched results in call.
            // -------------------------------------------------------------------------------------
            if (
                 (! ssUI_AaIf_pP1_eq_pP2 (ssUI_CmdParamWords[Cmd_i][tkn_i], pcOpAstrsk))
                 &&
                 (! ssUI_AaIf_pP1_eq_pP2 (ssUI_CmdParamWords[Cmd_i][tkn_i], gTokens[tkn_i].pAsciiA))
               )
            {
                break;          // for loop, each token this token did not match wild card or exact
            }   // no handler

            // -------------------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------
            if (ssUI_CmdParamFuncs[Cmd_i][tkn_i] == pHNull)
            {   // no handler for a named command
                CmdFsmResult = eCmdFsm_rTableFlaw;
                break;          // for loop
            }   // no handler

            // -------------------------------------------------------------------------------------
            // matched Asterisk or exact Word for this command or parameter
            // indicate to handler whether there is a next token and/or parameter,
            // handler indicates whether or not it recognized and acted on command/parameter
            // -------------------------------------------------------------------------------------
#ifdef SSUI_OPTIN_DEBUG_CMDS_CHANDLER
            CmdHandlerDebug (ssUI_CmdParamWords[Cmd_i][tkn_i], Cmd_i, tkn_i, S("handle"));
#endif  // SSUI_OPTIN_DEBUG_CMDS_CHANDLER
            HandlerResult =  ssUI_CmdParamFuncs[Cmd_i][tkn_i]
                    (
                        tkn_i,
                        // perhaps a better solution would be a row terminating NULL, in each row.
                        // The cost of the extra memory per FSM can be compared to bounding tkn_i.
                        // the conclusion whether there are more tokens or not is all that matters.
                        ((tkn_i < CMD_TOKEN_MAX_I) &&
                         (gTokens[tkn_i+1].pAsciiA != pAsciiANull))
                    );

            // we called the handler, what was the result?
            switch (HandlerResult)
            {
                case eH_rHandled :
                    done = true;    // terminate for each token and each command loop
                    CmdFsmResult = eCmdFsm_rNormal;
                    break;          // out of switch
                case eH_rError   :
                    done = true;    // terminate for each token and each command loop
                    CmdFsmResult = eCmdFsm_rByHandler;
                    break;          // out of switch
                case eH_rDefer   :
#ifdef SSUI_OPTIN_DEBUG_CMDS_CHANDLER
                    CmdHandlerDebug (ssUI_CmdParamWords[Cmd_i][tkn_i], Cmd_i, tkn_i, S("deferred"));
#endif  // SSUI_OPTIN_DEBUG_CMDS_CHANDLER
                    done = false;   // continue for each token for this command
                    break;          // out of switch
                case eH_rFlaw    :
                // protect other software and be sure CmdFsmResult is a valid value
                default          :
                    done = true;    // terminate for loop
                    CmdFsmResult = eCmdFsm_rTableFlaw;
                    break;          // out of switch
            }   // switch

            if (done)
            {
                break;     // terminate for each token loop, leave tkn_i pointing to current
            }
        }   // for tkn_i

        if (done)
        {
            break;          // terminate for each command loop, leave Cmd_i pointing to current
        }
    }   // for Cmd_i

#ifdef SSUI_OPTIN_DEBUG_CMDS_CHANDLER
    CmdHandlerDebug (ssUI_CmdParamWords[Cmd_i][tkn_i], Cmd_i, tkn_i, S("-completed-"));
#endif  // SSUI_OPTIN_DEBUG_CMDS_CHANDLER
    return (CmdFsmResult);
}   // ssUI_menuOp_CmdHandlerFSM
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_menuIf_CmdTknIsCmd (pAsciiA_t pAscii_Cmd)
{
    int Cmd_i;

    for (Cmd_i=0; Cmd_i <= CMD_MAX_I; Cmd_i++)
    {
        if (ssUI_CmdParamWords[Cmd_i][ssUI_Cmd_tkn_i] != pAsciiANull)
        {
#ifdef SSUI_OPTIN_DEBUG_CMDS_CHANDLER
ss_uiOp_emit_qAsciiA ("cmd search for:");
ss_uiOp_emit_pAsciiA (pAscii_Cmd);
ss_uiOp_emit_qAsciiA (" compared to:");
ss_uiOp_emit_pAsciiA (ssUI_CmdParamWords[Cmd_i][ssUI_Cmd_tkn_i]);
ss_uiOp_emit_newline ();
#endif  // SSUI_OPTIN_DEBUG_CMDS_CHANDLER
            if (
                   ssUI_AaIf_pP1_eq_pP2 (
                                        ssUI_CmdParamWords[Cmd_i][ssUI_Cmd_tkn_i],
                                        pAscii_Cmd
                                     )
                )
            {
                return (true);
            }
        }
    }   // for

    return (false);
}   // ssUI_menuIf_CmdTknIsCmd

// -------------------------------------------------------------------------------------------------
void  ssUI_menuOp_Show_Help_CmdsParams (pAsciiA_t pCmd)
{
    int     Cmd_i, tkn_i;

    ss_uiOp_emit_newline ();

    for (Cmd_i=0; Cmd_i <= CMD_MAX_I; Cmd_i++)
    {
        if (
            (ssUI_AaIf_pP1_eq_pP2 (pcOpAstrsk, pCmd))
             ||
            (ssUI_AaIf_pP1_eq_pP2 (ssUI_CmdParamWords[Cmd_i][ssUI_tkn1st_i], pCmd))
            )
        {
            for (tkn_i=ssUI_Cmd_tkn_i; tkn_i <= CMD_TOKEN_MAX_I; tkn_i++)
            {
                if (ssUI_CmdParamWords[Cmd_i][tkn_i] != pAsciiANull)
                {   // yes name, is there a handler?
                    if (ssUI_CmdParamFuncs[Cmd_i][tkn_i] != pHNull)
                    {   // name and handler, show name
                        ss_uiOp_emit_qAsciiA (ssUI_CmdParamWords[Cmd_i][tkn_i]);
                    }
                    else
                    {   // name without handler, error
                        ss_uiOp_emit_1 (Ascii_Hash);
                    }
                }   // yes name
                else
                {   // no name, yes handler, error
                    if (ssUI_CmdParamFuncs[Cmd_i][tkn_i] != pHNull)
                    {
                        ss_uiOp_emit_1 (Ascii_Hash);
                    }
                }

                if (tkn_i < CMD_TOKEN_MAX_I)        // non-canonical use of _i and _max_i, yes.
                {
                    if (ssUI_CmdParamWords[Cmd_i][tkn_i+1] != pAsciiANull)
                    {
                        ss_uiOp_emit_1 (Ascii_Colon);         // please don't put out a trailing Ascii_Colon
                    }
                }
            }   // for tkn_i

            ss_uiOp_emit_newline ();
        }   // for Cmd_i
    }   // matched '*' or command
}   // ssUI_menuOp_Show_Help_CmdsParams
// -------------------------------------------------------------------------------------------------
void  ssUI_menuOp_Show_Help_CmdsParamHints (pAsciiA_t pCmd)
{
    int     Cmd_i, tkn_i;

    ss_uiOp_emit_newline ();

    for (Cmd_i=0; Cmd_i <= CMD_MAX_I; Cmd_i++)
    {
        if (
            (ssUI_AaIf_pP1_eq_pP2 (pcOpAstrsk, pCmd))
             ||
            (ssUI_AaIf_pP1_eq_pP2 (ssUI_CmdParamWords[Cmd_i][ssUI_tkn1st_i], pCmd))
            )
        {
            for (tkn_i=ssUI_Cmd_tkn_i; tkn_i <= CMD_TOKEN_MAX_I; tkn_i++)
            {
                if (ssUI_CmdParamHints[Cmd_i][tkn_i] != pAsciiANull)
                {   // yes there is any string, show it
                    ss_uiOp_emit_pAsciiA (ssUI_CmdParamHints[Cmd_i][tkn_i]);

                    if (tkn_i < CMD_TOKEN_MAX_I)        // non-canonical use of _i and _max_i, yes.
                    {
                        if (ssUI_CmdParamHints[Cmd_i][tkn_i+1] != pAsciiANull)
                        {
                            ss_uiOp_emit_1 (Ascii_Colon);         // all to prevent a dangling separator.  wow.
                        }
                    }   // separator
                }   // name
            }   // for tkn_i

            ss_uiOp_emit_newline ();
        }   // matched '*' or command
    }   // for Cmd_i
}   // ssUI_menuOp_Show_Help_CmdsParamHints

#endif  // __SSUI_MENU_DEF_H

