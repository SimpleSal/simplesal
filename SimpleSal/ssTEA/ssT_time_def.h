/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssT_time_def.h       data and software definitions: ssTEA subcomponent Time
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SST_TIME_DEF_H
#define __SST_TIME_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As A Signal.note     start here to understand why math is necessary
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As An Arrow.note    the simplest way to do simple math using a Time
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As An FSM.note      the complex way to do complex math using a Time
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As A Signal.note                                 The source of Time
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As A Signal.note                 The source of knowledge about Time
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As A Signal.note              Time knowledge from perceived signals
// -------------------------------------------------------------------------------------------------
typedef struct ssTEA_Time_ctsDesc_s
{
    TimeUnitsBig_t      us_LoopStart_Last;  // previously captured Time when entered loop
    TimeUnitsBig_t      us_LoopFinish_Last; // previously captured Time when exited loop
    TimeUnitsBig_t      us_LoopStart_Curr;  // current value of Time when entered loop
    TimeUnitsBig_t      us_LoopFinish_Curr; // current value of Time when exited loop

    TimeUnitsBig_t      ms_LoopStart_Last;  // previously captured Time when entered loop
    TimeUnitsBig_t      ms_LoopFinish_Last; // previously captured Time when exited loop
    TimeUnitsBig_t      ms_LoopStart_Curr;  // current value of Time when entered loop
    TimeUnitsBig_t      ms_LoopFinish_Curr; // current value of Time when exited loop

    TimeUnitsBig_t      us_RootStart_Last;  // previously captured Time when entered ssTEA_Agency_Root
    TimeUnitsBig_t      us_RootFinish_Last; // previously captured Time when exited ssTEA_Agency_Root
    TimeUnitsBig_t      us_RootStart_Curr;  // current value of Time when entered ssTEA_Agency_Root
    TimeUnitsBig_t      us_RootFinish_Curr; // current value of Time when exited ssTEA_Agency_Root

    TimeUnitsBig_t      ms_RootStart_Last;  // previously captured Time when entered ssTEA_Agency_Root
    TimeUnitsBig_t      ms_RootFinish_Last; // previously captured Time when exited ssTEA_Agency_Root
    TimeUnitsBig_t      ms_RootStart_Curr;  // current value of Time when entered ssTEA_Agency_Root
    TimeUnitsBig_t      ms_RootFinish_Curr; // current value of Time when exited ssTEA_Agency_Root

    TimeUnitsBig_t      us_Total_Host;      // sum(all Time outside loop: interrupt background?
    TimeUnitsBig_t      us_Total_Loop;      // sum(all loop outside ssTEA_Agency_Root)
    TimeUnitsBig_t      us_Total_ssT;       // sum(inside ssTEA_Agency_Root)

    TimeUnitsBig_t      us_SampleCt_Host;   // Ave=(Total/sampleCt).  if (Ct==0) doing math, fault.
    TimeUnitsBig_t      us_SampleCt_Loop;   // Ave=(Total/sampleCt)
    TimeUnitsBig_t      us_SampleCt_ssT;    // Ave=(Total/sampleCt)

} ssTEA_Time_ctsDesc_t, *ssTEA_pTime_ctsDesc_t;

typedef         ssTEA_Time_ctsDesc_t    ssTEA_Time_cts_t;
typedef         ssTEA_pTime_ctsDesc_t   ssTEA_pTime_cts_t;
#define         ssT_pTime_ctsNull       ((ssTEA_pTime_cts_t) NULL)

void            ssTEA_Time_MathZeroAll ();

// -------------------------------------------------------------------------------------------------
ssTEA_Time_cts_t       ssTEA_Time_cts;

void  ssTEA_Time_MathZeroAll ()
{
    ssTEA_Time_cts.us_LoopStart_Last  = 0;
    ssTEA_Time_cts.us_LoopFinish_Last = 0;
    ssTEA_Time_cts.us_LoopStart_Curr  = 0;
    ssTEA_Time_cts.us_LoopFinish_Curr = 0;
    ssTEA_Time_cts.us_RootStart_Last  = 0;
    ssTEA_Time_cts.us_RootFinish_Last = 0;
    ssTEA_Time_cts.us_RootStart_Curr  = 0;
    ssTEA_Time_cts.us_RootFinish_Curr = 0;

    ssTEA_Time_cts.ms_LoopStart_Last  = 0;
    ssTEA_Time_cts.ms_LoopFinish_Last = 0;
    ssTEA_Time_cts.ms_LoopStart_Curr  = 0;
    ssTEA_Time_cts.ms_LoopFinish_Curr = 0;
    ssTEA_Time_cts.ms_RootStart_Last  = 0;
    ssTEA_Time_cts.ms_RootFinish_Last = 0;
    ssTEA_Time_cts.ms_RootStart_Curr  = 0;
    ssTEA_Time_cts.ms_LoopFinish_Curr = 0;

    ssTEA_Time_cts.us_Total_Host     = 0;
    ssTEA_Time_cts.us_Total_Loop     = 0;
    ssTEA_Time_cts.us_Total_ssT      = 0;
    ssTEA_Time_cts.us_SampleCt_Host  = 0;
    ssTEA_Time_cts.us_SampleCt_Loop  = 0;
    ssTEA_Time_cts.us_SampleCt_ssT   = 0;
}   // ssTEA_Time_MathZeroAll

// -------------------------------------------------------------------------------------------------
// Loop1 and Loop2 are points in Time when the App has granted Agency in Arduino's "loop" function.
// Loop1toLoop2 is the computation of the number of micros that occured betweet Loop1 and Loop2.
// This result accounts for all software consuming Time: ssTEA, Events, interrupts, Host, App.
// -------------------------------------------------------------------------------------------------
TimeUnitsBig_t      ssT_us_gLoop1toLoop2;
TimeUnitsBig_t      ssT_ms_gLoop1toLoop2;

// =================================================================================================
void ssTEA_Time_MathEntry (void)
{
    ssTEA_Time_cts.us_LoopStart_Curr = ssT_rawCt_usNow ();  // access hardware, at a cost bugbugbug
    ssTEA_Time_cts.ms_LoopStart_Curr = ssT_rawCt_msNow (ssTEA_Time_cts.us_LoopStart_Curr);

    ssT_us_gLoop1toLoop2 = ssT_rawCt_SubPastFromNow (ssTEA_Time_cts.us_LoopStart_Curr,
                                                     ssTEA_Time_cts.us_LoopStart_Last);
    ssT_ms_gLoop1toLoop2 = ssT_rawCt_SubPastFromNow (ssTEA_Time_cts.ms_LoopStart_Curr,
                                                     ssTEA_Time_cts.ms_LoopStart_Last);

    ssTEA_Time_cts.us_Total_Host += // when started this loop,    when left last iteration of loop
        ssT_rawCt_SubPastFromNow (ssTEA_Time_cts.us_LoopStart_Curr,
                                  ssTEA_Time_cts.us_LoopFinish_Last);
    ssTEA_Time_cts.us_SampleCt_Host++;

}   // ssTEA_Time_MathEntry (void)
// ------------------------------------------------------------------------------------------------
void ssTEA_Time_MathExit (void)
{
    // This instance of Agency is finishing now, we'll use this to measure Host load of total
    // The costly read of hardware to find the microseconds counter allows accurate info on Events
    ssTEA_Time_cts.us_LoopFinish_Curr = ssT_rawCt_usNow ();
    ssTEA_Time_cts.ms_LoopFinish_Curr = ssT_rawCt_msNow (ssTEA_Time_cts.us_LoopFinish_Curr);

    ssTEA_Time_cts.us_Total_Loop +=    // when finished this loop (now),  when started this loop
        ssT_rawCt_SubPastFromNow  (ssTEA_Time_cts.us_LoopFinish_Curr,
                                   ssTEA_Time_cts.us_LoopStart_Curr);
    ssTEA_Time_cts.us_SampleCt_Loop++;

    // current is the new last, we are about to give up Agency to the Host, ready for next Entry
    ssTEA_Time_cts.us_LoopStart_Last = ssTEA_Time_cts.us_LoopStart_Curr;
    ssTEA_Time_cts.ms_LoopStart_Last = ssTEA_Time_cts.ms_LoopStart_Curr;
    ssTEA_Time_cts.us_LoopFinish_Last = ssTEA_Time_cts.us_LoopFinish_Curr;
    ssTEA_Time_cts.ms_LoopFinish_Last = ssTEA_Time_cts.ms_LoopFinish_Curr;

}   // ssTEA_Time_MathExit

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A Time Stamp versus A Time Variable
//
// The capitalized words Stamp and Variable reference the definition of Stamp (a series of characters
// describing a Moment in Time) and the definition of Variable (a data structure with integer variables).
//
// Time stamp, or Time variable, without capitalization, references an instance of a Stamp or Variable.
//
// The definition of a valid Time Stamp's content can be found in the parsing routines in this file.
// The parsing routines convert that human representation into a computer representation in a Variable.
//
// The idea of stamping a ticket with a Time is used here as a metaphor, the specification of a
// Time describes one Moment in Time and no other.  The meaning of the Time Stamp is a function
// of the application: the Time at which the gates close; the Time at which the LED goes on.
// The value stamped on the ticket is equivalently contained in a stamp or a variable instance.
//
// Time variables are defined by ssTEA, are based on a defined data type, can be manipulated using
// defined operations.  This is exactly the same as can be done with integer data type variables.
//
// The most fundamental to programming with Time is the value in ssTEA's CurrentTime stamp.
//
// Within ssUI, a variety Time variables are available that may be referrred to by the App software.
// These include constant-defined Time variables useful in embedded software such as ssT_Time_1ms.
//
// Time variables may be used as operands, as well as the destination of the result of the operation.
// ssT provides time variables that may be used as operands in operations (e.g. TimeA, TimeCurr).
//
// The Operators supported are Assignment ('='), Add ('+'), and Subtract ('-').
//    for example:  TimeF = TimeCurr + 10s          Variable = variable + string
//
// The destination memory location for the result of an operation is called "the Lvalue", for fun.
// Time Stamps may not be Lvalue assignment destinations: i.e., "math 10.4ns = TimeA" looks invalid,
//     while "math timeA = 10.4ns" looks like a software assignment statement of a value to timeA.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Time variables may be used in algorithms and given a meaning, just as ints and chars are.
//       TimeWeAteCake = 0;
//       while (TimeCurrentDate < DateOfMyBirth)    waitForMyBirthday ();
//       if (TimeCurr == timeForCake)               TimeWeAteCake = TimeCurr;
// The difference is that a Time variable is invented; an int and char are inherent in the language.
// -------------------------------------------------------------------------------------------------
// ssT holds the knowledge of everything ssTEA about these variables, in this Time sequence.
//  <bigbang>   <build>   <current>   <ephemeral>
// -------------------------------------------------------------------------------------------------
// In order to perceive and measure, there must be a zero (or pretty close), current Time is derived
// from hardware counters such as "microseconds since hardware reset to 0".  These are in all chips,
// in part because heat management requires the knowledge of a True Time based on a cool processor.
// Accumulation of passed Time is done using deltas in microseconds since the last time the read
// and math was performed; allows for 71 minutes of microseconds to go by without losing data.
// -------------------------------------------------------------------------------------------------
// The BigBang value represents the first moment in the boot sequence that ssT received Agency.
// The hardware mechanism will not be zero, as all the code to boot and run the Host has used Time.
// Purists are free to assign zero to the BigBang value as the first thing done after ssTEA init.
//  <bigbang> = <zero-based time, from hardware, after reset>
// -------------------------------------------------------------------------------------------------
            ssT_Time_t  ssT_gTime_BigBang;
