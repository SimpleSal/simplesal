/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_input_def.h     definition of services to manipulate prompts, menus, serial/ssHL input streams.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_INPUT_DEF_H
#define __SSUI_INPUT_DEF_H

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define menuPromptEncapOn     (Ascii_Lparen)
#define menuPromptEncapOff    (Ascii_Rparen)

void ssUI_Prompt (void)
{
    int cmdZone_i;

    ss_uiOp_emit_newline ();
    for (cmdZone_i=0; cmdZone_i <= ssUI_control.cmdZone_stack_i; cmdZone_i++)
    {
        ss_uiOp_emit_1 (menuPromptEncapOn);
        ss_uiOp_emit_pAsciiA (gpcmdZoneTkns[gcmdZone_stack[cmdZone_i]]->pAsciiA);
        ss_uiOp_emit_1 (menuPromptEncapOff);
    }
}   // ssUI_Prompt

// -------------------------------------------------------------------------------------------------
void ssUI_inOp_Hold (void)
{
    int     x, y;
    Ascii_t aByte;

    while (ss_uiOp_recv_avail_ct () == 0)  { x += y; y++; }
    while (ss_uiOp_recv_avail_ct () > 0)   { aByte += ss_uiOp_recv_1 (); }
    y = x + aByte;
}   // ssUI_inOp_Hold

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Commands are grouped into "cmdZones", or ssUI's menu system is a tree of groups of related
// functions: "cmds on" and "cmds run" relate to the command.  This is just ssUI's menu implemented.
// -------------------------------------------------------------------------------------------------
// The menu FSM is given agency when ssUI's collector returns an input array of characters that have
// ALREADY ARRIVED and been collected, either from a serial port or an array of pointers to commands.
// What does "already arrived" imply?  The input service NEVER BLOCKS waiting for input.  The fact
// there is input available is not revealed by the input service until a carriage return is collected.
// -------------------------------------------------------------------------------------------------
// The menu FSM is given Agency and an array of Ascii characters terminated by CR and NUL, by someone.
// Someone means software that has been granted Agency, and by extension, the right to grant Agency.
// There are models that can be chosen:
//     The Arduino loop function calls ssUI_Main, granting Agency and control at the level of ssTEA.
//     A recurring Time Event calls ssUI_Main, granting Agency and control in the context of ssTEA.
// -------------------------------------------------------------------------------------------------
void ssUI_Main (void)
{
    boolean emit_prompt = THERES_NO_ACTION;

    // ssTEA_App is comprised of the Apps and the ssUI interface used to manipulate the Apps and ssTEA
    if (mesa_Ascii_RO_state == mesa_RO_state_SimpleSal)
    {
        // The only benefit from changing to a "Main" for cmds/loops/math etc is this menu trick:
        // the function prepends the command name "cmds" or "loop" or "math" to any input.
        switch (gcmdZone_stack[ssUI_control.cmdZone_stack_i])
        {
            case ssUI_root  :  emit_prompt = ssUI_rootMain (false);     break;
            case ssUI_cmds  :  emit_prompt = ssUI_CmdsMain ();          break;
            case ssUI_loop  :  emit_prompt = ssUI_LoopMain ();          break;
            case ssUI_math  :  emit_prompt = ssUI_MathMain ();          break;
            case ssUI_evapi :  emit_prompt = ssUI_EvapiMain ();         break;
            case ssUI_ss    :  emit_prompt = ssUI_ssMain ();            break;
            default         :  emit_prompt = IM_THE_TOWN_CRIER;         break;
        }   // switch
    }   // grant menu FSM agency

    //
    // It is possible that one of the menus above changed the ownership from ssUI to !ssUI.
    // Returning to ssUI state from !ssUI is when the User of the Interface needs a prompt.
    //
    // ssTEA_App is comprised of the Apps and the ssUI used to manipulate the Apps and ssTEA
    if (mesa_Ascii_RO_state == mesa_RO_state_SimpleSal)
    {
        if (emit_prompt == IM_THE_TOWN_CRIER)
        {
            ss_uiOp_emit_newline ();             // make sure to clear the "other's" output
            ssUI_Prompt ();
        }   // something moved the cursor from the start of a line with nothing displayed
    }   //
}   // ssUI_Main

// -------------------------------------------------------------------------------------------------
boolean ssUI_rootMain (boolean restart_ssUI)
{
    boolean returnboolean;

    if (restart_ssUI)
    {
        ssUI_cmdZone_InitStack ();
    }

    returnboolean = ssUI_inOp_Line_GetParseHandle ();
    return (returnboolean);
}   // ssUI_rootMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_MathMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle ());
}   // ssUI_MathMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_LoopMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle ());
}   // ssUI_loopmain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_CmdsMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle ());
}   // ssUI_CmdsMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_ssMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle ());
}   // ssUI_ssMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_EvapiMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle ());
}   // ssUI_EvapiMain
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// the buffer is doubly large for safely catching invalid input strings exceeding buffers in ssUI.
// The buffer is allocated by a function on the stack; only exists within the scope of that function.
// -------------------------------------------------------------------------------------------------
// input process's userMax_i parameter indicates the service routine the size of the caller's buffer.
// the service function refuses to receive more and does not manage bursts totaling more than _MAX_I.
// SimpleSal does not employ advanced ssHL tricks such as having a parameter determine the array size.
// -------------------------------------------------------------------------------------------------
#define  INPUT_BUFFER_ALLOC   (2 * SSUI_BUFFER_ALLOC)
#define  INPUT_BUFFER_MAX_I   (INPUT_BUFFER_ALLOC-1)

