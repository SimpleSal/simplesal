/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_include_def.h  ssTEA manages the order and dependencies of all data/code definitions.
 *
 **************************************************************************************************/
#ifndef __SS_INCLUDE_DEF_H
#define __SS_INCLUDE_DEF_H

// =================================================================================================
#ifdef SS_OPTIN_INCLUDE_SSTEA
// -------------------------------------------------------------------------------------------------
// The OPTIN control mechanism to create a resolution for all references is a secret of the uiOps.
// -------------------------------------------------------------------------------------------------
#include "ssTEA\ssTEA_uiOps_def.h"  // a user may see information from deep within functionality    /* using ssTEA source files */

// -------------------------------------------------------------------------------------------------
// The language used to communicate with ssTEA is implemented by the code and data defined by ssTEA.
// -------------------------------------------------------------------------------------------------
// Data types are words or phrases used to construct a higher level language in the definitions;
// functions allow you to use the words and phrases to do something useful with Event(s) over Time;
// data variables describe the location, or value, that will be useful when the Event Occurs;
// and then, finally, the API(s) responds to all Agency Api FSM and Agency Run FSM signals.
// -------------------------------------------------------------------------------------------------
#include "ssTEA\ssTEA_data_def.h"   // almost all SimpleSal data allocation occurs here             /* using ssTEA source files */
#include "ssTEA\ssT_TUmath_def.h"   // definition of time unit math operation with Time operators   /* using ssTEA source files */
#include "ssTEA\ssT_time_def.h"     // Time variables are manipulated by Time functions             /* using ssTEA source files */
#include "ssTEA\ssE_event_def.h"    // Event software is packaged to participate in Agency          /* using ssTEA source files */
#include "ssTEA\ssTEA_api_def.h"    // the API refers to everything declared so far; data and code. /* using ssTEA source files */
#include "ssTEA\ssA_fsms_def.h"     // Agency Api FSM and Run FSM interactions, some with the App.  /* using ssTEA source files */

// -------------------------------------------------------------------------------------------------
// The public interface to ssTEA precedes this, this shy interface is backstage magic-making stuff.
// -------------------------------------------------------------------------------------------------
#include "ssTEA\ssA_agency_def.h"   // Event management uses Time variables to run Event handlers   /* using ssTEA source files */

#endif  // SS_OPTIN_INCLUDE_SSTEA

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// use of SimpleSal UI requires inclusion of ssTEA API for the UI to reference.
// The relationships and complexity of the ssUI software are described elsewhere, within ssUI.
// -------------------------------------------------------------------------------------------------
#ifdef SS_OPTIN_INCLUDE_SSUI
#include "ssUI\ssUI_data_def.h"                                        /* using ssUI source files */
#include "ssUI\ssUI_input_def.h"                                       /* using ssUI source files */
#include "ssUI\ssUI_utils_def.h"                                       /* using ssUI source files */
#include "ssUI\ssUI_TUmath_def.h"                                      /* using ssUI source files */

// -------------------------------------------------------------------------------------------------
// ssUI allocates the data space required to 1) manage an Event, 2) be manageable as an Event; and,
// c) carry the signal data from the signaler to the Event, when the App's Event is granted Agency.
// -------------------------------------------------------------------------------------------------
// "manage an Event" includes references to ssTEA data types and structures using the ssTEA API.
// "be manageable" includes human names of Events that can be entered by an ssUI human user.
// "carry the signal" includes over time in the form of history and the instant Agency is granted.
// -------------------------------------------------------------------------------------------------
// Memory allocation occurs, within a sequence of included source files, when this inclusion occurs.
// -------------------------------------------------------------------------------------------------
#include "ssUI\ssUI_evapi_def.h"                                       /* using ssUI source files */

// -------------------------------------------------------------------------------------------------
// The definition of the content used by the ssUI "command line parser" FSM completes the UI view.
// The Command Handler FSM compares user input (as words) to arrays of Ascii values (as words) in a table.
// -------------------------------------------------------------------------------------------------
#include "ssUI\ssUI_cmds_def.h"                                        /* using ssUI source files */
#include "ssUI\ssUI_menu_def.h"                                        /* using ssUI source files */
#include "ssUI\ssUI_menucore_def.h"                                    /* using ssUI source files */

#endif // SS_OPTIN_INCLUDE_SSUI

#endif // __SS_INCLUDE_DEF_H

