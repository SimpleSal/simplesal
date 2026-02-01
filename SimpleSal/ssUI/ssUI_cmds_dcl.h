/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_cmds_dcl.h      data type, prototype declarations: command string management and publication.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_CMDS_DCL_H
#define __SSUI_CMDS_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\SW Developer Tricks.note]    stored input sequences: CMD arrays
// -------------------------------------------------------------------------------------------------
typedef     const char    *pCmd_t;       // pointer to instance in an array of characters
typedef     pCmd_t        *pCmdArray_t;  // pointer to instance in an array of pointers
typedef     pCmdArray_t   *ppCmdArray_t; // pointer to instance in an array of pointers to pointers

// -------------------------------------------------------------------------------------------------
typedef struct CmdArrayLinkDesc_s
{
    ssUI_pToken_t       pCmdArray_name_tkn;
    pCmdArray_t         pCmdArray;
    int                 CmdArray_max_i;
}   CmdArrayLinkDesc_t, *pCmdArrayLinkDesc_t;

typedef CmdArrayLinkDesc_t      CmdArrayLink_t;
typedef pCmdArrayLinkDesc_t     pCmdArrayLink_t;

#define pCmdArrayLinkNull       ((pCmdArrayLink_t) NULL)

// -------------------------------------------------------------------------------------------------
// If this function, using pFindThisCmd, using any method, finds an array of commands:
// the gpCmdArray pointer will be pointing at it and the return value will be true.
// -------------------------------------------------------------------------------------------------
boolean     ssUI_cmdsOp_IfFindName_SetLinkActive (pAsciiA_t pFindThisCmd);

void        ssUI_cmdsOp_Show_AllCommands        (void);
int         ssUI_cmdsOp_Find_LoopOff            (int from_pcCmd_i);
int         ssUI_cmdsOp_Find_CmdFromHere        (pAsciiA_t pFindThisCmd, int from_pcCmd_i);
int         ssUI_cmdsOp_max_i                   (void);
pAscii_t    ssUI_cmdsOp_nextCmdFromLinkActive   (int from_pcCmd_i);

// -------------------------------------------------------------------------------------------------
#ifdef  SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI
// #define SSUI_OPTIN_CMDS_ERRORS
// #define SSUI_OPTIN_CMDS_PASSES
// #define SSUI_OPTIN_CMDS_TIMERLOOP
// #define SSUI_OPTIN_CMDS_TEST
// #define SSUI_OPTIN_CMDS_SSUI
// #define SSUI_OPTIN_CMDS_MENUS
// #define SSUI_OPTIN_CMDS_TIMEOFDAY
// #define SSUI_OPTIN_CMDS_SSUICLOCK
// #define SSUI_OPTIN_CMDS_CMDZONES
// #define SSUI_OPTIN_CMDS_MATH
// #define SSUI_OPTIN_CMDS_MATHGOOD
// #define SSUI_OPTIN_CMDS_CARRY
// #define SSUI_OPTIN_CMDS_BORROW
// #define SSUI_OPTIN_CMDS_EVENTS
#endif // SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI

#endif  // __SSUI_CMDS_DCL_H

