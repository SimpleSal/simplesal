/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_data_def.h     data and software definitions: top-level allocation/management by ssTEA Api.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSTEA_DATA_DEF_H
#define __SSTEA_DATA_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The ssTEA Time and Agency FSMs operate continuously over Time and therefore need permanent data.
// Initialize the data with a coherent set of choices for Time and pace of Agency, states of both.
// -------------------------------------------------------------------------------------------------
ssTEA_control_t ssTEA_control
#ifdef SSTEA_ONEOF_CONTROL_INIT_BUILDTIME
=
{
    ssTEA_state_stopped,        // time state: stopped until application starts it
    ssTEA_TimeBase_reality,     // time basis: based on reality until application changes it

    ssTEA_state_stopped,        // agency state: stopped until App starts it
    ssTEA_Agency_Pace_period,   // ssTEA_Agency_Root is called as often as possible, or as desired
    1,                          // period has no meaning if pace is planck; valid period is 1-999ms.

    false,                      // emit ssTEA function's name on entry to ssTEA function (PATH)
    false,                      // emit extended messages explaining ssTEA error codes returned (CAUSE)
    false,                      // emit the API contents carried (Signals)
    false                       // emit the information behind the reason (Notes)
}
#endif // SSTEA_ONEOF_CONTROL_INIT_BUILDTIME
;

#endif  // __SSTEA_DATA_DEF_H

