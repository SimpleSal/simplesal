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
// -------------------------------------------------------------------------------------------------
// Input processing is performed asynchronously; the FSM depends on an InitState setting variables.
// -------------------------------------------------------------------------------------------------
// The buffer is doubly large for safely catching more than one "command line" arriving in one burst.
// -------------------------------------------------------------------------------------------------
#define  INPUT_FIFO_ALLOC       (2 * SSUI_UIBUFFER_ALLOC)
#define  INPUT_FIFO_MAX_I     (INPUT_FIFO_ALLOC-1)

int         ssUI_gInputCollected_Ct;
int         ssUI_gInputCollected_i;
AsciiA_t    ssUI_gCollected[INPUT_FIFO_ALLOC];

// -------------------------------------------------------------------------------------------------
// ssUI input fifo processing is ready to be granted Agency as often as "loop" is granted Agency.
// -------------------------------------------------------------------------------------------------
// Comparing a free-running millisecond counter to a value from the last Time granted Agency, see if
// enough Time has elapsed for the Host OS serial input device driver to receive some bytes, until
// enough Time has elapsed to receive bytes, don't spend resources checking for bytes to arrive.
// Because Agency is deferred until a 1 millisecond boundary occurs: if counter changed, 1ms occurred.
// -------------------------------------------------------------------------------------------------
TimeUnitsBig_t      ms_last_AvailCt_check;
#ifdef SSUI_ONEOF_SERIAL_RECEIVES_LINES
TimeUnitsBig_t      ms_EOL_Timer;
#endif // SSUI_ONEOF_SERIAL_RECEIVES_LINES