// -------------------------------------------------------------------------------------------------
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Ascii characters input are put an into a NUL-terminated array of bytes, a legacy ssHL string.
// The local input buffer is used to store bytes from an independent and asynchronous signaler;
//    the collection of bytes has no length specification built into the data, except NUL.
// -------------------------------------------------------------------------------------------------
// CR is defined as "the Ascii byte value received first when the user presses the <enter> key".
// -------------------------------------------------------------------------------------------------
// #define DEBUG_INPUT
#ifdef DEBUG_INPUT
#define InputDebugSignal(x)   mesa_uiOp_emit_1 (x)
#define InputDebugHex(x)      ss_uiOp_emit_Hex_2 (x)
#else   // not DEBUG_INPUT
#define InputDebugSignal(x)
#define InputDebugHex(x)
#endif  // DEBUG_INPUT


// -------------------------------------------------------------------------------------------------
// This fifo collects bytes in N-byte chunks, without direct attention to their values.  The fifo
// collection function only collects bytes when given Agency, and then only in a burst.  Agency is
// granted by one of: a millisecond counter Event using classic Time or an ssTEA-based Time Event.
// The discussion below correlates the size of the burst to the rate at which bytes are perceived.
// -------------------------------------------------------------------------------------------------
// After a burst of bytes has been collected by the Collector, and the collector sees a CR anywhere
// within the N bytes collected so far, the fifo returns a subset of the collected bytes: "a line".
// A line contains all bytes up to and including the CR and a NUL byte, the "count" of bytes in a
// line includes all bytes before the NUL byte, therefore the minimum length of a line is one (1)
// and the minimum space required to receive a line is two (2).  The maximum is set by the caller.
// -------------------------------------------------------------------------------------------------
// Returned length values: 0 bytes in a line; N bytes (fifo-defined minimum, user-defined maximum).
// -------------------------------------------------------------------------------------------------
// A fifo retains data that has not been read (or overwritten by unread incoming bytes).  If a burst
// of 10 bytes is perceived, stored and the 4th is a CR, "the line" is returned as defined above.
// The fifo moves the remaining bytes to the front of the buffer, so the next burst will append to
// any existing bytes.  This potential second line (<bytes0><cr0><bytes1><cr1>) will be seen by the
// collector the next time it is given Agency and copied caller's buffer as defined above.  Latency.
// The caller ready for a 2nd line can get it; otherwise it sits until the next collection Agency.
// -------------------------------------------------------------------------------------------------
// This implementation of the FSM watches the counter value in the classic Timekeeping mechanism.
// -------------------------------------------------------------------------------------------------
// The other implementation of the FSM uses a Time Event collector signaling lines with a Data Event.
// when collected_ct is zero, the collection is empty.
// When the next byte is received it is written at collected_ct, then collected_ct is incremented.
// -------------------------------------------------------------------------------------------------
AsciiA_t    Collected[INPUT_BUFFER_ALLOC];      //
int         collected_ct;

TimeUnitsBig_t      ms_last_AvailCt_check;

