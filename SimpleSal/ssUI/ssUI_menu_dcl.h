/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_menu_dcl.h      data type, prototype declarations: command and parameter handlers for UI
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_MENU_DCL_H
#define __SSUI_MENU_DCL_H

// -------------------------------------------------------------------------------------------------
// The command handler FSM searches the Command:parameter(s) Ascii array:  On finding a match in a
// position in the table, the FSM calls the Handler function in the function pointer table.
// The FSM
// the variations of results based on why the contents of an input string in an Ascii array can
// or cannot be converted into a command that was then handled or not handled are defined here.
// for example when the input Ascii Array has no values at all or all Ascii values are spaces.
// -------------------------------------------------------------------------------------------------
typedef enum CmdFsmResult_e
{
    eCmdFsm_rNormal,
    eCmdFsm_rNoInput,
    eCmdFsm_rNoTokens,
    eCmdFsm_rNoMatch,
    eCmdFsm_rByHandler,
    eCmdFsm_rUnresolved,
    eCmdFsm_rTableFlaw
}   eCmdFsmResult_t;

// -------------------------------------------------------------------------------------------------
// Each command or parameter in the string table has a corresponding function pointer to a Handler.
// The Handler returns an indication that affects the state of the command or parameter parsing FSM.
// -------------------------------------------------------------------------------------------------
typedef enum HandlerResult_e
{
    eH_rDefer,
    eH_rError,
    eH_rNextCmd,
    eH_rHandled,
    eH_rFlaw
}   eHandlerResult_t;

typedef eHandlerResult_t (* pHandler_t) (int  tkn_i, boolean moreInputTkns);
#define pHNull  ((pHandler_t) NULL)

// -------------------------------------------------------------------------------------------------
// This is only to create a demonstration of a basic handler that can be used as a reference.
// The command Handler table entry with no matching string should contain pHNull, not &unAssigned.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t unAssignedHandler (int tkn_i, boolean moreInputTkns);

// -------------------------------------------------------------------------------------------------
eCmdFsmResult_t     ssUI_menuOp_CmdHandlerFSM   (void);

boolean             ssUI_menuIf_CmdTknIsCmd      (pAsciiA_t pAscii_Cmd);

void                ssUI_menuOp_Show_Help_CmdsParams        (pAsciiA_t pAscii_Cmd);
void                ssUI_menuOp_Show_Help_CmdsParamHints    (pAsciiA_t pAscii_Cmd);

#endif  // __SSUI_MENU_DCL_H

