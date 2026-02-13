/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssTEA_data_dcl.h     data type, prototype declarations: The highest-level within SimpleSal.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSTEA_DATA_DCL_H
#define __SSTEA_DATA_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Communication Between App and ssTEA.note]
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// the single bi-directional API data structure requires a unique value for each signal.  The actual
// value of the enumeration just has to be unique; the default works well with switch statements.
// -------------------------------------------------------------------------------------------------
typedef enum ss_ApiSigValue_e
{
    ss_ApiSigMsgValue_AgApifsm_On,          // add to the ssTEA database of events (instantiate)
    ss_ApiSigMsgValue_AgApifsm_Off,         // remove from the ssTEA database of events (deinstantiate)
    ss_ApiSigMsgValue_AgApifsm_Run,         // Agency Run FSM is defined, active, in this state
    ss_ApiSigMsgValue_AgApifsm_Reset,       // Agency Run FSM is defined, not active, in this state
    ss_ApiSigMsgValue_AgApifsm_Validate,    // request evaluation of ssE_AboutEv and adoption by ssTEA
    ss_ApiSigMsgValue_AgApifsm_State,       // update user's API agency data with ssTEA dynamic values
    ss_ApiSigMsgValue_AgApifsm_Stats,       // update user's statistics data with ssTEA dynamic values

    ss_ApiSigMsgValue_AgRunfsm_Start,       // a stopped/paused event may be started
    ss_ApiSigMsgValue_AgRunfsm_Pause,       // a stopped/agencying event may be paused
    ss_ApiSigMsgValue_AgRunfsm_Resume,      // a paused/agencying event may be resumed
    ss_ApiSigMsgValue_AgRunfsm_Stop,        // a paused/agencying event may be stopped
    ss_ApiSigMsgValue_AgRunfsm_State,       // update user's API agency data with ssTEA dynamic values
    ss_ApiSigMsgValue_AgRunfsm_Stats,       // update user's statistics data with ssTEA dynamic values

    // these are returned by ssTEA as sigDir ss_ApiSigDir_FromApi, in response to a ss_ApiSigValue.
    ss_ApiSigMsgValue_Yacked,               // to signaler: signal "received and data good"
    ss_ApiSigMsgValue_Nacked,               // to signaler: signal "received and data NOT good"

    ss_ApiSigMsgValue_flaw                  //
}   ss_ApiSigMsgValue_t;

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// SimpleSal's philosophical view explained below.
// SimpleSal's physical-world support of the Arduino context: K&R C compilable within C++ files.
#ifdef HOST_ONEOF_ssHL_defines_boolean
#include <stdbool.h>
typedef   bool    boolean;          // SimpleSal uses the word "boolean" in source code, thank you.
// within stdbool is a definition of the keywords "false" and "true",
// as part of the type definition. probably.

// -- ifdef/else/endif
#else   // not HOST_ONEOF_ssHL_defines_boolean
// -- ifdef/else/endif
// a bare-bones implementation of SimpleSal on a non-Arduino mesa does not require an Arduino build.
typedef   int     boolean;            // SimpleSal uses the word "boolean" in source code, thank you.
#define   false   ((boolean)0)
#define   true    (!false)
#endif  // HOST_ONEOF_ssHL_defines_boolean

// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\ssTEA Use Of Function Pointers.note]         "pointer to void" data type
// -------------------------------------------------------------------------------------------------
typedef void * pVoid_t;

// -------------------------------------------------------------------------------------------------
// when working with data structures using pointers to data structures, a value that indicates
// "I'm not pointing at anything" is helpful.  This is another purpose of "a pointer to void",
// and is essentially hiding the value of 0 (one that is assignable to any pointer) behind pVoid_t.
// -------------------------------------------------------------------------------------------------
#ifdef HOST_ONEOF_ssHL_defines_Null
#else   // not HOST_ONEOF_ssHL_defines_Null
#define NULL    ((pVoid_t) 0)
#endif  // HOST_ONEOF_ssHL_defines_Null

// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------
//
// ssTEA declaration of purpose:
//
// Manage the granting of Agency to App events, using a Time value to control when the grant occurs.
//
// In order for ssTEA to manage and grant Agency to App events, ssTEA must be granted agency first.
// The App software must grant Agency to ssTEA; this occurs during "loop" in Arduino, for example.
// The granting of Agency could similarly occur during the idle loop of an RTOS or OS application.
// The User's loop software calls ssTEA_Agency_Root function to grant Agency to ssTEA.
// The ssT Time process constructs the Time from reality or computes a Time based on pending events.
// [This is an easter egg: future versions of ssTEA will simulate the passage of Time for Events.]
// -------------------------------------------------------------------------------------------------
//
// Agencying in ssTEA means: as requested, grant agency to event(s) when the "OccursAt" Time arrives.
// The Time an Event wants to Occur At is independent of the accuracy of Time as perceived by ssTEA.
//
// A pace period allows Time to occur in any "chunk size": e.g., every 100 milliseconds.  Time occurs
// when ssTEA perceives it, ssTEA normally perceives Time by referring to the mesa ssTEA is running on.
//
// From App's view, the pace period is the Time between Event's chances to be granted Agency.
// From ssTEA's view, the pace period is the Time between "loop" granting agency to ssTEA_Agency_Root.
//
//    planck: ssTEA_Agency_Root whenever the User-controlled "loop" grants Agency to ssTEA_Agency_Root.
//    paced:  ssTEA_Agency_Root when a period of Time has passed since the time we ran ssTEA_Agency_Root.
//
// Pacing gates entry into the Agencying process; the Time used by the process is a separate idea.
//
// -------------------------------------------------------------------------------------------------
//  Time_base
//     reality: the Current Time is established by a source of information based on clocks and chips
//              that are in turn based on electrical properties in silicon, as electrons move about.
//
//              A request may be made of some mesa component that performs this calculation service.
//              A response value, a perception of Time based on electrical properties, is affected
//              by electrical properties themselves, and so the perception of Time is unreliable.
//              Electron flow changes as a result of temperature changes, so a circuit heating up
//              will find the clocks on that circuit slowing down.  This is actually used to find
//              out whether the circuit you are on is heating up, by detecting your clock slowing.
//              Algorithm: get count, execute 100 instructions needing Time X at frequency active,
//              get count.  The delta will be higher than expected when it takes longer to do 100.
//              mesas/chips use a high-quality counter running off the fundamental power supply.
//
//     virtual: Current Time is established using the OccursAt Time of the next event in Time.
//              for example, a 60-second, once-per-second, event will run every planck, 60 times.
// -------------------------------------------------------------------------------------------------
typedef enum ssTEA_Time_base_e
{
    ssTEA_Time_base_reality,
    ssTEA_Time_base_virtual,
    ssTEA_Time_base_error,
    ssTEA_Time_base_flaw
}   ssTEA_Time_base_t;
// -------------------------------------------------------------------------------------------------
typedef enum ssTEA_Agency_pace_e
{
    ssTEA_Agency_pace_planck,
    ssTEA_Agency_pace_period,
    ssTEA_Agency_pace_error,
    ssTEA_Agency_pace_flaw
}   ssTEA_Agency_pace_t;