ReadOnly    ssT_pTime_t ssT_gpTime_BigBang = &ssT_gTime_BigBang;

// -------------------------------------------------------------------------------------------------
// Time of build contains the time this software image running now was built on another mesa.
// -------------------------------------------------------------------------------------------------
            ssT_Time_t  ssT_gTime_OfBuild;
ReadOnly    ssT_pTime_t  ssT_gpTime_Built = &ssT_gTime_OfBuild;

// -------------------------------------------------------------------------------------------------
// The mechanism used to determine the actual value of current Time is a service provided by ssT.
// Current is meant to be the source of the Time to use in any operations or information displays.
// -------------------------------------------------------------------------------------------------
//  set Time:
//    <current> = <bigbang + build>
//      <current> matches the mesa's current Time as microseconds are added each update Time
//  update Time:
//    <current> = <calculated time from hardware, in ssTEA_Agency_Root when granted Agency by the Host>
//                 OR
//                <manipulated time from software, in ssTEA_Agency_Root when granted Agency by the Host>
// -------------------------------------------------------------------------------------------------
            ssT_Time_t  ssT_gTime_Current;
ReadOnly    ssT_pTime_t ssT_gpTime_Current = &ssT_gTime_Current;

// -------------------------------------------------------------------------------------------------
// "Ephemeral" == "transient" == "everchanging" == "existing only while it is being referenced".
// Any software may use it for a math operation, and so it the Time variable must never be ReadOnly.
// An alternative is a Time Variable on the stack, which only exists while it is being referenced.
// ssTEA believes software A (using the ephemeral time variable) is not interrupted by software B
// due to hardware or software (using ...).  Either event may use ssT math functions, and use the
// ephemeral variable while running.  The rule is it only has your value while in a User function,
// but it is not going to change while you are using it in a series of straightforward ssHL code.
// ssTEA itself does not use the ephemeral Time to hold intermediate results during math operations.
// -------------------------------------------------------------------------------------------------
            ssT_Time_t  ssT_gTime_Ephemeral;
ReadOnly    ssT_pTime_t ssT_gpTime_Ephemeral = &ssT_gTime_Ephemeral;

// =================================================================================================
// A Time moved from below, inside the PREDEFINED_TIMES_AVAILABLE ifdef, becomes available to code.
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t   ssT_gTime_Zero =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    true,   0,  0,  0,  0,  0,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_Zero = &ssT_gTime_Zero;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_10sc =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0, 10,  0,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_10sc = &ssT_Time_10sc;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_1sc =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  1,  0,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_1sc = &ssT_Time_1sc;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_5sc =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  5,  0,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_5sc = &ssT_Time_5sc;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_3sc =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  3,  0,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_3sc = &ssT_Time_3sc;
// -------------------------------------------------------------------------------------------------
#ifdef  PREDEFINED_TIMES_AVAILABLE
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_1ms =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  0,  1,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_1ms = &ssT_Time_1ms;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_10ms =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  0, 10,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_10ms = &ssT_Time_10ms;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_100ms =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  0,100,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_100ms = &ssT_Time_100ms;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_1us =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  0,  0,  1,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_1us = &ssT_Time_1us;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t  ssT_Time_10us =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  0,  0, 10,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_10us = &ssT_Time_10us;
// -------------------------------------------------------------------------------------------------
ReadOnly    ssT_Time_t ssT_Time_100us =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0,  0,  0,100,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly    ssT_pTime_t ssT_gpTime_100us = &ssT_Time_100us;
#endif // PREDEFINED_TIMES_AVAILABLE

// =================================================================================================
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssT_Initialize (void)
{
    ss_macSNR_t  returnSNR;

    msg_ssTEA_Path ();

    // "I think, therefore I am....That's very good, Bomb." see the movie "Dark Star".
    returnSNR = ssT_TheBigBang ();
    if (returnSNR == ss_macSNR_OK)
    {   // now that Time exists, formalize all pointers/counters/allocations to init state
        returnSNR = ssT_InitData_RunTime ();
    }

    return (returnSNR);
}   // ssT_Initialize
// -------------------------------------------------------------------------------------------------
// ssTEA is based on the idea that any mesa will have a hardware true-Time counter starting from 0.
// Time forms the basis for an FSM being able to perform math with Time variables, which enables
// an FSM to execute through state transitions over Time.  Every Time is relative to the hardware.
// In order to keep the "simple" in "ssTEA", this counter is not allowed to reset during Run Time;
// in fact the counter may be so simplistic that it only resets when power goes from off to on.
// -------------------------------------------------------------------------------------------------
// The Time Unit "eon" measures the period of Time between the start of one Big Bang to the next.
// The recorded Time for the Big Bang is the completion of the event: Time began or existed, the
// BigBang began to occur at a specific moment in Time, moments of Time passed, the BigBang ended.
// App software was then granted Agency, giving it permission to think about the Current Time.
// bugbugbug note that the first use of "ss time stats" includes the microseconds BEFORE setup runs.
// -------------------------------------------------------------------------------------------------
// A simulated historical record of Time is created by the initialization state of the ssTEA FSM,
// one instance of information about "the last time ssTEA executed" is ready when loop first runs.
// -------------------------------------------------------------------------------------------------
// This is the first access to the hardware counter, during initialization of the application.
// Fundamental to ssTEA's publication of a Time: no subsequent access receives a smaller value.
// The fact that a 32-bit counter of microseconds can wrap back around to zero is handled by ssTEA.
// An interesting fact is the first read includes the time before ssTEA within setup gets to check.
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssT_TheBigBang (void)
{
    unsigned long int BigBangEndTime_us;

    msg_ssTEA_Path ();

    // ---------------------------------------------------------------------------------------------
    // Time starts now, with the value of zero in all of the fields of the Great Counter in the Sky.
    // This is ssTEA's representation of the current Time, not the value of the hardware right now.
    // ---------------------------------------------------------------------------------------------
    ssT_mathOp_P1getsZero (ssT_gpTime_Current);

    // ---------------------------------------------------------------------------------------------
    // many microseconds have passed since the caller asked for The Big Bang and we got here.
    // This software is  meant to manufacture a historical record that describes the sequence.
    // ---------------------------------------------------------------------------------------------
    // technically Current Time is not allowed to be zero (too late) so catch up to make nonzero,
    // also initializing the shortcut QuickTime used to optimize the calculations of Current Time.
    // BigBangStartTime is recorded when the hardware counter first signals that one us has passed.
    // BigBangEndTime is recorded when software firsts reads the us counter in setup (any read).
    // ---------------------------------------------------------------------------------------------
    // The only ssT option related to this is the display from "ss time stats"; the first use may
    // include (EndTime-StartTime) and so will report hundreds of thousands of microseconds, once.
    // ssT's use of hardware microsecond Time in ssT is always as a delta from the last value read.
    // ---------------------------------------------------------------------------------------------
    ssT_timeOp_InitQuickTimeForMath ();
// #define FIRST_READ_FROM_THE_CHIP
#ifdef FIRST_READ_FROM_THE_CHIP
    BigBangEndTime_us = ssT_rawCt_usNow ();     // how many microseconds elapsed since power on?
#else   // FIRST_READ_FROM_THE_CHIP
    BigBangEndTime_us = 1;                      // at minimum, one microsecond has elapsed
#endif  // FIRST_READ_FROM_THE_CHIP
    ssT_timeOp_Add_usToTimeCurr (BigBangEndTime_us);
    ssT_mathOp_P1getsP2 (ssT_gpTime_BigBang, ssT_gpTime_Current);

    // ---------------------------------------------------------------------------------------------
    // The statistical/counting features use an FSM that compares Now to Then; Init State runs here.
    // The Init state is that all of the numbers, relative to each other, look like a loop completed.
    // The assumption that TheBigBang always correlates to a reset of the HW counter is fundamental.
    // ---------------------------------------------------------------------------------------------
    ssTEA_Time_cts.us_LoopStart_Last = BigBangEndTime_us;      // bugbugbug
    ssTEA_Time_cts.us_LoopFinish_Last = BigBangEndTime_us+1;

    ssTEA_Time_cts.ms_LoopStart_Last = ssT_rawCt_msNow (ssTEA_Time_cts.us_LoopStart_Last);
    ssTEA_Time_cts.ms_LoopFinish_Last = ssT_rawCt_msNow (ssTEA_Time_cts.us_LoopFinish_Last);

    return (ss_macSNR_OK);
}   // ssT_TheBigBang
// -------------------------------------------------------------------------------------------------
ss_macSNR_t ssT_InitData_RunTime (void)
{
    ss_macSNR_t  returnSNR = ss_macSNR_OK;

    msg_ssTEA_Path ();

    ssTEA_Time_MathZeroAll ();

    return (returnSNR);
}   // ssT_InitData_RunTime
// -------------------------------------------------------------------------------------------------
// A math result with a negative EON is acceptable as a relative difference between two Times, but
// is not acceptable as a Time.  Note: acceptable means "valid for use as a Time value in ssTEA".
// -------------------------------------------------------------------------------------------------
void    ssT_timeOp_Check_MathPolicy (ssT_pTime_t pTime, int EON)
{
    if (EON == THIS_EON)
    {
        if (pTime->Valid)
        {
#ifdef SST_OPTIN_TIMEMATH_OKAY_ANNOUNCE
            ss_uiOp_qBanner (lfY, "ssT Math Operation: no error in math was signaled", lfY);
#endif // SST_OPTIN_TIMEMATH_OKAY_ANNOUNCE
        }
        else
        {
#ifdef SST_OPTIN_TIMEMATH_FAIL_ANNOUNCE
            ss_uiOp_qBanner (lfY, "ssT Math Operation: a Time Unit value ERROR in math was signaled", lfY);
#endif // SST_OPTIN_TIMEMATH_FAIL_ANNOUNCE
#ifdef SST_OPTIN_TIMEMATH_FAIL_ZERO_IT
            ssT_mathOp_P1getsZero (pTime);
#endif // SST_OPTIN_TIMEMATH_FAIL_ZERO_IT
        }
        return;
    }   // EON range was not exceeded in negative or positive direction

#ifdef SST_OPTIN_TIMEMATH_FAIL_ANNOUNCE
    ss_uiOp_qBanner (lfY, "ssT Math Operation: a Time Edge ERROR in math was signaled", lfY);
#endif // SST_OPTIN_TIMEMATH_FAIL_ANNOUNCE
#ifdef SST_OPTIN_TIMEMATH_FAIL_ZERO_IT      // bugbugbug propagate this action
    ssT_mathOp_P1getsZero (pTime);
#endif // SST_OPTIN_TIMEMATH_FAIL_ZERO_IT
}   //  ssT_timeOp_Check_MathPolicy
// -------------------------------------------------------------------------------------------------
boolean ssT_stampOp_TimeOfBuild_AsciiToTime  (pAsciiA_t pBuff)
{
    int             i;

    i = ss_uiOp_Count_Aa(S(__TIME__));
    i = ssUI_AaOp_pP1_gets_pP2max (pBuff, S(__TIME__), i);

    pBuff[i++] = Ascii_h;
    pBuff[i++] = Ascii_r;
    pBuff[i] = Ascii_NUL;

    for (i=i-1; i >= 0; i--)   {  if (pBuff[i] == Ascii_Colon)  pBuff[i] = Ascii_Period;  }

    return (ssT_stampOp_AsciiToBinary (pBuff, ssT_gpTime_Built));
}   // ssT_stampOp_TimeOfBuild_AsciiToTime
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
void    ssT_mathOp_P1compareP2_Compressed (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
}   // ssT_mathOp_P1compareP2_Compressed
// -------------------------------------------------------------------------------------------------
void ssT_mathOp_P1getsP1_Compressed (ssT_pTime_t pLvalue)
{
}   // ssT_mathOp_P1getsP1_Compressed
#endif  // SST_OPTIN_TIME_COMPRESSED_TUS
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef  SST_ONEOF_TIME_IS_AN_ARROW
// -------------------------------------------------------------------------------------------------
// This addition with carry is applied to each Time Unit within a Time, using the hierarchy of Units.
// The software is in a separate function here to enable a view of the processor instructions for
// this math operation, independent of the instructions involved in the math for multiple Time Units.
// -------------------------------------------------------------------------------------------------
boolean PositiveAddCarried (pTimeUnitValue_t     pTUValue,
                            TimeUnitValue_t      fieldMAX,
                            pTimeUnitValue_t     pParentTUValue)
{
    boolean DidCarryOrNot = false;

    // do we need to carry due to overflow of this Time Unit's maximum value (of 0..MAX)
    if (*pTUValue > fieldMAX)               // example: start with: 0 days, 23 hours, 59 minutes
    {                                       // example: add         0 days,  0 hours,  1 minute
        *pTUValue = *pTUValue - (fieldMAX+1);   // example: 60 minutes as one hour, minutes now 0
        (*pParentTUValue)++;                // example: the 60 minutes is now in the hours as 1
        DidCarryOrNot = true;               // example: signal that parent TU was changed by carry
    }   // excess Time for this Unit        // example: hours is now 24, when TUs added, will carry?

    return (DidCarryOrNot);
}   // PositiveAddCarried
// -------------------------------------------------------------------------------------------------
// if a result of subtraction is less than zero, one unit must be borrowed from the less precise
// field next in the hierarchy of fields.  when Y is a subunit of the unit X, the claim "1 of X
// is equivalent to N of Y", means if the Y value after a subtraction is negative, we need
// to borrow 1 of X, to account for the fieldMAX units added to get to positive field value range.
// -------------------------------------------------------------------------------------------------
void    SubBorrow_linear (pTimeUnitValue_t *pTUValue,
                          TimeUnitValue_t   fieldMAX,
                          pTimeUnitValue_t *pParentTUValue)
{
}   // SubBorrow_linear
// -------------------------------------------------------------------------------------------------
#define AddAsSigned(field)   pLvalue->field = pLvalue->field + pOperand1->field

