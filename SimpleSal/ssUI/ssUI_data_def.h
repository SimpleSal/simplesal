/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_data_def.h        data and software definitions:  data definitions for the UI
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_DATA_DEF_H
#define __SSUI_DATA_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// FSMs implementing menus and test sequence use these statically allocated and global variables.
// If all of an FSM is contained in a single function, these can be statically allocated and local.
// Build-time initialization of the variables implements the init state of each FSM; now good to go.
// -------------------------------------------------------------------------------------------------
// The highest-level FSMs control turn-taking sharing of a serial input/output device and local echo.
// ssTEA services run without UI services; ssUI is of course dependent on input/output devices.
// -------------------------------------------------------------------------------------------------
// ssFeature: ssUI's "restart everything" requires a run-time initialization: ssUI_InitState.
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// The focus control values are selected to create the Init state for the Ascii/LED ownership FSMs.
// The same is true for the CmdZone and loop control values, they are private to ssUI so not shared.
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]     select the method of initialization
// -------------------------------------------------------------------------------------------------
ssUI_control_t    ssUI_control
#ifdef SSUI_ONEOF_CONTROL_INIT_BUILDTIME
=
{
 // UI owner,                           verbosity,
    ssUI_Default_Ascii_RO_state,    ssUI_Default_Verbosity,

 // cmdZone_stack_i,  CmdsRunning,  Cmds_i,
    0,                false,        0,

 // loopActive,       loop on,      loop off
    false,            0,            0
}
#endif // SSUI_ONEOF_CONTROL_INIT_BUILDTIME
;
// -------------------------------------------------------------------------------------------------
// Other than initial values in data structures (method 1), this is where the DefaultOwner
// definitions FOR THIS BUILD are instantiated at run time (method 2).  The benefit of the definition
// is that multiple methods, as required by the mesa/build/App, need maintenance if not abstracted.
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]     select the method of initialization
// -------------------------------------------------------------------------------------------------
void ssUI_InitState (void)
{
    msg_ssTEA_Path ();

#ifdef SSUI_ONEOF_CONTROL_INIT_RUNTIME
    ssUI_control.BeVerbose = ssUI_Default_Verbosity;
    ssUI_cmdZone_InitStack ();

    ssUI_control.Cmds_i = 0;
    ssUI_control.CmdsAreRunning = false;

    ssUI_control.LoopIsActive = false;
    ssUI_control.LoopOn_i = 0;
    ssUI_control.LoopOff_i = 0;

    // assert LED, Ascii, and Matrix states to be in effect post-Init; not same as run-time init.
    // The default values are established by the App before including ssUI; correlated to App view.
    mesa_LED_RO_state         = ssUI_Default_LED_RO_state;
    mesa_LED_RO_substate      = ssUI_Default_LED_RO_substate;
    mesa_Ascii_RO_state       = ssUI_Default_Ascii_RO_state;
    mesa_Ascii_RO_substate    = ssUI_Default_Ascii_RO_substate;
    mesa_Matrix_RO_state      = ssUI_Default_Matrix_RO_state;
    mesa_Matrix_RO_substate   = ssUI_Default_Matrix_RO_substate;
#endif // SSUI_ONEOF_CONTROL_INIT_RUNTIME

    // before init or reinit references are made to time variables, fill with proper values.
    ssUI_dbOp_Init_TimeVars ();

    // establish the relationships between all the structures, which creates an accessible database.
    ssUI_dbOp_Init_AllEvApis (DEEPINIT);

} // ssUI_InitState

// -------------------------------------------------------------------------------------------------
// An input string to the UI is received, and tokenized by ssUI into individual tokens in an array;
//    each element in the gTokens array within range of gToken_Ct points to a proper ssHL string.
// The pointers of the gTokens array may be changed by ssUI while processing an input string.  This
//    is both a warning and a suggestion: don't assume anything more than proper ssUI strings and if
//    changes to ssUI to replace user-specific keywords with ssUI-specific keywords are desired: go!
// -------------------------------------------------------------------------------------------------
// ssUI token services that parse and modify input values copy the source values into RAM for use.
// A token pointer can point to memory of any attribute (ROM, Flash, RAM, const, static).  Stack?
// A token pointer to stack memory is only a valid pointer while in the scope of variable existence.
// -------------------------------------------------------------------------------------------------
ssUI_Token_t    gTokens[SSUI_TOKENS_ALLOC];
int             gToken_Ct;
int             gToken_i;

#define ssUI_Cmd_tkn_i          ssUI_tkn1st_i
#define ssUI_FirstParam_tkn_i   ssUI_tkn2nd_i