#define ssTEA_Agency_pace_period_minimum  (1)
#define ssTEA_Agency_pace_period_maximum  (999)
// -------------------------------------------------------------------------------------------------
typedef enum ssTEA_state_e
{
    ssTEA_state_stopped,
    ssTEA_state_running,
    ssTEA_state_error,
    ssTEA_state_flaw
}   ssTEA_state_t;
// -------------------------------------------------------------------------------------------------
// In processor documentation, at the level of the math-related machine level language instructions
// such as "add register X to register Y" or "divide register N by 3", there is a description of
// all of the state information maintained by the processor as it relates to that instruction.
// The math instruction being executed right now was loaded from the pipeline based on a clock,
// the instruction consumes one or more clocks as needed to complete the instruction (as described).
// How does the instruction set the "result is zero" flag?  moving data does not set the
// wrongwrongwrong zero flag, the processor does not maintain a value of zero to compare against at this level.
// During math operation instructions, the logic in the addition section DOES know if the result is
// zero.  This is needed for jump instructions which do so based on whether the value is zero or not.
// ssTEA's Time abstraction creates a parallel model to integer numbers within a processor's logic.
//
// As with any data type such as "signed int", zeroness and positivity are managed abstractions.
//
//
// A variable that contains a value like that can be used in integer math and value comparisons.
// The relationship of "billions:millions:thousands:hundreds:tens:ones" is parallel to Time's
// "days:hours:minutes:seconds:millis:micros", and there are parallel relationships between units.
// -------------------------------------------------------------------------------------------------
// While doing math, all units defined reach a limit where the next unit up or down must be adjusted.
// This is why the decisecond and centisecond units are not part of the integrated view of Time.
// The integrated view of Time requires contiguous Time Units that are always involved in the math.
// For example, "minutes.millis" is not a supportable Time, but "minutes.0.millis" is.
// -------------------------------------------------------------------------------------------------
// Rule: when the longest unit (e.g., day) over- or under-flows during math, the Time is not Valid.
//    The longest unit over- or under-flows when the result of math required a change to the EON.
//    This is an interesting example of an instantaneous transition of state similar to VHDL level.
// -------------------------------------------------------------------------------------------------
// The "relative to parent Time Unit" means "the Time Unit that increments by one when the value
// of the child Time Unit increases past the maximum value for that Time Unit". An ssTEA Time Variable
// has parent-child relationships, such that a parent Time Unit may be changed be a child Time Unit.
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As A Signal.note       A Time Variable Contains Time Unit Variables
// -------------------------------------------------------------------------------------------------
typedef enum TimeUnit_e
{
    ss_TimeUnit_none,           // need to be able to say "not defined" (value is zero)
    ss_TimeUnit_EON,            // a Defined Time has an EON value of 0, which means "this EON".
    ss_TimeUnit_dy,             // day:                         N per eon
    ss_TimeUnit_hr,             // hour:                       24 per day
    ss_TimeUnit_mn,             // minute:                     60 per hour
    ss_TimeUnit_sc,             // second:                     60 per minute
    ss_TimeUnit_ms,             // millisecond:             1,000 per second
    ss_TimeUnit_us,             // microsecond:             1,000 per millisecond
    ss_TimeUnit_ns,             // nanosecond:              1,000 per microsecond
    ss_TimeUnit_pk              // planck:                  unperceivable
} ss_TimeUnit_e;

typedef ss_TimeUnit_e   ss_TimeUnit_t;

// -------------------------------------------------------------------------------------------------
// This is the central control mechanism definition about Time and Agency, as implemented by ssTEA.
//
// Time and Agency are both operated using a basic FSM allowing the independent operation of both.
//
// The Time Base and Agency Pace/Period values are the basis for the perception of Time by ssTEA.
//   What is the source of ssTEA's knowledge of "the Current Time" (base)?
//   How often may a moment in Time be perceived when agency is granted to ssTEA (pace/period)?
// -------------------------------------------------------------------------------------------------
typedef struct ssTEA_controlDesc_s
{
    ssTEA_state_t       Time_state;
    ssTEA_Time_base_t   Time_base;

    ssTEA_state_t       Agency_state;
    ssTEA_Agency_pace_t Agency_pace;
    unsigned long int   Agency_period;

    boolean             Show_Path;
    boolean             Show_Cause;
    boolean             Show_Signals;
    boolean             Show_Notes;
}  ssTEA_controlDesc_t, *ssTEA_pcontrolDesc_t;

typedef ssTEA_controlDesc_t    ssTEA_control_t;
typedef ssTEA_pcontrolDesc_t   ssTEA_pcontrol_t;