// -------------------------------------------------------------------------------------------------
void  sssT_mathRoot_P1getsP1plusP2 (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1)
{
    TimeUnitValue_t         EON = THIS_EON;
    TimeUnitValueBig_t      SumOfAllFields = 0;

    pLvalue->Valid = true;

    // ---------------------------------------------------------------------------------------------
#ifdef SST_OPTIN_TIME_NSECS_TU
    AddAsSigned (nsecs);
    if (
         (PositiveAddCarried (&pLvalue->nsecs, SST_MAX_NSECS, &pLvalue->usecs)) &&
         (PositiveAddCarried (&pLvalue->usecs, SST_MAX_USECS, &pLvalue->msecs)) &&
         (PositiveAddCarried (&pLvalue->msecs, SST_MAX_MSECS, &pLvalue->secs )) &&
         (PositiveAddCarried (&pLvalue->secs,  SST_MAX_SECS,  &pLvalue->mins )) &&
         (PositiveAddCarried (&pLvalue->mins,  SST_MAX_MINS,  &pLvalue->hours)) &&
         (PositiveAddCarried (&pLvalue->hours, SST_MAX_HOURS, &pLvalue->days )) &&
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;;
        }
    SumOfAllFields += pLvalue->nsecs;   // what if: nsecs added 1 to 999, carried 1 to usecs, now 0
#endif  // SST_OPTIN_TIME_NSECS_TU
    // ---------------------------------------------------------------------------------------------
    AddAsSigned (usecs);
    if (
         (PositiveAddCarried (&pLvalue->usecs, SST_MAX_USECS, &pLvalue->msecs)) &&
         (PositiveAddCarried (&pLvalue->msecs, SST_MAX_MSECS, &pLvalue->secs )) &&
         (PositiveAddCarried (&pLvalue->secs,  SST_MAX_SECS,  &pLvalue->mins )) &&
         (PositiveAddCarried (&pLvalue->mins,  SST_MAX_MINS,  &pLvalue->hours)) &&
         (PositiveAddCarried (&pLvalue->hours, SST_MAX_HOURS, &pLvalue->days )) &&
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;;
        }
    SumOfAllFields += pLvalue->usecs;   // what if: sum added 0 but carry now changes sum to nonzero
    // ---------------------------------------------------------------------------------------------
    AddAsSigned (msecs);
    if (
         (PositiveAddCarried (&pLvalue->msecs, SST_MAX_MSECS, &pLvalue->secs )) &&
         (PositiveAddCarried (&pLvalue->secs,  SST_MAX_SECS,  &pLvalue->mins )) &&
         (PositiveAddCarried (&pLvalue->mins,  SST_MAX_MINS,  &pLvalue->hours)) &&
         (PositiveAddCarried (&pLvalue->hours, SST_MAX_HOURS, &pLvalue->days )) &&
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;;
        }
    SumOfAllFields += pLvalue->msecs;   // what if: each result causes carry until EON gets a carry
    // ---------------------------------------------------------------------------------------------
    AddAsSigned (secs);
    if (
         (PositiveAddCarried (&pLvalue->secs,  SST_MAX_SECS,  &pLvalue->mins )) &&
         (PositiveAddCarried (&pLvalue->mins,  SST_MAX_MINS,  &pLvalue->hours)) &&
         (PositiveAddCarried (&pLvalue->hours, SST_MAX_HOURS, &pLvalue->days )) &&
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;;
        }
    SumOfAllFields += pLvalue->secs;
    // ---------------------------------------------------------------------------------------------
    AddAsSigned (mins);
    if (
         (PositiveAddCarried (&pLvalue->mins,  SST_MAX_MINS,  &pLvalue->hours)) &&
         (PositiveAddCarried (&pLvalue->hours, SST_MAX_HOURS, &pLvalue->days )) &&
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;;
        }
    SumOfAllFields += pLvalue->mins;
    // ---------------------------------------------------------------------------------------------
    AddAsSigned (hours);
    if (
         (PositiveAddCarried (&pLvalue->hours, SST_MAX_HOURS, &pLvalue->days )) &&
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;;
        }
    SumOfAllFields += pLvalue->hours;
    // ---------------------------------------------------------------------------------------------
    AddAsSigned (days);
    if (
         (PositiveAddCarried (&pLvalue->days,  SST_MAX_DAYS,  &EON           ))
       )
        {
            pLvalue->Valid = false;     // Time has reached the End of Time and then went beyond it
        }
    SumOfAllFields += pLvalue->days;

    if (pLvalue->Valid)
    {
        pLvalue->Positive = true;
        pLvalue->Zero = (SumOfAllFields == 0);
        pLvalue->plancks = ssT_rawCt_NextPlanck ();
    }
    else
    {
        pLvalue->Positive = true;
        pLvalue->Zero = true;
    }
}   // sssT_mathRoot_P1getsP1plusP2


void  ssT_mathOp_P1getsP1operP2_is_an_arrow (ssT_pTime_t pLvalue, ssT_pTime_t pOperand, boolean Operation)
{
    if (Operation == AddOperation)
    {
        sssT_mathRoot_P1getsP1plusP2 (pLvalue, pOperand);
    }
    else
    {
        ss_uiOp_qBanner (lfY, "subtraction of one Time from another Time is not supported", lfY);
    }
}   // ssT_mathOp_P1getsP1operP2_is_an_arrow
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void    ssT_mathOp_P1compareP2_WithLogic (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
// bugbugbug    COMPARE_P1andP2 (&mathOpResult, pOperand1, pOperand2);
// bugbugbug
// bugbugbug    if (!mathOpResult.Valid)
// bugbugbug    {   // if result is invalid, Time1 is after Time2 regardless of value of either
// bugbugbug        DEBUG_IFS_Banner ("  math result not valid: Op1 not before Op2");
// bugbugbug        return (false);                     // something wrong with Time1/Time2 or result
// bugbugbug    }   // not valid result
// bugbugbug
// bugbugbug    if (pOperand1->Positive)
// bugbugbug    {
// bugbugbug        if (!pOperand2->Positive)
// bugbugbug        {   // operand 2 is negative
// bugbugbug            DEBUG_IFS_Banner ("  QED: pos Op1 and neg Op2: Op1 after Op2");
// bugbugbug             // posOp1 follows any negOp2
// bugbugbug            return (false);                                     // P1 is not before P2
// bugbugbug        }
// bugbugbug        DEBUG_IFS_Banner ("  resolved by pos and pos logical");
// bugbugbug        // pos P1 compared pos P2 with zero result means P1 IS NOT before P2
// bugbugbug        if (mathOpResult.Zero)        return (false);       // P1 is not before P2
// bugbugbug        // pos P1 compared pos P2 with positive result means P1 IS NOT before P2
// bugbugbug        if (mathOpResult.Positive)    return (false);       // P1 is after (more pos than) P2
// bugbugbug        // pos P1 compared pos P2 with negative result means P1 IS before P2
// bugbugbug        return (true);                                      // P1 is less positive than P2
// bugbugbug    }   // operand 1 is positive
// bugbugbug    else
// bugbugbug    {   // operand 1 is negative
// bugbugbug        if (pOperand2->Positive)
// bugbugbug        {
// bugbugbug            DEBUG_IFS_Banner ("  QED: neg Op1 with pos Op2: Op1 before Op2");
// bugbugbug            return (true);                                  // P1 is before P2
// bugbugbug        }   // Op1 negative, Op2 positive
// bugbugbug        DEBUG_IFS_Banner ("  resolved by comparing zero/positive");
// bugbugbug        if (mathOpResult.Zero)        return (false);       // P1 is not before P2
// bugbugbug        if (mathOpResult.Positive)    return (true);        // P1 is before (more neg than) P2
// bugbugbug        // neg P1 compared pos P2 with zero result means P1 IS NOT before P2
// bugbugbug        if (mathOpResult.Zero)        return (false);       // P1 is not before P2
// bugbugbug        // neg P1 compared pos P2 with positive result means P1 IS NOT before P2
// bugbugbug        if (mathOpResult.Positive)    return (true);        // P1 is before (less pos than) P2
// bugbugbug        // pos P1 compared pos P2 with negative result means P1 IS before P2
// bugbugbug        return (false);                                     // P1 is not before (less neg than) P2
// bugbugbug    }   // operand 1 is negative
// bugbugbug
}   // ssT_mathOp_P1compareP2_WithLogic

