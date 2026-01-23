/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_TUmath_dcl.h    data type, prototype declarations: Time math operations at the ssUI level.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_TUMATH_DCL_H
#define __SSUI_TUMATH_DCL_H
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// The main reason to create a variable with a Time in it is to compare that value to another Time.
// A Time Variable is a data structure containing the values used to perform math operations.
// A comparison for equality between Time variables will compare each equivalent structure field
// for equality; the question of one Time being greater than another will compare each field also.
// For equality, the order of evaluation does not matter; for greater than, the order is important.
// -------------------------------------------------------------------------------------------------
typedef enum ssUI_math_CompOps_e
{
    eComp_isEQ          = Bit0,                             // 0x01 // mutually exclusive with NEQ
    eComp_isNEQ         = Bit1,                             // 0x02 // no other bits allowed

    eComp_isAfter       = Bit4,                             // 0x10
    eComp_isBefore      = Bit5,                             // 0x20

    eComp_isEQorAfter   = (eComp_isEQ | eComp_isAfter),     // 0x01 | 0x10
    eComp_isEQorBefore  = (eComp_isEQ | eComp_isBefore),    // 0x01 | 0x20

    eComp_isMath        = Bit7 // 0x80  // "not a comparison operation"

} ssUI_math_CompOps_t;

typedef struct ssUI_MathOpDesc_s
{
    ssT_pTime_t             pLvalue;
    ssT_pTime_t             pOperand1;
    ssT_pTime_t             pOperand2;

    boolean                 IsSubtractOp;   // if not subtracting, adding

    ssUI_math_CompOps_t     CompareOper;   // what is the comparison or math Op being requested?
    ssUI_math_CompOps_t     compResult;     // what is the computed result of the comp/math Op?

    pAsciiA_t               pLvalue_Name;
    pAsciiA_t               pOperand1_Name;
    pAsciiA_t               pOperand2_Name;

}   ssUI_MathOpDesc_t, *ssUI_pMathOpDesc_t;

typedef ssUI_MathOpDesc_t   ssUI_mathOp_t;
typedef ssUI_pMathOpDesc_t  ssUI_pMathOp_t;

#define ssUI_pMathOpNull     ((ssUI_pMathOp_t) NULL)
// -------------------------------------------------------------------------------------------------
boolean     ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (pAsciiA_t pVarName);

void        ssUI_mathOp_Make_TimeMathWork   (ssUI_pMathOp_t pMO);
void        ssUI_mathOp_ClearMO_exceptLvalue(ssUI_pMathOp_t pMO);

void        ssUI_mathOp_Show_OneMathTerm    (pAsciiA_t pdesc, pAsciiA_t pname);
void        ssUI_mathOp_Show_AllMathTerms   (ssUI_pMathOp_t pMO, boolean ShowPreOp);
#define     SHOW_PREOP  true

#endif  // __SSUI_TUMATH_DCL_H