// =================================================================================================
//
// ssTEA Api declaration     The term "signal" does not refer to any Host Language keyword "signal".
//
// This is an abstraction, an idea that can be discussed, which describes architecture (relationship
// of objects and systems within a complete "product", or, a useful thing) for autonomous devices
// (human and machine) to communicate.  Included in "machine" is software granted the agency to act.
// The act of granting agency to software is a signal to the software that it is executing now.
// -------------------------------------------------------------------------------------------------
// Signal carriers usually work without error when they are enabled and functioning, because most
// signals are carried in an environment that is optimized to move the data without errors occuring.
// Designing a good solution is cheaper than the cost of moving the error data again.  The worst is
// the signaler is thinking (or not being able to know) that it worked, and then learning after a
// timeout that it didn't.   The design's quality moves data without errors within the intended use,
// and when the context introduces errors, the design must recognize and recover from those errors.
// Given highly layered communication signals, errors can occur from Data Link up to Presentation.
// -------------------------------------------------------------------------------------------------
// Management of the signal carrier is performed by a Media/Medium Access Controller (MAC).  A MAC
// uses the signal carrier to carry the signal.  Success will may be assessed, for example, as
// 0% to 100%.  The range 0-100 is arbitrary and varies by medium.  Every signal has its own range,
// and in fact the idea of "good signal" is often described by a ratio or algorithm using values.
// -------------------------------------------------------------------------------------------------
// Note that a half-duplex serial connection has one carrier, while full-duplex has two carriers.
// These both run on their own pair of wires, without knowledge of the other pair of wires.  In fact
// wires are twisted into pairs to reduce the amount of information each wire gets about the other.
// -------------------------------------------------------------------------------------------------
// Signal to noise ratio is usually written either S/N or (for use in ssHL) SNR.
// in ssTEA, where the signal carrier and medium are constructed from memory and algorithms,
// a purely digital world exists where the only "noise" is an actual error or mistake in some data.
// The "SNR" reported is a number that corresponds to a particular problem in the data
// itself, or, the data structures used to manage the signal (such as buffers to hold packets).
//
// medium-layer signaling:
//      there is a MAC accessing a medium which carries a signal,
//      and the MAC's ability to use the medium is expressed as an SNR.
//      An SNR is always specific to the medium (some indicate electrical
//      interference or the physics involved); ssTEA's signal is abstract.
//
// API signaling:
//      there is an API with requests and responses "traversing" memory in ssHL data structures.
//
//      The signal that is carried and has an SNR traverses the API medium under control of the App.
//      Any of the ssTEA services behind the API may indicate an API SNR specific to service.
//
// The relationship between the MAC SNR and the API SNR is that if the MAC SNR indicates a failure
// to communicate the signal, the API SNR should be assumed to be some safe value if used by code.
// -------------------------------------------------------------------------------------------------
typedef enum  ss_macSNR_e
{
    ss_macSNR_OK = 0,          // logic depends on value zero for OK
    ss_macSNR_signalInvalid,   // all signaling in the SimpleSal domain may refer to this
    ss_macSNR_MACerror,        // all signaling in the SimpleSal domain may refer to this
    ss_macSNR_notOK_reason1,   // signal carrier medium access controllers (MACs) may extend these
    ss_macSNR_notOK_reason2,
    ss_macSNR_flaw
} ss_macSNR_t;

typedef enum  ss_apiSNR_e
{
    ss_apiSNR_OK = 0,               // logic depends on value zero for OK
    ss_apiSNR_dataBad_Type,         // something is wrong with the event type field
    ss_apiSNR_dataBad_Time,         // something is wrong with the Time field picked by "recurs"
    ss_apiSNR_dataBad_pEvFunc,      // the pointer must appear to be a pointer to a function
    ss_apiSNR_dataBad_Incomplete,   // let a signal receiver say "problem, but maybe not an error".
    ss_apiSNR_databad_Channel,      // the Api Channel does not name a valid ssE Descriptor
    ss_apiSNR_stateWrong,           // not the right state for that request
    ss_apiSNR_stateInvalid,         // not a valid state
    ss_apiSNR_signalInvalid,        // not a valid signal type or value for the API
    ss_apiSNR_resourceMemory,       // there is not enough of a memory resource
    ss_apiSNR_resourceTime,         // there is not enough of a Time resource
    ss_apiSNR_EvExResult,           // when an Event return anything but Normal, says "check RunResult"
    ss_apiSNR_noEvAgencyData,       // except for Api FSM On, all signals must have a valid ssE_pAboutEv
    ss_apiSNR_flaw
} ss_apiSNR_t;

// -------------------------------------------------------------------------------------------------
// signals are sent To the Api and and signals are received From the Api.
// -------------------------------------------------------------------------------------------------
typedef enum ss_ApiSigDir_e
{
    ss_ApiSigDir_ToApi,
    ss_ApiSigDir_FromApi,
    ss_ApiSigDir_flaw
}   ss_ApiSigDir_t;

// -------------------------------------------------------------------------------------------------
// ssTEA controls the data that is critical to "true" operation of the event Agency engine, after
// event agency data structures containing requests are validated, the contents is copied to "true".
// Calls to the API must provide a magic number used by ssTEA to validate the caller to the API.
// When the Event Agency Api FSM is Off, there is no "true" data within ssTEA describing the Event.
// Signaling the Event Agency Api FSM to enter On_and_Reset state results in a channel magic number.
// -------------------------------------------------------------------------------------------------
typedef unsigned long ss_EvInfoChannel_t;
#define pChannelNull ((ss_EvInfoChannel_t) NULL)

