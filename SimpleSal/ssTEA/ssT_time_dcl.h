/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssT_time_dcl.h       data type, prototype declarations: the Time subsystem of ssTEA.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SST_TIME_DCL_H
#define __SST_TIME_DCL_H
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The focus of This software is : "Time" as a data type; operations implemented using Time values.
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As A Signal.note     start here to understand why math is necessary
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As An Arrow.note    the simplest way to do simple math using a Time
// -------------------------------------------------------------------------------------------------
// .\SimpleSal\ssDocs\ssTEA\Time As An FSM.note      the complex way to do complex math using a Time
// -------------------------------------------------------------------------------------------------
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// ssTEA's Agency only operates at a moment in Time that is occuring during the current "eon", which
// may be define as the largest unit of Time.
// math consumes and creates valid negative Time values, which cannot be used with ssTEA's Agency,
// The sign of the result from a math operation between two times indicates their relative order.
// -------------------------------------------------------------------------------------------------
#define PREV_EON                        (-1)            // only matters if result is too far back
#define THIS_EON                        (0)             // any moment perceived is within this EON
#define NEXT_EON                        (1)             // only matters if result is too far forward

#define EONvalueValid                   (THIS_EON)      // only Defined Times have EON == THIS_EON
#define EONvalueInvalid(signSignal)     (1*signSignal)  // Time is undefined in a direction

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This is not describing the code or data requirements to work with these values, just values.
// -------------------------------------------------------------------------------------------------
// Problem: you have a counter of usecs and you want to know how many whole seconds it contains,
//      solution: divide and use quotient           in ssHL-language, '/' operator, "divided by"
// or how many microseconds have occured since the last whole second occured
//      solution: divide and use remainder          in ssHL-language, '%' operator, "modulo"
// -------------------------------------------------------------------------------------------------
#define SST_DAYS_PER_EON    (1000)
#define SST_MAX_DAYS        (SST_DAYS_PER_EON-1)
#define SST_HOURS_PER_DAY   (24)
#define SST_MAX_HOURS       (SST_HOURS_PER_DAY-1)
#define SST_MINS_PER_HOUR   (60)
#define SST_MAX_MINS        (SST_MINS_PER_HOUR-1)
#define SST_SECS_PER_MIN    (60)
#define SST_MAX_SECS        (SST_SECS_PER_MIN-1)
#define SST_MSECS_PER_SEC   (1000)
#define SST_MAX_MSECS       (SST_MSECS_PER_SEC-1)
#define SST_USECS_PER_MSEC  (1000)
#define SST_MAX_USECS       (SST_USECS_PER_MSEC-1)
#define SST_NSECS_PER_USEC  (1000)
#define SST_MAX_NSECS       (SST_NSECS_PER_USEC-1)

// -------------------------------------------------------------------------------------------------
// These computations include a typecast to force the first number to a true 32-bit size, then math.
// This makes the math error "overflow 32 bits" occur due to truncation on assignment to a variable
// whose size is smaller than 32 bits, such as a TimeValueSmall possibly.
// -------------------------------------------------------------------------------------------------
//  max millis per minute:      60,000 or 0x0000EA60;       (note 16-bit)
#define MSECS_PER_MIN       ( ((TimeUnitsBig_t) SST_MSECS_PER_SEC) * SST_SECS_PER_MIN)
// max usecs per second:     1,000,000 or 0x000F4240;       (note 20-bit)
#define USECS_PER_SEC       ( ((TimeUnitsBig_t) SST_MSECS_PER_SEC) * SST_USECS_PER_MSEC)
 // max millis per hour:     3,600,000 or 0x0036EE80        (note 22-bit)
#define MSECS_PER_HOUR      ( ((TimeUnitsBig_t) MSECS_PER_MIN) * SST_MINS_PER_HOUR)
// max millis per day:      86,400,000 or 0x05265c00        (note 27-bit)
#define MSECS_PER_DAY       ( ((TimeUnitsBig_t) MSECS_PER_HOUR) * SST_HOURS_PER_DAY)

// -------------------------------------------------------------------------------------------------
// The data structures and relationships between data structures in ssTEA must be established.
// -------------------------------------------------------------------------------------------------
ss_macSNR_t     ssT_Initialize (void);
ss_macSNR_t     ssT_InitData_RunTime (void);
ss_macSNR_t     ssT_TheBigBang (void);

