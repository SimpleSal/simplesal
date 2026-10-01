/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * mesas_configuration.h        ssTEA and SimpleSal build choices and allocation choices are possible.
 *                              This SimpleSal file becomes "mesas_configuration.h" in a new App project.
 *                              The App project file's revision over Time is under control of the App.
 *
 **************************************************************************************************/
#ifndef __MESAS_CONFIGURATION_H
#define __MESAS_CONFIGURATION_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Naming convention: "applies to each mesa equally" (mesas_) or "applies to this mesa" (mesa_).
// Another way to look at it: all mesas_ meet requirements of SimpleSal, each mesa_ does as it wants.
// -------------------------------------------------------------------------------------------------
// This SimpleSal App runs on UnoR4Wifo or Due, the specific implementation for this build is picked:
// e.g., the configuration for Due does not include the code/data space used by the UnoR4 LED Matrix.
// -------------------------------------------------------------------------------------------------
#include "mesa_SelectThisBuild.h"

// =================================================================================================
// Configuration section: Mesa software provided by "mesa_min" or replaced by App version of same.
// -------------------------------------------------------------------------------------------------
// A configuration specification only has to include those OPTIN features that are used by the App.
// Letter-writers may complain that is should be "_OPTION_", but OPTIN is action so it works better.
// -------------------------------------------------------------------------------------------------
// The "mesas_min_*.h" files provide some minimal implementations of Random, Ascii and LED functions.
// Ascii and LED resources are used by ssTEA and ssUI but not required for functionality (the plan).
// Unlike Ascii and LED, random has two levels: random numbers Y/N, if Y whose implementation to use.
// -------------------------------------------------------------------------------------------------
#define     MESA_OPTIN_RANDOM_NUMBERS

// -------------------------------------------------------------------------------------------------
// The file "mesa_min_dcl.h" has the definition of the interface/services expected by SimpleSal.
// To replace the definition by ssTEA (functions and data) in "mesas_min_def.h" or elsewhere:
//    The choice must be made to "opt out" of the ssTEA implementation
//       then reproduce the base functionality expected of "mesas_min_*.h" at least.
//       The place to start: opt out and then duplicate the ssTEA implementation right next to it.
//
// For random, after having "opted in" Random Numbers:
//    Then ALSO choose to use or reproduce the base functionality expected of "mesa_min_*.h".
// -------------------------------------------------------------------------------------------------
// #define MESA_MIN_OPTOUT_RANDOM_NUMBERS
// #define MESA_MIN_OPTOUT_LED
// #define MESA_MIN_OPTOUT_ASCII

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// define which functionality within the MIN definition should be included, before including this.
// -------------------------------------------------------------------------------------------------
#include "mesas_min_dcl.h"

// =================================================================================================
// ------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\mesa\Mesa Pin Basics.note] any mesa has external pins for device connections
// ------------------------------------------------------------------------------------------------
typedef enum PinMode_e
{
// ------------------------------------------------------------------------------------------------
// MrBlinky is a complex Mesa with multiple boards, sensors, LEDs; also, based on an older Mesa.
// ------------------------------------------------------------------------------------------------
#ifdef MESA_ONEOF_mesa_IsMrBlinky
    mesaPinMode_INPUT_NOPULLUP = INPUT,
    mesaPinMode_INPUT_PULLEDUP = INPUT_PULLUP,
    mesaPinMode_OUTPUT = OUTPUT
#endif // MESA_ONEOF_mesa_IsMrBlinky
// ------------------------------------------------------------------------------------------------
// Once a variation has been isolated, the proper values must be named by Mesa and kept up to date
// ------------------------------------------------------------------------------------------------
#ifdef MESA_ONEOF_mesa_IsUnoR4wifi
    mesaPinMode_INPUT_NOPULLUP = INPUT,
    mesaPinMode_INPUT_PULLEDUP = INPUT_PULLUP,
    mesaPinMode_OUTPUT = OUTPUT
#endif // MESA_ONEOF_mesa_IsUnoR4wifi
// ------------------------------------------------------------------------------------------------
// A made-up example: hiding the variation in the Mesa's value behind the shared abtract symbol.
// ------------------------------------------------------------------------------------------------
#ifdef MESA_ONEOF_mesa_UNO_R7
    mesaPinMode_INPUT_NOPULLUP  = PINMODE_INPUT_FLOATING,
    mesaPinMode_INPUT_PULLEDUP  = PINMODE_INPUT_PULLEDUP,
    mesaPinMode_OUTPUT          = PINMODE_OUTPUT
#endif // MESA_ONEOF_mesa_UNO_R7
}   PinMode_t;

#endif  // __MESAS_CONFIGURATION_H