#endif  // SST_ONEOF_TIME_IS_AN_ARROW

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// The sole and true specification of ssTEA Time Stamp Notation is here, for any instance or use.
// If this code does not implement it, the description in "a document" has no meaning or effect.
//
// In this API, a string with a proper specification is a valid Time Stamp; when there is any problem
// with the characters used or the value is improper, the result is called an invalid Time Stamp.
//
// Given a valid Time Stamp, populate fields of a Time Descriptor with values extracted from the string.
// Given a non-NULL pointer to a time descriptor structure, and a valid Time Stamp, populate, return true.
// Given a NULL pointer to a time descriptor structure, and a valid specification, return true.
//
// A true return:  the string is a valid Time Stamp; a non-NULL pointer to contains the value.
// A false return: the string was not a valid Time Stamp, the Time Descriptor pointed to is unchanged.
//
// The + or - sign (positivity/negativity) of the value may be specified as the first character only.
//
// A Time subunit indicator (dy/hr/mn/sc/ms/us/ns) may be appended; it specifies the Time subunit of
// the first field in the Stamp.  For example, "1.3.10dy" expresses "1 day, 3 hours and 10 minutes",
// while "1.3.10mn" expresses "1 minute, 3 milliseconds, 10 microseconds".
//
// A delimiter is any value outside the Ascii character range of
//          Ascii_A..Ascii_Z, or Ascii_a..Ascii_z, or Ascii_0..Ascii_9.
// A space is a separator and is treated the same as finding the NUL byte at the end of the string.
// -------------------------------------------------------------------------------------------------
#define ITSA_POSITIVE (1)
#define ITSA_NEGATIVE (-1)
boolean ssT_stampOp_AsciiToBinary (pAsciiA_t  pAsciiValue, ssT_pTime_t pTime)
{
    ssT_Time_t          workTime;                           // a temporary Time Descriptor
    AsciiA_t            workTimeStamp[TimeStamp_ALLOC];       // a temporary character buffer

    pAsciiA_t           pStartPassHere;

    ss_TimeUnit_t       TimeUnitTracker;
    ss_TimeUnit_t       parsedTimeUnit;
    // TimeUnits are signed numbers; all TimeUnits have the same sign value for each signed number.
    TimeUnitsBig_t      ValueThisTimeUnit = 0;
    // A sum of all the TimeUnit values will be the same sign; if total is zero, Time is zero.
    TimeUnitsBig_t      SumOfTimeUnits = 0;
    int                 signSignal;

    int                 i;
    int                 pass;
#define SEARCH_PASS 1
#define PARSE_PASS  2

    // process case of no string at all, and, empty string pointed to by valid pointer
    if ((!pAsciiValue) || (!(*pAsciiValue)))
    {
        msg_ssTEA_Cause ("  No string variable or no string in AsciiA_t");
        return (false);
    }

    // Caller's TimeStamp may be a constant, can't parse in place; copy to local stack-based copy
    i = 0;
    while (true)
    {
        if (i > TimeStamp_MAX_I)
        {
            msg_ssTEA_Cause ("  data error");
            return (false);
        }
        if (*pAsciiValue == Ascii_NUL)
        {  // the value at that position in the array is Ascii_NUL, length is valid, TimeStamp array valid
            workTimeStamp[i] = Ascii_NUL;
            break;  // out of while
        }
        if (!Ascii_isAlpha (*pAsciiValue))
        {
            if (!Ascii_isDigit (*pAsciiValue))
            {
                if (!ssT_stampOp_IsSeparator (*pAsciiValue))
                {
                    ss_uiOp_emit_qAsciiA ("rejected nonalpha, nondigit, nonseparator");
                    ss_uiOp_emit_newline ();
                    return (false);
                }   // not is separator by ssT's definition
                else
                {   // is a separator by ssT's definition
                     workTimeStamp[i] = *pAsciiValue;
                }
            }   // not is digit
            else
            {   // is a digit, use it directly
                workTimeStamp[i] = *pAsciiValue;
            }   //
        }   // not is Alpha
        else
        {   // is alpha, make sure lowercase
            workTimeStamp[i] = Ascii_toLower (*pAsciiValue);
        }
        i++;
        pAsciiValue++;
    }
    // new target of search and parse: the local proper Ascii array with validated lowercased copy
    pAsciiValue = &workTimeStamp[0];

#ifdef SST_BLDIN_DEBUG_TIMESTAMP_INPUT
    ss_uiOp_emit_qAsciiA ("accepted for parsing");
    ss_uiOp_emit_1 (Ascii_EncapL);
    ss_uiOp_emit_pAsciiA (pAsciiValue);
    ss_uiOp_emit_1 (Ascii_EncapR);
    ss_uiOp_emit_newline ();
#endif  // SST_BLDIN_DEBUG_TIMESTAMP_INPUT
    // create a clean copy of a Time Variable, clear default value of Valid until complete parsing
    ssT_mathOp_P1getsZero (&workTime);
    workTime.Valid = false;

    // process case of first character being a positivity indicator (+ or -):
    // characters that match are consumed, characters that do not match are processed below
    workTime.Positive = true;           // default for case with no +/-
    switch (*pAsciiValue)
    {
        case Ascii_Plus  :
            workTime.Positive = true;
            signSignal = ITSA_POSITIVE;
            pAsciiValue++;
            break;
        case Ascii_Minus :
            workTime.Positive = false;
            signSignal = ITSA_NEGATIVE;
            pAsciiValue++;
            break;
        default          : /* allow any other character, leave pointer to character */
            signSignal = 1;
            break;
    }   // switch

    // remember where we started, we need to make two passes: one for precision, one for the number.
    pStartPassHere = pAsciiValue;

    // the first pass through the whole string is trying to find a precision indicator
    // the second pass works on the numeric fields with any precision indicator removed
    TimeUnitTracker = ss_TimeUnit_dy;
    for (pass=SEARCH_PASS; pass <= PARSE_PASS; pass++)
    {
        // continue from [1st or 2nd character], to [past the the last character] of the specification, if any
        while (*pAsciiValue)
        {
            if (pass == SEARCH_PASS)
            {
                parsedTimeUnit = ssT_stampOp_ParseTimeUnit (pAsciiValue);
                if (parsedTimeUnit != ss_TimeUnit_none)
                {
                    // we found a valid one or two-character sequence, need to truncate from Stamp.
                    // Weird thing: precision tracker names the precision of the first field,
                    // not the last.  Similar to how "AM" applies to hours not minutes in "12:30AM".
                    TimeUnitTracker = parsedTimeUnit;
                    *pAsciiValue = Ascii_NUL;
                    // don't fall through, input string search done, action taken below for parse
                    continue;
                }
            }   // pass == SEARCH_PASS
            else
            {   // pass == PARSE_PASS
                if (Ascii_isDigit (*pAsciiValue))          // decimal integer processing under way
                {
                    ValueThisTimeUnit *= 10;
                    // Ascii_lc can't be both IsDigit AND !0-9m so just add the integer
                    ValueThisTimeUnit += Ascii_digit_toInt (*pAsciiValue);
                }

                // if this character is a separator or if this character is the last character
                //    (this may be the last character in the string AND a digit as processed above):
                //   The end of the number being accumulated for this precision has been reached.
                if ((ssT_stampOp_IsSeparator (*pAsciiValue)) || (*(pAsciiValue+1) == Ascii_NUL))
                {
                    switch (TimeUnitTracker)
                    {
                        case ss_TimeUnit_dy       :
                            workTime.days   = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.days;
                            TimeUnitTracker = ss_TimeUnit_hr;
                            break;
                        case ss_TimeUnit_hr      :
                            workTime.hours  = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.hours;
                            TimeUnitTracker = ss_TimeUnit_mn;
                            break;
                        case ss_TimeUnit_mn       :
                            workTime.mins   = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.mins;
                            TimeUnitTracker = ss_TimeUnit_sc;
                            break;
                        case ss_TimeUnit_sc       :
                            workTime.secs   = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.secs;
                            TimeUnitTracker = ss_TimeUnit_ms;
                            break;
                        case ss_TimeUnit_ms        :
                            workTime.msecs  = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.msecs;
                            TimeUnitTracker = ss_TimeUnit_us;
                            break;
                        case ss_TimeUnit_us        :
                            workTime.usecs  = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.usecs;
#ifdef SST_OPTIN_TIME_NSECS_TU
                            TimeUnitTracker = ss_TimeUnit_ns;
#else   // SST_OPTIN_TIME_NSECS_TU
                            TimeUnitTracker = ss_TimeUnit_pk;
#endif  // SST_OPTIN_TIME_NSECS_TU
                            break;
                        case ss_TimeUnit_ns        :
// The ifdef above will set the switch to do plancks case next time around; or parse nanos before pk
#ifdef SST_OPTIN_TIME_NSECS_TU
                            workTime.nsecs  = ValueThisTimeUnit * signSignal;
                            SumOfTimeUnits += workTime.nsecs;
                            TimeUnitTracker = ss_TimeUnit_pk;
#endif // SST_OPTIN_TIME_NSECS_TU
                            break;
                        case ss_TimeUnit_pk        :    // bugbugbug    math command disallows this code
                            workTime.plancks  = ValueThisTimeUnit;
                            TimeUnitTracker = ss_TimeUnit_none;
                            break;
                        case ss_TimeUnit_EON        :
                        case ss_TimeUnit_none       :
                            break;
                    }   // switch
                    ValueThisTimeUnit = 0;
                }   // separator
            }   // pass == PARSE_PASS

            // this is the while that contains both Pass 1 and Pass 2 logic, looking at the input.
            pAsciiValue++;
        }   // while characters to parse during this pass

        pAsciiValue = pStartPassHere;
    }   // for 2 passes

    if (TimeUnitTracker != ss_TimeUnit_none)
    {
        // workTime.Positive was established by the parser, and will be referred to by AnyTUError?
        workTime.Zero = !(SumOfTimeUnits > 0);
        workTime.Valid = true;

        // The claim is made that the workTime Time variable value is comprised of "coherent TUs".
        // The question is answered by using the metadata without question, but, checking all TUs.
        if (ssT_timeIf_P1hasTUerror (&workTime) == ss_TimeUnit_none)
        {
            ssT_timeOp_Check_MathPolicy (&workTime, EONvalueValid);
            ssT_mathOp_P1getsP2 (pTime, &workTime);
            return (true);
        }
    }   // no error found in Time Units
    // -----------------------------------------------------------------------------------------
    // Valid is false means positive/zero have no meaning, Positive and Zero false by choice.
    // -----------------------------------------------------------------------------------------
    workTime.Valid = false;
    workTime.Zero = false;
    workTime.Positive = false;
    ssT_mathOp_P1getsP2 (pTime, &workTime);
    msg_ssTEA_Cause ("  Time Unit data or Time Unit Name parsing error");
    return (false);
}   // ssT_stampOp_AsciiToBinary
// -------------------------------------------------------------------------------------------------
// This function implements the rules for the "Time Unit" that a "Time Stamp" format may include.
// -------------------------------------------------------------------------------------------------
ss_TimeUnit_t  ssT_stampOp_ParseTimeUnit (pAscii_t  p_achar)
{
    ss_TimeUnit_t   TimeUnit;          // all paths through must set a TimeUnit before the return
    boolean         ValidTimeUnit = true;

    Ascii_t     first   = Ascii_toLower (*p_achar);
    Ascii_t     second  = Ascii_toLower (*(p_achar+1));

    switch (first)
    {
        case Ascii_d :
            TimeUnit = (second == Ascii_y) ? ss_TimeUnit_dy : ss_TimeUnit_none;
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        case Ascii_h :
            TimeUnit = (second == Ascii_r) ? ss_TimeUnit_hr : ss_TimeUnit_none;
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        case Ascii_m :
            switch (second)
            {
                case Ascii_n    :  TimeUnit = ss_TimeUnit_mn;    break;
                case Ascii_s    :  TimeUnit = ss_TimeUnit_ms;    break;
                default         :  TimeUnit = ss_TimeUnit_none;  break;
            }   // switch on 2nd char
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        case Ascii_s :
            TimeUnit = (second == Ascii_c) ? ss_TimeUnit_sc : ss_TimeUnit_none;
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        case Ascii_u :
            TimeUnit = (second == Ascii_s) ? ss_TimeUnit_us : ss_TimeUnit_none;
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        case Ascii_n :
            TimeUnit = (second == Ascii_s) ? ss_TimeUnit_ns : ss_TimeUnit_none;
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        case Ascii_p :
            TimeUnit = (second == Ascii_k) ? ss_TimeUnit_pk : ss_TimeUnit_none;
            ValidTimeUnit = (TimeUnit != ss_TimeUnit_none);
            break;
        default :
            // all mismatches after a matching first are processed above, this is mismatch of first
            TimeUnit = ss_TimeUnit_none;
            break;          // first character doesn't match allowed, we're done
    }   // switch on 1st char

    if (!ValidTimeUnit)
    {
        ss_uiOp_emit_qAsciiA ("ssT parser of Time Unit found error in Time Unit, defaulting to Day");
        TimeUnit = ss_TimeUnit_dy;
    }
    return (TimeUnit);
}   // ssT_stampOp_ParseTimeUnit
// -------------------------------------------------------------------------------------------------
// this specific definition limits the exact characters that ssT will consider a "separator",
// during the process of decoding an array of characters that are organized as a "Time Stamp".
// -------------------------------------------------------------------------------------------------
boolean ssT_stampOp_IsSeparator (Ascii_t Ascii)
{
    switch (Ascii)
    {
        case Ascii_Space    :
        case Ascii_Period   :
        case Ascii_Minus    :
        case Ascii_Plus     :
        case Ascii_EQ       :
            return (true);
        default:
            return (false);
    }   // switch
}   // ssT_stampOp_IsSeparator
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// These macros are applicable and usable only with Time Counter variables, which are accumulations.
// These macros help to convert a Time Counter value into a Time value, using divide operations.
// -------------------------------------------------------------------------------------------------
// divide answers "what is the non-fractional <number of units contained in a subunit count>".
// For example "how many days are in 37 hours?".  1 whole day is represented, the remainder is lost.
// -------------------------------------------------------------------------------------------------
// modulo is a math operation used to calculate the remainder when the important question is:
//    What is the remainder when I divide <count of the number of subunits> by
//                                        <the number of subunits that comprise a unit>?
// for example, "what is the remainder when I divide 37 hours by 24 hours?". 13 hours.
// -------------------------------------------------------------------------------------------------
#define DaysInAllHours(hrCt)          (hrCt / SST_HOURS_PER_DAY)
#define HoursInAllMins(mnCt)          (mnCt / SST_MINS_PER_HOUR)
#define MinsInAllSecs(scCt)           (scCt / SST_SECS_PER_MIN)
#define SecsInAllMsecs(msCt)          (msCt / SST_MSECS_PER_SEC)
#define MsecsInAllUsecs(usCt)         (usCt / SST_USECS_PER_MSEC)