// -------------------------------------------------------------------------------------------------
// SimpleSal needs to co-exist with an application and any software built along with the application,
// so the name space is formal and completely specified in SimpleSal source files.  Humans writing
// software with ONLY completely specified names may find it tiresome.  Any name can be renamed,
// to something shorter, something that has more direct meaning in the application, something crazy.
// -------------------------------------------------------------------------------------------------
#define Signal_ApiFsm_On    ss_ApiSigMsgValue_AgApifsm_On
#define Signal_ApiFsm_Off   ss_ApiSigMsgValue_AgApifsm_Off

#define AskThePumpFSMToStop ss_ApiSigMsgValue_AgRunfsm_Stop

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Reality Time
//
// Software is executing as Time passes and generally can get information about the current Time.
// This can range from "the number of microseconds since the processor powered on" or today's date.
//
// With any sized chunk of time, the 0-lengthed instant where one chunk stops and the next starts
// can be described with a value which can be used in math operations.  This is a "Point" in Time.
//
// What in existence, in Reality, can perceive a point in Time, and compare one Point to another?
//    minerals:    no perception or need for time, as far as we can tell with our observations.
//    plants:      a perception of Time based on the Sun or temperature or genetics or water or...
//    humans:      includes physical reality measurements useable by a human's software
//
// Math operations by humans, and software written by huamns, will reference two Points in Time.
// A Time is expressed as a collection of systematically interrelated chunk sizes: 24 hours,
// 60 minutes, 60 seconds, 1000 milliseconds, 1000 microseconds, 1000 nanoseconds, infinite plancks.
//
// Time is not in fact contiguous, so the expression of a point in Time consists of integer values.
// Therefore the results of math operations using Points in Time will always be integer values.
//
// How short is the shortest amount of time between two Points in Time that humans can perceive.
//          Answer: one second divided by (the number of all the atoms in the Universe): a Planck
//
// in reality, a Planck is a valid measure of time: several plancks occur between the movement of electrons.
// This is observable by humans, at the very limit of the ability to see Time: who measures out a Planck?
// The actual unique value of an ssT_Planck is unknowable, but the Point in Time occurs.   Any comparisons
// or computations cannot include a Planck value, but there are other uses for a unique identifier of Time.
//
// A ssT-generated planck value can be treated as a unique identifier.  A field in the 32-bit value
// is controlled and manipulated to create information helpful while working with time-based events.
// Extracting the index field allows comparisons; more important, visually while tracking with ssTEA.
// The bit width of the field controls the number of unique, sequential, Planck values that can be
// uniquely "created" (as a result of initialization or as the destination of an operation result.
// Regardless of the field's bit width, the current planck is the newest, and its index is the newest.
// so a bit width of one would have a value of 0 then 1 then 0; 1 is a later planck than 0 in Time.
//
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Abstracted Time
//
// what is provided, using legacy ssHL, by the API to allow logic and action to be performed by code
// using Time in the same way that software can perform logic and action using standard ssHL data types?
//
//  1) A Time Descriptor  (which is the same set of information a CPU uses to do integer math)
//      * has an indication of the positivity or negativity of the complete specification of a Time.
//      * has an indication whether the value is zero (zero is by definition considered positive).
//      * has a set of fields with values constrained by their function within an integrated Time.
//
//  2) Time Operations and Operands  (which are the same set of operands a CPU uses to do integer math)
//      * is this Time before that Time?  Are these two Times equal?  Add these two Times together.
//      * The TimeAPI is usually not about specific Time values, but the relationship between Times.
//      * A Valid description of the time "zero" can be obtained from ssTEA, used with other Times.
//
// The ssTEA  API abstracts the data and math by providing all of the operands that operations require.
// -------------------------------------------------------------------------------------------------
// Current Time
//
//   The Time value for NOW is measured (reality) or calculated (virtual), then presented as an idea:
//   "the Current Time".  All references to the idea "the Current Time" must refer to ssTEA's value
//   when interacting with ssTEA or calculating a future Time by adding the Current Time to a Time.
//
// -------------------------------------------------------------------------------------------------
// Math considerations for Counters (using 32-bit counters of milliseconds to measure Time)
//
//    The use of counters is based on using the number of milliseconds since the Big Bang, over "loop" iterations.
//          What matters when using millisecond and microsecond counters is how the math works when they wrap.
//    A "32-bit unsigned long int" is the base counter type, this type has a maximum value of (4,294,967,295).
//    Of course, an 8-bit microprocessor will not have 32-bit math registers but will be able to do 32-bit math.
//
// Math considerations for Time Descriptor
//
//    A Time may be positive or negative, or zero.
//    As part of comparing the values of two Times, for example.  One way to compare whether TimeA is before
//      TimeB is to compare every unit between them and see which Time is greater, in a coherent unit order.
//      Another way to compare is to use the subtract operation (which you already have), and see result.
///
//      A math operation result value that is a negative Time will have
//          all fields nonnegative and within the range of the field's units, and, Positive == false.
//
//      This might be called a "computed Time" or "compared Time", and the usefulness is in the attributes
//      of a resulting Time.  If you subtract 8 from 3, you get a negative number with the value 5.  The
//      negative result tells you that 8 is greater than 3, or, you could compare individually each unit.
//
//    A "Moment in Time" specifies the Time anything occurs (or is imagined to occur):
//      a point in time which is greater than or equal to zero, zero being the Time of the Big Bang.
//    Time fields in the ssTEA API are always moments in Time, including recurring Time interval values.
//
//    The type defined for fields within a Time is signed and larger than the basic unit where possible
//    because the borrow and carry operations in signed math create an answer bigger than the question.
//
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
typedef unsigned short int  TimeUnitsSmall_t;       // assumed to be 16-bits, biggest TUs use 10 bits
typedef unsigned long  int  TimeUnitsBig_t;         // expected to be 32-bits containing an unsigned integer