// -------------------------------------------------------------------------------------------------
// if the array of tokens contains non-NULL pointers, they point to NUL-byte terminated Ascii array
// sequences copied into the gCmdTrmCond buffer when the original Loop command was processed.  This
// allows the loop to click through all commands and then recall the original terminating condition.
// -------------------------------------------------------------------------------------------------
AsciiA_t        gCmdTermCondSaved[SSUI_BUFFER_ALLOC];
ssUI_Token_t    gTknsTermCond[SSUI_TOKENS_ALLOC];

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcSS                = S("ss");
ssUI_Token_t        gTknSS              = { pcSS       , 2, NoBananas };
ReadOnly pAsciiA_t  pcssUI              = S("ssUI");
ssUI_Token_t        gTknssUI            = { pcssUI     , 4, NoBananas };
ReadOnly pAsciiA_t  pcCmds              = S("cmds");
ssUI_Token_t        gTknCmds            = { pcCmds     , 4, NoBananas };
ReadOnly pAsciiA_t  pcEvApi             = S("evapi");
ssUI_Token_t        gTknEvApi           = { pcEvApi    , 5, NoBananas };
ReadOnly pAsciiA_t  pcMath              = S("math");
ssUI_Token_t        gTknMath            = { pcMath     , 4, NoBananas };
ReadOnly pAsciiA_t  pcLoop              = S("loop");
ssUI_Token_t        gTknLoop            = { pcLoop     , 4, NoBananas };
ReadOnly pAsciiA_t  pcWhile             = S("while");
ssUI_Token_t        gTknWhile           = { pcWhile    , 5, NoBananas };
ReadOnly pAsciiA_t  pcUntil             = S("until");
ssUI_Token_t        gTknUntil           = { pcUntil    , 5, NoBananas };

// -------------------------------------------------------------------------------------------------
// this is a database that defines the tokens that are valid for reference by the "command zone"
// -------------------------------------------------------------------------------------------------
ssUI_pToken_t   gpcmdZoneTkns[] =
    { &gTknssUI, &gTknCmds, &gTknLoop, &gTknMath, &gTknEvApi, &gTknSS };

// -------------------------------------------------------------------------------------------------
// This is a stack that allows the user (human or test) to change to a cmdZone and then come back.
// The contents of the stack varies over time; commands are pushed onto and popped off of the stack.]
// Management of the contents is done through ssUI_control: global variables within a structure.
// -------------------------------------------------------------------------------------------------
#define ssUI_CMDZONE_ALLOC (5)
#define ssUI_CMDZONE_MAX_I (ssUI_CMDZONE_ALLOC-1)

ssUI_cmdZone_t gcmdZone_stack[ssUI_CMDZONE_ALLOC];

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// within ssUI, cmdZones may be defined; their use allows a shorthand for command entry within a
// ssUI submenu by prepending (inserting in the front) the current "command zone" submenu names.

// careful, cmdZone_stack_i is an array index into array of cmdZones, indicating the current cmdZone
// a stack is a complex data structure best initialized at runtime rather than at build time.
// The cmdZone stack comes into existence with one item in it; that item does not have to be root.
// -------------------------------------------------------------------------------------------------
void    ssUI_cmdZone_InitStack (void)
{
    ssUI_control.cmdZone_stack_i = 0;
    gcmdZone_stack[ssUI_control.cmdZone_stack_i] = ssUI_root;
}   // ssUI_cmdZone_InitStack

// -------------------------------------------------------------------------------------------------
// this use of a stack consists of an index into an array; a "push" changes the focus for "current".
//   by increasing the "current" index.  The value "pushed" is indexed by cmdZone_stack_i after the
// function returns because the value is the new "current" value.
// -------------------------------------------------------------------------------------------------
void  ssUI_cmdZone_StackPush (ssUI_cmdZone_t new_cmdZone)
{
    // less than MAX_I because we need an open slot to fill
    if (ssUI_control.cmdZone_stack_i < ssUI_CMDZONE_MAX_I)
    {
        ssUI_control.cmdZone_stack_i++;
        gcmdZone_stack[ssUI_control.cmdZone_stack_i] = new_cmdZone;
    }
    else
    {
        // take no action if stack full
    }
}   // ssUI_cmdZone_StackPush

// -------------------------------------------------------------------------------------------------
// the minimum stack size is 1 so there is always something to see.
// this use of a stack consists of an index into an array; a "pop" changes the focus for "current",
//   by reducing the "current" index.  The value "popped" is not important in this model, and
// the new current value is the one pointed to by the "current" index, after this function returns.
// -------------------------------------------------------------------------------------------------
void  ssUI_cmdZone_StackPop (void)
{
    if (ssUI_control.cmdZone_stack_i > 0)
    {
        ssUI_control.cmdZone_stack_i--;
    }
}  // ssUI_cmdZone_StackPop
// =================================================================================================
// -------------------------------------------------------------------------------------------------
         ssT_Time_t         ssUI_gTimeA;
ReadOnly ssT_pTime_t        ssUI_gpTimeA         = &ssUI_gTimeA;
         ssT_Time_t         ssUI_gTimeB;