// -------------------------------------------------------------------------------------------------
// "InCurrent" and "InAll" in the macro names means "how many subunits are the current unit?".
// If we have enough milliseconds to count 37 hours, we know 24 hours plus 13 hours have occured.
// -------------------------------------------------------------------------------------------------
#define DaysInCurrentEON(dyCt)             (dyCt % SST_DAYS_PER_EON)
#define HoursInCurrentDay(hrCt)            (hrCt % SST_HOURS_PER_DAY)
#define MinsInCurrentHour(mnCt)            (mnCt % SST_MINS_PER_HOUR)
#define SecsInCurrentMin(scCt)             (scCt % SST_SECS_PER_MIN)
#define MsecsInCurrentSec(msCt)            (msCt % SST_MSECS_PER_SEC)
#define UsecsInCurrentMsec(usCt)           (usCt % SST_USECS_PER_MSEC)

// -------------------------------------------------------------------------------------------------
// This is a shortcut method used by Time math.   "TimeConst" is a constant value that can be given
// a value in usecs (only) and then used in a Time math operation to add microseconds to timeCurr.
// -------------------------------------------------------------------------------------------------
ssT_Time_t QuickTimeForMath;

void    ssT_timeOp_InitQuickTimeForMath   (void)
{
    ssT_mathOp_P1getsZero (&QuickTimeForMath);          // bugbugbug runtime/buildtime init
}   // ssT_timeOp_InitQuickTimeForMath
// -------------------------------------------------------------------------------------------------
// Inter-Agency Gap or IAG is <microseconds that have occured since the last microseconds check>.
// The purpose of this function is to store the IAG (32-bit number of microseconds) in a time Var.
// In a fully active agencying set of events within a fully active application with no humans,
// the IAG will be a relatively small number; probably less than 1000us.  If the ssTEA Time is
// stopped, the hardware counter will no longer be read; value read next, later, could be a big one.
// -------------------------------------------------------------------------------------------------
// there is a Time price paid for all the math to decode a full 32-bit number (of unknown range).
// this is a method to create a Time in a known range: a semi-constant Time with only us/ms changing.
// Still incurring the cost of the math operations across all Time fields, including carry/borrow.
// -------------------------------------------------------------------------------------------------
void    ssT_timeOp_Add_usToTimeCurr (TimeUnitsBig_t us_counter)
{
    QuickTimeForMath.secs = 0;              // always clear, always add 0 secs unless msecs > 1 second
    QuickTimeForMath.msecs = 0;             // always clear, always add 0 msecs unless usecs > 1 milli

    while (us_counter > SST_USECS_PER_MSEC)
    {
        QuickTimeForMath.msecs++;
        us_counter -= SST_USECS_PER_MSEC;
    }   // while
    QuickTimeForMath.usecs = us_counter;      // the big value has had all of the millis taken out

    // it is possible that the us_counter value caused the loop to count up > 1000 millis?  yes
    while (QuickTimeForMath.msecs > SST_MSECS_PER_SEC)
    {
        QuickTimeForMath.secs++;
        QuickTimeForMath.msecs-= SST_MSECS_PER_SEC;
    }   // while

    // assumption: we are adding as part of calculating a future time for agency as an event... so
    // assumption: we will depend on the mathOp to also instantiate compressed TimeMoment values.
    ssT_mathOp_P1getsP1plusP2 (ssT_gpTime_Current, &QuickTimeForMath);
}   // ssT_timeOp_Add_usToTimeCurr
// -------------------------------------------------------------------------------------------------
// based on the limitation of the number of milliseconds that can be represented by a 32-bit number:
// the days field is limited as the milliseconds cannot get big enough to include >49 days in a Time.
// 49:23:59:59:999:xxx is the maximum: 49 days, 23 hours, 59 minutes, 59 seconds, 999 ms (??? us/ns).
// This limitation applies to two situations: Time is from 0 and reaches a maximum for 32-bits, and,
// the delta in Time between two Times cannot be represented if it is longer than 32-bits can hold.
// The Time (that is fully and accurately described at the millisecond level) can be defined as an
//      unsigned 32-bit number in ssHL      "unsigned long int".
//
// This is all to say: the current Time can be "553:22:40dy" and be useful, but the number of millis
// since the BigBang is not useful.  This difference between millisecond counters and ssT_Time_t
// proper variable with values only matters when converting milliseconds to/from ssT_Time_t values.
// A software feature that needs a conversion from ssT_Time_t to milliseconds may be written by you.
// -------------------------------------------------------------------------------------------------
void    ssT_timeOp_TimeFromMillis (ssT_pTime_t pTime, TimeUnitsBig_t bigvalue)
{
    pTime->Zero = false;
    pTime->Positive = true;
    pTime->Valid = true;
    pTime->plancks = ssT_rawCt_NextPlanck ();   // within any time unit, # plancks is random
#ifdef SST_OPTIN_TIME_NSECS_TU
#ifndef SST_ONEOF_TIME_NSECS_NOTHING            // App uses with no mods by ssTEA
    pTime->nsecs = ssT_rawCt_NextNsec ();       // within one microsecond, # nanos is random
    pTime->usecs = ssT_rawCt_NextNsec ();       // within one millisecond, # micros is random
#endif  // not SST_ONEOF_TIME_NSECS_NOTHING
#endif // SST_OPTIN_TIME_NSECS_TU

    // how many millis have occured during the current second?
    // (what is the remainder when bigvalue is divided by millis per second?)
    pTime->msecs = MsecsInCurrentSec (bigvalue);
    // how many seconds have occured during all of the milliseconds?
    // (what is the quotient when bigvalue is divided by millis per second?)
    // bigvalue as count of millis is transformed to -> count of secs
    bigvalue = SecsInAllMsecs (bigvalue);
    // how many seconds have occured during the current minute?
    // (what is the remainder when bigvalue is divided by seconds per minute?)
    pTime->secs = SecsInCurrentMin (bigvalue);
    // how many minutes have occured during all of the seconds?
    // (what is the quotient when bigvalue is divided by seconds?)
    // bigvalue as count of secs -> count of mins
    bigvalue = MinsInAllSecs (bigvalue);
    // how many minutes have occured during the current hour?
    // (what is the remainder when bigvalue is divided by minutes per hour?)
    pTime->mins = MinsInCurrentHour (bigvalue);
    // how many hours have occured during all of the minutes?
    // (what is the quotient when bigvalue is divided by minutes?)
    // big value as count of mins -> count of hours
    bigvalue = HoursInAllMins (bigvalue);
    // how many hours have occured during the current day
    // (what is the remainder when bigvalue is divided by hours per day?)
    pTime->hours = HoursInCurrentDay (bigvalue);
    // how many days have occured during this EON?
    // (what is the quotient when bigvalue is divided by hours per day?)
    // big value as count of hours -> count of days
    bigvalue = DaysInAllHours (bigvalue);
    // reached the top level of the coherent Time Unit definition, see built-in limits due to 32-bits.
    pTime->days = bigvalue;
    // The top level, the EON, is always implied in math and comparisons, and has the value zero.
}   // ssT_timeOp_TimeFromMillis
// =================================================================================================
#ifdef SST_OPTIN_TIME_NSECS_TU
// -------------------------------------------------------------------------------------------------
// The Incrementing pattern is used with nanoseconds to change the nanoseconds TU from Time to Time.
// -------------------------------------------------------------------------------------------------
TimeUnitsSmall_t  ssT_rawCt_NextNsec (void)
{
#ifdef SST_ONEOF_TIME_NSECS_INCR
    static TimeUnitsBig_t   Incrementer = 0;        //  goes up by one each next requested
#endif  // SST_ONEOF_TIME_NSECS_INCR

    TimeUnitsSmall_t  CurrNsec = 2;           // if Nsecs always 2, ONEOF missing

#ifdef SST_ONEOF_TIME_NSECS_RANDOM
    CurrNsec = ( (TimeUnitsBig_t) mesa_Random_From1000_Seq () );
#endif  // SST_ONEOF_TIME_NSECS_RANDOM

#ifdef SST_ONEOF_TIME_NSECS_INCR
    CurrNsec = Incrementer;
    Incrementer += 1;
#endif  // SST_ONEOF_TIME_NSECS_INCR

#ifdef SST_ONEOF_TIME_NSECS_ZERO
    CurrNsec = 0;
#endif  // SST_ONEOF_TIME_NSECS_ZERO

    return (CurrNsec);
}   // ssT_rawCt_NextNsec
#endif // SST_OPTIN_TIME_NSECS_TU
// -------------------------------------------------------------------------------------------------
// The Incrementing pattern is used with plancks to alter the upper 24-bits of the 32-bit value.
// -------------------------------------------------------------------------------------------------
TimeUnitsBig_t  ssT_rawCt_NextPlanck (void)
{
    TimeUnitsBig_t          ThisPlanck = 0;

    static TimeUnitsBig_t   Incrementer = 0;        //  goes up by one each planck generated

    // by definition, but unlike actual Planck counter values, there is a pattern to the numbers:
    // 8-bit value from 0 to 255, incrementing, rolling past zero when incremented at 255.
    ThisPlanck = (Incrementer & 0xFF);
    Incrementer += 1;

#ifdef SST_ONEOF_TIME_PLANCKS_RANDOM
#ifdef MESA_OPTIN_RANDOM_NUMBERS
    // Each planck value in Random mode has an incrementing portion and the rest is as selected,
    //   because the incrementing trick is restricted to the least significant 8 bits.
    ThisPlanck |= ( (TimeUnitsBig_t) (mesa_Random_From256_Seq () << 24) );
    ThisPlanck |= ( (TimeUnitsBig_t) (mesa_Random_From256_Seq () << 16) );
    ThisPlanck |= ( (TimeUnitsBig_t) (mesa_Random_From256_Seq () <<  8) );
#else   // MESA_OPTIN_RANDOM_NUMBERS
    ThisPlanck |= 0x8675309;            // indicate a build conflict between source wanted/enabled
#endif  // MESA_OPTIN_RANDOM_NUMBERS
#endif  // SST_ONEOF_TIME_PLANCKS_RANDOM

#ifdef SST_ONEOF_TIME_PLANCKS_ZERO
    // Each planck value in non-Random Zero mode is greater than the last planck value,
    //   HOWEVER because the incrementing trick is restricted to the least significant 8 bits,
    //   and the upper 24 bits are always zero, a Planck is only unique until the 8 bits rollover.
    // Assignment of incrementer with 8-bit mask zeros MS 24 bits, no other action leaves it zero.
#endif  // SST_ONEOF_TIME_PLANCKS_ZERO

#ifdef SST_ONEOF_TIME_PLANCKS_INCR
    // Each planck value in non-Random non-Zero mode is greater than the last planck value,
    //   because the incrementing trick is restricted to the least significant 8 bits of Incrementer,
    //   and the 24-bits (above LS 8 bits) is a free running counter based on Incrementer.  Just a trick.
    ThisPlanck |= (Incrementer << 12);
#endif  // SST_ONEOF_TIME_PLANCKS_INCR

    return (ThisPlanck);
}   // ssT_rawCt_NextPlanck
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// This is one instance in the ssTEA software where the information considered important is not the
// Current Time or a Time in the Past, but the difference between the one Time and another Time.
// -------------------------------------------------------------------------------------------------
// The Time measurements being used as operands in the difference calculation are 32-bit integer
// value retrieved from 32-bit registers the mesa provides to track the passage of Time in reality.
// The assumption built into the order of operands is that "now" is more than "past", within a
// number system that has a limited number of digits. For example, with only 1 digit: the difference
// between 4 as current and 2 as past is 2; the difference between current 2 and past 4 is...8.
// -------------------------------------------------------------------------------------------------
// The intention behind the design and implementation of this specific software algorithm depends
// on the processor being a 32-bit processor: 32-bit registers, 32-bit math, 32-bit memory access.
// Under an implementation of ssTEA for a 64-bit processor, the number for the Past will never be
// larger than the number for the Current Time, and, a 64-bit micro/milli constant (here) as well as
// mesa-level 64-bit counter register would be required.   Only the constant MAX is dependent on
// these assumptions; a SimpleSal is free to change TimeUnitsBig_t to a 64-bit number and try it out.
// -------------------------------------------------------------------------------------------------
#define    MAX_TimeInPast  (0xFFFFFFFF)     // the largest unsigned value for a 32-bit number