// -------------------------------------------------------------------------------------------------
// these convert AsciiA values into a valid binary Time descriptor value, or, binary to Ascii.
// -------------------------------------------------------------------------------------------------
boolean         ssT_stampOp_AsciiToBinary       (pAsciiA_t pBuff, ssT_pTime_t pTime);

void            ss_uiOp_Show_Time            (ssT_pTime_t pTime, boolean AddNewline);
void            ss_uiOp_Show_TimeLegend      (void);

boolean         ssT_stampOp_TimeOfBuild_AsciiToTime     (pAsciiA_t pBuff);

// -------------------------------------------------------------------------------------------------
// ssTEA supports the conversion of a series of Ascii characters into an ssTEA Time Variable.
// This maximum only refers to the data allocated while the conversion of any TimeStamp occurs.
// -------------------------------------------------------------------------------------------------
#define         TimeStamp_ALLOC (64)       // room for a fully-expressed Time Stamp at 48 chars.
#define         TimeStamp_MAX_I (TimeStamp_ALLOC-1)

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean         ssT_timeif_P1equalsZero             (ssT_pTime_t pTime);
boolean         ssT_timeif_P1equalsP2               (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void            ssT_mathOp_P1getsZero           (ssT_pTime_t pTime);
void            ssT_mathOp_P1getsP2             (ssT_pTime_t pP1, ssT_pTime_t pP2);

void            ssT_mathOp_P1getsP1plusP2       (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1);
void            ssT_mathOp_P1getsP2plusP3       (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

void            ssT_mathOp_P1getsP1minusP2      (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1);
void            ssT_mathOp_P1getsP2minusP3      (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
void            ssT_mathOp_P1getsP1_Compressed  (ssT_pTime_t pLvalue);
#endif // SST_OPTIN_TIME_COMPRESSED_TUS

// -------------------------------------------------------------------------------------------------
// Central to the operation of ssTEA, and the rules of Agency implemented by ssA in particular, is
// the answer to the question "if this Time before or after this other Time?".  Signal Algorithms
// have a dependency on the Expected Time of Arrival of information, or, Expected Time of Response.
// -------------------------------------------------------------------------------------------------
// There are a number of ways to decide:
//
// 1) use the Time Unit Math FSM to compute a result that indicates the relative order of two Times.
//   TU values initialize the TU Math FSM, the WF FSM executes the operations, a result is signaled.
//
// 2) compare Time Unit values relatively without considering any specific values or range.
//   TU values are assumed to be valid and in range as negative, zero, or positive integer values.
//
// 3) compare a compressed value of all the Time Units in a form retaining the Time Unit hierarchy.
//   TU values compress into two 32-bit integer values, direct comparisons respect binary order.
// -------------------------------------------------------------------------------------------------
#define  AddOperation    (true)
#define  SubOperation    (!AddOperation)

#ifdef SST_ONEOF_TIME_IS_AN_ARROW

void  ssT_mathOp_P1getsP1operP2_is_an_arrow (ssT_pTime_t pLvalue, ssT_pTime_t pOperand, boolean Operation);
#undef  TUMATHOP_P1operP2
#define TUMATHOP_P1operP2   ssT_mathOp_P1getsP1operP2_is_an_arrow

#endif  // SST_ONEOF_TIME_IS_AN_ARROW

// -------------------------------------------------------------------------------------------------
#ifdef SST_ONEOF_TIME_IS_AN_FSM

void  ssT_mathOp_P1getsP1operP2_as_an_FSM (ssT_pTime_t pLvalue, ssT_pTime_t pOperand, boolean Operation);
// bugbugbug should compiler not "see" LINE's TUMATHOP_P1andP2 unless both ONEOF_TIME_IS are defined???
#undef  TUMATHOP_P1operP2
#define TUMATHOP_P1operP2   ssT_mathOp_P1getsP1operP2_as_an_FSM

#endif  // SST_ONEOF_TIME_IS_AN_FSM

// -------------------------------------------------------------------------------------------------

void    ssT_mathOp_P1compareP2_WithLogic (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);
#undef  COMPARE_P1andP2
#define COMPARE_P1andP2     ssT_mathOp_P1compareP2_WithLogic

// -------------------------------------------------------------------------------------------------

void ssT_timeOp_Show_MathSmallRaw (TimeMathValueSmall_t MathValueSmall);
void ssT_timeOp_Show_TimeUnitsRaw (pAsciiA_t pName, ssT_Time_t *pTime, TimeMathValueSmall_t EON);

// -------------------------------------------------------------------------------------------------
// compressed Time value is independent from whether the uncompressed comparison is math or logic.
// -------------------------------------------------------------------------------------------------
#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
void ssT_mathOp_P1compareP2_Compressed (ssT_pTime_t pLvalue, ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);
#endif  // SST_OPTIN_TIME_COMPRESSED_TUS

// -------------------------------------------------------------------------------------------------
// These "If" functions make use of the "compare" functions as selected IN by the configuration.
// The state value is used both as a question "what is the state" and a report "this is the state".
// -------------------------------------------------------------------------------------------------
typedef enum ssT_math_CmpState_e
{
    eCmpState_Equal,
    eCmpState_After,
    eCmpState_Before
}  ssT_math_CmpState_t;

ssT_math_CmpState_t ssT_timeIf_P1compareP2      (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

boolean             ssT_timeIf_P1equalsP2       (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

boolean             ssT_timeIf_P1beforeP2       (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);
boolean             ssT_timeIf_P1afterP2        (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

boolean             ssT_timeIf_P1equalsORafterP2    (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);
boolean             ssT_timeIf_P1equalsORbeforeP2   (ssT_pTime_t pOperand1, ssT_pTime_t pOperand2);

ss_TimeUnit_t       ssT_timeIf_P1hasTUerror     (ssT_pTime_t pTime);

// -------------------------------------------------------------------------------------------------
// The following function declarations are functions used by ssTEA to calculate a counter value.
// A counter value, not a Time value.  A counter can be used to track micros (us) and millis (ms),
// the math difference between two us (or two ms) counters is a measurement of Time in us (or ms).
// The "usNow" and "msNow" functions are DECLARED here, the DEFINITION (code) must be in the App.
// for microseconds, need to know a hardware (HW) value, but milliseconds can be HW or math.
// If there is HW that is readable at low cost, msNow can ignore the parameter and return HW.
// If there is no HW returning millis, or, if it is expensive, msNow may be math-based and accurate.
// -------------------------------------------------------------------------------------------------
unsigned long int   ssT_rawCt_usNow             (void);
unsigned long int   ssT_rawCt_msNow             (TimeUnitsBig_t    usCt);

TimeUnitsBig_t      ssT_rawCt_SubPastFromNow    (TimeUnitsBig_t TimeNow, TimeUnitsBig_t TimeInPast);

TimeUnitsBig_t      ssT_rawCt_Min               (TimeUnitsBig_t a, TimeUnitsBig_t b);
TimeUnitsBig_t      ssT_rawCt_Max               (TimeUnitsBig_t a, TimeUnitsBig_t b);
TimeUnitsBig_t      ssT_rawCt_NextPlanck        (void);
#ifdef SST_OPTIN_TIME_NSECS_TU
TimeUnitsSmall_t    ssT_rawCt_NextNsec          (void);
#endif // SST_OPTIN_TIME_NSECS_TU

// -------------------------------------------------------------------------------------------------
// these are Time functions relating to ssTEA's use of Time to know when to grant Agency to Events.
// They bridge the gap between counters of chunks from HW and the value of the Current Time global.
// -------------------------------------------------------------------------------------------------
void            ssT_timeOp_InitQuickTimeForMath (void);
void            ssT_timeOp_Add_usToTimeCurr     (TimeUnitsBig_t bigvalue);

ss_TimeUnit_t   ssT_stampOp_ParseTimeUnit       (pAsciiA_t  p_achar);
boolean         ssT_stampOp_IsSeparator         (Ascii_t Ascii);

void            ssT_timeOp_Check_MathPolicy (ssT_pTime_t pTime, int EON);

// -------------------------------------------------------------------------------------------------
// When ssTEA shows a Time, the values shown are the Time Value as a whole, and also each one of the
// Time Units (TUs) has a Value.  Seeing the TU sign value is made useful in the Time Math TU_FSM.
// The TIME option is logical, based on the metadata in the Time Variable about the Time Value.
// The TU option is math, based on the evaluation of the signed integer value of any Time Unit.
// -------------------------------------------------------------------------------------------------
#define mSIGN_OF_TIME(pTime)   \
            ( (pTime->Zero) ? Ascii_Tilde : ((pTime->Positive) ? Ascii_Plus : Ascii_Minus) )
#define mSIGN_OF_TU(TU)        \
            ( (TU == 0)     ? Ascii_Tilde : ((TU > 0)          ? Ascii_Plus : Ascii_Minus) )

#endif // __SST_TIME_DCL_H