ReadOnly ssT_pTime_t        ssUI_gpTimeB         = &ssUI_gTimeB;
         ssT_Time_t         ssUI_gTimeC;
ReadOnly ssT_pTime_t        ssUI_gpTimeC         = &ssUI_gTimeC;
         ssT_Time_t         ssUI_gTimeD;
ReadOnly ssT_pTime_t        ssUI_gpTimeD         = &ssUI_gTimeD;
         ssT_Time_t         ssUI_gTimeE;
ReadOnly ssT_pTime_t        ssUI_gpTimeE         = &ssUI_gTimeE;
         ssT_Time_t         ssUI_gTimeF;
ReadOnly ssT_pTime_t        ssUI_gpTimeF         = &ssUI_gTimeF;

         ssT_Time_t         ssUI_gWhile;
ReadOnly ssT_pTime_t        ssUI_gpTime_While    = &ssUI_gWhile;
         ssT_Time_t         ssUI_gUntil;
ReadOnly ssT_pTime_t        ssUI_gpTime_Until    = &ssUI_gUntil;

// -------------------------------------------------------------------------------------------------
// The EvApiData variables are describing the Event Apis, allocating Time variables for each EvApi.
// -------------------------------------------------------------------------------------------------
// the naming convention "EvApiN" assumes singular ownership of all Objects for EvApi N while in use.
// Said another way, the use case in ssUI is each event has a parallel independent set of resources.
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcEvApiData0       = S("evapi0");
ReadOnly pAsciiA_t  pcEvApiData1       = S("evapi1");
ReadOnly pAsciiA_t  pcEvApiData2       = S("evapi2");
ReadOnly pAsciiA_t  pcEvApiData3       = S("evapi3");
ReadOnly pAsciiA_t  pcEvApiData4       = S("evapi4");
ReadOnly pAsciiA_t  pcEvApiData5       = S("evapi5");
ReadOnly pAsciiA_t  pcEvApiData6       = S("evapi6");
ReadOnly pAsciiA_t  pcEvApiData7       = S("evapi7");
ReadOnly pAsciiA_t  pcEvApiData8       = S("evapi8");
ReadOnly pAsciiA_t  pcEvApiData9       = S("evapi9");

#define NoB  NoBananas

ReadOnly  ssUI_Token_t    ssUI_db_tknEvApiNames_alloc[] =
{
  { pcEvApiData0, 6, NoB }, { pcEvApiData1, 6, NoB }, { pcEvApiData2, 6, NoB },
  { pcEvApiData3, 6, NoB }, { pcEvApiData4, 6, NoB }, { pcEvApiData5, 6, NoB },
  { pcEvApiData6, 6, NoB }, { pcEvApiData7, 6, NoB }, { pcEvApiData8, 6, NoB },
  { pcEvApiData9, 6, NoB }
};  // ssUI_db_tknEvApiNames_alloc

ReadOnly pAsciiA_t  pcEvTime0          = S("evtime0");
ReadOnly pAsciiA_t  pcEvTime1          = S("evtime1");
ReadOnly pAsciiA_t  pcEvTime2          = S("evtime2");
ReadOnly pAsciiA_t  pcEvTime3          = S("evtime3");
ReadOnly pAsciiA_t  pcEvTime4          = S("evtime4");
ReadOnly pAsciiA_t  pcEvTime5          = S("evtime5");
ReadOnly pAsciiA_t  pcEvTime6          = S("evtime6");
ReadOnly pAsciiA_t  pcEvTime7          = S("evtime7");
ReadOnly pAsciiA_t  pcEvTime8          = S("evtime8");
ReadOnly pAsciiA_t  pcEvTime9          = S("evtime9");

ReadOnly pAsciiA_t  pcNoLabelThanks    = S(" ");

// -------------------------------------------------------------------------------------------------
// Valid AsciiA pointers are used to make sure that invalid data does not cause a hard fault.
// -------------------------------------------------------------------------------------------------
// The ssHL software language problem of correlating _CT values for allocations and algorithms with
// actual data item allocations and references to actual functions and data structures during init.
// An arbitrary choice here to have 10 names in an array makes the maximum EVAPI_ALLOC_CT be 10.
// The alternative is much cleaner but is not known yet by a learner: don't refer to invalid data.
// -------------------------------------------------------------------------------------------------
ReadOnly  ssUI_Token_t    ssUI_db_tknTimeVarNames_alloc[] =
{
    { pcEvTime0, 7, NoB },  { pcEvTime1, 7, NoB },  { pcEvTime2, 7, NoB },
    { pcEvTime3, 7, NoB },  { pcEvTime4, 7, NoB },  { pcEvTime5, 7, NoB },
    { pcEvTime6, 7, NoB },  { pcEvTime7, 7, NoB },  { pcEvTime8, 7, NoB },
    { pcEvTime9, 7, NoB }
};  // ssUI_db_tknTimeVarNames_alloc