typedef signed         int  TimeMathValueSmall_t;   // big enough to handle any TimeUnitsSmall values
typedef signed   long  int  TimeMathValueBig_t;     // 31-bit range limited by MSB bit used for sign

typedef TimeUnitsSmall_t    TimeUnitValue_t;        // each TU has a range, all of them within Small
typedef TimeUnitValue_t    *pTimeUnitValue_t;       // TUs are pointed to by the Math FSM

typedef TimeUnitsBig_t      TimeUnitValueBig_t;     // only the plancks have a Big Time Units count

typedef unsigned long  int  TimeMomentValue_t;      // compacted version of a Time Descriptor

// -------------------------------------------------------------------------------------------------
// The fields [days:hours:mins:secs:ms:us] are maintained as an "integrated value", a Time.
// This allows: "if (pTime->hours > 12) && (pTime->mins > 30))" to say "is it after 12:30pm?".
// The initial value of fields [ns:pl] are psuedo-randomized (with planck having incrementing N bits).
// Because nanoseconds exceed ssT's ability to perceive Time, the field can be used for anything.
// -------------------------------------------------------------------------------------------------
typedef struct ssT_TimeDesc_s
{
    boolean             Valid;          // true: all Time Unit values are valid and coherent
    boolean             Positive;       // true: value zero or nonzero positive; false: negative
    boolean             Zero;           // true: value zero; false: value nonzero.

    // ---------------------------------------------------------------------------------------------
    // it might seem space-efficient to use an 8-bit byte for hours, mins, seconds, BUT the math
    // operations for a unit refer to other units to perform borrow or carry needed for the math,
    // and the ability to do this is based on all of the units fields being of the same data type.
    // ---------------------------------------------------------------------------------------------
    // DURING a math operation, fields may be positive or negative, may be outside the valid range.
    //     Math operations all reference 16-bit signed fields, plenty of bits above the maximum.
    // AFTER any math operation, all fields have a value in range, whose sign matches Positive.
    // ---------------------------------------------------------------------------------------------
    // DURING any math operation, the Valid flag may be set to false, due to overflow or underflow.
    // An "Eon" is a Time Unit encompassing all Times, and therefore one without a Parent Time Unit.
    // Therefore all Defined Time is within the current Eon, because there is no TU for borrow/carry.
    // The current Eon is implicit in the math, changing the Eon while doing math is an "edge" error.
    // An edge error only exists when the result of a math operation is negative and used as a Time?
    // ---------------------------------------------------------------------------------------------
    TimeUnitValue_t     days;           // 0-999    (requires 10 bits: 0000000000 to 1111100111
    TimeUnitValue_t     hours;          // 0-23     (requires  5 bits:      00000 to      10111
    TimeUnitValue_t     mins;           // 0-59     (requires  6 bits:     000000 to     111011
    TimeUnitValue_t     secs;           // 0-59     (requires  6 bits:     000000 to     111011
    TimeUnitValue_t     msecs;          // 0-999    (requires 10 bits: 0000000000 to 1111100111
    TimeUnitValue_t     usecs;          // 0-999    (requires 10 bits: 0000000000 to 1111100111
#ifdef SST_OPTIN_TIME_NSECS_TU
    TimeUnitValue_t     nsecs;          // even if use of field enabled, values excluded from math
#endif  // SST_OPTIN_TIME_NSECS_TU
    TimeUnitValueBig_t  plancks;        // psuedo-random 32-bits with a secret index 8-bit field

    // ---------------------------------------------------------------------------------------------
    // A nonzero bit in either momentU or momentL: each field matches each TU's absolute value.
    // The Time 0 is represented with all bits zero, except the zero bit (bit 0 of momentL) is one.
    // A negative Time result is represented with the sign bit set and absolute values of TU values.
    // ---------------------------------------------------------------------------------------------
    TimeMomentValue_t   momentU;        // |days(10)|gap|hours(6)|gap|mins   (6)|gap|secs(6)|(sign)
    TimeMomentValue_t   momentL;        // |  ms(10)|gap|  us(10)|gap|plancks(8)|gap        |(zero)

}   ssT_TimeDesc_t, *ssT_pTimeDesc_t;