TimeUnitsBig_t  ssT_rawCt_SubPastFromNow (TimeUnitsBig_t TimeNow, TimeUnitsBig_t TimeInPast)
{
    TimeUnitsBig_t          Math;

    if (TimeNow < TimeInPast)
    {   // resolves to true when "now" has wrapped at zero to a "past" greater than or equal to zero
        // The difference is the value between the Past Time and the Max Time, plus the Current Time.
        // proof: TimeNow==0x00000000 and TimeInPast==0xFFFFFFFF
        //          0x00000000 + ((MAX_TimeInPast - TimeInPast) + 1)
        //          0x00000000 + ((0x00000000) + 1)
        //          0x00000000 + (0x00000001)
        //   result: Elapsed time is 1

        Math = TimeNow + ((MAX_TimeInPast - TimeInPast) + 1);

        // In fact this would seem to work at the other end of the possible value ranges:
        // proof: TimeNow==0xFFFFFFFF and TimeInPast==0x00000000
        //          0xFFFFFFFF + ((MAX_TimeInPast - 0x00000000) + 1)
        //          0xFFFFFFFF + ((0xFFFFFFFF) + 1)
        //          0xFFFFFFFF + (0x00000000)       // assumption: overflow 32-bits, wrap to zero
        //   result: Elapsed time is 0xFFFFFFFF
    }
    else
    {   // just doing 32-bit unsigned math between friends, is this equivalent to other method?
        // should this add 1 as the other branch does?  no: compare and sub are apples to apples
        Math = TimeNow - TimeInPast;
    }

    return (Math);
}   // ssT_rawCt_SubPastFromNow

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_TimeLegend (void)
{
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA ("TimeStamp legend--- [ Valid Sign Zero |");
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[DY]);
    ss_uiOp_emit_1 (Ascii_Period);
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[HR]);
    ss_uiOp_emit_1 (Ascii_Period);
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[MN]);
    ss_uiOp_emit_1 (Ascii_Period);
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[SC]);
    ss_uiOp_emit_1 (Ascii_Period);
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[MS]);
    ss_uiOp_emit_1 (Ascii_Period);
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[US]);
    ss_uiOp_emit_1 (Ascii_Period);
#ifdef SST_OPTIN_TIME_NSECS_TU
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[NS]);
    ss_uiOp_emit_1 (Ascii_Period);
#endif  // SST_OPTIN_TIME_NSECS_TU
    ss_uiOp_emit_pAsciiA (ss_pTimeUnitNames[PK]);
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_pAsciiA (pcHelpUnits);
    ss_uiOp_emit_newline ();
}   // ss_uiOp_Show_TimeLegend
// -------------------------------------------------------------------------------------------------
// bugbugbug: example of how to make a run-time decision based on a build-time choice with same code
// -------------------------------------------------------------------------------------------------
void ss_uiOp_Show_Time (ssT_pTime_t pTime, boolean AddNewline)
{
#ifdef SST_OPTIN_TIME_SHOW_TU_SIGN
    boolean showSign = true;
#else   // not SST_OPTIN_TIME_SHOW_TU_SIGN
    boolean showSign = false;
#endif  // SST_OPTIN_TIME_SHOW_TU_SIGN

    if (!pTime)
    {
        // A NULL pointer is a fatal flaw and so this is just blasted out
        ss_uiOp_qBanner (lfY, "flaw: null pointer to {ss_uiOp_Show_Time} function", AddNewline);
        return;
    }
#ifdef SST_OPTIN_TIME_DEBUG_ADDRESS
    ss_uiOp_emit_qAsciiA ("Time data @");
    ss_uiOp_emit_Hex_32bits (pTime);
#endif  // SST_OPTIN_TIME_DEBUG_ADDRESS
    ss_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_1 ((pTime->Valid) ? Ascii_V : Ascii_v);

    // an invalid Time by definition has no values in all of the other fields
    if (pTime->Valid)
    {
        // This is the positivity or negative of the Time, all Time Units must have the same sign
        ss_uiOp_emit_1 (mSIGN_OF_TIME (pTime));
        ss_uiOp_emit_1 ((pTime->Zero) ? Ascii_Z : Ascii_z);
        ss_uiOp_emit_1 (Ascii_verticalBar);
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->days));
        ss_uiOp_emit_Int_999 (pTime->days);
        ss_uiOp_emit_1 (Ascii_Period);
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->hours));
        ss_uiOp_emit_Int_999 (pTime->hours);
        ss_uiOp_emit_1 (Ascii_Period);
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->mins));
        ss_uiOp_emit_Int_999 (pTime->mins);
        ss_uiOp_emit_1 (Ascii_Period);
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->secs));
        ss_uiOp_emit_Int_999 (pTime->secs);
        ss_uiOp_emit_1 (Ascii_Period);
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->msecs));
        ss_uiOp_emit_Int_999 (pTime->msecs);
        ss_uiOp_emit_1 (Ascii_Period);
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->usecs));
        ss_uiOp_emit_Int_999 (pTime->usecs);
        ss_uiOp_emit_1 (Ascii_Period);
#ifdef SST_OPTIN_TIME_NSECS_TU
        if (showSign) ss_uiOp_emit_1 (mSIGN_OF_TU (pTime->nsecs));
        ss_uiOp_emit_Int_999 (pTime->nsecs);
        ss_uiOp_emit_1 (Ascii_Period);
