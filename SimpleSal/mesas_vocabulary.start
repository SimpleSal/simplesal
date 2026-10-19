/* -- SimpleSal: ssTEA, ssUI, and ssIO | Copyright (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * mesas_vocabulary.start     Fundamental build and allocation choices are defined in source files.
 *
 * The App version of this file, "mesas_vocabulary.h", may change over Time under control of the App.
 * SimpleSal suggests the App version exist outside the SimpleSal repo managed object (.\SimpleSal\).
 * The contents of this file can of course be edited into any App file, the ownership is the App's.
 *
 **************************************************************************************************/
#ifndef __MESAS_VOCABULARY_H
#define __MESAS_VOCABULARY_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// SimpleSal has a software philosphy: everything that happens in life does NOT involve magic. All
// of the challenges to portability have technical solutions in the build environment and software.
// -------------------------------------------------------------------------------------------------
// Software portability across build environments (compilers, memory maps, allocations) may be
// managed at the very highest level by separating Arduino, Linux, Windows, RTOS, barebones, etc.
// -------------------------------------------------------------------------------------------------
// Each platform has a variety of memory types, control over what goes where is often in the C code.
// For example: removing "static" from the ReadOnly #define might move strings from Flash to RAM.
// (note: The strings arrive in Flash with the binary image; a copy is accessed by code in RAM.)
// -------------------------------------------------------------------------------------------------
// Software portability across Time from the same human brain requires some words remain constant.
// A HOST build environment that defines boolean and Null can be quite cranky about a re-define.
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Begin "mesas_vocabulary.*" content as released by SimpleSal.
// -------------------------------------------------------------------------------------------------
// SimpleSal was written to be portable across languages and Hosts; portability requires the use of
// words about abstract ideas, and, an implementation of the words in a completely hidden mechanism.
// -------------------------------------------------------------------------------------------------
// SimpleSal Apps that are "portable" adapt the configuration of their contents while being "built".
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Host OS Arduino as a choice activates many well-defined assumptions Arduino has about a sketch.
// The only specific dependency on a version of the Host Language (C): adoption of boolean and Null,
// which incorporates older compilers which only compile Kernigan & Ritchie C without the C++ magic.
// -------------------------------------------------------------------------------------------------
#define     HOST_ONEOF_hostOS_Is_Arduino
// #define     HOST_ONEOF_hostOS_Is_MicroPy
// #define     HOST_ONEOF_hostOS_Is_FreeRTOS
// #define     HOST_ONEOF_hostOS_Is_Linux

#define HOST_ONEOF_ssHL_IsArduinoCPP
// #define HOST_ONEOF_ssHL_IsDriverC
// #define HOST_ONEOF_ssHL_IsMicroPy

#ifdef HOST_ONEOF_ssHL_IsArduinoCPP
#define HOST_ONEOF_ssHL_defines_boolean
#define HOST_ONEOF_ssHL_defines_Null
#endif // HOST_ONEOF_ssHL_IsArduinoCPP

#ifdef HOST_ONEOF_ssHL_IsDriverC
// -------------------------------------------------------------------------------------------------
// The implementation of the software may be more narrow and C-specific for Device Driver Software
// because the tricks applied to make support of Arduino configurations appear magic are not simple.
// -------------------------------------------------------------------------------------------------
#define HOST_ONEOF_ssHL_defines_boolean
#define HOST_ONEOF_ssHL_defines_Null
#endif // HOST_ONEOF_ssHL_IsDriverC

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef HOST_ONEOF_ssHL_Arduino_MicroPy
// -------------------------------------------------------------------------------------------------
// MicroPython is considered a Host and an ssHL because it provides a broader context than Arduino.
// Integration of MicroPython provides access to higher-level signaling with a specific build model,
// while little about ssTEA as a Timekeeper should affect MicroPython code invoked by ssTEA Events.
// -------------------------------------------------------------------------------------------------
// The first challenge is to conform to the organizational model of microPyThon itself, the second
// is writing an interface into ssTEA that calls ssHL C++ code from microPy and to MicroPy from C++.
// MicroPython implementations conform to extensive standards that C++ languages also conform to,
// the challenge is to complete all of the steps required, the first step being memory conformance.
// -------------------------------------------------------------------------------------------------
#endif // HOST_ONEOF_ssHL_Arduino_MicroPy

// -------------------------------------------------------------------------------------------------
// Properties of memory locations: read only, read/write, executable. Read Only need not be in ROM?
// Properties are specified not as a type of a variable but as an "attribute" the type possesses.
// Writable memories that are not accessible directly have an API for use (e.g. flash pages/banks).
// -------------------------------------------------------------------------------------------------
// An apparently uncorrectable mistake with the original K&R C language was overloading "static".
// Compiler docs say whether a write reference to a type with a static attribute is an error, the
// use of static as an attribute on memory also says "this memory always exists, within its scope".
// ssTEA's convention is that ReadOnly is just that, and only permanently allocated data is static.
// -------------------------------------------------------------------------------------------------
#ifdef HOST_ONEOF_ssHL_IsArduinoCPP
#define   ReadOnly  static
#endif  // HOST_ONEOF_ssHL_IsArduinoCPP

#ifdef HOST_ONEOF_ssHL_IsMicroPy
#define   ReadOnly
#endif  // HOST_ONEOF_ssHL_IsMicroPy

#endif  // __MESAS_VOCABULARY_H