typedef ssT_TimeDesc_t  ssT_Time_t;
typedef ssT_pTimeDesc_t ssT_pTime_t;
#define ssT_pTimeNull ((ssT_pTime_t) NULL)

#define  ssT_PLANCK_INDEX_FIELD (0xFF)

#define aMASK_1bit              (0x01)
#define aMASK_6bits             (0x3F)
#define aMASK_10bits            (0x3FF)

#define MASK_1bit(x32bits)      (x32bits & aMASK_1bit)
#define MASK_6bits(x32bits)     (x32bits & aMASK_6bits)
#define MASK_10bits(x32bits)    (x32bits & aMASK_10bits)

#define ssT_Moment_oust_sign(x32bits)    (MASK_1bit  (x32bits >> 31))
#define ssT_Moment_oust_days(x32bits)    (MASK_10bits(x32bits >> 21))
#define ssT_Moment_oust_hours(x32bits)   (MASK_6bits (x32bits >> 14))
#define ssT_Moment_oust_mins(x32bits)    (MASK_6bits (x32bits >>  7))
#define ssT_Moment_oust_secs(x32bits)    (MASK_6bits (x32bits >>  0))

#define ssT_Moment_oust_ms(x32bits)      (MASK_10bits(x32bits >> 22))
#define ssT_Moment_oust_us(x32bits)      (MASK_10bits(x32bits >> 11))
#define ssT_Moment_oust_plancks(x32bits) (MASK_10bits(x32bits >>  0))

#define ssT_Moment_embed_sign(x32bits)    (MASK_1bit  (x32bits) << 31)
#define ssT_Moment_embed_days(x32bits)    (MASK_10bits(x32bits) << 21)
#define ssT_Moment_embed_hours(x32bits)   (MASK_6bits (x32bits) << 14)
#define ssT_Moment_embed_mins(x32bits)    (MASK_6bits (x32bits) <<  7)
#define ssT_Moment_embed_secs(x32bits)    (MASK_6bits (x32bits) <<  0)

#define ssT_Moment_embed_ms(x32bits)      (MASK_10bits(x32bits) >> 22)
#define ssT_Moment_embed_us(x32bits)      (MASK_10bits(x32bits) >> 11)
#define ssT_Moment_embed_plancks(x32bits) (MASK_10bits(x32bits) >>  0)

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Statistics Issues.note]    describes types and a hierarchical data tree
// -------------------------------------------------------------------------------------------------
#ifdef SSA_OPTIN_STATS_FSM
typedef Bits8_t             TinyStatCt_t;
typedef unsigned int        BigStatCt_t;