ssT_Time_t      ssUI_db_timeVars_alloc[SSUI_EVAPI_ALLOC_CT];    // allocate an array of structures

// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcTimeA             = S("timea");
ssUI_Token_t        gTknTimeA           = { pcTimeA        , 5, NoBananas };
ReadOnly pAsciiA_t  pcTimeB             = S("timeb");
ssUI_Token_t        gTknTimeB           = { pcTimeB        , 5, NoBananas };
ReadOnly pAsciiA_t  pcTimeC             = S("timec");
ssUI_Token_t        gTknTimeC           = { pcTimeC        , 5, NoBananas };
ReadOnly pAsciiA_t  pcTimeD             = S("timed");
ssUI_Token_t        gTknTimeD           = { pcTimeD        , 5, NoBananas };
ReadOnly pAsciiA_t  pcTimeE             = S("timee");
ssUI_Token_t        gTknTimeE           = { pcTimeE        , 5, NoBananas };
ReadOnly pAsciiA_t  pcTimeF             = S("timef");
ssUI_Token_t        gTknTimeF           = { pcTimeF        , 5, NoBananas };
ReadOnly pAsciiA_t  pcTimeCurr          = S("timecurr");
ssUI_Token_t        gTknTimeCurr        = { pcTimeCurr     , 8, NoBananas };
ReadOnly pAsciiA_t  pcTimeBuilt         = S("timebuilt");
ssUI_Token_t        gTknTimeBuilt       = { pcTimeBuilt    , 9, NoBananas };
ReadOnly pAsciiA_t  pcTimeBigBang       = S("timebigbang");
ssUI_Token_t        gTknTimeBigBang     = { pcTimeBigBang  , 11, NoBananas };
ReadOnly pAsciiA_t  pcTimeEphemeral     = S("timeephemeral");
ssUI_Token_t        gTknTimeEphemeral   = { pcTimeEphemeral, 13, NoBananas };

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// In order to provide a ssUI feature (named Time variables and ssUI operations that reference them)
// this type provide a mapping between a name usable with ssUI and any ssHL-language variable.
// -------------------------------------------------------------------------------------------------
typedef struct ssUI_TimeVarDesc_s
{
    ssUI_Token_t       VarToken;
    ssT_pTime_t        pCodesName;
}   ssUI_TimeVarDesc_t,  *ssUI_pTimeVarDesc_t;

typedef ssUI_TimeVarDesc_t     ssUI_TimeVar_t;
typedef ssUI_pTimeVarDesc_t    ssUI_pTimeVar_t;

ssUI_TimeVar_t  ssUI_TimeVars[] =
{   // this token                       refers to this Time Variable
    {  gTknTimeCurr                     , ssT_gpTime_Current      },
    {  gTknTimeBuilt                    , ssT_gpTime_Built        },
    {  gTknTimeBigBang                  , ssT_gpTime_BigBang      },
    {  gTknTimeEphemeral                , ssT_gpTime_Ephemeral    },
    {  gTknWhile                        , ssUI_gpTime_While       },
    {  gTknUntil                        , ssUI_gpTime_Until       },
    {  gTknTimeA                        , ssUI_gpTimeA            },
    {  gTknTimeB                        , ssUI_gpTimeB            },
    {  gTknTimeC                        , ssUI_gpTimeC            },
    {  gTknTimeD                        , ssUI_gpTimeD            },
    {  gTknTimeE                        , ssUI_gpTimeE            },
    {  gTknTimeF                        , ssUI_gpTimeF            },
    {  ssUI_db_tknTimeVarNames_alloc[0] , &ssUI_db_timeVars_alloc[0]  },
    {  ssUI_db_tknTimeVarNames_alloc[1] , &ssUI_db_timeVars_alloc[1]  },
    {  ssUI_db_tknTimeVarNames_alloc[2] , &ssUI_db_timeVars_alloc[2]  },
    {  ssUI_db_tknTimeVarNames_alloc[3] , &ssUI_db_timeVars_alloc[3]  },
    {  ssUI_db_tknTimeVarNames_alloc[4] , &ssUI_db_timeVars_alloc[4]  },
    {  ssUI_db_tknTimeVarNames_alloc[5] , &ssUI_db_timeVars_alloc[5]  },
    {  ssUI_db_tknTimeVarNames_alloc[6] , &ssUI_db_timeVars_alloc[6]  },
    {  ssUI_db_tknTimeVarNames_alloc[7] , &ssUI_db_timeVars_alloc[7]  },
    {  ssUI_db_tknTimeVarNames_alloc[8] , &ssUI_db_timeVars_alloc[8]  },
    {  ssUI_db_tknTimeVarNames_alloc[9] , &ssUI_db_timeVars_alloc[9]  }
};
#define TimeVar_TUPLE_CT    (ComputeArray_CT (ssUI_TimeVars, sizeof (ssUI_TimeVar_t)))
#define TimeVar_TUPLE_MAX_I (TimeVar_TUPLE_CT-1)