// -------------------------------------------------------------------------------------------------
// The Ascii receive FSM can be tested and stressed by changing the baud rate to a slow/high rate.
// -------------------------------------------------------------------------------------------------
// If life delivers bytes to the serial input device as fast as they can be emitted at this baud,
// now many bytes is that per millisecond?  The FSM is bounded by Time periods, each period is
// 1 millisecond long; at each boundary the fifo grants itself Agency to receive bytes into the fifo.
// -------------------------------------------------------------------------------------------------
// RS232 uses 10 bits per byte, 38400 bits in 1 second means 3840 10-bit bytes. During each of the
// 1-millisecond periods in 1 second, the capacity of the medium was 3.8 bytes.  Round result up and
// check for new bytes every millisecond, and at most the collection will get 3 or 4 bytes added.
// If no bytes arrive during one millisecond, 3 or 4 BYTE TIME PERIODS have gone by; LINES done.
// -------------------------------------------------------------------------------------------------
#define Ascii_Computed_BytesPerMS    (((THIS_BUILDS_BAUD_RATE / 10) + 500) / 1000)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void   ssUI_inOp_fifo_InitState (void)
{
    // the ssUI input processing FSM doesn't care about switching the Time Initialization occurs
    ssUI_gInputCollected_Ct  = 0;
    ssUI_gInputCollected_i   = 0;
    ms_last_AvailCt_check    = mesa_gCtOf_1msFreeRunning;

#ifdef SSUI_ONEOF_SERIAL_RECEIVES_LINES
    ms_EOL_Timer             = 0;
#endif // SSUI_ONEOF_SERIAL_RECEIVES_LINES

}   // ssUI_inOp_fifo_InitState

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void   ssUI_inOp_fifo_ReInitState (void)
{
#ifdef SSUI_ONEOF_SERIAL_RECEIVES_BYTES
    // Future: this should move collection to start position 0 or make the buffer circular
    ssUI_gInputCollected_Ct  = 0;
    ssUI_gInputCollected_i   = 0;
#endif // SSUI_ONEOF_SERIAL_RECEIVES_BYTES
#ifdef SSUI_ONEOF_SERIAL_RECEIVES_LINES
    // Everything received when CR entered; source of bytes must be buffering not-yet-emitted
    ssUI_gInputCollected_Ct  = 0;
    ssUI_gInputCollected_i   = 0;
#endif // SSUI_ONEOF_SERIAL_RECEIVES_LINES
}   // ssUI_inOp_fifo_ReInitState
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
    ss_uiOp_emit_1 (Ascii_Colon);
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
// -------------------------------------------------------------------------------------------------
// Commands are grouped into "cmdZones", or ssUI's menu system is a tree of groups of related
// functions: "cmds on" and "cmds run" relate to the command.  This is just ssUI's menu implemented.
// -------------------------------------------------------------------------------------------------
// The menu FSM is given agency when ssUI's collector returns an input array of Ascii values that have
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
// The Init State of ssUI occurs when ssUI_Main is invoked ONLY THE FIRST TIME ssUI_Main is called.
// -------------------------------------------------------------------------------------------------
void ssUI_Main (void)
{
    static  boolean  InitStateOccured = false;

    boolean emit_prompt = THERES_NO_ACTION;

    if (InitStateOccured == false)  // variable initialized at build-time, or grouped with init 0
    {
        ssUI_rootMain (true);       // the only call to rootMain with the value true: Init State
        InitStateOccured = true;    // continue on to make another call to rootMain NOT in Init State
    }   // Init State for command zones only the first time called

    // SimpleSal App is comprised of the Apps and the ssUI interface to manipulate the Apps and ssTEA
    if ((Ascii_ROs == RO_owner_ssUI) || (Ascii_ROs == RO_owner_Anybody))
    {
        if ((Ascii_RO_active & RO_active_OneOfUIs_ss))
        {
            // The only benefit from changing to a "Main" for cmds/loops/math etc is this menu trick:
            // the function prepends the command name "cmds" or "loop" or "math" to any input.
            switch (gcmdZone_stack[ssUI_control.cmdZone_stack_i])
            {
                case ssUI_root  :  emit_prompt = ssUI_rootMain (false);     break;  // no init
                case ssUI_cmds  :  emit_prompt = ssUI_CmdsMain ();          break;
                case ssUI_loop  :  emit_prompt = ssUI_LoopMain ();          break;
                case ssUI_math  :  emit_prompt = ssUI_MathMain ();          break;
                case ssUI_evapi :  emit_prompt = ssUI_EvapiMain ();         break;
                case ssUI_ss    :  emit_prompt = ssUI_ssMain ();            break;
                default         :  emit_prompt = IM_THE_TOWN_CRIER;         break;
            }   // switch

            // It is possible that one of the menus above changed the ownership from ssUI to !ssUI.
            // Returning to ssUI state from !ssUI is when the User of the Interface needs a prompt.
            // ssTEA_App is comprised of the Apps and the ssUI used to manipulate the Apps and ssTEA
            if (emit_prompt == IM_THE_TOWN_CRIER)   // when change to App UI, don't prompt ssUI
            {
                if ((Ascii_ROs == RO_owner_ssUI) || (Ascii_ROs == RO_owner_Anybody))
                {
                    if ((Ascii_RO_active & RO_active_OneOfUIs_ss))
                    {
                        ss_uiOp_emit_newline ();             // make sure to clear the "other's" output
                        ssUI_Prompt ();
                    }   // ssUI is active and is the focus
                }   // ssUI is STILL an owner of the UI
            }   // the menu that executed indicated a prompt was needed
        }   // ssUI is active and is the focus
    }   // ssUI is an owner of the UI
}   // ssUI_Main