typedef struct ssE_EvExAction_Runfsm_StatsDesc_s
{
    BigStatCt_t     AgencyGranted_ct;
    BigStatCt_t     AbbyNormal_ct;

    BigStatCt_t     Auto_RecurrenceAg_ct;
    BigStatCt_t     Auto_StopAg_ct;
    BigStatCt_t     Auto_PauseAg_ct;

    BigStatCt_t     Self_RecurrenceAg_ct;
    BigStatCt_t     Self_StopAg_ct;
    BigStatCt_t     Self_PauseAg_ct;

}  ssE_EvExAction_Runfsm_StatsDesc_t, *ssE_pEvExAction_Runfsm_StatsDesc_t;

typedef ssE_EvExAction_Runfsm_StatsDesc_t     ssE_EvExAction_Runfsm_Stats_t;
typedef ssE_pEvExAction_Runfsm_StatsDesc_t    ssE_pEvExAction_Runfsm_Stats_t;
#define ssE_pEvExAction_Runfsm_StatsNull      ((ssE_pEvExAction_Runfsm_Stats_t) NULL)
// -------------------------------------------------------------------------------------------------
typedef struct ssE_AppAction_Runfsm_StatsDesc_s
{
    BigStatCt_t     StoppedToAgencying_ct;
    BigStatCt_t     StoppedToPaused_ct;
    BigStatCt_t     PausedToAgencying_ct;
    BigStatCt_t     PausedToStopped_ct;
    BigStatCt_t     AgencyingToStopped_ct;
    BigStatCt_t     AgencyingToPaused_ct;
    BigStatCt_t     AgencyingToAgencying_ct;
}   ssE_AppAction_Runfsm_StatsDesc_t, *ssE_pAppAction_Runfsm_StatsDesc_t;

typedef ssE_AppAction_Runfsm_StatsDesc_t  ssE_AppAction_Runfsm_Stats_t;
typedef ssE_pAppAction_Runfsm_StatsDesc_t ssE_pAppAction_Runfsm_Stats_t;
#define ssE_pAppAction_Runfsm_StatsNull   ((ssE_pAppAction_Runfsm_Stats_t) NULL)
// -------------------------------------------------------------------------------------------------
typedef struct ssA_Runfsm_StatsDesc_s
{
    ssE_AppAction_Runfsm_Stats_t   App;
    ssE_EvExAction_Runfsm_Stats_t  EvEx;
}  ssA_Runfsm_StatsDesc_t, *ssA_pRunfsm_StatsDesc_t;

typedef ssA_Runfsm_StatsDesc_t      ssA_Runfsm_Stats_t;
typedef ssA_pRunfsm_StatsDesc_t     ssA_pRunfsm_Stats_t;
#define ssA_pRunfsm_StatsNull       ((ssA_pRunfsm_Stats_t) NULL)
// -------------------------------------------------------------------------------------------------
typedef struct ssA_Apifsm_StatsDesc_s
{
    BigStatCt_t     Off2OnReset_ct;
    BigStatCt_t     OnReset2Off_ct;
    BigStatCt_t     Off2OnRunning_ct;
    BigStatCt_t     OnRunning2Off_ct;
    BigStatCt_t     OnRunning2OnReset_ct;
    BigStatCt_t     OnReset2OnRunning_ct;
}   ssA_Apifsm_StatsDesc_t, *ssA_pApifsm_StatsDesc_t;

typedef ssA_Apifsm_StatsDesc_t      ssA_Apifsm_Stats_t;
typedef ssA_pApifsm_StatsDesc_t     ssA_pApifsm_Stats_t;
#define ssA_pApifsm_StatsNull       ((ssA_pApifsm_Stats_t) NULL)
// -------------------------------------------------------------------------------------------------
typedef struct ssA_All_StatsDesc_s
{
#ifdef SSA_OPTIN_STATS_APIFSM
    ssA_Apifsm_Stats_t      Apifsm;
#endif  // SSA_OPTIN_STATS_APIFSM
#ifdef SSA_OPTIN_STATS_RUNFSM
    ssA_Runfsm_Stats_t      Runfsm;
#endif  // SSA_OPTIN_STATS_RUNFSM
}  ssA_All_StatsDesc_t, *ssA_pAll_StatsDesc_t;

typedef ssA_All_StatsDesc_t     ssA_All_Stats_t;
typedef ssA_pAll_StatsDesc_t    ssA_pAll_Stats_t;
#define ssA_pAll_StatsNull      ((ssA_pAll_Stats_t) NULL)
#endif // SSA_OPTIN_STATS_FSM

#endif  // __SSTEA_DATA_DCL_H

