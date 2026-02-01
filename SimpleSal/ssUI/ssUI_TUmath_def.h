/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_TUmath_def.h    data and software definitions:  Time math operations at the ssUI level.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_TUMATH_DEF_H
#define __SSUI_TUMATH_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// when math operations occur, this variable points to the math operation's data variables.
// The data structure contains UI names for Lvalue and operands for debug and informational purpose.
// -------------------------------------------------------------------------------------------------
ssUI_mathOp_t    gLive_MO;
ssUI_pMathOp_t   gpLive_MO = &gLive_MO;

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void    ssUI_mathOp_Make_TimeMathWork (ssUI_pMathOp_t pMO)
{
    if (pMO->pOperand2)
    {
        switch (pMO->CompareOper)
        {
            // operand1, math op, and operand2 present; do the math to create a new value
            case eComp_isMath        :
                if (pMO->IsSubtractOp)
                {   // Time1 = Time2 - Time3
                    ssT_mathOp_P1getsP2minusP3 (pMO->pLvalue, pMO->pOperand1, pMO->pOperand2);
                }
                else
                {   // Time1 = Time2 + Time3
                    ssT_mathOp_P1getsP2plusP3 (pMO->pLvalue, pMO->pOperand1, pMO->pOperand2);
                }
                break;
            // a Time Unit to Time Unit comparison results in equal or not equal
            case eComp_isNEQ            :
                if (ssT_timeif_P1equalsP2 (pMO->pOperand1, pMO->pOperand2))
                {
                    pMO->compResult = eComp_isEQ;
                }
                else
                {
                    pMO->compResult = eComp_isNEQ;
                }
                // no other comparison bits are considered: if NEQ, no other bits possible
                break;
            // a relative-time-based comparison between the values results in after, equal, before
            case eComp_isAfter          :
            case eComp_isEQ             :
            case eComp_isBefore         :
                if (ssT_timeif_P1equalsP2 (pMO->pOperand1, pMO->pOperand2))
                {
                    pMO->compResult = eComp_isEQ;
                }
                else
                {
                    if (ssT_timeIf_P1afterP2 (pMO->pOperand1, pMO->pOperand2))
                    {
                        pMO->compResult = eComp_isAfter;
                    }
                    else
                    {
                        pMO->compResult = eComp_isBefore;
                    }
                }
                break;
            case eComp_isEQorAfter        :
                if (ssT_timeIf_P1equalsORafterP2 (pMO->pOperand1, pMO->pOperand2))
                {
                    pMO->compResult = eComp_isEQorAfter;
                }
                else
                {
                    pMO->compResult = eComp_isBefore;
                }
                break;
            case eComp_isEQorBefore        :
                if (ssT_timeIf_P1equalsORbeforeP2 (pMO->pOperand1, pMO->pOperand2))
                {
                    pMO->compResult = eComp_isEQorBefore;
                }
                else
                {
                    pMO->compResult = eComp_isAfter;
                }
                break;
        }   // switch
    }   // operand1, math op, and operand2 present
    else
    {   // if only operand1 present, there can be no compare or math operations
        if (pMO->pOperand1)
        {   // Time1 = Time2
            ssT_mathOp_P1getsP2 (pMO->pLvalue, pMO->pOperand1);
        }
    }   // no operand 2
}   // ssUI_mathOp_Make_TimeMathWork
// -------------------------------------------------------------------------------------------------
// once we've found an Lvalue, we need a clean well-defined data set to do math operations.
//  pMO->pLvalue = <address of destination for result>; must have occured for mathOp to be valid.
// -------------------------------------------------------------------------------------------------
void ssUI_mathOp_ClearMO_exceptLvalue (ssUI_pMathOp_t pMO)
{
    pMO->pOperand1 = ssT_pTimeNull;
    pMO->pOperand2 = ssT_pTimeNull;

    pMO->IsSubtractOp = false;
    pMO->CompareOper = eComp_isMath;         // default state is positive numbers
    pMO->compResult = eComp_isEQ;             // that will turn out to be equal
}   // ssUI_mathOp_ClearMO_exceptLvalue
// =================================================================================================
// -------------------------------------------------------------------------------------------------
boolean ssUI_mathOp_Match_Varname_SetLvalue_ClearMO (pAsciiA_t pVarName)
{
    if (gpLive_MO->pLvalue == ssT_pTimeNull)
    {
        gpLive_MO->pLvalue = ssUI_dbOp_Get_pTimeVar_Using_VarName (pVarName);
    }

    if (gpLive_MO->pLvalue)
    {
        if (gpLive_MO->pLvalue == ssT_gpTime_Ephemeral)
        {   // bugbugbug why can't ephemeral be used as lvalue?  what can't it be implied in "math 10sc"
            // bugbugbug is there any purpose in being able to evaluate math TimeStamps in command input?
            ss_uiOp_emit_qAsciiA ("  an Ephemeral TimeStamp cannot be the destination (Lvalue) of an assignment");
            ss_uiOp_emit_newline ();
            return (false);
        }
    }
    else
    {
        ss_uiOp_emit_qAsciiA ("  no match found for Time Variable name");
        ss_uiOp_emit_newline ();
        return (false);
    }

    // ClearMO sets pOperand1 to ssT_pTimeNull, after making the call to initialize the data,
    // check the pOperand1 field to make sure the assignment worked on the memory pointed to.
    // It is true that the gpLive_MO data set has an Lvalue that is non-NULL and also that is
    // not the same as the Ephemeral, but that doesn't mean it points to changeable memory.
    ssUI_mathOp_ClearMO_exceptLvalue (gpLive_MO);
    gpLive_MO->pLvalue_Name = ssUI_dbOp_Get_pName_Using_VarName (pVarName);
    // to catch allocation of data that appears to be constant to the ssHL compiler; maybe in flash.
    if (gpLive_MO->pOperand1 != ssT_pTimeNull)
    {
        // This is a fatal flaw and so is just blasted out
        ss_uiOp_emit_qAsciiA ("*** In {ssUI_mathOp_Match_Varname_SetLvalue_ClearMO},");
        ss_uiOp_emit_newline ();
        ss_uiOp_emit_qAsciiA ("*** the mathOp data structure was unmodified after software cleared it.");
        ss_uiOp_emit_newline ();
        return (false);
    }
    return (true);
}   //  ssUI_mathOp_Match_Varname_SetLvalue_ClearMO
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define SHOWMATH_DESC_LIMIT  10
#define SHOWMATH_NAME_LIMIT  25

