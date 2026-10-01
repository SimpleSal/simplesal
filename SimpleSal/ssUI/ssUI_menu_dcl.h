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
typedef enum CmdFSM_Result_e
{
    eCmdFSM_rNormal,
    eCmdFSM_rNoInput,
    eCmdFSM_rNoTokens,
    eCmdFSM_rNoMatch,
    eCmdFSM_rByHandler,
    eCmdFSM_rUnresolved,
    eCmdFSM_rTableFlaw
}   eCmdFSM_Result_t;

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

// -------------------------------------------------------------------------------------------------
// A complex C expression but only due to levels of the same idea rather than adding sophistication:
//
//    A "function with parameters that returns a result"
//         is of the type "function returning result"
//
//    A variable that is "a pointer to a function with parameters that returns a result"
//        is of the type "pHandler_t" or "pointer to function returning result".
// -------------------------------------------------------------------------------------------------
// It's just a pointer, with strange-looking parenthesis placement to make it a function pointer.
// The only reference causing the compiler to generate the address is in a table of menu handlers.
// When the FSM finds the input array of Ascii in an Array of Ascii command/parameter arrays,
//  the FSM invokes the handler from a parallel array of pointers to functions.
// -------------------------------------------------------------------------------------------------
typedef eHandlerResult_t (* pHandler_t) (int  tkn_i, boolean moreInputTkns);
#define pHNull  ((pHandler_t) NULL)

// -------------------------------------------------------------------------------------------------
// The command Handler table entry with no matching string should contain pHNull, not &unAssigned.
// -------------------------------------------------------------------------------------------------
eHandlerResult_t unAssignedHandler (int tkn_i, boolean moreInputTkns);

// -------------------------------------------------------------------------------------------------
// Command FSM implements ssUI's characters-into-words-into-tokens-into-commands-with-params model.
// -------------------------------------------------------------------------------------------------
eCmdFSM_Result_t     ssUI_menuOp_CmdFSM   (void);

boolean             ssUI_menuIf_CmdTknIsCmd      (pAsciiA_t pAscii_Cmd);

void                ssUI_menuOp_Show_Help_CmdsParams    (pAsciiA_t pAscii_Cmd);
void                ssUI_menuOp_Show_Help_CmdsHints     (pAsciiA_t pAscii_Cmd);

#endif  // __SSUI_MENU_DCL_H

