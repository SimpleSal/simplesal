/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_data_dcl.h      data type, prototype declarations: co-existence with SimpleSal's UI.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_DATA_DCL_H
#define __SSUI_DATA_DCL_H

typedef struct ssUI_controlDesc_s
{
    boolean             Ascii_UI_owner;         // when ssUI, access to serial input/output for ssUI

    boolean             BeVerbose;              // extra output during normal operation

    int                 cmdZone_stack_i;        // the currently active ssUI command menu subsection

    boolean             CmdsAreRunning;         // global static used during cmds array processing
    int                 Cmds_i;                 // global static used during cmds array processing

    boolean             LoopIsActive;           // global static used during loop command processing
    int                 LoopOn_i;               // global static used during loop command processing
    int                 LoopOff_i;              // global static used during loop command processing

}  ssUI_controlDesc_t, *ssUI_pcontrolDesc_t;

typedef ssUI_controlDesc_t    ssUI_control_t;
typedef ssUI_pcontrolDesc_t   ssUI_pcontrol_t;

// -------------------------------------------------------------------------------------------------
// The mesa Resource Owner and RO FSMs
#define     ssUI_isNot_owner    (false)
#define     ssUI_is_owner       (true)

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// ssHL compile-time algorithm to compute the number of elements in an instantiated/allocated array.
// When does the developer not know how many elements are in an array in full view?  The algorithm
// using this array must be local, with embedded information about the number and size of elements.
//    int  array[5];   // it is obvious how many elements exist, as is the size of each element.
//
// An array containing pointers to arrays of characters is used by the ssUI input parser, varying by
// depending on build options and allowing user additions.  The ssUI parser FSM is built around it.
// The macros allow the algorithm to use the ssHL loop mechanism "for (i=0; i<= MyArray_MAX_I; i++)".
// -------------------------------------------------------------------------------------------------
#define ComputeArray_CT(Array,sizeof_Array_element_t) ((int) (sizeof(Array) / sizeof_Array_element_t))

// -------------------------------------------------------------------------------------------------
// These two macro examples can be created using ComputeArray_CT and used at compile time.
// define MyArray_CT     (ComputeArray_CT (MyArray, (size of sometype)))
// define MyArray_MAX_I  (MyArray_CT-1)     // compile-time limit used by all array reference loops.
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// to allow the UI to process AsciiCharacter[N] as ssHL strings, allow one extra byte for termination.
// This means the longest proper ssHL string that can be stored in a buffer of size N bytes is (N-1).
// Bounds checking on operations across the string's life use MAX_I: index range is 0 to <= MAX_I,
// or, iteration stops if (index > MAX_I). when there is an exception, there is a comment about it.
// All buffers within ssUI are this length, except for the serial input buffer itself (below).
// -------------------------------------------------------------------------------------------------
#define SSUI_BUFFER_ALLOC             (SSUI_UIBUFFER_ALLOC)
#define SSUI_BUFFER_MAX_I             (SSUI_BUFFER_ALLOC-1)

// -------------------------------------------------------------------------------------------------
// The idea of creating tokens or collections of tokens is provided in support of the ssUI Commands
// feature that allows a looping mechanism with a terminating condition (such as a Time occuring).
// Strings are tokenized in place, and theoretically there could be one token and the buffer full.
// But, place an arbitrary limit of the number of characters in an individual token, someone has to.
// This is because the feature for looping copies the Command and then continues running Commands;
// the copied Command is re-evaluated each time the terminated condition needs to be determined.
// The tokenized result is copied, not the original buffer, as this would require re-tokenizing.
// the tokenized buffer should be copied and arbitrary limits removed // bugbugbug
// -------------------------------------------------------------------------------------------------
#define SSUI_TOKENS_ALLOC           (6)
#define SSUI_TOKENS_MAX_I           (SSUI_TOKENS_ALLOC-1)

// =================================================================================================
// -------------------------------------------------------------------------------------------------
typedef struct ssUI_TokenDesc_s
{
    pAsciiA_t   pAsciiA;                // anywhere in memory the software has permission to read
    Bits8_t     valueCt;                // # of Ascii values before terminating Ascii NUL value
    Bits8_t     bananas;                // # of bananas discovered in the house through this array
}   ssUI_TokenDesc_t, *ssUI_pTokenDesc_t;

typedef ssUI_TokenDesc_t         ssUI_Token_t;
typedef ssUI_pTokenDesc_t               ssUI_pToken_t;

#define pTokenNull              ((ssUI_pToken_t) NULL)
#define NoBananas               (0)

// -------------------------------------------------------------------------------------------------
// the UI cmdZones are names for the individual FSMs implementing each cmdZone.
// while "in" a cmdZone, commands and parameters may be stated without the first parameter,
// which without this feature is always required to be the first parameter (<cmdZone> <action>).
// the order of enumerations is not relevant, but the order of token arrays with names describing
// must match and there has to be a token for each cmdZone named (not just each one actually used).
// -------------------------------------------------------------------------------------------------
typedef enum ssUI_cmdZone_e
{
    ssUI_root, ssUI_cmds, ssUI_loop, ssUI_math, ssUI_evapi, ssUI_ss
}  ssUI_cmdZone_t;

void            ssUI_cmdZone_InitStack (void);
boolean         ssUI_cmdZone_IsThisOne (pAsciiA_t pAscii_Cmd);
void            ssUI_cmdZone_StackPush (ssUI_cmdZone_t new_cmdZone);
void            ssUI_cmdZone_StackPop (void);

// -------------------------------------------------------------------------------------------------
// logging system accepts a #defined parameter which tells it whether to log it or not.
// This removes the need for #_ifdef sections littered throughout, for debug purposes.
// -------------------------------------------------------------------------------------------------
#define LOG_INPUT   true
#define LOG_STACK   false

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void        ssUI_Initialize      (void);

#endif  // __SSUI_DATA_DCL_H

