/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_include_dcl.h  ssTEA manages the order and dependencies of all data/code declarations.
 *
 **************************************************************************************************/
#ifndef __SS_INCLUDE_DCL_H
#define __SS_INCLUDE_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The language used to communicate with ssTEA is established by the declarations of data types.
// Data types allow for objects to be created and referenced by an App's algorithms and operations.
// A design goal for ssTEA is to create types as a basis for a language that can describe reality,
// in support of the design goal for SimpleSal to support a straightforward simulation of reality.
// -------------------------------------------------------------------------------------------------
#ifdef SS_OPTIN_INCLUDE_SSTEA
// -------------------------------------------------------------------------------------------------
// The OPTIN control mechanism to create a resolution for all references is a secret of the uiOps.
// -------------------------------------------------------------------------------------------------
#include "ssTEA\ssTEA_uiOps_dcl.h"  // a user may see information from deep within the functionality

// -------------------------------------------------------------------------------------------------
// The reason that the ssTEA data type declarations are not included here, as all the other includes
// are treated, is to allow configuration selections to influence types that are referenced by ssTEA.
// The data types file must be included before this file, which creates a space between the two
// includes for App types created that refer to ssTEA fundmental types and are referenced by ssTEA.
// -------------------------------------------------------------------------------------------------
// If the user forgets this step? The Aruino model is that all include files found in the directory
// are included; referenced in source code or not.  While this works, and proves that the ssTEA data
// types are only based on fundamental C compiler types, it is too magical for SimpleSal's taste.
// -------------------------------------------------------------------------------------------------
#include "ssTEA\ssT_TUmath_dcl.h"   // declaration of time unit math operation within a Time
#include "ssTEA\ssT_time_dcl.h"     // Time is described by a fancy type nobody every thought of.
#include "ssTEA\ssE_event_dcl.h"    // Events share commmon characteristics distinct from Agency
#include "ssTEA\ssA_agency_dcl.h"   // The core functionality of ssTEA's concept of Agency.
#include "ssTEA\ssTEA_api_dcl.h"    // The API supports App agency; this is the public face.
#include "ssTEA\ssA_fsms_dcl.h"     // API and Run FSMs interact with the App and also count actions.

#endif  // SS_OPTIN_INCLUDE_SSTEA

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// use of SimpleSal UI requires inclusion of ssTEA API for the UI to reference.
// The relationships and complexity of ssUI components are described by ssUI software.
// -------------------------------------------------------------------------------------------------
#ifdef SS_OPTIN_INCLUDE_SSUI
#include "ssUI\ssUI_data_dcl.h"
#include "ssUI\ssUI_input_dcl.h"
#include "ssUI\ssUI_utils_dcl.h"
#include "ssUI\ssUI_TUmath_dcl.h"

#include "ssUI\ssUI_evapi_dcl.h"              /* using ssTEA source files */

#include "ssUI\ssUI_cmds_dcl.h"
#include "ssUI\ssUI_menu_dcl.h"
#include "ssUI\ssUI_menucore_dcl.h"
#endif  // SS_OPTIN_INCLUDE_SSUI          ssTEA requirement met

#endif // __SS_INCLUDE_DCL_H