// -------------------------------------------------------------------------------------------------
boolean ssUI_inOp_CollectFromSerial_Char (pAscii_t pAsciiA)
{
    return (false);     // bugbugbug what was this?
}   // ssUI_inOp_CollectFromSerial_Char
// -------------------------------------------------------------------------------------------------
boolean ssUI_inOp_Line_FromSerial (pAsciiA_t pUserBuff, int userMax_i)
{
    Ascii_t     aByte;
    int         AvailNow = 0;
    int         i;                          // character buffer manipulation uses indexing a lot
    int         CR_i;                       // if a CR is found, remember where it was found
    int         moved_ct;                   // character buffer manipulation needs familiar terms
#ifdef DEBUG_INPUT
    pAsciiA_t   pUserBuff_Orig = pUserBuff;
#endif  // DEBUG_INPUT

    // if the ms counter value has not changed since the last call, still in the same millisecond.
    // We are not going to use any more of the resources here until the gate opens at next boundary.
    if (mesa_gCtOf_1msThis100ms == ms_last_AvailCt_check)
    {
        return (false);         // too little Time, don't care if CR has been received (latency)
    }
    // record where we are now in Time so "future we" can see if "past we" waited long enough yet
    ms_last_AvailCt_check = mesa_gCtOf_1msThis100ms;

    // Gating using Time is done to reduce the ask of "Available?" more often than necessary.
    AvailNow = ss_uiOp_recv_avail_ct ();

    // if nothing is left over from the last collection (post CR), and nothing in this one, done.
    if ( (AvailNow == 0) && (collected_ct == 0) )
    {
        return (0);
    }

    // the rest of the state table (aside from "exit if collected_ct == 0 and AvailNow == 0")
    // shows the remaining combinations and the action taken by the collector in each case.
    //   Available == 0  and  collected_ct is > 0       check for CR;   (no entry into while)
    //   Available > 0   and  collected_ct is == 0      serial into fifo; check for CR
    //   Available > 0   and  collected_ct is > 0       serial into fifo; check for CR
    // transfer available from the Serial device (probably a buffer in memory) to this buffer
    while (AvailNow > 0)
    {
        aByte = ss_uiOp_recv_1 ();
        AvailNow--;

        InputDebugSignal ('(');
        InputDebugHex (aByte);
        InputDebugSignal (')');

        // store the byte in the buffer first (using count zero-based) then incr count (one-based)
        Collected[collected_ct] = aByte;        // location[0] = value
        collected_ct++;                         // count = 1
    }   // while AvailNow > 0

    // check for CR  (may be inefficient in that every collection searches the entire buffer from 0)
    // count as index so for loop is from 0 to count-1
    for (i = 0; i < collected_ct; i++)
    {
        if (Collected[i] == Ascii_CR)
        {
            CR_i = i;

            // remove collected[N] to userBuff[N]
            // index into the collection means the for loop index is from 0 to index or "i <= CR_i"
            // index as index so for loop is from 0 to index
            for (i = 0; i < CR_i; i++)
            {
                *pUserBuff = Collected[i];
                pUserBuff++;                                    // added one
                collected_ct--;                                 // removed one
            }   // for each byte up to and including CR
            collected_ct--;                 // didn't move the CR to user's buffer
            *pUserBuff = Ascii_NUL;         // <chars><CR><NUL> for any N chars

            // move any remaining to the start of buffer (characters after <enter> are a new line)
            if (collected_ct > 0)
            {
                moved_ct = 0;
                CR_i++;                         // character after CR
                // count as index so while loop is from N to M
                while (moved_ct < collected_ct)
                {
                    Collected[moved_ct] = Collected[CR_i];
                    moved_ct++;
                    CR_i++;
                }  // for each byte after CR to end of collected
            }   // remaining after line moved out

#ifdef DEBUG_INPUT
            mesa_uiOp_emit_qAsciiA ("line [");
            mesa_uiOp_emit_pAsciiA (pUserBuff_Orig);
            mesa_uiOp_emit_qAsciiA ("]");
            mesa_uiOp_emit_newline ();
#endif // DEBUG_INPUT
            return (true);              // found at least one line in the input stream
        }   // found CR
    }   // for loop looking for CR

    return (false);
}   // ssUI_inOp_Line_FromSerial
// -------------------------------------------------------------------------------------------------
void ssUI_AnnounceFailure (int len, int MAX_I)
{
    // there is serial available, and read in up to <enter>, and length 0, two causes:
    // too few bytes before <enter> or too many bytes before end of buffer.
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("Input ignored: string too short or too long to capture.");
    ss_uiOp_emit_Int_999 (len);
    ss_uiOp_emit_qAsciiA (" bytes input > ");
    ss_uiOp_emit_Int_999 (MAX_I);
    ss_uiOp_emit_qAsciiA (" bytes allowed");
    ss_uiOp_emit_newline ();
}   // ssUI_AnnounceFailure
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Paired with Arduino's IDE, Available Now Count returns zero until user hits CR in Serial Monitor.
// The read of Available Now Count here does not affect values recv_UpToEnter sees once count > 0.
// Arduino update: when the IDE 2.3.5 accepts user input terminated by a CR carriage return, the
// first byte is transmitted, then a long gap occurs where Available Now count is 0 (although there
// is always AT LEAST the CR character after the gap for any input), then the remaining
// characters up to and including the CR are transmitted.
//
// To handle Arduino IDE properly then, a delay waiting for additional characters is required.
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_inOp_Line_FromCmds (pAsciiA_t pUserBuff, int userBuff_MAX_I)
{
    pAsciiA_t    pCmdsStringPointer;
    int         length;

    // if next == Null, there are no more pointers in the array of pointers to command strings,
    // OR               the end-of-commands string was found in the command list
    // as with Ascii arrays, the length is not known, instead the end is demarcated with a value.
    pCmdsStringPointer = ssUI_cmdsOp_nextCmdFromLinkActive (ssUI_control.Cmds_i);
    if (!pCmdsStringPointer)
    {
        return (false);
    }

    // the serial inputIfAny function also checks for maximum as the bytes are being stored
    length = ss_uiOp_Count_Aa(pCmdsStringPointer);
    if (length > userBuff_MAX_I)
    {
        ssUI_AnnounceFailure (length, userBuff_MAX_I);
        return (false);
    }
    else
    {
        // The pCmdsStringPointer is a pointer to a string, most likely in an array of pointers to strings;
        // both or either of the pointer array or the string array(s) may be in non-writable memory.
        // This copy gets the string into writable memory.
        ss_uiOp_Duplicate_Aa (pUserBuff, pCmdsStringPointer);
        return (true);
    }   // valid length (0->userBuff_MAX_I)
}   // ssUI_inOp_Line_FromCmds
// -------------------------------------------------------------------------------------------------
// In Arduino IDE, "no characters reported until user hits enter" causes IfAny to be true,
//        capture into localBuff, SawAnyInput=true and input_len>=0.    bugbugbug
// In TeraTerm, characters are transmitted before the user hits enter, as they are typed.
// expression says "if (didn't get a string or did but it has no length) & commands on",
// Commands mode runs through an array of input strings, unless user input precedes.
// -------------------------------------------------------------------------------------------------
boolean ssUI_inOp_Line_GetParseHandle (void)
{
    boolean         SawAnyInput;        // an FSM where an input string goes from "not SawAnyInput" to SawAnyInput
    eCmdFsmResult_t CmdFsmResult;

    boolean         crunched_semicolon; // an FSM where a crunched semicolon is treated as end-of-line
    int             command_start_i;    // points to first character in the command (line)
    int             command_finish_i;   // points to the NUL character in the command line
    pAsciiA_t       pResultAnnounce;

    AsciiA_t        localBuff[SSUI_BUFFER_ALLOC];

    SawAnyInput = false;

    if (ssUI_inOp_Line_FromSerial (localBuff, SSUI_BUFFER_MAX_I))
    {
        SawAnyInput = true;
    }
    else
    {
        if (ssUI_control.CmdsAreRunning)
        {
            if (ssUI_inOp_Line_FromCmds (localBuff, SSUI_BUFFER_MAX_I))
            {
                ssUI_control.Cmds_i++;
                SawAnyInput = true;
            }
            else
            {
                ssUI_control.CmdsAreRunning = false;
                ss_uiOp_emit_newline ();
                ss_uiOp_emit_Dash (40);
                ss_uiOp_emit_qAsciiA ("commands completed");
                ss_uiOp_emit_newline ();
                return (IM_THE_TOWN_CRIER);
            }
        }   // no entry, just enter, or command on
    }   // no line collected

    // input parse and handle does nothing when nothing is provided to parse and handle
    if (!SawAnyInput)
    {
        return (THERES_NO_ACTION);
    }   // no input detected or generated...

    // input parse and handle consumes an "echo" line and the UI/parsers never get to see it
    if (ssUI_BufferStartsWith_echo (localBuff))
    {
        ss_uiOp_pBanner (lfY, localBuff, lfN);
        return (IM_THE_TOWN_CRIER);
    }   // echo ...

    // input parse and handle gives the input buffer to what it assumes is the App's input handler
    if (ssUI_BufferStartsWith_app (localBuff))
    {
        ss_uiOp_emit_newline ();
        App_HeyProcessThis (localBuff);

        // App input parse and handle consumes an "app" token and the UI/parsers never get to see it.
        // Different from no input seen or echo seen, assume that the app made UI need a fresh prompt.
        // Also, if the App changed the owner to ssUI from the App, there needs to be a new prompt.
        return (IM_THE_TOWN_CRIER);
    }   // app ...

    // find the first Ascii character of the input string that is non-space
    // find the first NUL Ascii character or a line terminator such as carriage return or linefeed.
    // 01234567890
    // <abc>          "abc"
    // <  abc>        "abc"
    // <abc;def>      "abc"
    command_start_i = 0;
    command_finish_i = 0;
    while (localBuff[command_start_i] != Ascii_NUL)
    {
        // skip over any leading spaces, maintain the relationship between first/last,
        // first and last are equal at the start of each loop that "extracts" a valid AsciiA_t.
        while (localBuff[command_start_i] == Ascii_Space)
        {
            command_finish_i++;
            command_start_i++;
        }   // while space

        // find the NUL Ascii terminator of the variable-length array of Ascii characters
        while (
                (localBuff[command_finish_i] != Ascii_NUL)
             &&
                (localBuff[command_finish_i] != Ascii_sColon)
              )
        {
            command_finish_i++;
        }

        // reached here because the while above found a NUL or a semicolon at command_finish_i;
        // when we find a semi-colon, make it look like an end-of-array for this command.
        // The key point is that "command_finish_i" includes the NUL byte at [command_finish_i]
        crunched_semicolon = false;
        if (localBuff[command_finish_i] == Ascii_sColon)
        {
            crunched_semicolon = true;
            localBuff[command_finish_i] = Ascii_NUL;
        }

        // in ssUI, commands are executed/processed when strings are matched, during this MenuTick.
        // CmdMatch processes the string in the buffer with activities, but does not consume the characters.
        // CmdFsmResult == handled            // command processor ate the input string and took all actions
        //      length is length of command in buffer, but we processed it as valid or error or defer
        // CmdFsmResult != handled            //  means "there may be a TimeStamp, there is no command"
        //      length is length of non-command with at least one character
        if (ssUI_control.BeVerbose)
        {
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_1 (Ascii_squareLBracket);
            ss_uiOp_emit_pAsciiA (&localBuff[command_start_i]);
            ss_uiOp_emit_1 (Ascii_squareRBracket);
        }
        ss_uiOp_emit_newline ();

        gToken_Ct = ssUI_tknOp_ParseAllTokens (&localBuff[command_start_i]);
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ss_uiOp_emit_newline ();
        ssUI_tknOp_Show_AllTkns (S("parsed   "));
#endif  // SSUI_OPTIN_DEBUG_TOKENS
        gToken_Ct = ssUI_tknOp_Prepend_cmdZone ();
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("prepended"));
#endif  // SSUI_OPTIN_DEBUG_TOKENS
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("before fix ="));
#endif  // SSUI_OPTIN_DEBUG_TOKENS
        gToken_Ct = ssUI_tknOp_MinusPlus_Fixup (gToken_Ct);             // in gTokens, fix [=-], [=+], [=][-], [=][+]
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("after fix  ="));
#endif  // SSUI_OPTIN_DEBUG_TOKENS

    // bugbugbug   clean up comments    promote the agency transfer to the FSM with tokens ready
        if (gToken_Ct > 0)
        {
            CmdFsmResult = ssUI_menuOp_CmdHandlerFSM ();
        }
        else
        {
            CmdFsmResult = eCmdFsm_rNoTokens;
        }

        // ssUI is a UI so explanations of results are important sometimes
        if (ssUI_control.BeVerbose)
        {
            switch (CmdFsmResult)
            {
                case eCmdFsm_rNormal        : pResultAnnounce = pcMsg_CmdFsm_NormalResult;  break;
                case eCmdFsm_rNoInput       : pResultAnnounce = pcMsg_CmdFsm_FoundNoInput;  break;
                case eCmdFsm_rNoTokens      : pResultAnnounce = pcMsg_CmdFsm_FoundNoTokens; break;
                case eCmdFsm_rNoMatch       : pResultAnnounce = pcMsg_CmdFsm_FoundNoMatch;  break;
                default                     : pResultAnnounce = pAsciiANull;
            }   // switch
        }       // verbose
        else
        {       // reticent
            switch (CmdFsmResult)
            {
                case eCmdFsm_rByHandler     : pResultAnnounce = pcMsg_CmdFsm_ByHandler;     break;
                case eCmdFsm_rUnresolved    : pResultAnnounce = pcMsg_CmdFsm_Unresolved;    break;
                case eCmdFsm_rTableFlaw     : pResultAnnounce = pcMsg_CmdFsm_TableFlaw;     break;
                default                     : pResultAnnounce = pAsciiANull;                break;
            }   // switch
        }   // reticent

        if (pResultAnnounce)
        {
            ss_uiOp_pBanner (lfY, pResultAnnounce, lfY);
        }
        // completed the current command (crunched semicolon or end of array), two actions:
        //  if crunched semicolon, the next byte after command_finish_i MAY BE the NUL byte, but
        //      need to skip the crunched semicolon (now a NUL byte) for while termination.
        //  if reached end of the array, the byte AT command_finish_i is in fact the NUL byte,
        //      leave the first_byte_i indicator pointing to the proper and original NUL byte.
        command_start_i = (crunched_semicolon) ? ++command_finish_i : command_finish_i;
    }   // while string to parse

    return (IM_THE_TOWN_CRIER);
}   // ssUI_inOp_Line_GetParseHandle
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferStartsWith_echo (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_e)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_c)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_h)      return (false);
    if (Ascii_toLower (pUserBuff[3]) != Ascii_o)      return (false);
    // the character after the "echo" must be a space or a NUL-byte
    if (Ascii_toLower (pUserBuff[4]) == Ascii_NUL)    return (true);
    if (Ascii_toLower (pUserBuff[4]) == Ascii_Space)  return (true);

    return (false);
}   // ssUI_BufferStartsWith_echo
// -------------------------------------------------------------------------------------------------
// the collection of characters used to mean "app" just has to be Ascii and contain no token separators.
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferStartsWith_app (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_a)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_p)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_p)      return (false);
    // the character after the "echo" must be a space
    if (Ascii_toLower (pUserBuff[3]) == Ascii_NUL)    return (true);
    if (Ascii_toLower (pUserBuff[3]) == Ascii_Space)  return (true);
    return (true);
}   // ssUI_BufferStartsWith_app
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferOnlyHas_appui (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_a)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_p)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_p)      return (false);
    if (Ascii_toLower (pUserBuff[3]) != Ascii_u)      return (false);
    if (Ascii_toLower (pUserBuff[4]) != Ascii_i)      return (false);
    // the character after the "app" must be a NUL byte
    if (Ascii_toLower (pUserBuff[5]) != Ascii_NUL)     return (false);
    return (true);
}   // ssUI_BufferOnlyHas_appui
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferOnlyHas_ssui (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_s)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_s)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_u)      return (false);
    if (Ascii_toLower (pUserBuff[3]) != Ascii_i)      return (false);
    // the character after the "ssui" must be a NUL byte
    if (Ascii_toLower (pUserBuff[4]) != Ascii_NUL)     return (false);
    return (true);
}   // ssUI_BufferOnlyHas_ssui
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This function prepends the cmdZone based on the rules implied by that long paragraph right there.
// -------------------------------------------------------------------------------------------------
int ssUI_tknOp_Prepend_cmdZone (void)
{
    int         tkn_i;                      // must be signed so can go negative in terminating

    // enforce some basic rules about when cmdZone is possible:
    //   rule: can't assume anything at ssUI root "ssUI" cmdZone, must fully specify cmdZone
    if (gcmdZone_stack[ssUI_control.cmdZone_stack_i] == ssUI_root)
    {
        return (gToken_Ct);
    }
    //   rule: any collection of commands with the same value are a CmdZone
    if (ssUI_menuIf_CmdTknIsCmd (gTokens[ssUI_Cmd_tkn_i].pAsciiA))
    {
       return (gToken_Ct);
    }
    // fundamental ssHL algorithm to move an array right, from right to left, to avoid overwriting.
    // move all contents one slot to the right: if max_I token present, it gets crunched and lost.
    for (tkn_i = (SSUI_TOKENS_MAX_I-1); tkn_i >= 0; tkn_i--)       // yes diff-diff.
    {
        ssUI_tknOp_iP1_gets_iP2 (tkn_i+1, tkn_i);           // 5->6, 4->5, 3->4, etc.
    }   // for

    // to exit loop, tkn_i went negative.  We know we made a hole at tkn1st_i.
    gTokens[ssUI_Cmd_tkn_i].pAsciiA =
        gpcmdZoneTkns[gcmdZone_stack[ssUI_control.cmdZone_stack_i]]->pAsciiA;
    gTokens[ssUI_Cmd_tkn_i].valueCt =
        gpcmdZoneTkns[gcmdZone_stack[ssUI_control.cmdZone_stack_i]]->valueCt;
    return (gToken_Ct+1);
}   // ssUI_tknOp_Prepend_cmdZone

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssUI_tknOp_MinusPlus_Prepend (Ascii_t thisValue, ssUI_pToken_t pToken)
{
    AsciiA_t    FixerForEqOp[TimeStamp_ALLOC];
    int         i;

    FixerForEqOp[0] = thisValue;

    // unconventional loop control to include copying the null byte of the original token
    for (i=0; i <= pToken->valueCt; i++)
    {
        FixerForEqOp[i+1] = pToken->pAsciiA[i];
    }   // for

    pToken->valueCt++;
    pToken->pAsciiA = &FixerForEqOp[0];
}   // ssUI_tknOp_MinusPlus_Prepend
// -------------------------------------------------------------------------------------------------
int ssUI_tknOp_MinusPlus_Fixup (int foundTokenCt)
{

    // TimeStamp conversion allows signed numbers; a timestamp is assembled from user input values:
    // Requirement: see "math timevar=-time" in any form and associate the sign with the timestamp.
    // The - or + is associated with the [time] at human level; the token parser sees separators.
    // ---------------------------------------------------------------------------------------------
    // Requirement: a sign may only be applied to "lvalue = oper1" expressions, because
    // a sign on either operand in the math expression "lvalue = oper1 op oper2" creates 7 tokens
    // unless the parser is smart enough to embed the analysis/actions contained in this function.
    // ---------------------------------------------------------------------------------------------
    // 7 or more tokens: tokenizer stops at 6
    //          [math] [timevar] [=] [-] [time] [op] [time]         too complicated in any case
    //          [math] [timevar] [=] [time] [op] [-] [time]         too complicated in any case
    // ---------------------------------------------------------------------------------------------
    // find this: <pattern> in      [math][timevar]<pattern>[time]      math timea = -10sc
    // parser result <pattern>: either one token ([=-] or [=+]), or two tokens ([=][-] or [=][+]).
    // ---------------------------------------------------------------------------------------------
    // fewer than 4 tokens:
    //       are not this problem (commands are "[command] [timevar] operation [timevar]" minimum).
    // ---------------------------------------------------------------------------------------------
    if (foundTokenCt < 4)
    {
        return (foundTokenCt);
    }   // ct < 4
    // ---------------------------------------------------------------------------------------------
    //          [math] [timevar] [=-] [time]                        most useful "math timea=-10sc".
    // ---------------------------------------------------------------------------------------------
    if (foundTokenCt == 4)
    {
        // moving the minus sign from the end of one token to the beginning of another token
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn3rd_i], S("=-")) )
        {
            gTokens[ssUI_tkn3rd_i].pAsciiA[1] = Ascii_NUL;
            gTokens[ssUI_tkn3rd_i].valueCt = 1;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Minus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        // moving the plus sign from the end of one token to the beginning of another token
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn3rd_i], S("=+")) )
        {
            gTokens[ssUI_tkn3rd_i].pAsciiA[1] = Ascii_NUL;
            gTokens[ssUI_tkn3rd_i].valueCt = 1;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Plus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        return (foundTokenCt);
    }   // ct == 4
    // ---------------------------------------------------------------------------------------------
    // 5 tokens:
    //          [math] [timevar] [=-] [time] [any]                  valid, prepend to time, shift all
    //                      this will not be parsed successfully as a math operation
    //          [math] [timevar] [=] [-] [time]                     valid, shift time and prepend
    //                      this will be parsed successfully as a math operation
    // ---------------------------------------------------------------------------------------------
    if (foundTokenCt == 5)
    {
        // moving the minus sign from the end of one token to the beginning of another token
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn3rd_i], S("=-")) )
        {
            gTokens[ssUI_tkn3rd_i].pAsciiA[1] = Ascii_NUL;
            gTokens[ssUI_tkn3rd_i].valueCt = 1;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Minus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        // moving the plus sign from the end of one token to the beginning of another token
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn3rd_i], S("=+")) )
        {
            gTokens[ssUI_tkn3rd_i].pAsciiA[1] = Ascii_NUL;
            gTokens[ssUI_tkn3rd_i].valueCt = 1;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Plus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn4th_i], S("-")) )
        {
            gTokens[ssUI_tkn4th_i].pAsciiA = gTokens[ssUI_tkn5th_i].pAsciiA;
            gTokens[ssUI_tkn4th_i].valueCt = gTokens[ssUI_tkn5th_i].valueCt;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Minus, &gTokens[ssUI_tkn4th_i]);
            gTokens[ssUI_tkn5th_i].pAsciiA = pAsciiANull;
            gTokens[ssUI_tkn5th_i].valueCt = 0;
            foundTokenCt--;
            return (foundTokenCt);
        }
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn4th_i], S("+")) )
        {
            gTokens[ssUI_tkn4th_i].pAsciiA = gTokens[ssUI_tkn5th_i].pAsciiA;
            gTokens[ssUI_tkn4th_i].valueCt = gTokens[ssUI_tkn5th_i].valueCt;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Plus, &gTokens[ssUI_tkn4th_i]);
            gTokens[ssUI_tkn5th_i].pAsciiA = pAsciiANull;
            gTokens[ssUI_tkn5th_i].valueCt = 0;
            foundTokenCt--;
            return (foundTokenCt);
        }
        return (foundTokenCt);
    }   // ct == 5
    // ---------------------------------------------------------------------------------------------
    // 6 tokens:
    //          [math] [timevar] [=-] [time] [op] [time]            valid, crunch -, prepend to time
    //                      this will be parsed successfully as a math operation
    //          [math] [timevar] [=] [-] [time] [op]                valid, prepend to time, shift all
    //                      this will not be parsed successfully as a math operation
    // ---------------------------------------------------------------------------------------------
    if (foundTokenCt == 6)
    {
        if ( ssUI_tknIf_P1_eq_pAa (&gTokens[ssUI_tkn3rd_i], S("=-")) )
        {
            gTokens[ssUI_tkn3rd_i].pAsciiA[1] = Ascii_NUL;
            gTokens[ssUI_tkn3rd_i].valueCt = 1;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Minus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn3rd_i], S("=+")) )
        {
            gTokens[ssUI_tkn3rd_i].pAsciiA[1] = Ascii_NUL;
            gTokens[ssUI_tkn3rd_i].valueCt = gTknOpAssign.valueCt--;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Plus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn4th_i], S("-")) )
        {
            gTokens[ssUI_tkn4th_i].pAsciiA = gTokens[ssUI_tkn5th_i].pAsciiA;
            gTokens[ssUI_tkn4th_i].valueCt = gTokens[ssUI_tkn5th_i].valueCt;
            gTokens[ssUI_tkn5th_i].pAsciiA = gTokens[ssUI_tkn6th_i].pAsciiA;
            gTokens[ssUI_tkn5th_i].valueCt = gTokens[ssUI_tkn6th_i].valueCt;
            gTokens[ssUI_tkn6th_i].pAsciiA = pAsciiANull;
            gTokens[ssUI_tkn6th_i].valueCt = 0;
            foundTokenCt--;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Minus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        if ( ssUI_tknIf_pP1_eq_pAa (&gTokens[ssUI_tkn4th_i], S("+")) )
        {
            gTokens[ssUI_tkn4th_i].pAsciiA = gTokens[ssUI_tkn5th_i].pAsciiA;
            gTokens[ssUI_tkn4th_i].valueCt = gTokens[ssUI_tkn5th_i].valueCt;
            gTokens[ssUI_tkn5th_i].pAsciiA = gTokens[ssUI_tkn6th_i].pAsciiA;
            gTokens[ssUI_tkn5th_i].valueCt = gTokens[ssUI_tkn6th_i].valueCt;
            gTokens[ssUI_tkn6th_i].pAsciiA = pAsciiANull;
            gTokens[ssUI_tkn6th_i].valueCt = 0;
            foundTokenCt--;
            ssUI_tknOp_MinusPlus_Prepend (Ascii_Plus, &gTokens[ssUI_tkn4th_i]);
            return (foundTokenCt);
        }
        return (foundTokenCt);
    }   // ct == 6
    return (foundTokenCt);
}   // ssUI_tknOp_MinusPlus_Fixup
// -------------------------------------------------------------------------------------------------
boolean ssUI_tknIf_P1_eq_pAa (ssUI_pToken_t pToken, pAsciiA_t pAsciiA)
{
    return (ssUI_AaIf_pP1_eq_pP2 (pToken->pAsciiA, pAsciiA));
}   // ssUI_tknIf_P1_eq_pAa
// -------------------------------------------------------------------------------------------------
boolean  ssUI_tknIf_P1eqP2 (ssUI_pToken_t pToken1, ssUI_pToken_t pToken2)
{
    if (pToken1->valueCt != pToken2->valueCt) return (false);

    return (ssUI_AaIf_pP1_eq_pP2 (pToken1->pAsciiA, pToken2->pAsciiA));
}   // ssUI_tknIf_P1eqP2
// -------------------------------------------------------------------------------------------------
// the current contents of the gTokens array (tokenized input string) point to the input buffer.
// The tokenized strings must be copied and the token pointers adjusted to the copied version.
// When the "loop on" command is encountered, the captured tokens and strings are evaluated.
// -------------------------------------------------------------------------------------------------
// the problem is basically an array of random pointers to Ascii values that need to be copied.
// After the values themselves are copied (up to and including the NUL byte), the starting
// byte and length of that copy is stored in a token data structure.  The starting position in
// the copied values buffer is adjusted based on the length of the buffer just copied.
// -------------------------------------------------------------------------------------------------
void  ssUI_tknOp_SaveACopy_TermCondCmd (void)
{
    int         token_i;
    pAsciiA_t    pTknSaved;

    pTknSaved = &gCmdTermCondSaved[0];
    for (token_i=0; token_i <= SSUI_TOKENS_MAX_I; token_i++)
    {
        // from the gTokens array copy only those non-Null tokens to a hidden buffer and point at it,
        // OR make sure that the gTokens array pointer is Null so the Restore knows when to stop.
        if ( (gTokens[token_i].pAsciiA != pAsciiANull) && (gTokens[token_i].valueCt > 0) )
        {
            // copy a ssHL series of nonzero-values, followed by the proper terminating NUL byte
            ss_uiOp_Duplicate_Aa (pTknSaved, gTokens[token_i].pAsciiA);
            gTknsTermCond[token_i].pAsciiA = pTknSaved;
            gTknsTermCond[token_i].valueCt = gTokens[token_i].valueCt;
            pTknSaved += (gTokens[token_i].valueCt + 1);        // don't forget the NUL byte
        }   // if there is a token
        else
        {
            // make sure every slot not pointing to a token has a zero length string at Null
            gTknsTermCond[token_i].pAsciiA = pAsciiANull;
            gTknsTermCond[token_i].valueCt = 0;
        }
    }   // for

#ifdef SSUI_OPTIN_DEBUG_TOKENS_LOOP
    ss_uiOp_emit_newline ();
    ssUI_tknOp_Show_AllTkns (S("<gTokens> -> <saved>"));
#endif  // SSUI_OPTIN_DEBUG_TOKENS_LOOP
}   // ssUI_tknOp_SaveACopy_TermCondCmd
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssUI_tknOp_Restore_TermCondCmd (void)
{
    int found_Ct = 0;

    for (gToken_Ct=0; gToken_Ct <= SSUI_TOKENS_MAX_I; gToken_Ct++)
    {
        if (
             (gTknsTermCond[gToken_Ct].pAsciiA != pAsciiANull)
           &&
             (gTknsTermCond[gToken_Ct].valueCt > 0)
           )
        {
            gTokens[gToken_Ct].pAsciiA = gTknsTermCond[gToken_Ct].pAsciiA;
            gTokens[gToken_Ct].valueCt = gTknsTermCond[gToken_Ct].valueCt;
            found_Ct++;
        }   // if there is a token
        else
        {
            gTokens[gToken_Ct].pAsciiA = pAsciiANull;
            gTokens[gToken_Ct].valueCt = 0;
        }
    }   // for

    // update the global control value for the tokens based on the number actually found
    gToken_Ct = found_Ct;

#ifdef SSUI_OPTIN_DEBUG_TOKENS_LOOP
    ss_uiOp_emit_newline ();
    ssUI_tknOp_Show_AllTkns (S("<saved> -> <gTokens>"));
#endif  // SSUI_OPTIN_DEBUG_TOKENS_LOOP
}   // ssUI_tknOp_Restore_TermCondCmd
// -------------------------------------------------------------------------------------------------
// tokenize a copy of a string, group non-separators together by finding math operations or spaces.
// Pointers stored in the caller's token array are pointing to static data in a "local" buffer.
// -------------------------------------------------------------------------------------------------
int     ssUI_tknOp_ParseAllTokens (pAsciiA_t pInput)
{
    // N token strings, each maximum length (TknMax) with a separator, requires (N * (TknMax + 1)).
    static      AsciiA_t     inputCopy[SSUI_BUFFER_ALLOC];
    int         i;
    pAscii_t    p_charInToken;
    pAscii_t    p_copyNullByte;
    pAscii_t    p_startThisToken;
    int         foundTokenCt;

    for (foundTokenCt = ssUI_Cmd_tkn_i; foundTokenCt <= SSUI_TOKENS_MAX_I; foundTokenCt++)
    {
        gTokens[foundTokenCt].pAsciiA = pAsciiANull;
        gTokens[foundTokenCt].valueCt = 0;
    }   // for each token pointer in array
    foundTokenCt = 0;

    // because we don't want to input limit strings to RAM locations by tokenizing the user's copy,
    //   Return pointers to our copy, where chunks of the string have been tokenized in place.
    // The copy string is allocated at compile time and the current value is accessible through
    // the "pointer" field in gTokens[n], but the tokens are only valid until parse occurs again.
    i = ssUI_AaOp_pP1_gets_pP2maxpadded (inputCopy, pInput, SSUI_BUFFER_ALLOC);

#ifdef SSUI_OPTIN_DEBUG_TOKENS
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_AsciiA (S("presented to tokenizer"), inputCopy);
    ss_uiOp_emit_newline ();
#endif  // SSUI_OPTIN_DEBUG_TOKENS

    p_copyNullByte = &inputCopy[i];
    p_startThisToken = &inputCopy[0];
    p_charInToken = p_startThisToken;

    // design choice, space characters must envelope characters meant to be considered a token.
    // this loop is editing the local copy of the input string, not moving strings around.
    while ((p_charInToken < p_copyNullByte) && (foundTokenCt <= SSUI_TOKENS_MAX_I))
    {
        // any spaces before a Token are whitespace and outside our interest.
        while (*p_charInToken == Ascii_Space)
        {
            p_charInToken++;
            p_startThisToken++;
        }   // while spaces
        gTokens[foundTokenCt].pAsciiA = p_startThisToken;

        // step over ANY actual characters between spaces, that is, find the token's last character.
        while ( (*p_charInToken != Ascii_NUL) && (*p_charInToken != Ascii_Space) )
        {
            gTokens[foundTokenCt].valueCt++;
            p_charInToken++;
        }   // while token

        //bugbugbug  if found all characters in token because found a space, workaround for
        // input " timea ! =" (maintenanceTask: why not "!="?).
        // Also ignoring the error for "timea !=" as input, the focus here is creating valid tokens.
        if (*(p_charInToken) == Ascii_Space)
        {   // does the current token of any length start with '!'?
            if (*(p_startThisToken) == Ascii_Epoint)
            {   // is the token starting with '!' only one character long?
                if (p_charInToken == (p_startThisToken+1))
                {   // the space character follows the '!' character,
                    // is the character following the space an '='?
                    if (*(p_charInToken+1) == Ascii_EQ)
                    {
                        *(p_charInToken) = Ascii_EQ;            // write new EQ over old space
                        p_charInToken++;                        // put NUL where old EQ was
                        gTokens[foundTokenCt].valueCt++;        // added a character to token
                    }   // !_=
                }   // input processing puts space between '!' and '=' when finding '!='
            }   // started with '!'
        }   // found a space

        // tokenize: make a proper legacy ssHL string: 0 or more "characters" terminated by NUL value.
        // writing a NUL at the current location finishes the definition of a token.
        *p_charInToken = Ascii_NUL;
        p_charInToken++;
        foundTokenCt++;                        // been tokenized, count it as a token
        p_startThisToken = p_charInToken;
    }   // while string to parse and token array elements to hold pointers

    return (foundTokenCt);
}   // ssUI_tknOp_ParseAllTokens