// -------------------------------------------------------------------------------------------------
// return true means "a new prompt needs to be emitted by ssUI because some output occurred".
// -------------------------------------------------------------------------------------------------
boolean ssUI_rootMain (boolean isInit_State)
{
    if (isInit_State)
    {
        ssUI_cmdZone_InitStack ();
        return (true);
    }

    return (ssUI_inOp_Line_GetParseHandle (pInputNull));
}   // ssUI_rootMain
// -------------------------------------------------------------------------------------------------
// Parameter to GetParseHandle of Null pointer tells the function: this is not a command line that
// can be parsed and handled, so the function will first get input from a source to parse and handle.
// -------------------------------------------------------------------------------------------------
boolean ssUI_MathMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle (pInputNull));
}   // ssUI_MathMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_LoopMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle (pInputNull));
}   // ssUI_loopmain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_CmdsMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle (pInputNull));
}   // ssUI_CmdsMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_ssMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle (pInputNull));
}   // ssUI_ssMain
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_EvapiMain (void)
{
    return (ssUI_inOp_Line_GetParseHandle (pInputNull));
}   // ssUI_EvapiMain
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The "command line" interface supported by ssUI collects input from a source, parses it, and then
// handles it (performs the activities the words in the parsed input indicate the user wants done).
// -------------------------------------------------------------------------------------------------
// Ascii characters input are grouped into a NUL-terminated array of bytes, a legacy ssHL string.
// The local input buffer is used to store bytes from an independent and asynchronous signaler;
// the collection of bytes are analyzed and transformed into an input sequence in caller's buffer.
// -------------------------------------------------------------------------------------------------
// One of these two models is used to react to serial data as it arrives:
//      SSUI_ONEOF_SERIAL_RECEIVES_LINES        Arduino IDE Serial Monitor
//      SSUI_ONEOF_SERIAL_RECEIVES_BYTES        the fancy terminal emulation software of your choice
// Dependency: the differences are in behavior between Arduino Serial Monitor and terminal programs.
// The Arduino IDE requires a timeout to notice when the Arduino IDE has stopped emitting bytes.
// The Arduino IDE starts emitting when the user presses <enter> then stops without sending the CR.
// -------------------------------------------------------------------------------------------------
// #define DEBUG_BYTE_INPUT
#ifdef DEBUG_BYTE_INPUT
#define InputDebugSignal(d,x)   { mesa_uiOp_emit_1 (d); ss_uiOp_emit_Hex_2 (x); mesa_uiOp_emit_1 (d); }
#else   // not DEBUG_BYTE_INPUT
#define InputDebugSignal(d,x)
#endif  // DEBUG_BYTE_INPUT

#define TOO_BIG_FOR_CR_I        (INPUT_FIFO_ALLOC)        // lowest invalid index

