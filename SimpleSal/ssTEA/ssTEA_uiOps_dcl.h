/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_uiOps_dcl.h    data type, prototype declarations: utilities supporting ssTEA services.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSTEA_UIOPS_DCL_H
#define __SSTEA_UIOPS_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
void        ss_uiOp_emit_Hex_1   (Bits8_t        thisByte);
void        ss_uiOp_emit_Hex_2   (Bits8_t        thisByte);
void        ss_uiOp_emit_Hex_999 (unsigned long  this32bits);

void        ss_uiOp_emit_ByteAsBits (Bits8_t     thisByte, int numBits, boolean MSBtoLSBorder);
#define     InMSBtoLSBorder (true)
#define     InLSBtoMSBorder (false)

void        ss_uiOp_emit_newline         (void);

void        ss_uiOp_emit_ThisRecursively (int numberOfTimes, Ascii_t ThisAscii);
void        ss_uiOp_emit_Zero            (int numberOfTimes);
void        ss_uiOp_emit_Dash            (int numberOfTimes);
void        ss_uiOp_emit_Space           (int numberOfTimes);
void        ss_uiOp_emit_Period          (int numberOfTimes);

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\SW Developer Tricks.note]    helpful display of any 32-bit value in hex
// -------------------------------------------------------------------------------------------------
#define     ss_uiOp_emit_Hex_32bits(anyThing)    CTRICK_Show_Hex_32bits((unsigned long) anyThing)

// -------------------------------------------------------------------------------------------------
// occasionally, it is necessary to examine or move the input or output arrays of Ascii characters.
// -------------------------------------------------------------------------------------------------
int         ss_uiOp_Count_Aa (pAscii_t  pAsciiA);
void        ss_uiOp_Duplicate_Aa (pAsciiA_t pDuplicateAa, pAsciiA_t pSourceAa);

void        ss_uiOp_do_emit_lbld_boolean      (pAsciiA_t pAA_label, boolean value);
#define     ss_uiOp_emit_lbld_boolean(pAA,V)  ss_uiOp_do_emit_lbld_boolean (S(pAA), V)
void        ss_uiOp_do_emit_lbld_hex          (pAsciiA_t pAA_label, unsigned long aValue);
#define     ss_uiOp_emit_lbld_hex(pAA,V)      ss_uiOp_do_emit_lbld_hex (S(pAA), V)
void        ss_uiOp_do_emit_lbld_hex8         (pAsciiA_t pAA_label, Bits8_t aValue);
#define     ss_uiOp_emit_lbld_hex8(pAA,V)     ss_uiOp_do_emit_lbld_hex8 (S(pAA), V)
void        ss_uiOp_do_emit_lbld_time         (pAsciiA_t pAA_label, ssT_pTime_t pTime);
#define     ss_uiOp_emit_lbld_time(pAA,V)     ss_uiOp_do_emit_lbld_time (S(pAA), V)

void        ss_uiOp_do_emit_lbld_AsciiA       (pAsciiA_t pAA_label, pAsciiA_t pValue);
#define     ss_uiOp_emit_lbld_AsciiA(pxx,px)  ss_uiOp_do_emit_lbld_AsciiA (S(pxx), S(px))

void        ss_uiOp_do_emit_lbld_int (pAsciiA_t pAA_label, unsigned long int Value);
#define     ss_uiOp_emit_lbld_int(pAA,V)      ss_uiOp_do_emit_lbld_int (S(pAA), V)

void        ss_uiOp_do_emit_plot_int (pAsciiA_t pAA_label, unsigned long int Value);
#define     ss_uiOp_plot_int(pAA,V)      ss_uiOp_do_emit_plot_int (S(pAA), V)

void        ss_uiOp_pBanner (boolean lfBefore, pAsciiA_t pAsciiA, boolean lfAter);
#define     lfN             false
#define     lfY             true

#define     ss_uiOp_qBanner(xlfB,xpqAA,xlfA)    ss_uiOp_pBanner (xlfB, S(xpqAA), xlfA)

#define OK_to_Blast_Ascii_Live() (                                                      \
                                  (ssTEA_control.Agency_pace == ssTEA_Agency_pace_period) \
                                  &&                                                    \
                                  (ssTEA_control.Agency_period >= 100)                   \
                                 )

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A working SimpleSal App might only communicate programmatically with software and never a user;
// it does not need to include all the Ascii character messages or the code to emit the messages.
// -------------------------------------------------------------------------------------------------
// A failing SimpleSal App can include the messages and the code to communicate reasons and causes.
// -------------------------------------------------------------------------------------------------
// These #defines makes software exist or not exist; variables gate Agency granting to the software.
// When software does not exist, the reference it does not make causes the message to not exist.
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// In order to provide information to the Human User of ssTEA and SimpleSal, allow the User to
// decide whether ssTEA and SimpleSal should emit a signal with a human-UI encoded message:
//  "a message carrying the name of the ssTEA function executing this ssTEA software right now"
// -------------------------------------------------------------------------------------------------
// This is a not-User-friendly feature of ssTEA; also it is not related to Time and/or Agency.
// On the other hand, the trick is a very useful debug tool for logging the software flow over Time.
// -------------------------------------------------------------------------------------------------
// Path descriptions are output encoded as "{name}", sequentially this appears as "{name}{name}".
// -------------------------------------------------------------------------------------------------
#ifdef SSTEA_OPTIN_SHOW_PATH
#define msg_ssTEA_Path()  if (ssTEA_control.Show_Path) { CTRICK_Show_FunctionsName ((unsigned char *) __func__); }
#else   // make macro have no content
#define msg_ssTEA_Path()
#endif  // SSTEA_OPTIN_SHOW_PATH

// -------------------------------------------------------------------------------------------------
// Cause descriptions are output in banner form without regard to whatever the App has displayed.
// They are also the debug tool built-in for the times when an error in state machine data is found.
// -------------------------------------------------------------------------------------------------
#ifdef SSTEA_OPTIN_SHOW_CAUSE
#define msg_ssTEA_Cause(msg)    if (ssTEA_control.Show_Cause)  { ss_uiOp_emit_qAsciiA (S(msg)); ss_uiOp_emit_newline (); }
#else   // make macro have no content
#define msg_ssTEA_Cause(msg)
#endif  // SSTEA_OPTIN_SHOW_CAUSE

// -------------------------------------------------------------------------------------------------
// Signal carriers and signals are complex; debug displays are large scale: everything or nothing.
// Which is to say this macro is rarely used; the signal API interaction always builds into the App.
// -------------------------------------------------------------------------------------------------
#ifdef SSTEA_OPTIN_SHOW_SIGNALS
#define msg_ssTEA_Signals(msg)  if (ssTEA_control.Show_Signals)  { ss_uiOp_emit_qAsciiA (S(msg)); }
#else   // make macro have no content
#define msg_ssTEA_Signals(msg)
#endif  // SSTEA_OPTIN_SHOW_SIGNALS

// -------------------------------------------------------------------------------------------------
// The raw facts are often useful clues when the software written on the page appears to be correct.
// Which is to say this macro is rarely used; the display of facts requires more manipulation.
// -------------------------------------------------------------------------------------------------
#ifdef SSTEA_OPTIN_SHOW_FACTS
#define msg_ssTEA_Facts(msg)    if (ssTEA_control.Show_Facts)  { ss_uiOp_emit_qAsciiA (S(msg)); ss_uiOp_emit_newline (); }
#else   // make macro have no content
#define msg_ssTEA_Facts(msg)
#endif  // SSTEA_OPTIN_SHOW_FACTS

#endif  // __SSTEA_UIOPS_DCL_H