void ssUI_mathOp_Show_OneMathTerm (pAsciiA_t pdesc, pAsciiA_t pName)
{
    int i, max_i;

    if (!pName)
    {
        // This is a fatal flaw and so is just blasted out
        ss_uiOp_emit_qAsciiA ("codeflaw: null term name pointer in show math term function");
        ss_uiOp_emit_newline ();
        return;
    }

    for (i=0; i < SHOWMATH_DESC_LIMIT; i++)
    {
        if (pdesc[i])
        {
            ss_uiOp_emit_1 (pdesc[i]);
        }
        else
        {
            break;
        }   // NUL value
    }   // up to limit

    max_i = (SHOWMATH_DESC_LIMIT-i);
    for (i=0; i < max_i; i++)
        ss_uiOp_emit_1 (Ascii_Space);
    ss_uiOp_emit_1 (Ascii_Colon);

    for (i=0; i < SHOWMATH_NAME_LIMIT; i++)
    {
        if (pName[i])
        {
            ss_uiOp_emit_1 (pName[i]);
        }
        else
        {
            break;
        }   // NUL value
    }   // up to limit

    max_i = (SHOWMATH_NAME_LIMIT-i);
    for (i=0; i < max_i; i++)
        ss_uiOp_emit_1 (Ascii_Space);
}   // ssUI_mathOp_Show_OneMathTerm
// -------------------------------------------------------------------------------------------------
void    ssUI_mathOp_Show_AllMathTerms (ssUI_pMathOp_t pMO, boolean IsPreOp)
{
#ifdef SST_OPTIN_TIME_DEBUG_ADDRESS
    ss_uiOp_emit_newline ();
    ss_uiOp_emit_qAsciiA (( (IsPreOp) ? "pre-op " : "post-op" ));
    ss_uiOp_emit_qAsciiA (":   pLvalue  <");
    ss_uiOp_emit_Hex_32bits   ((unsigned long) pMO->pLvalue);
    ss_uiOp_emit_qAsciiA (">     pOperand1 <");
    ss_uiOp_emit_Hex_32bits   ((unsigned long) pMO->pOperand1);
    ss_uiOp_emit_qAsciiA (">     pOperand2 <");
    ss_uiOp_emit_Hex_32bits   ((unsigned long) pMO->pOperand2);
    ss_uiOp_emit_1      (Ascii_Rarrow);
    ss_uiOp_emit_newline ();
#endif  // SST_OPTIN_TIME_DEBUG_ADDRESS
    if (!IsPreOp)
    {
        ss_uiOp_emit_newline ();
    }

    if (pMO->pOperand2)
    {   // operand1, math op, and operand2 present; Lvalue must be memory
        if (IsPreOp)
        {
            ss_uiOp_emit_newline ();
            ssUI_mathOp_Show_OneMathTerm (S("operand1"), pMO->pOperand1_Name);
            ss_uiOp_emit_1 (Ascii_Space);
            ss_uiOp_emit_1 (Ascii_Space);
            ss_uiOp_Show_Time (pMO->pOperand1, lfY);
            ssUI_mathOp_Show_OneMathTerm (S("operand2"), pMO->pOperand2_Name);

            switch (pMO->CompareOper)
            {
                case eComp_isMath     :
                    ss_uiOp_emit_1 ((pMO->IsSubtractOp) ? Ascii_Minus : Ascii_Plus);
                    ss_uiOp_emit_1 (Ascii_Space);
                    break;
                case  eComp_isAfter     :
                    ss_uiOp_emit_1 (Ascii_LT);
                    ss_uiOp_emit_1 (Ascii_Space);
                    break;
                case  eComp_isEQorAfter    :
                    ss_uiOp_emit_1 (Ascii_LT);
                    ss_uiOp_emit_1 (Ascii_EQ);
                    break;
                case  eComp_isEQ      :
                    ss_uiOp_emit_1 (Ascii_EQ);
                    ss_uiOp_emit_1 (Ascii_EQ);
                    break;
                case  eComp_isNEQ     :
                    ss_uiOp_emit_1 (Ascii_Epoint);
                    ss_uiOp_emit_1 (Ascii_EQ);
                    break;
                case  eComp_isBefore  :
                    ss_uiOp_emit_1 (Ascii_GT);
                    ss_uiOp_emit_1 (Ascii_Space);
                    break;
                case  eComp_isEQorBefore    :
                    ss_uiOp_emit_1 (Ascii_GT);
                    ss_uiOp_emit_1 (Ascii_EQ);
                    break;
            }   // switch

            ss_uiOp_Show_Time (pMO->pOperand2, lfY);
        }   //
        else            // results phase
        {
            ssUI_mathOp_Show_OneMathTerm (S("Lvalue"), pMO->pLvalue_Name);
            ss_uiOp_emit_1 (Ascii_Assign);
            ss_uiOp_emit_1 (Ascii_Space);
            ss_uiOp_Show_Time (pMO->pLvalue, lfN);
            if (pMO->CompareOper != eComp_isMath)
            {
                ss_uiOp_emit_newline ();
                ss_uiOp_emit_pAsciiA (pMO->pOperand1_Name);
                switch (pMO->compResult)
                {
                    // result isMath not possible, complete switch cases
                    case eComp_isMath       :  ss_uiOp_emit_qAsciiA (" <result error> ");  break;
                    case eComp_isAfter      :  ss_uiOp_emit_qAsciiA (" is after ");        break;
                    case eComp_isEQorAfter  :  ss_uiOp_emit_qAsciiA (" is after or equal to "); break;
                    case eComp_isEQ         :  ss_uiOp_emit_qAsciiA (" is equal to ");     break;
                    case eComp_isNEQ        :  ss_uiOp_emit_qAsciiA (" is not equal to "); break;
                    case eComp_isBefore     :  ss_uiOp_emit_qAsciiA (" is before ");       break;
                    case eComp_isEQorBefore :  ss_uiOp_emit_qAsciiA (" is before or equal to "); break;
               }   // switch comparison result
                ss_uiOp_emit_pAsciiA (pMO->pOperand2_Name);
            }   // comparison made
        }
    }   // operand1, math op, and operand2 present
    else
    {   // operand1 present, show result
        if (pMO->pOperand1)
        {
            if (!IsPreOp)
            {
                ssUI_mathOp_Show_OneMathTerm (S("Lvalue =="), pMO->pLvalue_Name);
                ss_uiOp_emit_1 (Ascii_EQ);
                ss_uiOp_emit_1 (Ascii_Space);
            }   // results phase
        }
        else        // only Lvalue present
        {
            if (!IsPreOp)
            {
                ssUI_mathOp_Show_OneMathTerm (S("Time == "), pMO->pLvalue_Name);
                ss_uiOp_emit_1 (Ascii_Space);
            }   // results phase
        }   // only Lvalue: this is the result of a constant string entered as a math term

        if (!IsPreOp)
        {
            ss_uiOp_Show_Time (pMO->pLvalue, lfN);
        }
    }   // no operand 2, just operand 1
}   // ssUI_mathOp_Show_AllMathTerms
// -------------------------------------------------------------------------------------------------

#endif  // __SSUI_TUMATH_DEF_H

