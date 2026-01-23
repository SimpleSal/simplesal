/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * ssUI_menucore_dcl.h  data type, prototype declarations: command and parameter handlers for UI.
 *
 ---------------------------------------------------------------------------------------------------- */
#ifndef __SSUI_MENUCORE_DCL_H
#define __SSUI_MENUCORE_DCL_H

// -------------------------------------------------------------------------------------------------
eHandlerResult_t        H_ss                    (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_Time               (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_TimeStart          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_TimeStop           (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_TimeBigBang        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_TimeStats          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_Agency             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_AgencyPace         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_AgencyPeriod       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_AgencyStart        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_AgencyStop         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_AgencyStats        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_MsgSSM             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_RO                 (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_ROs_LED             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_ROs_Ascii           (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_ROs_Matrix          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_ROs_Describe        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ss_ROs_Update          (int tkn_i, boolean moreInputTkns);


// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
eHandlerResult_t        H_evapi                 (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_clearEvApi         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_apivar             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access_on          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access_off         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access_include     (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access_exclude     (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access_lock        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_access_unlock      (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_on          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_off         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_run         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_reset       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_validate    (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_state       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Apifsm_stats       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm_start       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm_stop        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm_pause       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm_resume      (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm_state       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_Runfsm_stats       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_occurs             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_occurs_oper1       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_occurs_oper2       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_recurs             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_period             (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_period_oper1       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_period_oper2       (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_recurs_on          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_recurs_off         (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_ev_op_math            (int tkn_i, boolean moreInputTkns);

eHandlerResult_t        H_up                    (int tkn_i, boolean moreInputTkns);

eHandlerResult_t        H_cmds                  (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_cmds_run              (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_cmds_runName          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_cmds_halt             (int tkn_i, boolean moreInputTkns);

eHandlerResult_t        H_help                  (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_help_Show_ByTopic     (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_help_Show_ByCommand   (int tkn_i, boolean moreInputTkns);

// The math operations are at the center of all event/loop/other math operations.
// The specific Lvalue or destination varies, all call "H_math_oper1" to get a result.
eHandlerResult_t        H_math                  (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_math_Lvalue           (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_math_op_assign        (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_math_oper1            (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_math_op_math          (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_math_op_cmp           (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_math_oper2            (int tkn_i, boolean moreInputTkns);

eHandlerResult_t        H_loop                  (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_on               (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_off              (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_while            (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_until            (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_assign           (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_order            (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_oper1            (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_op_cmp           (int tkn_i, boolean moreInputTkns);
eHandlerResult_t        H_loop_oper2            (int tkn_i, boolean moreInputTkns);

boolean         H_loop_IsThisTermCondition (void);

#endif  // __SSUI_MENUCORE_DCL_H