#ifdef SSUI_OPTIN_DEBUG_TOKENS
// #define DO_DUMP_ADDR_LEN
#define DO_DUMP_TOKENS

void    ssUI_tknOp_Show_AllTkns (pAsciiA_t   pDescription)
{
    int tkn_i;

    ss_uiOp_emit_lbld_AsciiA (S("tokens"), pDescription);
    ss_uiOp_emit_Hex_1 (gToken_Ct);
    ss_uiOp_emit_1 (Ascii_Colon);
    for (tkn_i = ssUI_Cmd_tkn_i; tkn_i <= SSUI_TOKENS_MAX_I; tkn_i++)
    {
#ifdef DO_DUMP_ADDR_LEN
        ss_uiOp_emit_1 (Ascii_Larrow);
        ss_uiOp_emit_Hex_32bits (gTokens[tkn_i].pAsciiA);
        ss_uiOp_emit_1 (Ascii_Colon);
        ss_uiOp_emit_Int_999 (gTokens[tkn_i].valueCt);
        ss_uiOp_emit_1 (Ascii_Rarrow);
#endif
#ifdef DO_DUMP_TOKENS
        ss_uiOp_emit_1 (Ascii_Colon);
        ss_uiOp_emit_lbld_AsciiA (S("token[i]"), gTokens[tkn_i].pAsciiA);
#endif
    }   // for
    ss_uiOp_emit_newline ();
}   // ssUI_tknOp_Show_AllTkns
#endif  // SSUI_OPTIN_DEBUG_TOKENS

#endif  // __SSUI_INPUT_DEF_H