ReadOnly pAsciiA_t  pcHelpTopics        = S("topics");
ssUI_Token_t        gTknHelpTopics      = { pcHelpTopics    , 6, NoBananas };
ReadOnly pAsciiA_t  pcHelpTimeVars      = S("timevars");
ssUI_Token_t        gTknHelpTimeVars    = { pcHelpTimeVars  , 8, NoBananas };
ReadOnly pAsciiA_t  pcHelpCommands      = S("commands");
ssUI_Token_t        gTknHelpCommands    = { pcHelpCommands  , 8, NoBananas };
ReadOnly pAsciiA_t  pcHelpParams        = S("params");
ssUI_Token_t        gTknHelpParams      = { pcHelpParams    , 6, NoBananas };

// These are the tokens that can be used to get help, displayable and searchable here.
// rather than maintain a single string with N topics, maintain an array and show each one later.
ReadOnly pAsciiA_t  pcHelpTopicNames[]   =
    { pcHelpTopics, pcHelpTimeVars, pcHelpCommands, pcHelpParams };

#define TOPIC_CT    (ComputeArray_CT (pcHelpTopicNames, sizeof (pAsciiA_t)))
#define TOPIC_MAX_I (TOPIC_CT-1)

ReadOnly pAsciiA_t  pcHelp              = S("help");
ssUI_Token_t        gTknHelp            = { pcHelp          , 4, NoBananas };
ReadOnly pAsciiA_t  pcHelpCmds          = S("cmds");
ssUI_Token_t        gTknHelpCmds        = { pcHelpCmds      , 4, NoBananas };

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcHelpEvApiN        = S("evapiN");
ReadOnly pAsciiA_t  pcHelpLvalue        = S("Lvalue");
ReadOnly pAsciiA_t  pcHelpCmpOp         = S("(<= | >= | < | > | == | !=)");
ReadOnly pAsciiA_t  pcHelpMathOp        = S("(+ | -)");

ReadOnly pAsciiA_t  pcHelpOper1         = S("operand1");
ReadOnly pAsciiA_t  pcHelpOper2         = S("operand2");

ReadOnly pAsciiA_t  pcHelpCmdsRun       = S("see choices with 'cmds run'");
ReadOnly pAsciiA_t  pcFlaw              = S("flawerror");

ReadOnly pAsciiA_t  pcHelpPacePeriod    = S("Time period from Agency Root(T1) to Agency Root(T2)");

ReadOnly pAsciiA_t  pcCompIsMath        = S("Math");
ReadOnly pAsciiA_t  pcCompIsAfter       = S("After");
ReadOnly pAsciiA_t  pcCompIsEQAfter     = S("EQAfter");
ReadOnly pAsciiA_t  pcCompIsEQ          = S("EQ");
ReadOnly pAsciiA_t  pcCompIsNEQ         = S("NEQ");
ReadOnly pAsciiA_t  pcCompIsBefore      = S("Before");
ReadOnly pAsciiA_t  pcCompIsEQBefore    = S("EQBefore");

// -------------------------------------------------------------------------------------------------
// The token parser returns an array of pointers to null-terminated ssHL strings, each with a length.
// The Token parser always works on the gTokens array, which can be indexed in source with these.
// -------------------------------------------------------------------------------------------------
#define ssUI_tkn1st_i           (0)
#define ssUI_tkn2nd_i           (1)
#define ssUI_tkn3rd_i           (2)
#define ssUI_tkn4th_i           (3)
#define ssUI_tkn5th_i           (4)
#define ssUI_tkn6th_i           (5)

#ifdef BLD_DEBUG_CMDHANDLER
ReadOnly pAsciiA_t pTkn_i_name[] = { S("1st"), S("2nd"), S("3rd"), S("4th"), S("5th"), S("6th") };
#endif  // BLD_DEBUG_CMDHANDLER