// -------------------------------------------------------------------------------------------------
// This fifo collects bytes in N-byte chunks, without attention to values as they become available.
// The fifo only collects bytes when given Agency, and then only collects the bytes that have already
// been received by the serial byte device (of some form), and then only collects the bytes that
// there is room for in the fifo (letting serial queue them).  The sequence of bytes resulting from
// a human/machine emitting bytes varies depending on the behavior of the "terminal" software in use.
// -------------------------------------------------------------------------------------------------
// After a burst of bytes has been added by the Collector, the entire collection is searched for the
// value CR, when a CR is found or deduced, the fifo returns a subset of the collection: "a line".
// Changed definition: now a line contains all bytes up to, but excluding the CR, and a NUL.
// -------------------------------------------------------------------------------------------------
// The discussion below correlates the size of the burst to the rate at which bytes are perceived.
// -------------------------------------------------------------------------------------------------
boolean ssUI_inOp_Line_FromSerial (pAsciiA_t pUserBuff, int userMax_i)
{
    Ascii_t     aByte;
    int         AvailNow = 0;
    int         AvailOrig = 0;
    int         i;                          // character buffer manipulation uses indexing a lot
    int         CR_i;                       // if a CR is found, remember where it was found
    boolean     ms_EOL_Timeout;

#ifdef DEBUG_BYTE_INPUT
    pAsciiA_t   pUserBuff_Orig = pUserBuff; // pUserBuff moves around, remember the start point
#endif // DEBUG_BYTE_INPUT

    // ---------------------------------------------------------------------------------------------
    // Gating using Time is done to reduce the ask of "Available?" more often than necessary.
    // if the ms counter value has not changed since the last call, still in the same millisecond.
    // We are not going to use any more of the resources here until the gate opens at next boundary.
    // ---------------------------------------------------------------------------------------------
    if (mesa_gCtOf_1msFreeRunning == ms_last_AvailCt_check)
    {
        return (false);         // too little Time, don't care if CR has been received (latency)
    }
    ms_last_AvailCt_check = mesa_gCtOf_1msFreeRunning;

    // ---------------------------------------------------------------------------------------------
    // The rest of this function assumes that everything being done only occurs once a millisecond.
    // ---------------------------------------------------------------------------------------------
    AvailNow = AvailOrig = ss_uiOp_recv_avail_ct ();

#ifdef SSUI_ONEOF_SERIAL_RECEIVES_LINES
    if (AvailNow > 0)
    {
        ms_EOL_Timer = 10;
        ms_EOL_Timeout = false;
    }
    else
    {   // available now must be 0
        if (ms_EOL_Timer != 0)
        {
            InputDebugSignal ('t', ms_EOL_Timer);
            ms_EOL_Timer--;
            ms_EOL_Timeout = (ms_EOL_Timer == 0);       // the only place flag may become True
        }   // timeout exists
        else
        {   // Timer must be 0, align Timeout indicator
            ms_EOL_Timeout = false;
//            InputDebugSignal ('T', ms_EOL_Timer);
        }
    }   // nothing available
#endif // SSUI_ONEOF_SERIAL_RECEIVES_LINES

    CR_i = TOO_BIG_FOR_CR_I;
    while (AvailNow > 0)
    {
        aByte = ss_uiOp_recv_1 ();
        AvailNow--;

        // -----------------------------------------------------------------------------------------
        // Arduino Serial Monitor does not return until a CR is seen, then it keeps the CR secret.
        // normal terminal programs sends each character as it is typed (CR is seen and stored)
        // -----------------------------------------------------------------------------------------
        // store the byte in the buffer first (using count zero-based) then incr count (one-based)
        // -----------------------------------------------------------------------------------------
        ssUI_gCollected[ssUI_gInputCollected_i] = aByte;

#ifdef SSUI_ONEOF_SERIAL_RECEIVES_BYTES
        // BYTES model sees the CR and notes its location; LINES model does not see CR, forces and notes.
        if (aByte == Ascii_CR)
        {
            CR_i = ssUI_gInputCollected_i;      // location of CR in BYTES is detected
            InputDebugSignal ('!', Ascii_CR);
        }
#endif // SSUI_ONEOF_SERIAL_RECEIVES_BYTES

        ssUI_gInputCollected_i++;
        ssUI_gInputCollected_Ct++;

        InputDebugSignal ('(', aByte);
    }   // while AvailNow > 0

    // ---------------------------------------------------------------------------------------------
    // every byte available have been collected, that is, every byte since the last CR was seen.
    // if there were no bytes collected, it just means no input was received since the last CR.
    // ---------------------------------------------------------------------------------------------
    if (ssUI_gInputCollected_i == 0)
    {
        return (false);
    }

#ifdef SSUI_ONEOF_SERIAL_RECEIVES_LINES
    // above, after no data is received during a 1ms period and zero data has been collected
    // here, after no data is received during a 1ms period and data has been collected
    if (ms_EOL_Timeout)
    {
        // make the result of "CR received after 0-N chars" look like "0-N chars with CR received".
        // if circular? the location of the CR is not the same as the number of characters collected.
        // BYTES model sees the CR and notes its location; LINES model does not see CR, forces and notes.
        ssUI_gCollected[ssUI_gInputCollected_i] = Ascii_CR;
        CR_i = ssUI_gInputCollected_i;                      // location of CR in LINES is deduced
        ssUI_gInputCollected_i++;
        ssUI_gInputCollected_Ct++;                         //

        InputDebugSignal ('!', Ascii_CR);
    }
#endif // SSUI_ONEOF_SERIAL_RECEIVES_LINES

    // ---------------------------------------------------------------------------------------------
    // move collected[X] to userBuff[Y] up to and not including the CR
    // ---------------------------------------------------------------------------------------------
    // Note hidden logic: if CR_i is 0, nothing is copied from collected, Buff[0] gets Ascii_NUL.
    // Most important in addition, true is returned when just carriage return was entered into terminal.
    // ---------------------------------------------------------------------------------------------
    if (CR_i < TOO_BIG_FOR_CR_I)
    {
        if (CR_i > (userMax_i-1))
        {
            CR_i = userMax_i-1;
        }
        for (i = 0; i < CR_i; i++)
        {
            *pUserBuff = ssUI_gCollected[i];
            pUserBuff++;                                    // added one
        }   // for each byte up to and including CR
        *pUserBuff = Ascii_NUL;                     // <chars><NUL> for any N chars

        ssUI_gInputCollected_i = 0;
        ssUI_gInputCollected_Ct = 0;                         //

#ifdef DEBUG_BYTE_INPUT
        mesa_uiOp_emit_qAsciiA ("[");
        mesa_uiOp_emit_pAsciiA (pUserBuff_Orig);
        mesa_uiOp_emit_qAsciiA ("]");
        mesa_uiOp_emit_newline ();
#endif // DEBUG_BYTE_INPUT
        return (true);              // found at least one "line" in the input stream
    }   // for

    return (false);
}   // ssUI_inOp_Line_FromSerial
// -------------------------------------------------------------------------------------------------
void ssUI_AnnounceFailure (int len, int max_i)
{
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("Input ignored: string too short or too long to capture.");
    ss_uiOp_emit_Int_999 (len);
    ss_uiOp_emit_qAsciiA (" bytes input > ");
    ss_uiOp_emit_Int_999 (max_i+1);
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
boolean ssUI_inOp_Line_FromCmds (pAsciiA_t pUserBuff, int userBuff_max_i)
{
    pAsciiA_t    pCmdsStringPointer;
    int         length;

    // if next == Null, there are no more pointers in the array of pointers to command arrays of Ascii values,
    // OR               the end-of-commands string was found in the command list
    // as with Ascii arrays, the length is not known, instead the end is demarcated with a value.
    pCmdsStringPointer = ssUI_cmdsOp_nextCmdFromLinkActive (ssUI_control.Cmds_i);
    if (!pCmdsStringPointer)
    {
        return (false);
    }

    // the serial inputIfAny function also checks for maximum as the bytes are being stored
    length = ss_uiOp_Count_Aa(pCmdsStringPointer);
    if (length > userBuff_max_i)        // length may be 0 (first byte is NUL) or > 0 (isn't NUL)
    {
        ssUI_AnnounceFailure (length, userBuff_max_i);
        return (false);
    }
    else
    {
        // The pCmdsStringPointer is a pointer to an array, most likely in an array of pointers;
        // both or either of the pointer array or the string array(s) may be in non-writable memory.
        // This copy gets the string into writable memory.
        ss_uiOp_Duplicate_Aa (pUserBuff, pCmdsStringPointer);
        return (true);
    }   // valid length (0->userBuff_Max_i)
}   // ssUI_inOp_Line_FromCmds
// -------------------------------------------------------------------------------------------------
// The ssUI command line interface is implemented, within ssUI Input services, as "the command FSM".
// -------------------------------------------------------------------------------------------------
// This is the software path from "command FSM receives a collection of characters" to the "command
// FSM finds and matches tokens in the commands array" and "command FSM runs the Handler function.
// -------------------------------------------------------------------------------------------------
// There are (at least) two (2) input streams available as a source of input stream of characters.
// 1) through a terminal program such as the Arduino IDE Serial Monitor where users type directly
// 2) through a terminal program such as TermTerm where users type directly OR files may be emitted.
// 3) within ssUI, "commands" mode works through an array of input arrays of Ascii values, consuming one at a time.
// -------------------------------------------------------------------------------------------------
boolean ssUI_inOp_Line_GetParseHandle (pAsciiA_t pInputFromOutside)
{
    boolean         SawAnyInput;        // an FSM where an input string goes from "not SawAnyInput" to SawAnyInput
    eCmdFSM_Result_t CmdFSM_Result;

    boolean         crunched_semicolon; // an FSM where a crunched semicolon is treated as end-of-line
    int             command_start_i;    // points to first character in the command (line)
    int             command_finish_i;   // points to the NUL character in the command line
    pAsciiA_t       pResultAnnounce;

    AsciiA_t        localBuff[SSUI_UIBUFFER_ALLOC];
    pAsciiA_t       pInputBuff;

    if (pInputFromOutside != pInputNull)
    {
        SawAnyInput = true;
        pInputBuff = pInputFromOutside;
    }
    else
    {
        SawAnyInput = false;
        pInputBuff = localBuff;

        // ---------------------------------------------------------------------------------------------
        // if ssUI is 1) at least a partial owner and 2) currently the focus as a UI, consume input.
        // When the RO_owner is "anybody" and both UIs are active, a race condition is created.
        // This comment, with App UI and ssUI reversed, should be placed with other inOp_Line_From calls.
        // ---------------------------------------------------------------------------------------------
        if (RO_IS_ONEOF_Ascii_ROs (RO_owner_ssUI, RO_active_OneOfUIs_ss))
        {
            if (ssUI_inOp_Line_FromSerial (localBuff, SSUI_UIBUFFER_MAX_I))
            {
                SawAnyInput = true;
            }   // let serial input interrupt commands running by asking this first
            else
            {
                if (ssUI_control.CmdsAreRunning)
                {
                    if (ssUI_inOp_Line_FromCmds (localBuff, SSUI_UIBUFFER_MAX_I))
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
        }   // is ssUI the current owner of the Serial (or cmds processing faking it like Serial)

        // input parse and handle does nothing when nothing is provided to parse and handle
        if (!SawAnyInput)
        {
            return (THERES_NO_ACTION);
        }   // no input detected or generated...
    }   // a caller provided the input to be parsed and handled

//    ss_uiOp_emit_newline ();                      // get parse handle echoes all input
    ss_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pAsciiA (pInputBuff);
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_newline ();

    // input parse and handle consumes an "echo" line and the UI/parsers never get to see it
    if (ssUI_BufferStartsWith_echo (pInputBuff))
    {
        ss_uiOp_emit_pBanner (lfY, pInputBuff, lfN);
        return (IM_THE_TOWN_CRIER);
    }   // echo ...

    // ssUI and App UI both watch for the other's name in the buffer. If the other's name is seen,
    // AND in addition, the input consists only of app or ssui, the current UI owner is toggled.
    // If anything follows the other's name, App_Select_UI only gives the input string to the other.
    // In any case, the input string is fully consumed and the ssUI assumes a new prompt is needed.
    if (ssUI_BufferStartsWith_app (pInputBuff))
    {
        // The choice of method and implementation of switching between UIs is granted to the App.
        App_Select_UI (pInputBuff);

        // App UI chooser consumes "app" or "ssui"; current UI/parsers/handlers never see it.
        // Different from no input seen or echo seen, assume the "other" UI needs a fresh prompt.
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
    while (pInputBuff[command_start_i] != Ascii_NUL)
    {
        // skip over any leading spaces, maintain the relationship between first/last,
        // first and last are equal at the start of each loop that "extracts" a valid AsciiA_t.
        while (pInputBuff[command_start_i] == Ascii_Space)
        {
            command_finish_i++;
            command_start_i++;
        }   // while space

        // find the NUL Ascii terminator of the variable-length array of Ascii characters
        while (
                (pInputBuff[command_finish_i] != Ascii_NUL)
             &&
                (pInputBuff[command_finish_i] != Ascii_sColon)
              )
        {
            command_finish_i++;
        }

        // reached here because the while above found a NUL or a semicolon at command_finish_i;
        // when we find a semi-colon, make it look like an end-of-array for this command.
        // The key point is that "command_finish_i" includes the NUL byte at [command_finish_i]
        crunched_semicolon = false;
        if (pInputBuff[command_finish_i] == Ascii_sColon)
        {
            crunched_semicolon = true;
            pInputBuff[command_finish_i] = Ascii_NUL;
        }

        // in ssUI, commands are executed/processed when arrays of Ascii values are matched, during this MenuTick.
        // CmdMatch processes the string in the buffer with activities, but does not consume the characters.
        // CmdFSM_Result == handled            // command processor ate the input string and took all actions
        //      length is length of command in buffer, but we processed it as valid or error or defer
        // CmdFSM_Result != handled            //  means "there may be a TimeStamp, there is no command"
        //      length is length of non-command with at least one character
        if (ssUI_control.BeVerbose == YES_BE_VERBOSE)
        {
            ss_uiOp_emit_newline ();
            ss_uiOp_emit_Space (ssTEA_standard_fieldgap);
            ss_uiOp_emit_1 (Ascii_squareLBracket);
            ss_uiOp_emit_pAsciiA (&pInputBuff[command_start_i]);
            ss_uiOp_emit_1 (Ascii_squareRBracket);
            ss_uiOp_emit_newline ();
        }

        gToken_Ct = ssUI_tknOp_ParseAllTokens (&pInputBuff[command_start_i]);    // establish gToken_Ct
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("parsed   "));
#endif  // SSUI_OPTIN_DEBUG_TOKENS

        // if not in the root command zone, prepend the command zone on to the tokens
        gToken_Ct = ssUI_tknOp_Prepend_cmdZone ();              // if changed, update gToken_Ct
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("prepended cmd zone"));
#endif  // SSUI_OPTIN_DEBUG_TOKENS

        // transform correct and error forms of [-=], [=-], [+=], [=+], [-][=][-], [+][=][+]
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("before fix ="));
#endif  // SSUI_OPTIN_DEBUG_TOKENS
        gToken_Ct = ssUI_tknOp_MinusPlus_Fixup (gToken_Ct);     // if changed, update gToken_Ct
#ifdef SSUI_OPTIN_DEBUG_TOKENS
        ssUI_tknOp_Show_AllTkns (S("after fix  ="));
#endif  // SSUI_OPTIN_DEBUG_TOKENS

        // =========================================================================================
        if (gToken_Ct > 0)
        {
            CmdFSM_Result = ssUI_menuOp_CmdFSM ();
        }
        else
        {
            CmdFSM_Result = eCmdFSM_rNoTokens;
        }

        // ssUI is a UI so explanations of results are important sometimes
        if (ssUI_control.BeVerbose == YES_BE_VERBOSE)
        {
            switch (CmdFSM_Result)
            {
                case eCmdFSM_rNormal        : pResultAnnounce = pcMsg_CmdFSM_NormalResult;  break;
                case eCmdFSM_rNoInput       : pResultAnnounce = pcMsg_CmdFSM_FoundNoInput;  break;
                case eCmdFSM_rNoTokens      : pResultAnnounce = pcMsg_CmdFSM_FoundNoTokens; break;
                case eCmdFSM_rNoMatch       : pResultAnnounce = pcMsg_CmdFSM_FoundNoMatch;  break;
                default                     : pResultAnnounce = pAsciiANull;
            }   // switch
        }       // verbose
        else
        {       // reticent
            switch (CmdFSM_Result)
            {
                case eCmdFSM_rByHandler     : pResultAnnounce = pcMsg_CmdFSM_ByHandler;     break;
                case eCmdFSM_rUnresolved    : pResultAnnounce = pcMsg_CmdFSM_Unresolved;    break;
                case eCmdFSM_rTableFlaw     : pResultAnnounce = pcMsg_CmdFSM_TableFlaw;     break;
                default                     : pResultAnnounce = pAsciiANull;                break;
            }   // switch
        }   // reticent

        if (pResultAnnounce)
        {
            ss_uiOp_emit_pBanner (lfY, pResultAnnounce, lfY);
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
// App calls when input is "ssui xxx". The declaration imposed by ssUI is made in "ssUI_input_dcl.h"
// -------------------------------------------------------------------------------------------------
void  ssUI_HeyProcessThis (pAsciiA_t pAscii_After_ssUI)
{
    if (!ssUI_inOp_Line_GetParseHandle (pAscii_After_ssUI))
    {
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("some error found by ssUI");
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_pAsciiA (pAscii_After_ssUI);
        ss_uiOp_emit_newline ();
    }
}   // ssUI_HeyProcessThis
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferStartsWith_echo (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_e)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_c)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_h)      return (false);
    if (Ascii_toLower (pUserBuff[3]) != Ascii_o)      return (false);
    return (true);
}   // ssUI_BufferStartsWith_echo
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferStartsWith_app (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_a)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_p)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_p)      return (false);
    return (true);
}   // ssUI_BufferStartsWith_app
// -------------------------------------------------------------------------------------------------
// Given a buffer known to contain "app", does the buffer contain any letters after "app"?
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferOnlyHas_app (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[3]) != Ascii_NUL)    return (false);
    return (true);
}   // ssUI_BufferOnlyHas_app
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferStartsWith_ssui (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[0]) != Ascii_s)      return (false);
    if (Ascii_toLower (pUserBuff[1]) != Ascii_s)      return (false);
    if (Ascii_toLower (pUserBuff[2]) != Ascii_u)      return (false);
    if (Ascii_toLower (pUserBuff[3]) != Ascii_i)      return (false);
    return (true);
}   // ssUI_BufferStartsWith_ssui
// -------------------------------------------------------------------------------------------------
// Given a buffer known to contain "ssui", does the buffer contain any letters after "ssui"?
// -------------------------------------------------------------------------------------------------
boolean ssUI_BufferOnlyHas_ssui (pAsciiA_t pUserBuff)
{
    if (Ascii_toLower (pUserBuff[4]) != Ascii_NUL)    return (false);
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
    // ---------------------------------------------------------------------------------------------
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
// The tokenized arrays of Ascii values must be copied and the token pointers adjusted to the copied version.
// When the "loop on" command is encountered, the captured tokens and arrays of Ascii values are evaluated.
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
// tokenize a copy of an array, group non-separators together by finding math operations or spaces.
// Pointers stored in the caller's token array are pointing to static data in a "local" buffer.
// -------------------------------------------------------------------------------------------------
int     ssUI_tknOp_ParseAllTokens (pAsciiA_t pInput)
{
    // N token arrays of Ascii values, each maximum length (TknMax) with a separator, requires (N * (TknMax + 1)).
    static      AsciiA_t     inputCopy[SSUI_UIBUFFER_ALLOC];
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

    // because we don't want to input limit arrays of Ascii values to RAM locations by tokenizing the user's copy,
    //   Return pointers to our copy, where chunks of the string have been tokenized in place.
    // The copy string is allocated at compile time and the current value is accessible through
    // the "pointer" field in gTokens[n], but the tokens are only valid until parse occurs again.
    i = ssUI_AaOp_pP1_gets_pP2maxpadded (inputCopy, pInput, SSUI_UIBUFFER_ALLOC);

#ifdef SSUI_OPTIN_DEBUG_TOKENS
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_AsciiA (S("presented to tokenizer"), inputCopy);
    ss_uiOp_emit_newline ();
#endif  // SSUI_OPTIN_DEBUG_TOKENS

    p_copyNullByte = &inputCopy[i];
    p_startThisToken = &inputCopy[0];
    p_charInToken = p_startThisToken;

    // design choice, space characters must envelope characters meant to be considered a token.
    // this loop is editing the local copy of the input string, not moving arrays of Ascii values around.
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

        // Future: if found all characters in token because found a space, workaround for fixing
        // Future: input " timea ! =" (maintenanceTask: why not "!="?).
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


