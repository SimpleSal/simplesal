/* -- SimpleSal: ssTEA, ssUI, and ssIO | Copyright (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * ss_RO_configuration.start    indicate choices for the default Mesa and ssUI Resource Ownership
 *
 * The App version of this file, "ss_RO_configuration.h", will change over Time under control of the App.
 * SimpleSal suggests the App version exist outside the SimpleSal repo managed object (.\SimpleSal\).
 * The contents of this file can of course be edited into any App file, the ownership is the App's.
 *
 **************************************************************************************************/
#ifndef __SS_RO_CONFIGURATION_H
#define __SS_RO_CONFIGURATION_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The ssTEA declarations in "mesas_min_dcl.h" start with the definition of Resource Ownership (RO).
// The #defines in this file refer to those RO enumerations and bit maps, but the #defines in this
// are not referenced until the allocation occurs of the fundamental RO control data structures.
// This software techinique pushes the varying choices out to the user, allocation knows all modes.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Efficient FSMs operate at full speed using the data available under the assumption it is valid.
// The App sets a permutation, the App and ssUI politely ask the question "May I?" before acting.
// -------------------------------------------------------------------------------------------------
// This complex #define sequence, like a lot of ssTEA and ssUI, is an example of a solution space.
// The author always imagines 3 software applications depending on the proposed design or software.
// -------------------------------------------------------------------------------------------------
// On one hand, the intent is an application-specific file most users don't ever edit. On the other,
// attempts to describe the purpose of specific permutations are App-specific and being local helps.
// -------------------------------------------------------------------------------------------------
// An embedded software program begins to execute at some point in Time; reality emerges over Time.
// The software engineering design challenges when working with shared resources is the point in
// Time the ownership becomes a reality (RO Init State) and changes during reality (RO FSM states).
// -------------------------------------------------------------------------------------------------
// "build-time" means the image builder of the embedded software causes the memory location in use
//     by the software to contain the value specified when the software is given Agency to execute.
//
// "run-time" means mesa, App, or SimpleSal software causes the memory location in use by the
//     software to contain the value specified when any of that software is given Agency to execute.
// -------------------------------------------------------------------------------------------------
// The philosophical contruct proposed is that Reality does not exist until software creates it.
// Both the mesa and ssuI assert own their perception of Reality at build-time or run-time.
// -------------------------------------------------------------------------------------------------
// The mesa is the initial specifier of "the current RO states", either at build- or run-time.
// IF ssUI is integrated with the App, "the current RO states" may be inherited from the mesa,
// OR the ssUI's Init State will set the permutation specified here without regard to the mesa,
// OR anyone may change, at run-time, "the current RO states" to any permutation, coherent or not.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// mesa Init State configuration: default owners of Resources
// -------------------------------------------------------------------------------------------------
// At the top level, choose a default owner of each resource.  This triggers a specific permutation.
// -------------------------------------------------------------------------------------------------
// #define     mesa_ONEOF_Default_LED_App
// #define     mesa_ONEOF_Default_LED_ssUI
#define     mesa_ONEOF_Default_LED_Anybody
// #define     mesa_ONEOF_Default_LED_Nobody
// -------------------------------------------------------------------------------------------------
// #define     mesa_ONEOF_Default_Ascii_App
// #define     mesa_ONEOF_Default_Ascii_ssUI
#define     mesa_ONEOF_Default_Ascii_Anybody
// #define     mesa_ONEOF_Default_Ascii_Nobody
// ------------------------------------------------------------------------------------------------
// #define     mesa_ONEOF_Default_LED_Matrix_App
// #define     mesa_ONEOF_Default_LED_Matrix_ssUI
#define     mesa_ONEOF_Default_LED_Matrix_Anybody
// #define     mesa_ONEOF_Default_LED_Matrix_Nobody
// -------------------------------------------------------------------------------------------------
// The selection to here determines the Init State values assigned to the RO FSM control variables.
// The values to change, in order to change the permutation, are the enumeration and the bit map.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// --------------
#ifdef mesa_ONEOF_Default_LED_App
#define mesa_ONEOF_Default_LED_RO_owner             RO_owner_App            // enumerated type
#define mesa_ONEOF_Default_LED_RO_active            RO_active_classic       // bit map
#endif  // mesa_ONEOF_Default_LED_App
// --------------
#ifdef mesa_ONEOF_Default_LED_ssUI
#define mesa_ONEOF_Default_LED_RO_owner             RO_owner_ssUI
#define mesa_ONEOF_Default_LED_RO_active            RO_active_classic
#endif  // mesa_ONEOF_Default_LED_ssUI
// --------------
#ifdef mesa_ONEOF_Default_LED_Anybody
#define mesa_ONEOF_Default_LED_RO_owner             RO_owner_Anybody
#define mesa_ONEOF_Default_LED_RO_active            RO_active_classic
#endif  // mesa_ONEOF_Default_LED_Anybody
// --------------
#ifdef mesa_ONEOF_Default_LED_Nobody
#define mesa_ONEOF_Default_LED_RO_owner             RO_owner_Nobody
#define mesa_ONEOF_Default_LED_RO_active            RO_active_none
#endif  // mesa_ONEOF_Default_LED_Nobody
// =================================================================================================
// --------------
#ifdef mesa_ONEOF_Default_Ascii_App
#define mesa_ONEOF_Default_Ascii_RO_owner           RO_owner_App
#define mesa_ONEOF_Default_Ascii_RO_active          RO_active_OneOfUIs_App  // ssUI blocked until...
#endif  // mesa_ONEOF_Default_Ascii_App
// --------------
#ifdef mesa_ONEOF_Default_Ascii_ssUI
#define mesa_ONEOF_Default_Ascii_RO_owner           RO_owner_ssUI
#define mesa_ONEOF_Default_Ascii_RO_active          RO_active_AllOfUIs  // ssUI does not block App
#endif  // mesa_ONEOF_Default_Ascii_ssUI
// --------------
#ifdef mesa_ONEOF_Default_Ascii_Anybody
#define mesa_ONEOF_Default_Ascii_RO_owner           RO_owner_Anybody
#define mesa_ONEOF_Default_Ascii_RO_active          RO_active_AllOfUIs
#endif  // mesa_ONEOF_Default_Ascii_Anybody
// --------------
#ifdef mesa_ONEOF_Default_Ascii_Nobody
#define mesa_ONEOF_Default_Ascii_RO_owner           RO_owner_Nobody
#define mesa_ONEOF_Default_Ascii_RO_active          RO_active_none
#endif  // mesa_ONEOF_Default_Ascii_Nobody
// =================================================================================================
// --------------
#ifdef mesa_ONEOF_Default_LED_Matrix_App
#define mesa_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_App
#define mesa_ONEOF_Default_LED_Matrix_RO_active     (RO_active_group0 | RO_active_group1 | RO_active_shape0 | RO_active_shape1)
#endif  // mesa_ONEOF_Default_LED_Matrix_App
// --------------
#ifdef mesa_ONEOF_Default_LED_Matrix_ssUI
#define mesa_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_ssUI
#define mesa_ONEOF_Default_LED_Matrix_RO_active     (RO_active_group0 | RO_active_group1 | RO_active_shape0 | RO_active_shape1)
#endif  // mesa_ONEOF_Default_LED_Matrix_ssUI
// --------------
#ifdef mesa_ONEOF_Default_LED_Matrix_Anybody
#define mesa_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_Anybody
#define mesa_ONEOF_Default_LED_Matrix_RO_active     (RO_active_group0 | RO_active_group1 | RO_active_shape0 | RO_active_shape1)
#endif  // mesa_ONEOF_Default_LED_Matrix_Anybody
// --------------
#ifdef mesa_ONEOF_Default_LED_Matrix_Nobody
#define mesa_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_Nobody
#define mesa_ONEOF_Default_LED_Matrix_RO_active     RO_active_none
#endif  // mesa_ONEOF_Default_LED_Matrix_Nobody
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// ssUI Init State configuration of default owners of Resources
// -------------------------------------------------------------------------------------------------
// #define     ssUI_ONEOF_Default_LED_App
// #define     ssUI_ONEOF_Default_LED_ssUI
// #define     ssUI_ONEOF_Default_LED_Anybody
// #define     ssUI_ONEOF_Default_LED_Nobody
#define     ssUI_ONEOF_Default_LED_MesaAsIs
// -------------------------------------------------------------------------------------------------
// #define     ssUI_ONEOF_Default_Ascii_App
// #define     ssUI_ONEOF_Default_Ascii_Anybody
// #define     ssUI_ONEOF_Default_Ascii_Nobody
#define     ssUI_ONEOF_Default_Ascii_ssUI
// #define     ssUI_ONEOF_Default_Ascii_MesaAsIs
// ------------------------------------------------------------------------------------------------
// #define     ssUI_ONEOF_Default_LED_Matrix_App
// #define     ssUI_ONEOF_Default_LED_Matrix_Anybody
// #define     ssUI_ONEOF_Default_LED_Matrix_Nobody
// #define     ssUI_ONEOF_Default_LED_Matrix_ssUI
#define     ssUI_ONEOF_Default_LED_Matrix_MesaAsIs
// -------------------------------------------------------------------------------------------------
// The selection to here establishes the Init State values assigned to resource FSM control variables
// by defining a set specific to the choice, based on the definition named here of who may have access.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// --------------
#ifdef ssUI_ONEOF_Default_LED_App
#define ssUI_ONEOF_Default_LED_RO_owner             RO_owner_App
#define ssUI_ONEOF_Default_LED_RO_active            RO_active_classic
#endif  // ssUI_ONEOF_Default_LED_App
// --------------
#ifdef ssUI_ONEOF_Default_LED_ssUI
#define ssUI_ONEOF_Default_LED_RO_owner             RO_owner_ssUI
#define ssUI_ONEOF_Default_LED_RO_active            RO_active_classic
#endif  // ssUI_ONEOF_Default_LED_ssUI
// --------------
#ifdef ssUI_ONEOF_Default_LED_Anybody
#define ssUI_ONEOF_Default_LED_RO_owner             RO_owner_Anybody
#define ssUI_ONEOF_Default_LED_RO_active            RO_active_classic
#endif  // ssUI_ONEOF_Default_LED_Anybody
// --------------
#ifdef ssUI_ONEOF_Default_LED_Nobody
#define ssUI_ONEOF_Default_LED_RO_owner             RO_owner_Nobody
#define ssUI_ONEOF_Default_LED_RO_active            RO_active_none
#endif  // ssUI_ONEOF_Default_LED_Nobody
// --------------
#ifdef ssUI_ONEOF_Default_LED_MesaAsIs
#define ssUI_ONEOF_Default_LED_RO_owner             mesa_ONEOF_Default_LED_RO_owner
#define ssUI_ONEOF_Default_LED_RO_active            mesa_ONEOF_Default_LED_RO_active
#endif  // ssUI_ONEOF_Default_LED_MesaAsIs
// =================================================================================================
// --------------
#ifdef ssUI_ONEOF_Default_Ascii_App
#define ssUI_ONEOF_Default_Ascii_RO_owner           RO_owner_App
#define ssUI_ONEOF_Default_Ascii_RO_active          RO_active_AllOfUIs      // mesa chooses App only
#endif  // ssUI_ONEOF_Default_Ascii_App
// --------------
#ifdef ssUI_ONEOF_Default_Ascii_ssUI
#define ssUI_ONEOF_Default_Ascii_RO_owner           RO_owner_ssUI
#define ssUI_ONEOF_Default_Ascii_RO_active          RO_active_AllOfUIs
#endif  // ssUI_ONEOF_Default_Ascii_ssUI
// --------------
#ifdef ssUI_ONEOF_Default_Ascii_Anybody
#define ssUI_ONEOF_Default_Ascii_RO_owner           RO_owner_Anybody
#define ssUI_ONEOF_Default_Ascii_RO_active          RO_active_AllOfUIs
#endif  // ssUI_ONEOF_Default_Ascii_Anybody
// --------------
#ifdef ssUI_ONEOF_Default_Ascii_Nobody
#define ssUI_ONEOF_Default_Ascii_RO_owner           RO_owner_Nobody
#define ssUI_ONEOF_Default_Ascii_RO_active          RO_active_none
#endif  // ssUI_ONEOF_Default_Ascii_Nobody
// --------------
#ifdef ssUI_ONEOF_Default_Ascii_MesaAsIs
#define ssUI_ONEOF_Default_Ascii_RO_owner           mesa_ONEOF_Default_Ascii_RO_owner
#define ssUI_ONEOF_Default_Ascii_RO_active          mesa_ONEOF_Default_Ascii_RO_active
#endif  // ssUI_ONEOF_Default_Ascuu_MesaAsIs
// =================================================================================================
// --------------
#ifdef ssUI_ONEOF_Default_LED_Matrix_App
#define ssUI_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_App
#define ssUI_ONEOF_Default_LED_Matrix_RO_active     (RO_active_group0 | RO_active_group1 | RO_active_shape0 | RO_active_shape1)
#endif  // ssUI_ONEOF_Default_LED_Matrix_App
// --------------
#ifdef ssUI_ONEOF_Default_LED_Matrix_ssUI
#define ssUI_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_ssUI
#define ssUI_ONEOF_Default_LED_Matrix_RO_active     (RO_active_group0 | RO_active_group1 | RO_active_shape0 | RO_active_shape1)
#endif  // ssUI_ONEOF_Default_LED_Matrix_ssUI
// --------------
#ifdef ssUI_ONEOF_Default_LED_Matrix_Anybody
#define ssUI_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_Anybody
#define ssUI_ONEOF_Default_LED_Matrix_RO_active     (RO_active_group0 | RO_active_group1 | RO_active_shape0 | RO_active_shape1)
#endif  // ssUI_ONEOF_Default_LED_Matrix_Anybody
// --------------
#ifdef ssUI_ONEOF_Default_LED_Matrix_Nobody
#define ssUI_ONEOF_Default_LED_Matrix_RO_owner      RO_owner_Nobody
#define ssUI_ONEOF_Default_LED_Matrix_RO_active     RO_active_none
#endif  // ssUI_ONEOF_Default_LED_Matrix_Nobody
// --------------
#ifdef ssUI_ONEOF_Default_LED_Matrix_MesaAsIs
#define ssUI_ONEOF_Default_LED_Matrix_RO_owner      mesa_ONEOF_Default_LED_Matrix_RO_owner
#define ssUI_ONEOF_Default_LED_Matrix_RO_active     mesa_ONEOF_Default_LED_Matrix_RO_active
#endif  // ssUI_ONEOF_Default_LED_Matrix_MesaAsIs

// -------------------------------------------------------------------------------------------------
// The selection to here has specified #define or macro names for each of "mesa_ONEOF_Default" and
// "ssUI_ONEOF_Default" owner/active pairs associated with Resources: the LED, Ascii, LED Matrix.
// Each of the macro names is referenced by build-time or run-time initialization and configuration.
// -------------------------------------------------------------------------------------------------
// The terms of co-existence may be simply asserted without including, or after #undef'ing defaults.
// -------------------------------------------------------------------------------------------------
#endif  // __SS_RO_CONFIGURATION_H