// -------------------------------------------------------------------------------------------------
// each pcMsg strings is the contents of a message carried in a signal to the user; Ascii characters.
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcOpAssign              = S("=");
ssUI_Token_t        gTknOpAssign            = { pcOpAssign       , 1, NoBananas };
ReadOnly pAsciiA_t  pcOpEqualTo             = S("=");
ssUI_Token_t        gTknOpEqualTo           = { pcOpEqualTo      , 1, NoBananas };
ReadOnly pAsciiA_t  pcOpAstrsk              = S("*");
ssUI_Token_t        gTknOpAstrsk            = { pcOpAstrsk       , 1, NoBananas };
ReadOnly pAsciiA_t  pcOpPlus                = S("+");
ssUI_Token_t        gTknOpPlus              = { pcOpPlus         , 1, NoBananas };
ReadOnly pAsciiA_t  pcOpMinus               = S("-");
ssUI_Token_t        gTknOpMinus             = { pcOpMinus        , 1, NoBananas };
ReadOnly pAsciiA_t  pcOpMinusEqual          = S("-=");
ssUI_Token_t        gTknOpMinusEqual        = { pcOpMinusEqual   , 2, NoBananas };
ReadOnly pAsciiA_t  pcOpPlusEqual           = S("+=");
ssUI_Token_t        gTknOpPlusEqual         = { pcOpPlusEqual    , 2, NoBananas };
ReadOnly pAsciiA_t  pcOpLT                  = S("<");
ssUI_Token_t        gTknOpLT                = { pcOpLT           , 1, NoBananas };
ReadOnly pAsciiA_t  pcOpGT                  = S(">");
ssUI_Token_t        gTknOpGT                = { pcOpGT           , 1, NoBananas };
ReadOnly pAsciiA_t  pcAny                   = S("any");
ssUI_Token_t        gTknAny                 = { pcAny            , 3, NoBananas };
ReadOnly pAsciiA_t  pcAll                   = S("all");
ssUI_Token_t        gTknAll                 = { pcAll            , 3, NoBananas };
ReadOnly pAsciiA_t  pcNone                  = S("none");
ssUI_Token_t        gTknNone                = { pcNone           , 4, NoBananas };
ReadOnly pAsciiA_t  pcNormal                = S("normal");
ssUI_Token_t        gTknNormal              = { pcNormal         , 6, NoBananas };
ReadOnly pAsciiA_t  pcClear                 = S("clear");
ssUI_Token_t        gTknClear               = { pcClear          , 5, NoBananas };

ReadOnly pAsciiA_t  pcUp                    = S("up");
ssUI_Token_t        gTknUp                  = { pcUp             , 2, NoBananas };
ReadOnly pAsciiA_t  pcOn                    = S("on");
ssUI_Token_t        gTknOn                  = { pcOn             , 2, NoBananas };
ReadOnly pAsciiA_t  pcOff                   = S("off");
ssUI_Token_t        gTknOff                 = { pcOff            , 3, NoBananas };
ReadOnly pAsciiA_t  pcInclude               = S("include");
ssUI_Token_t        gTknInclude             = { pcInclude        , 6, NoBananas };
ReadOnly pAsciiA_t  pcExclude               = S("exclude");
ssUI_Token_t        gTknExclude             = { pcExclude        , 6, NoBananas };
ReadOnly pAsciiA_t  pcLock                  = S("lock");
ssUI_Token_t        gTknLock                = { pcLock           , 4, NoBananas };
ReadOnly pAsciiA_t  pcUnlock                = S("unlock");
ssUI_Token_t        gTknUnlock              = { pcUnlock         , 6, NoBananas };
ReadOnly pAsciiA_t  pcAccess                = S("access");
ssUI_Token_t        gTknAccess              = { pcAccess         , 6, NoBananas };
ReadOnly pAsciiA_t  pcRun                   = S("run");
ssUI_Token_t        gTknRun                 = { pcRun            , 3, NoBananas };
ReadOnly pAsciiA_t  pcHalt                  = S("halt");
ssUI_Token_t        gTknHalt                = { pcHalt           , 4, NoBananas };
ReadOnly pAsciiA_t  pcReset                 = S("reset");
ssUI_Token_t        gTknReset               = { pcReset          , 5, NoBananas };
ReadOnly pAsciiA_t  pcValidate              = S("validate");
ssUI_Token_t        gTknValidate            = { pcValidate       , 8, NoBananas };
ReadOnly pAsciiA_t  pcStart                 = S("start");
ssUI_Token_t        gTknStart               = { pcStart          , 5, NoBananas };
ReadOnly pAsciiA_t  pcStop                  = S("stop");
ssUI_Token_t        gTknStop                = { pcStop           , 4, NoBananas };
ReadOnly pAsciiA_t  pcResume                = S("resume");
ssUI_Token_t        gTknResume              = { pcResume         , 6, NoBananas };
ReadOnly pAsciiA_t  pcPause                 = S("pause");
ssUI_Token_t        gTknPause               = { pcPause          , 5, NoBananas };
ReadOnly pAsciiA_t  pcOccurs                = S("occurs");
ssUI_Token_t        gTknOccurs              = { pcOccurs         , 6, NoBananas };
ReadOnly pAsciiA_t  pcRecurs                = S("recurs");
ssUI_Token_t        gTknRecurs              = { pcRecurs        , 6, NoBananas };
ReadOnly pAsciiA_t  pcState                 = S("state");
ssUI_Token_t        gTknState               = { pcState         , 5, NoBananas };
ReadOnly pAsciiA_t  pcPeriod                = S("period");
ssUI_Token_t        gTknPeriod              = { pcPeriod        , 6, NoBananas };
ReadOnly pAsciiA_t  pcHeartBeat             = S("heartbeat");
ssUI_Token_t        gTknHrtbt               = { pcHeartBeat     , 9, NoBananas };
ReadOnly pAsciiA_t  pcDec                   = S("dec");
ssUI_Token_t        gTknDec                 = { pcDec           , 3, NoBananas };
ReadOnly pAsciiA_t  pcHex                   = S("hex");
ssUI_Token_t        gTknHex                 = { pcHex           , 3, NoBananas };
ReadOnly pAsciiA_t  pcPaused                = S("paused");
ssUI_Token_t        gTknPaused              = { pcPaused        , 6, NoBananas };
ReadOnly pAsciiA_t  pcStopped               = S("stopped");
ssUI_Token_t        gTknStopped             = { pcStopped       , 7, NoBananas };
ReadOnly pAsciiA_t  pcAgencying             = S("agencying");
ssUI_Token_t        gTknAgencying           = { pcAgencying     , 9, NoBananas };