#endif // SST_OPTIN_TIME_NSECS_TU
#ifdef SST_OPTIN_TIME_SHOW_32BIT_PLANCKS
        ss_uiOp_emit_Hex_2 ((pTime->plancks >> 24)  & 0xFF);
        ss_uiOp_emit_1 (Ascii_Colon);
        ss_uiOp_emit_Hex_2 ((pTime->plancks >> 16)  & 0xFF);
        ss_uiOp_emit_1 (Ascii_Colon);
        ss_uiOp_emit_Hex_2 ((pTime->plancks >>  8)  & 0xFF);
        ss_uiOp_emit_1 (Ascii_Colon);
#endif  // SST_OPTIN_TIME_SHOW_32BIT_PLANCKS
        ss_uiOp_emit_Hex_2 ((pTime->plancks)        & 0xFF);
    }   // valid Time
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    if (AddNewline)
    {
        ss_uiOp_emit_newline ();
    }   // after time output, output LF or do not output LF
}   // ss_uiOp_Show_Time
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This function clears all fields, without decisions, to known acceptable and coherent values.
// -------------------------------------------------------------------------------------------------
void    ssT_mathOp_P1getsZero (ssT_pTime_t pTime)
{
    pTime->momentU = 0;
    pTime->momentL = 0;

    pTime->plancks = ssT_rawCt_NextPlanck ();
#ifdef SST_OPTIN_TIME_NSECS_TU
    pTime->nsecs = 0;           // to get a true zero for this Time, even nanoseconds has value 0
#endif // SST_OPTIN_TIME_NSECS_TU
    pTime->usecs = 0;
    pTime->msecs = 0;
    pTime->secs = 0;
    pTime->mins = 0;
    pTime->hours = 0;
    pTime->days = 0;

    pTime->Positive = true;
    pTime->Zero = true;
    pTime->Valid = true;
}   // ssT_mathOp_P1getsZero
// -------------------------------------------------------------------------------------------------
// Order is "to, from" because that is the order an ssHL assignment statement such as "a = b" uses.
// The ssHL-language sees assignment statements as "lvalue gets value", this matches that model.
// This flows through to the parameter order for ssUI assignments, which is <Lvalue> <=> <value>.
// -------------------------------------------------------------------------------------------------
void ssT_mathOp_P1getsP2 (ssT_pTime_t pP1, ssT_pTime_t pP2)
{
    pP1->momentU    = pP2->momentU;
    pP1->momentL    = pP2->momentL;
    pP1->plancks    = pP2->plancks;
#ifdef SST_OPTIN_TIME_NSECS_TU
    pP1->nsecs      = pP2->nsecs;
#endif // SST_OPTIN_TIME_NSECS_TU
    pP1->usecs      = pP2->usecs;
    pP1->msecs      = pP2->msecs;
    pP1->secs       = pP2->secs;
    pP1->mins       = pP2->mins;
    pP1->hours      = pP2->hours;
    pP1->days       = pP2->days;
    pP1->Positive   = pP2->Positive;
    pP1->Zero       = pP2->Zero;
    pP1->Valid      = pP2->Valid;
}   // ssT_mathOp_P1getsP2
// -------------------------------------------------------------------------------------------------
ssT_math_CmpState_t  ssT_timeIf_P1compareP2 (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    if      (pOperand1->days  > pOperand2->days )  return (eCmpState_After);
    else if (pOperand1->days  < pOperand2->days )  return (eCmpState_Before);

    if      (pOperand1->hours > pOperand2->hours)  return (eCmpState_After);
    else if (pOperand1->hours < pOperand2->hours)  return (eCmpState_Before);

    if      (pOperand1->mins  > pOperand2->mins )  return (eCmpState_After);
    else if (pOperand1->mins  < pOperand2->mins )  return (eCmpState_Before);

    if      (pOperand1->secs  > pOperand2->secs )  return (eCmpState_After);
    else if (pOperand1->secs  < pOperand2->secs )  return (eCmpState_Before);

    if      (pOperand1->msecs > pOperand2->msecs)  return (eCmpState_After);
    else if (pOperand1->msecs < pOperand2->msecs)  return (eCmpState_Before);

    if      (pOperand1->usecs > pOperand2->usecs)  return (eCmpState_After);
    else if (pOperand1->usecs < pOperand2->usecs)  return (eCmpState_Before);

#ifdef SST_OPTIN_TIME_NSECS_TU
    if      (pOperand1->nsecs > pOperand2->nsecs)  return (eCmpState_After);
    else if (pOperand1->nsecs < pOperand2->nsecs)  return (eCmpState_Before);
#endif // SST_OPTIN_TIME_NSECS_TU

    // every value found not greater and not less, result is Times are equal
    return (eCmpState_Equal);
}   // ssT_timeIf_P1compareP2
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\Software Developer Tricks.note]    macros make function calls disappear
// -------------------------------------------------------------------------------------------------
#ifdef SST_OPTIN_TIME_DEBUG_IFS
#define DEBUG_IFS_Banner_lfY(pAA)  ss_uiOp_qBanner (lfN, S(pAA), lfY)
#define DEBUG_IFS_Banner_lfN(pAA)  ss_uiOp_qBanner (lfN, S(pAA), lfN)
ReadOnly pAsciiA_t pc_NoItIsNotMsg = S("no it is not");
ReadOnly pAsciiA_t pc_YesItIsMsg = S("yes it is");
#else   // not debugging IFS
#define DEBUG_IFS_Banner_lfY(pAA)
#define DEBUG_IFS_Banner_lfN(pAA)
#endif  // SST_OPTIN_TIME_DEBUG_IFS
// -------------------------------------------------------------------------------------------------
// a Time value of Zero is defined as "Valid, Positive, all Time Units have the value Zero".
// asking the question about a Time causes the Zero flag to be updated based on the current value.
// -------------------------------------------------------------------------------------------------
boolean  ssT_timeif_P1equalsZero (ssT_pTime_t pTime)
{
    ssT_math_CmpState_t math_CmpState;

    math_CmpState = ssT_timeIf_P1compareP2 (pTime, ssT_gpTime_Zero);
    switch (math_CmpState)
    {
        case eCmpState_Equal    : return (true);
        case eCmpState_After    :
        case eCmpState_Before   : return (false);
    }   // switch compare result state
    return (false);     // the ssHL compiler cannot detect that every switch case is covered?
}   // ssT_timeif_P1equalsZero
// -------------------------------------------------------------------------------------------------
// This comparison is the equivalent of the ARM processor asking if two registers are equal using
// the processor instruction "compare reg1, reg2".  Every detail of both must have the same value.
// -------------------------------------------------------------------------------------------------
boolean ssT_timeif_P1equalsP2 (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_math_CmpState_t math_CmpState;

    math_CmpState = ssT_timeIf_P1compareP2 (pOperand1, pOperand2);
    switch (math_CmpState)
    {
        case eCmpState_Equal    : return (true);
        case eCmpState_After    :
        case eCmpState_Before   : return (false);
    }   // switch compare result state
    return (false);     // the ssHL compiler cannot detect that every switch case is covered?
}   // ssT_timeif_P1equalsP2
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssT_timeIf_P1beforeP2 (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_math_CmpState_t math_CmpState;

#ifdef SST_OPTIN_TIME_DEBUG_IFS
    if (OK_to_Blast_Ascii_Live ())
    {
        DEBUG_IFS_Banner_lfY ("is Op1 before Op2?");
        DEBUG_IFS_Banner_lfN ("    Op1 : ");
        ss_uiOp_Show_Time (pOperand1, lfY);
        DEBUG_IFS_Banner_lfN ("    Op2 : ");
        ss_uiOp_Show_Time (pOperand2, lfY);
    }
#endif  // SST_OPTIN_TIME_DEBUG_IFS

    math_CmpState = ssT_timeIf_P1compareP2 (pOperand1, pOperand2);
    switch (math_CmpState)
    {
        case eCmpState_Equal    :
        case eCmpState_After    :
            if (OK_to_Blast_Ascii_Live ())   DEBUG_IFS_Banner_lfY (pc_NoItIsNotMsg);
            return (false);
        case eCmpState_Before   :
            if (OK_to_Blast_Ascii_Live ())   DEBUG_IFS_Banner_lfY (pc_YesItIsMsg);
            return (true);
    }   // switch compare result state
    return (false);     // the ssHL compiler cannot detect that every switch case is covered?
}   // ssT_timeIf_P1beforeP2
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssT_timeIf_P1equalsORbeforeP2 (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_math_CmpState_t math_CmpState;

#ifdef SST_OPTIN_TIME_DEBUG_IFS
    DEBUG_IFS_Banner_lfY ("is Op1 EQ or before Op2?");
    DEBUG_IFS_Banner_lfN ("    Op1 : ");
    ss_uiOp_Show_Time (pOperand1, lfY);
    DEBUG_IFS_Banner_lfN ("    Op2 : ");
    ss_uiOp_Show_Time (pOperand2, lfY);
#endif  // SST_OPTIN_TIME_DEBUG_IFS

    math_CmpState = ssT_timeIf_P1compareP2 (pOperand1, pOperand2);
    switch (math_CmpState)
    {
        case eCmpState_After    :
            if (OK_to_Blast_Ascii_Live ())   DEBUG_IFS_Banner_lfY (pc_NoItIsNotMsg);
            return (false);
        case eCmpState_Equal    :
        case eCmpState_Before   :
            if (OK_to_Blast_Ascii_Live ())   DEBUG_IFS_Banner_lfY (pc_YesItIsMsg);
            return (true);
    }   // switch compare result state
    return (false);     // the ssHL compiler cannot detect that every switch case is covered?
}   // ssT_timeIf_P1equalsORbeforeP2
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssT_timeIf_P1afterP2 (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_math_CmpState_t math_CmpState;

#ifdef SST_OPTIN_TIME_DEBUG_IFS
    if (OK_to_Blast_Ascii_Live ())
    {
        DEBUG_IFS_Banner_lfY ("is Op1 after Op2?");
        DEBUG_IFS_Banner_lfN ("    Op1 : ");
        ss_uiOp_Show_Time (pOperand1, lfY);
        DEBUG_IFS_Banner_lfN ("    Op2 : ");
        ss_uiOp_Show_Time (pOperand2, lfY);
    }
#endif  // SST_OPTIN_TIME_DEBUG_IFS

    math_CmpState = ssT_timeIf_P1compareP2 (pOperand1, pOperand2);
    switch (math_CmpState)
    {
        case eCmpState_Equal    :
        case eCmpState_Before   :
            if (OK_to_Blast_Ascii_Live ())  DEBUG_IFS_Banner_lfY (pc_NoItIsNotMsg);
            return (false);
        case eCmpState_After   :
            if (OK_to_Blast_Ascii_Live ())  DEBUG_IFS_Banner_lfY (pc_YesItIsMsg);
            return (true);
    }   // switch compare result state
    return (false);     // the ssHL compiler cannot detect that every switch case is covered?
}   // ssT_timeIf_P1afterP2
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean ssT_timeIf_P1equalsORafterP2 (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_math_CmpState_t math_CmpState;

#ifdef SST_OPTIN_TIME_DEBUG_IFS
    if (OK_to_Blast_Ascii_Live ())
    {
        DEBUG_IFS_Banner_lfY ("is Op1 EQ or after Op2?");
        DEBUG_IFS_Banner_lfN ("    Op1 : ");
        ss_uiOp_Show_Time (pOperand1, lfY);
        DEBUG_IFS_Banner_lfN ("    Op2 : ");
        ss_uiOp_Show_Time (pOperand2, lfY);
    }
#endif  // SST_OPTIN_TIME_DEBUG_IFS

    math_CmpState = ssT_timeIf_P1compareP2 (pOperand1, pOperand2);
    switch (math_CmpState)
    {
        case eCmpState_Before   :
#ifdef SST_OPTIN_TIME_DEBUG_IFS
            if (OK_to_Blast_Ascii_Live ())  DEBUG_IFS_Banner_lfY (pc_NoItIsNotMsg);
#endif  // SST_OPTIN_TIME_DEBUG_IFS
            return (false);
        case eCmpState_Equal    :
        case eCmpState_After    :
#ifdef SST_OPTIN_TIME_DEBUG_IFS
            if (OK_to_Blast_Ascii_Live ())  DEBUG_IFS_Banner_lfY (pc_YesItIsMsg);
#endif  // SST_OPTIN_TIME_DEBUG_IFS
            return (true);
    }   // switch compare result state
    return (false);     // the ssHL compiler cannot detect that every switch case is covered?
}   // ssT_timeIf_P1equalsORafterP2
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
void  ssT_mathOp_P1getsP1plusP2 (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1)
{
#ifdef SST_OPTIN_TIME_DEBUG_RAWDATA
    if (pOperand1 != &QuickTimeForMath)
    {
        ssT_timeOp_Show_TimeUnitsRaw (S(" raw: Lvalue  "), pLvalue, 0);
        ssT_timeOp_Show_TimeUnitsRaw (S(" raw: Operand "), pOperand1, 0);
    }
#endif // SST_OPTIN_TIME_DEBUG_RAWDATA

    // --------------------------------------------------------------------------------------------
    // Time Math doesn't care about the methods of TU Math; Time as a Line or FSM chooses P1operP2.
    // --------------------------------------------------------------------------------------------
    TUMATHOP_P1operP2 (pLvalue, pOperand1, AddOperation);
#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
    ssT_mathOp_P1getsP1_Compressed (pLvalue);
#endif  // SST_OPTIN_TIME_COMPRESSED_TUS
    // --------------------------------------------------------------------------------------------

#ifdef SST_OPTIN_TIME_DEBUG_IFS
    if (pOperand1 != &QuickTimeForMath)
    {
        ss_uiOp_qBanner (lfY, "Time: P1 gets P1 plus P2:", lfN);
        ss_uiOp_Show_Time (pLvalue, lfY);
#ifdef SST_OPTIN_TIME_DEBUG_RAWDATA
        ssT_timeOp_Show_TimeUnitsRaw (S(" raw: result  "), pLvalue, 0);
#endif  // SST_OPTIN_TIME_DEBUG_RAWDATA
    }
#endif  // SST_OPTIN_TIME_DEBUG_IFS
}   // ssT_mathOp_P1getsP1plusP2
// -------------------------------------------------------------------------------------------------
// On error, leave the Lvalue zeroed but then set !Valid, easily changed later to be a valid zero.
// -------------------------------------------------------------------------------------------------
// There is a rule: "Lvalue can be used as an Operand", e.g., (var1=var1+var2) or (var1=var2+var1).
// note this expression results when the users uses the form "var1 += var2"; Lvalue also Operand1.
// Because the Lvalue may be an operand, must create result in local Time, write Lvalue on success.
// -------------------------------------------------------------------------------------------------
void  ssT_mathOp_P1getsP2plusP3 (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_Time_t  mathOpResult;

    ssT_mathOp_P1getsP2 (&mathOpResult, pOperand1);
    ssT_mathOp_P1getsP1plusP2 (&mathOpResult, pOperand2);

    // valid result, copy to Lvalue which will have the Valid flag set after P1getsP2
    if (mathOpResult.Valid)
    {
        ssT_mathOp_P1getsP2 (pLvalue, &mathOpResult);
    }
}   // ssT_mathOp_P1getsP2plusP3
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssT_mathOp_P1getsP1minusP2 (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1)
{
#ifdef SST_OPTIN_TIME_DEBUG_RAWDATA
    if (pOperand1 != &QuickTimeForMath)
    {
        ssT_timeOp_Show_TimeUnitsRaw (S(" raw: Lvalue  "), pLvalue, 0);
        ssT_timeOp_Show_TimeUnitsRaw (S(" raw: Operand "), pOperand1, 0);
    }
#endif  // SST_OPTIN_TIME_DEBUG_RAWDATA

    // --------------------------------------------------------------------------------------------
    // simple math cannot perform subtraction operations; there is no subtracting time in baseball
    // --------------------------------------------------------------------------------------------
    TUMATHOP_P1operP2 (pLvalue, pOperand1, SubOperation);
#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
    ssT_mathOp_P1getsP1_Compressed (pLvalue);
#endif  // SST_OPTIN_TIME_COMPRESSED_TUS
    // --------------------------------------------------------------------------------------------

#ifdef SST_OPTIN_TIME_DEBUG_IFS
    if (pOperand1 != &QuickTimeForMath)
    {
        ss_uiOp_qBanner (lfY, "Time: P1 gets P1 minus P2:", lfN);
        ss_uiOp_Show_Time (pLvalue, lfY);
#ifdef SST_OPTIN_TIME_DEBUG_RAWDATA
        ssT_timeOp_Show_TimeUnitsRaw (S(" raw:  lvalue result"), pLvalue, 0);
#endif  // SST_OPTIN_TIME_DEBUG_RAWDATA
    }
#endif  // SST_OPTIN_TIME_DEBUG_IFS
}   // ssT_mathOp_P1getsP1minusP2
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  ssT_mathOp_P1getsP2minusP3 (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2)
{
    ssT_Time_t mathOpResult;

    ssT_mathOp_P1getsP2 (&mathOpResult, pOperand1);
    ssT_mathOp_P1getsP1minusP2 (&mathOpResult, pOperand2);

    if (mathOpResult.Valid)
    {
        ssT_mathOp_P1getsP2 (pLvalue, &mathOpResult);
    }
}   // ssT_mathOp_P1getsP2minusP3
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// This function checks for an invalid value in each TimeUnit, useful when parsing stamp-to-binary
// performing math operations such as Addition and Subtraction, and protecting self from data in.
// ssTEA Math operations on variables will never RESULT in out-of-range values in any Time Unit, but
// a Time variable with values assigned by the user App may have some out-of-range values in it.
// Each Time Unit value's sign must be the same as the metadata Pos/Zero/Neg
// -------------------------------------------------------------------------------------------------
ss_TimeUnit_t ssT_timeIf_P1hasTUerror (ssT_pTime_t pTime)
{
    if (!pTime->Valid)  { return (ss_TimeUnit_dy); }

    if (pTime->Zero)
    {
        if (pTime->days  != 0)      { return (ss_TimeUnit_dy); }
        if (pTime->hours != 0)      { return (ss_TimeUnit_hr); }
        if (pTime->mins  != 0)      { return (ss_TimeUnit_mn); }
        if (pTime->secs  != 0)      { return (ss_TimeUnit_sc); }
        if (pTime->msecs != 0)      { return (ss_TimeUnit_ms); }
        if (pTime->usecs != 0)      { return (ss_TimeUnit_us); }
#ifdef SST_OPTIN_TIME_NSECS_TU
        if (pTime->nsecs != 0)      { return (ss_TimeUnit_ns); }
#endif // SST_OPTIN_TIME_NSECS_TU
        return (ss_TimeUnit_none);
    }

    // The value 0 in any TU is coherent with all other coherent TUs (all neg/pos if nonzero)
    if (pTime->Positive)
    {
        if (pTime->days  < 0)       { return (ss_TimeUnit_dy); }
        if (pTime->hours < 0)       { return (ss_TimeUnit_hr); }
        if (pTime->mins  < 0)       { return (ss_TimeUnit_mn); }
        if (pTime->secs  < 0)       { return (ss_TimeUnit_sc); }
        if (pTime->msecs < 0)       { return (ss_TimeUnit_ms); }
        if (pTime->usecs < 0)       { return (ss_TimeUnit_us); }
#ifdef SST_OPTIN_TIME_NSECS_TU
        if (pTime->nsecs < 0)      { return (ss_TimeUnit_ns); }
#endif // SST_OPTIN_TIME_NSECS_TU
    }
    else
    {
        if (pTime->days  > 0)       { return (ss_TimeUnit_dy); }
        if (pTime->hours > 0)       { return (ss_TimeUnit_hr); }
        if (pTime->mins  > 0)       { return (ss_TimeUnit_mn); }
        if (pTime->secs  > 0)       { return (ss_TimeUnit_sc); }
        if (pTime->msecs > 0)       { return (ss_TimeUnit_ms); }
        if (pTime->usecs > 0)       { return (ss_TimeUnit_us); }
#ifdef SST_OPTIN_TIME_NSECS_TU
        if (pTime->nsecs > 0)       { return (ss_TimeUnit_ns); }
#endif // SST_OPTIN_TIME_NSECS_TU
    }
    return (ss_TimeUnit_none);
}   // ssT_timeIf_P1hasTUerror
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void ssT_timeOp_Show_MathSmallRaw (TimeMathValueSmall_t MathValueSmall)
{
    ss_uiOp_emit_1 (mSIGN_OF_TU (MathValueSmall));
    ss_uiOp_emit_Int_999 (ssUI_IntsAbsValue(MathValueSmall));
}   // ssT_timeOp_Show_MathSmallRaw
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void ssT_timeOp_Show_TimeUnitsRaw (pAsciiA_t pName, ssT_pTime_t pTime, TimeMathValueSmall_t EON)
{
    ss_uiOp_emit_qAsciiA (ss_pTimeUnitNames[NONE]);
    ss_uiOp_emit_pAsciiA (pName);
    ss_uiOp_emit_1 (Ascii_squiggleLBracket);
    ss_uiOp_emit_1 ((EON == 0) ? Ascii_0 : (EON < 0) ? '-' : '+');
    ss_uiOp_emit_1 (Ascii_squiggleRBracket);
    ssT_timeOp_Show_MathSmallRaw (pTime->days);
    ssT_timeOp_Show_MathSmallRaw (pTime->hours);
    ssT_timeOp_Show_MathSmallRaw (pTime->mins);
    ssT_timeOp_Show_MathSmallRaw (pTime->secs);
    ssT_timeOp_Show_MathSmallRaw (pTime->msecs);
    ssT_timeOp_Show_MathSmallRaw (pTime->usecs);
#ifdef SST_OPTIN_TIME_NSECS_TU
    ssT_timeOp_Show_MathSmallRaw (pTime->nsecs);
#endif // SST_OPTIN_TIME_NSECS_TU
    ss_uiOp_emit_newline ();
    // plancks are not perceivable and range from 8-bit to 32-bit so leave them out
}   // ssT_timeOp_Show_TimeUnitsRaw

#endif    // __SST_TIME_DEF_H