ReadOnly pAsciiA_t  pcRO                    = S("ro");
ReadOnly pAsciiA_t  pcApp                   = S("app");
ReadOnly pAsciiA_t  pcfsmDemo               = S("fsmdemo");
ReadOnly pAsciiA_t  pcLED                   = S("led");
ReadOnly pAsciiA_t  pcAscii                 = S("ascii");
ReadOnly pAsciiA_t  pcMatrix                = S("matrix");
ReadOnly pAsciiA_t  pcClassic               = S("classic");
ReadOnly pAsciiA_t  pcDelay                 = S("delay");
ReadOnly pAsciiA_t  pcPace                  = S("pace");
ReadOnly pAsciiA_t  pcAgency                = S("agency");
ReadOnly pAsciiA_t  pcGroup0                = S("group0");
ReadOnly pAsciiA_t  pcGroup1                = S("group1");
ReadOnly pAsciiA_t  pcShape0                = S("shape0");
ReadOnly pAsciiA_t  pcShape1                = S("shape1");

ReadOnly pAsciiA_t  pcTime                  = S("time");
ssUI_Token_t        gTknTime                = { pcTime          , 4, NoBananas };
ReadOnly pAsciiA_t  pcEvent                 = S("event");
ssUI_Token_t        gTknEvent               = { pcEvent         , 5, NoBananas };
ReadOnly pAsciiA_t  pcMessage               = S("message");
ssUI_Token_t        gTknSignaler            = { pcMessage       , 7, NoBananas };
ReadOnly pAsciiA_t  pcEvHandle              = S("handle");
ssUI_Token_t        gTknEvHandle            = { pcEvHandle      , 6, NoBananas };
ReadOnly pAsciiA_t  pcEvAction              = S("action");
ssUI_Token_t        gTknEvAction            = { pcEvAction      , 6, NoBananas };
ReadOnly pAsciiA_t  pcUnits                 = S("units");
ssUI_Token_t        gTknUnits               = { pcUnits         , 5, NoBananas };
ReadOnly pAsciiA_t  pcStats                 = S("stats");
ssUI_Token_t        gTknStats               = { pcStats         , 5, NoBananas };
ReadOnly pAsciiA_t  pcMsgSet                = S("msgset");
ssUI_Token_t        gTknMsgSet              = { pcMsgSet        , 6, NoBananas };
ReadOnly pAsciiA_t  pcBigBang               = S("bigbang");
ssUI_Token_t        gTknBigBang             = { pcBigBang       , 7, NoBananas };

ReadOnly pAsciiA_t  pcApifsm                = S("apifsm");
ssUI_Token_t        gTknApifsm              = { pcApifsm        , 6, NoBananas };
ReadOnly pAsciiA_t  pcRunfsm                = S("runfsm");
ssUI_Token_t        gTknRunfsm              = { pcRunfsm        , 6, NoBananas };

ReadOnly pAsciiA_t  pcMillis                = S("millis");
ssUI_Token_t        gTknMillis              = { pcMillis        , 6, NoBananas };

ReadOnly pAsciiA_t  pcHelpTime              = S("(TimeStamp | timevar)");
ssUI_Token_t        gTknHelpTime            = { pcHelpTime      ,22, NoBananas };
ReadOnly pAsciiA_t  pcHelpSetSignal         = S("signal(s)");
ssUI_Token_t        gTknHelpMsgSetSignal    = { pcHelpSetSignal , 9, NoBananas };
ReadOnly pAsciiA_t  pcHelpSetSensor         = S("sensor(s)");
ssUI_Token_t        gTknHelpMsgSetSensor    = { pcHelpSetSensor , 9, NoBananas };
ReadOnly pAsciiA_t  pcHelpSetMsg            = S("message(s)");
ssUI_Token_t        gTknHelpMsgSetMsg       = { pcHelpSetMsg    ,10, NoBananas };

ReadOnly pAsciiA_t  pcMsg_VarnameHelp               = S("time variable names (may be used as Lvalue, Operand1 or Operand2)");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_ErrorApiNotOn    = S("Agency Api FSM not On for this event.");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_ErrorApiNotOff   = S("Agency Api FSM already On for this event.");
ReadOnly pAsciiA_t  pcMsg_AgApiSignalError          = S("!@#$%^&* Agency Api failure");
ReadOnly pAsciiA_t  pcMsg_AgApiSignalSuccess        = S("........ Agency Api success");
ReadOnly pAsciiA_t  pcMsg_ArrowsReqDir              = S(">>to>>");
ReadOnly pAsciiA_t  pcMsg_ArrowsRespDir             = S("<<to<<");
ReadOnly pAsciiA_t  pcMsg_AgApiReqError             = S("<error in request data>");
ReadOnly pAsciiA_t  pcMsg_apiSignalOp_Worked        = S(", Api signal success.");
ReadOnly pAsciiA_t  pcMsg_apiSignalOp_Failed        = S(", Api signal failure.");

ReadOnly pAsciiA_t  pcMsg_plbl_TimeVariable         = S(" Time variable   :");

ReadOnly pAsciiA_t  pcMsg_AgApifsm_OnReq            = S("Agency Api FSM On request...");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_OffReq           = S("Agency Api FSM Off request...");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_ResetReq         = S("Agency Api FSM Reset request...");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_RunReq           = S("Agency Api FSM Run request...");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_ValidateReq      = S("Agency Api FSM Validate Event Data request...");
ReadOnly pAsciiA_t  pcMsg_AgApifsm_StateReq         = S("Agency Api FSM State request...");
ReadOnly pAsciiA_t  pcMsg_AgRunfsm_StartReq         = S("Agency Run FSM Start request...");
ReadOnly pAsciiA_t  pcMsg_AgRunfsm_StopReq          = S("Agency Run FSM Stop request...");
ReadOnly pAsciiA_t  pcMsg_AgRunfsm_PauseReq         = S("Agency Run FSM Pause request...");
ReadOnly pAsciiA_t  pcMsg_AgRunfsm_ResumeReq        = S("Agency Run FSM Resume request...");
ReadOnly pAsciiA_t  pcMsg_AgRunfsm_StateReq         = S("Agency Run FSM State request...");
ReadOnly pAsciiA_t  pcMsg_CurrentViewData           = S("   current App view of all ssTEA Data:");
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
ReadOnly pAsciiA_t  pcMsg_UpdatedViewData           = S("   updated App view of all ssTEA Data:");
#endif // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

ReadOnly pAsciiA_t  pcEvApiNameLbl_EvApi            = S("evapi");
ReadOnly pAsciiA_t  pcEvApiNamelbl_Name             = S("name");
ReadOnly pAsciiA_t  pcEvApiNameLbl_ssDB             = S("ssDB");
ReadOnly pAsciiA_t  pcEvApiNameLbl_App              = S("App");

ReadOnly pAsciiA_t  pcMsg_CmdFsm_NormalResult       = S("normal command FSM result");
ReadOnly pAsciiA_t  pcMsg_CmdFsm_FoundNoInput       = S("no input received by command FSM");
ReadOnly pAsciiA_t  pcMsg_CmdFsm_FoundNoTokens      = S("no tokens found in input received");
ReadOnly pAsciiA_t  pcMsg_CmdFsm_FoundNoMatch       = S("no match found in tokens parsed");
ReadOnly pAsciiA_t  pcMsg_CmdFsm_ByHandler          = S("a problem is reported by command handler");
ReadOnly pAsciiA_t  pcMsg_CmdFsm_Unresolved         = S("parameter not expected by command handler");
ReadOnly pAsciiA_t  pcMsg_CmdFsm_TableFlaw          = S("FSM reported problem with table(s)");

ReadOnly pAsciiA_t  pcMsg_MillisRangeError_T1toT2   = S("  ssTEA # of milliseconds from T1 to T2 must be an integer > 0");

ReadOnly pAsciiA_t  pcMsg_NotAllowed                = S("sorry, the requested change to Time or Agency is not allowed:");
ReadOnly pAsciiA_t  pcMsg_NoAgencyWithoutTime       = S("  ssTEA can't make use of Agency while Time is stopped");

ReadOnly pAsciiA_t  pcMsg_NoTimeStopAgencyIsRunning = S("  ssTEA can't change Time's state while Agency is not stopped");
ReadOnly pAsciiA_t  pcMsg_NoStartWhileRunning       = S("  ssTEA can't start Time or Agency while Time or Agency is not stopped");
ReadOnly pAsciiA_t  pcMsg_NoStopWhileNotRunning     = S("  ssTEA can't stop Time or Agency while Time or Agency is stopped");

ReadOnly pAsciiA_t  pcMsg_FSMStats_ReqFailed = S("FSM stats are in the build but request failed");

#endif  // __SSUI_DATA_DEF_H

