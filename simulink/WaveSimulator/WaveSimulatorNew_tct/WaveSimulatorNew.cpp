﻿// ****************** WaveSimulatorNew.cpp *******************************
// Generated by TwinCAT Target for MATLAB/Simulink (TE1400)
// MATLAB R2019a (win64)
// TwinCAT 3.1.4022
// TwinCAT Target 1.2.1237
// Beckhoff Automation GmbH & Co. KG     (www.beckhoff.com)
// *************************************************************
/*
 * WaveSimulatorNew.cpp
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "WaveSimulatorNew".
 *
 * Model version              : 1.865
 * Simulink Coder version : 9.1 (R2019a) 23-Nov-2018
 * C++ source code generated on : Fri Dec 20 06:02:23 2019
 *
 * Target selection: TwinCAT.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Execution efficiency
 * Validation result: Passed (7), Warnings (2), Error (0)
 */

#include "stdafx.h"
#include "CWaveSimulatorNew.h"
#include "WaveSimulatorNew.h"
#include "WaveSimulatorNew_private.h"
/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

#endif


void CWaveSimulatorNew::rate_scheduler (void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  ((&m_SimStruct)->Timing.TaskCounters.TID[2])++;
  if (((&m_SimStruct)->Timing.TaskCounters.TID[2]) > 1) {/* Sample time: [0.01s, 0.0s] */
    (&m_SimStruct)->Timing.TaskCounters.TID[2] = 0;
  }

  (&m_SimStruct)->Timing.sampleHits[2] =
    ((&m_SimStruct)->Timing.TaskCounters.TID[2] == 0);
  ((&m_SimStruct)->Timing.TaskCounters.TID[3])++;
  if (((&m_SimStruct)->Timing.TaskCounters.TID[3]) > 199) {/* Sample time: [1.0s, 0.0s] */
    (&m_SimStruct)->Timing.TaskCounters.TID[3] = 0;
  }

  (&m_SimStruct)->Timing.sampleHits[3] =
    ((&m_SimStruct)->Timing.TaskCounters.TID[3] == 0);
}

void CWaveSimulatorNew::rt_ertODEUpdateContinuousStates (RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 30;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  WaveSimulatorNew_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  WaveSimulatorNew_output();
  WaveSimulatorNew_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  WaveSimulatorNew_output();
  WaveSimulatorNew_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T CWaveSimulatorNew::rt_urand_Upu32_Yd_f_pw_snf (uint32_T *u)
{
  uint32_T lo;
  uint32_T hi;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return static_cast<real_T>(*u) * 4.6566128752457969E-10;
}

real_T CWaveSimulatorNew::rt_nrand_Upu32_Yd_f_pw_snf (uint32_T *u)
{
  real_T y;
  real_T sr;
  real_T si;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = std::sqrt(-2.0 * std::log(si) / si) * sr;
  return y;
}

void CWaveSimulatorNew::WaveSimulatorNew_output (void)
{
  static const real_T a[36] = { 1.4690053746876147E-7, 0.0,
    -4.8180009592214244E-10, 0.0, 1.8427800367678206E-10, -0.0, 0.0,
    1.3291241933627674E-7, -0.0, -4.1523910573686178E-9, 0.0,
    2.246118561264065E-11, -4.8180009592214244E-10, -0.0, 5.755014152022931E-8,
    -0.0, -3.9714920529310617E-10, -0.0, 0.0, -4.1523910573686178E-9, -0.0,
    2.8964882301736963E-9, 0.0, 4.8718802883020507E-11, 1.8427800367678206E-10,
    0.0, -3.9714920529310617E-10, 0.0, 1.5190088863133132E-10, -0.0, -0.0,
    2.246118561264065E-11, -0.0, 4.8718802883020507E-11, -0.0,
    2.8099979955902821E-10 };

  static const real_T b_a[36] = { 589675.19075222965, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.9628508E+6, 0.0, 1.9009739E+6, 0.0, 5.7397055E+6, 0.0, 0.0,
    1.5460518E+6, 0.0, 3.7347568E+7, 0.0, 0.0, 1.9009739E+6, 0.0,
    3.536832185572774E+7, 0.0, -4.7547112E+7, 0.0, 0.0, 3.7347568E+7, 0.0,
    1.5862845E+9, 0.0, 0.0, 5.7397055E+6, 0.0, -4.7547112E+7, 0.0, 8.9457563E+8
  };

  static const real_T c_a[36] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 1.3910359E+7, 0.0, 7.0729128E+7, 0.0, 0.0, 0.0, 0.0,
    1.3379735E+8, 0.0, 0.0, 0.0, 0.0, 7.0729128E+7, 0.0, 6.467778E+9, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0 };

  real_T J[36];
  real_T T[9];
  real_T a_0;
  static const int8_T b[36] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };

  static const int8_T c[3] = { 1, 0, 0 };

  int32_T iU;
  int32_T i;
  real_T c_a_0[6];
  real_T tmp[6];
  real_T tmp_0[9];
  real_T tmp_1[9];
  real_T c_0[9];
  int32_T i_0;
  real_T a_idx_1;
  real_T a_idx_3;
  real_T c_tmp;
  real_T c_tmp_0;
  if (rtmIsMajorTimeStep((&m_SimStruct))) {
    /* set solver stop time */
    if (!((&m_SimStruct)->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&(&m_SimStruct)->solverInfo,
                            (((&m_SimStruct)->Timing.clockTickH0 + 1) *
        (&m_SimStruct)->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&(&m_SimStruct)->solverInfo,
                            (((&m_SimStruct)->Timing.clockTick0 + 1) *
        (&m_SimStruct)->Timing.stepSize0 +
        (&m_SimStruct)->Timing.clockTickH0 *
        (&m_SimStruct)->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep((&m_SimStruct))) {
    (&m_SimStruct)->Timing.t[0] = rtsiGetT(&(&m_SimStruct)->solverInfo);
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[1] == 0) {
    for (i = 0; i < 6; i++) {
      /* UnitDelay: '<S5>/Unit Delay' */
      m_BlockIO.UnitDelay[i] = m_DWork.UnitDelay_DSTATE[i];

      /* Saturate: '<S5>/Saturation1' */
      if (m_BlockIO.UnitDelay[i] >
          m_ModelParameters.Saturation1_UpperSat) {
        /* Outport: '<Root>/scale' */
        m_Output.scale[i] = m_ModelParameters.Saturation1_UpperSat;
      } else if (m_BlockIO.UnitDelay[i] <
                 m_ModelParameters.Saturation1_LowerSat) {
        /* Outport: '<Root>/scale' */
        m_Output.scale[i] = m_ModelParameters.Saturation1_LowerSat;
      } else {
        /* Outport: '<Root>/scale' */
        m_Output.scale[i] = m_BlockIO.UnitDelay[i];
      }

      /* End of Saturate: '<S5>/Saturation1' */
    }
  }

  for (i = 0; i < 6; i++) {
    /* Abs: '<S5>/Abs1' incorporates:
     *  Inport: '<Root>/etaMax'
     */
    m_BlockIO.Abs1[i] = std::abs(m_Input.etaMax[i]);

    /* Integrator: '<S1>/Integrator1' */
    m_BlockIO.Integrator1[i] = m_ContState.Integrator1_CSTATE[i];

    /* Product: '<S5>/Product' incorporates:
     *  Outport: '<Root>/scale'
     */
    m_BlockIO.Product[i] = m_BlockIO.Integrator1[i] *
      m_Output.scale[i];

    /* RelationalOperator: '<S54>/LowerRelop1' */
    m_BlockIO.LowerRelop1[i] = (m_BlockIO.Product[i] >
      m_BlockIO.Abs1[i]);

    /* UnaryMinus: '<S5>/Unary Minus' */
    m_BlockIO.UnaryMinus[i] = -m_BlockIO.Abs1[i];

    /* RelationalOperator: '<S54>/UpperRelop' */
    m_BlockIO.UpperRelop[i] = (m_BlockIO.Product[i] <
      m_BlockIO.UnaryMinus[i]);

    /* Switch: '<S54>/Switch' */
    if (m_BlockIO.UpperRelop[i]) {
      m_BlockIO.Gain[i] = m_BlockIO.UnaryMinus[i];
    } else {
      m_BlockIO.Gain[i] = m_BlockIO.Product[i];
    }

    /* End of Switch: '<S54>/Switch' */

    /* Switch: '<S54>/Switch2' */
    if (m_BlockIO.LowerRelop1[i]) {
      m_BlockIO.Gain[i] = m_BlockIO.Abs1[i];
    }

    /* End of Switch: '<S54>/Switch2' */

    /* Outport: '<Root>/eta' */
    m_Output.eta[i] = m_BlockIO.Gain[i];

    /* Integrator: '<S1>/Integrator' */
    m_BlockIO.Integrator[i] = m_ContState.Integrator_CSTATE[i];

    /* Outport: '<Root>/v' */
    m_Output.v[i] = m_BlockIO.Integrator[i];
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[3] == 0) {
    /* MATLAB Function: '<S1>/LinearWaveModel' incorporates:
     *  Inport: '<Root>/lambda'
     *  Inport: '<Root>/sigma'
     *  Inport: '<Root>/w0'
     */
    a_idx_1 = -(m_Input.w0 * m_Input.w0);
    a_idx_3 = std::abs(m_Input.lambda * m_Input.w0) * -2.0;
    memset(&m_BlockIO.A[0], 0, 144U * sizeof(real_T));
    m_BlockIO.A[0] = 0.0;
    m_BlockIO.A[26] = 0.0;
    m_BlockIO.A[52] = 0.0;
    m_BlockIO.A[78] = 0.0;
    m_BlockIO.A[104] = 0.0;
    m_BlockIO.A[130] = 0.0;
    m_BlockIO.A[1] = a_idx_1;
    m_BlockIO.A[27] = a_idx_1;
    m_BlockIO.A[53] = a_idx_1;
    m_BlockIO.A[79] = a_idx_1;
    m_BlockIO.A[105] = a_idx_1;
    m_BlockIO.A[131] = a_idx_1;
    m_BlockIO.A[12] = 1.0;
    m_BlockIO.A[38] = 1.0;
    m_BlockIO.A[64] = 1.0;
    m_BlockIO.A[90] = 1.0;
    m_BlockIO.A[116] = 1.0;
    m_BlockIO.A[142] = 1.0;
    m_BlockIO.A[13] = a_idx_3;
    m_BlockIO.A[39] = a_idx_3;
    m_BlockIO.A[65] = a_idx_3;
    m_BlockIO.A[91] = a_idx_3;
    m_BlockIO.A[117] = a_idx_3;
    m_BlockIO.A[143] = a_idx_3;
    a_idx_1 = std::abs(2.0 * m_Input.lambda * m_Input.w0 *
                       m_Input.sigma);
    memset(&m_BlockIO.B[0], 0, 72U * sizeof(real_T));
    memset(&m_BlockIO.C[0], 0, 72U * sizeof(real_T));
    m_BlockIO.B[0] = 0.0;
    m_BlockIO.B[14] = 0.0;
    m_BlockIO.B[28] = 0.0;
    m_BlockIO.B[42] = 0.0;
    m_BlockIO.B[56] = 0.0;
    m_BlockIO.B[70] = 0.0;
    m_BlockIO.B[1] = a_idx_1;
    m_BlockIO.B[15] = a_idx_1;
    m_BlockIO.B[29] = a_idx_1;
    m_BlockIO.B[43] = a_idx_1;
    m_BlockIO.B[57] = a_idx_1;
    m_BlockIO.B[71] = a_idx_1;
    m_BlockIO.C[0] = 0.0;
    m_BlockIO.C[13] = 0.0;
    m_BlockIO.C[26] = 0.0;
    m_BlockIO.C[39] = 0.0;
    m_BlockIO.C[52] = 0.0;
    m_BlockIO.C[65] = 0.0;
    m_BlockIO.C[6] = 1.0;
    m_BlockIO.C[19] = 1.0;
    m_BlockIO.C[32] = 1.0;
    m_BlockIO.C[45] = 1.0;
    m_BlockIO.C[58] = 1.0;
    m_BlockIO.C[71] = 1.0;
    memset(&m_BlockIO.D[0], 0, 36U * sizeof(real_T));
  }

  /* Integrator: '<S8>/Integrator' */
  memcpy(&m_BlockIO.x[0], &m_ContState.Integrator_CSTATE_f[0],
         12U * sizeof(real_T));

  /* Product: '<S8>/Product3' */
  for (iU = 0; iU < 6; iU++) {
    m_BlockIO.Gain[iU] = 0.0;
    for (i_0 = 0; i_0 < 12; i_0++) {
      m_BlockIO.Gain[iU] += m_BlockIO.C[6 * i_0 + iU] *
        m_BlockIO.x[i_0];
    }
  }

  /* End of Product: '<S8>/Product3' */
  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[2] == 0) {
    /* RandomNumber: '<S1>/Random Sequence' */
    for (iU = 0; iU < 6; iU++) {
      m_BlockIO.RandomSequence[iU] = m_DWork.NextOutput[iU];
    }

    /* End of RandomNumber: '<S1>/Random Sequence' */

    /* Product: '<S8>/Product2' */
    for (iU = 0; iU < 6; iU++) {
      m_BlockIO.Du[iU] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        m_BlockIO.Du[iU] += m_BlockIO.D[6 * i_0 + iU] *
          m_BlockIO.RandomSequence[i_0];
      }
    }

    /* End of Product: '<S8>/Product2' */
  }

  for (iU = 0; iU < 6; iU++) {
    /* Sum: '<S8>/Sum1' */
    m_BlockIO.Gain[iU] += m_BlockIO.Du[iU];

    /* Gain: '<S1>/Gain' */
    m_BlockIO.Gain[iU] *= m_ModelParameters.Gain_Gain[iU];
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[1] == 0) {
    /* Saturate: '<S1>/Input Saturation' incorporates:
     *  Constant: '<S1>/u'
     */
    for (i = 0; i < 6; i++) {
      if (m_ModelParameters.u_Value[i] >
          m_ModelParameters.InputSaturation_UpperSat[i]) {
        m_BlockIO.InputSaturation[i] =
          m_ModelParameters.InputSaturation_UpperSat[i];
      } else if (m_ModelParameters.u_Value[i] <
                 m_ModelParameters.InputSaturation_LowerSat[i]) {
        m_BlockIO.InputSaturation[i] =
          m_ModelParameters.InputSaturation_LowerSat[i];
      } else {
        m_BlockIO.InputSaturation[i] = m_ModelParameters.u_Value[i];
      }
    }

    /* End of Saturate: '<S1>/Input Saturation' */
  }

  /* Sum: '<S1>/Sum1' */
  for (i = 0; i < 6; i++) {
    m_BlockIO.SumD[i] = m_BlockIO.InputSaturation[i] -
      m_BlockIO.Integrator1[i];
  }

  /* End of Sum: '<S1>/Sum1' */
  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[3] == 0) {
    /* Saturate: '<S1>/Poles Saturation' incorporates:
     *  Constant: '<S1>/poles'
     */
    if (m_ModelParameters.poles_Value >
        m_ModelParameters.PolesSaturation_UpperSat) {
      m_BlockIO.PolesSaturation =
        m_ModelParameters.PolesSaturation_UpperSat;
    } else if (m_ModelParameters.poles_Value <
               m_ModelParameters.PolesSaturation_LowerSat) {
      m_BlockIO.PolesSaturation =
        m_ModelParameters.PolesSaturation_LowerSat;
    } else {
      m_BlockIO.PolesSaturation = m_ModelParameters.poles_Value;
    }

    /* End of Saturate: '<S1>/Poles Saturation' */

    /* MATLAB Function: '<S1>/PolePlacer' */
    for (i = 0; i < 6; i++) {
      m_BlockIO.Kp[i] = 0.0;
      m_BlockIO.Kd[i] = 0.0;
    }

    a_idx_1 = std::abs(m_BlockIO.PolesSaturation);
    a_idx_3 = a_idx_1 * a_idx_1;
    m_BlockIO.Kp[0] = (a_idx_3 * 11.557450930296984 - 100.0) /
      0.00016957249073865904;
    m_BlockIO.Kp[1] = (a_idx_3 * 4.0268412070014108 - 100.0) /
      5.1838649785438191E-5;
    m_BlockIO.Kp[5] = (a_idx_3 * 4.0706796111413865 - 100.0) /
      1.1383490937706065E-7;
    m_BlockIO.Kd[0] = (((23.114901860593967 * a_idx_1 - 1.0) -
      m_BlockIO.Kp[0] * 1.6957249073865904E-6) - 1155.7450930296984) /
      0.00016957249073865904;
    m_BlockIO.Kd[1] = (((8.0536824140028216 * a_idx_1 - 1.0) -
      m_BlockIO.Kp[1] * 5.1838649785438193E-7) - 402.68412070014108) /
      5.1838649785438191E-5;
    m_BlockIO.Kd[5] = (((8.1413592222827731 * a_idx_1 - 1.0) -
      m_BlockIO.Kp[5] * 1.1383490937706064E-9) - 407.06796111413865) /
      1.1383490937706065E-7;
    for (i = 0; i < 6; i++) {
      m_BlockIO.Kp[i] = std::abs(m_BlockIO.Kp[i]);
      m_BlockIO.Kd[i] = std::abs(m_BlockIO.Kd[i]);
    }

    m_BlockIO.N = 100.0;

    /* End of MATLAB Function: '<S1>/PolePlacer' */
  }

  for (i = 0; i < 6; i++) {
    /* Product: '<S43>/PProd Out' */
    m_BlockIO.Sum_d[i] = m_BlockIO.SumD[i] *
      m_BlockIO.Kp[i];

    /* Product: '<S32>/DProd Out' */
    m_BlockIO.SumD[i] *= m_BlockIO.Kd[i];

    /* Integrator: '<S33>/Filter' */
    m_BlockIO.Filter[i] = m_ContState.Filter_CSTATE[i];

    /* Sum: '<S33>/SumD' */
    m_BlockIO.SumD[i] -= m_BlockIO.Filter[i];

    /* Product: '<S41>/NProd Out' */
    m_BlockIO.NProdOut[i] = m_BlockIO.SumD[i] *
      m_BlockIO.N;

    /* Sum: '<S47>/Sum' */
    m_BlockIO.Sum_d[i] += m_BlockIO.NProdOut[i];

    /* Sum: '<S1>/Sum' */
    m_BlockIO.Sum[i] = m_BlockIO.Gain[i] +
      m_BlockIO.Sum_d[i];

    /* MATLAB Function: '<S1>/StateSpaceModel' */
    a_idx_1 = 0.0;
    c_a_0[i] = 0.0;
    for (iU = 0; iU < 6; iU++) {
      i_0 = 6 * iU + i;
      a_idx_1 += b_a[i_0] * m_BlockIO.Integrator[iU];
      c_a_0[i] += c_a[i_0] * m_BlockIO.Integrator1[iU];
    }

    tmp[i] = (m_BlockIO.Sum[i] - a_idx_1) - c_a_0[i];
  }

  for (i = 0; i < 6; i++) {
    /* MATLAB Function: '<S1>/StateSpaceModel' */
    m_BlockIO.v_t[i] = 0.0;
    for (iU = 0; iU < 6; iU++) {
      m_BlockIO.v_t[i] += a[6 * iU + i] * tmp[iU];
    }

    /* Outport: '<Root>/v_t' */
    m_Output.v_t[i] = m_BlockIO.v_t[i];
  }

  /* MATLAB Function: '<S1>/BodyToWorld' */
  for (iU = 0; iU < 36; iU++) {
    J[iU] = b[iU];
  }

  a_idx_1 = std::cos(m_BlockIO.Integrator1[5]);
  T[0] = a_idx_1;
  a_idx_3 = std::sin(m_BlockIO.Integrator1[5]);
  T[3] = -a_idx_3;
  T[6] = 0.0;
  T[1] = a_idx_3;
  T[4] = a_idx_1;
  T[7] = 0.0;
  a_idx_1 = std::cos(m_BlockIO.Integrator1[4]);
  tmp_0[0] = a_idx_1;
  tmp_0[3] = 0.0;
  a_idx_3 = std::sin(m_BlockIO.Integrator1[4]);
  tmp_0[6] = a_idx_3;
  T[2] = 0.0;
  tmp_0[1] = 0.0;
  T[5] = 0.0;
  tmp_0[4] = 1.0;
  T[8] = 1.0;
  tmp_0[7] = 0.0;
  tmp_0[2] = -a_idx_3;
  tmp_0[5] = 0.0;
  tmp_0[8] = a_idx_1;
  for (iU = 0; iU < 3; iU++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      i = iU + 3 * i_0;
      tmp_1[i] = 0.0;
      tmp_1[i] += tmp_0[3 * i_0] * T[iU];
      tmp_1[i] += tmp_0[3 * i_0 + 1] * T[iU + 3];
      tmp_1[i] += tmp_0[3 * i_0 + 2] * T[iU + 6];
    }

    c_0[3 * iU] = c[iU];
  }

  c_0[1] = 0.0;
  c_tmp_0 = std::cos(m_BlockIO.Integrator1[3]);
  c_0[4] = c_tmp_0;
  c_tmp = std::sin(m_BlockIO.Integrator1[3]);
  c_0[7] = -c_tmp;
  c_0[2] = 0.0;
  c_0[5] = c_tmp;
  c_0[8] = c_tmp_0;
  for (iU = 0; iU < 3; iU++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      i = i_0 + 6 * iU;
      J[i] = 0.0;
      J[i] += c_0[3 * iU] * tmp_1[i_0];
      J[i] += c_0[3 * iU + 1] * tmp_1[i_0 + 3];
      J[i] += c_0[3 * iU + 2] * tmp_1[i_0 + 6];
    }
  }

  memset(&T[0], 0, 9U * sizeof(real_T));
  T[0] = 1.0;
  T[4] = 1.0;
  T[8] = 1.0;
  if (a_idx_1 != 0.0) {
    a_0 = 1.0 / a_idx_1;
    T[0] = a_0 * a_idx_1;
    T[3] = c_tmp * a_idx_3 * a_0;
    T[6] = c_tmp_0 * a_idx_3 * a_0;
    T[1] = a_0 * 0.0;
    T[4] = c_tmp_0 * a_idx_1 * a_0;
    T[7] = -c_tmp * a_idx_1 * a_0;
    T[2] = a_0 * 0.0;
    T[5] = a_0 * c_tmp;
    T[8] = a_0 * c_tmp_0;
  }

  for (iU = 0; iU < 3; iU++) {
    i = 6 * (3 + iU);
    J[3 + i] = T[3 * iU];
    J[4 + i] = T[3 * iU + 1];
    J[5 + i] = T[3 * iU + 2];
  }

  for (iU = 0; iU < 6; iU++) {
    m_BlockIO.eta_t[iU] = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      m_BlockIO.eta_t[iU] += J[6 * i_0 + iU] *
        m_BlockIO.Integrator[i_0];
    }
  }

  /* End of MATLAB Function: '<S1>/BodyToWorld' */
  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[1] == 0) {
    /* UnitDelay: '<S53>/Delay Input1' */
    m_BlockIO.Uk1 = m_DWork.DelayInput1_DSTATE;
  }

  /* RelationalOperator: '<S55>/Compare' incorporates:
   *  Constant: '<S55>/Constant'
   *  Inport: '<Root>/bReset'
   */
  m_BlockIO.Compare = (static_cast<int32_T>(m_Input.bReset) >
    static_cast<int32_T>(m_ModelParameters.Constant_Value_l));

  /* RelationalOperator: '<S53>/FixPt Relational Operator' */
  m_BlockIO.FixPtRelationalOperator = (static_cast<int32_T>
    (m_BlockIO.Compare) > static_cast<int32_T>(m_BlockIO.Uk1));
  for (i = 0; i < 6; i++) {
    /* Switch: '<S5>/Switch1' incorporates:
     *  Constant: '<S5>/Constant'
     */
    if (m_BlockIO.FixPtRelationalOperator) {
      m_BlockIO.Switch1[i] = m_ModelParameters.Constant_Value[i];
    } else {
      /* Abs: '<S5>/Abs2' */
      m_BlockIO.Divide[i] = std::abs(m_BlockIO.Product[i]);

      /* RelationalOperator: '<S5>/Relational Operator' */
      m_BlockIO.RelationalOperator[i] = (m_BlockIO.Divide[i] >=
        m_BlockIO.Abs1[i]);

      /* Abs: '<S5>/Abs' */
      m_BlockIO.Divide[i] = std::abs(m_BlockIO.Integrator1[i]);

      /* Saturate: '<S5>/Saturation' */
      if (m_BlockIO.Divide[i] > m_ModelParameters.Saturation_UpperSat)
      {
        m_BlockIO.Divide[i] = m_ModelParameters.Saturation_UpperSat;
      } else {
        if (m_BlockIO.Divide[i] <
            m_ModelParameters.Saturation_LowerSat) {
          m_BlockIO.Divide[i] = m_ModelParameters.Saturation_LowerSat;
        }
      }

      /* End of Saturate: '<S5>/Saturation' */

      /* Product: '<S5>/Divide' */
      m_BlockIO.Divide[i] = m_BlockIO.Abs1[i] /
        m_BlockIO.Divide[i];

      /* Switch: '<S5>/Switch' */
      if (m_BlockIO.RelationalOperator[i]) {
        m_BlockIO.Switch[i] = m_BlockIO.Divide[i];
      } else {
        m_BlockIO.Switch[i] = m_BlockIO.UnitDelay[i];
      }

      /* End of Switch: '<S5>/Switch' */
      m_BlockIO.Switch1[i] = m_BlockIO.Switch[i];
    }

    /* End of Switch: '<S5>/Switch1' */
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[2] == 0) {
    /* Product: '<S8>/Product' */
    for (iU = 0; iU < 12; iU++) {
      m_BlockIO.Bu[iU] = 0.0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        m_BlockIO.Bu[iU] += m_BlockIO.B[12 * i_0 + iU] *
          m_BlockIO.RandomSequence[i_0];
      }
    }

    /* End of Product: '<S8>/Product' */
  }

  for (i = 0; i < 12; i++) {
    /* Product: '<S8>/Product1' */
    m_BlockIO.Ax[i] = 0.0;
    for (iU = 0; iU < 12; iU++) {
      m_BlockIO.Ax[i] += m_BlockIO.A[12 * iU + i] *
        m_BlockIO.x[iU];
    }

    /* End of Product: '<S8>/Product1' */

    /* Sum: '<S8>/Sum' */
    m_BlockIO.dx[i] = m_BlockIO.Bu[i] +
      m_BlockIO.Ax[i];
  }
}

void CWaveSimulatorNew::WaveSimulatorNew_update (void)
{
  int32_T i;
  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[1] == 0) {
    /* Update for UnitDelay: '<S5>/Unit Delay' */
    for (i = 0; i < 6; i++) {
      m_DWork.UnitDelay_DSTATE[i] = m_BlockIO.Switch1[i];
    }

    /* End of Update for UnitDelay: '<S5>/Unit Delay' */

    /* Update for UnitDelay: '<S53>/Delay Input1' */
    m_DWork.DelayInput1_DSTATE = m_BlockIO.Compare;
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[2] == 0) {
    /* Update for RandomNumber: '<S1>/Random Sequence' */
    for (i = 0; i < 6; i++) {
      m_DWork.NextOutput[i] = rt_nrand_Upu32_Yd_f_pw_snf
        (&m_DWork.RandSeed[i]) *
        m_ModelParameters.RandomSequence_StdDev[i] +
        m_ModelParameters.RandomSequence_Mean[i];
    }

    /* End of Update for RandomNumber: '<S1>/Random Sequence' */
  }

  if (rtmIsMajorTimeStep((&m_SimStruct))) {
    rt_ertODEUpdateContinuousStates(&(&m_SimStruct)->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++(&m_SimStruct)->Timing.clockTick0)) {
    ++(&m_SimStruct)->Timing.clockTickH0;
  }

  (&m_SimStruct)->Timing.t[0] = rtsiGetSolverStopTime
    (&(&m_SimStruct)->solverInfo);

  {
    /* Update absolute timer for sample time: [0.005s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++(&m_SimStruct)->Timing.clockTick1)) {
      ++(&m_SimStruct)->Timing.clockTickH1;
    }

    (&m_SimStruct)->Timing.t[1] = (&m_SimStruct)->Timing.clockTick1 *
      (&m_SimStruct)->Timing.stepSize1 +
      (&m_SimStruct)->Timing.clockTickH1 *
      (&m_SimStruct)->Timing.stepSize1 * 4294967296.0;
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[2] == 0) {
    /* Update absolute timer for sample time: [0.01s, 0.0s] */
    /* The "clockTick2" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick2"
     * and "Timing.stepSize2". Size of "clockTick2" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick2 and the high bits
     * Timing.clockTickH2. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++(&m_SimStruct)->Timing.clockTick2)) {
      ++(&m_SimStruct)->Timing.clockTickH2;
    }

    (&m_SimStruct)->Timing.t[2] = (&m_SimStruct)->Timing.clockTick2 *
      (&m_SimStruct)->Timing.stepSize2 +
      (&m_SimStruct)->Timing.clockTickH2 *
      (&m_SimStruct)->Timing.stepSize2 * 4294967296.0;
  }

  if (rtmIsMajorTimeStep((&m_SimStruct)) &&
      (&m_SimStruct)->Timing.TaskCounters.TID[3] == 0) {
    /* Update absolute timer for sample time: [1.0s, 0.0s] */
    /* The "clockTick3" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick3"
     * and "Timing.stepSize3". Size of "clockTick3" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick3 and the high bits
     * Timing.clockTickH3. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++(&m_SimStruct)->Timing.clockTick3)) {
      ++(&m_SimStruct)->Timing.clockTickH3;
    }

    (&m_SimStruct)->Timing.t[3] = (&m_SimStruct)->Timing.clockTick3 *
      (&m_SimStruct)->Timing.stepSize3 +
      (&m_SimStruct)->Timing.clockTickH3 *
      (&m_SimStruct)->Timing.stepSize3 * 4294967296.0;
  }

  rate_scheduler();
}

void CWaveSimulatorNew::WaveSimulatorNew_derivatives (void)
{
  int32_T i;
  XDot_WaveSimulatorNew_T *_rtXdot;
  _rtXdot = ((XDot_WaveSimulatorNew_T *) (&m_SimStruct)->derivs);
  for (i = 0; i < 6; i++) {
    /* Derivatives for Integrator: '<S1>/Integrator1' */
    _rtXdot->Integrator1_CSTATE[i] = m_BlockIO.eta_t[i];

    /* Derivatives for Integrator: '<S1>/Integrator' */
    _rtXdot->Integrator_CSTATE[i] = m_BlockIO.v_t[i];
  }

  /* Derivatives for Integrator: '<S8>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_f[0], &m_BlockIO.dx[0], 12U *
         sizeof(real_T));

  /* Derivatives for Integrator: '<S33>/Filter' */
  for (i = 0; i < 6; i++) {
    _rtXdot->Filter_CSTATE[i] = m_BlockIO.NProdOut[i];
  }

  /* End of Derivatives for Integrator: '<S33>/Filter' */
}

void CWaveSimulatorNew::WaveSimulatorNew_initialize (void)
{
  {
    uint32_T tseed;
    int32_T r;
    int32_T t;
    int32_T i;
    real_T tmp;
    for (i = 0; i < 6; i++) {
      /* InitializeConditions for UnitDelay: '<S5>/Unit Delay' */
      m_DWork.UnitDelay_DSTATE[i] =
        m_ModelParameters.UnitDelay_InitialCondition;

      /* InitializeConditions for Integrator: '<S1>/Integrator1' */
      m_ContState.Integrator1_CSTATE[i] =
        m_ModelParameters.Integrator1_IC[i];

      /* InitializeConditions for Integrator: '<S1>/Integrator' */
      m_ContState.Integrator_CSTATE[i] =
        m_ModelParameters.Integrator_IC[i];
    }

    /* InitializeConditions for Integrator: '<S8>/Integrator' */
    memcpy(&m_ContState.Integrator_CSTATE_f[0],
           &m_ModelParameters.Integrator_IC_k[0], 12U * sizeof(real_T));
    for (i = 0; i < 6; i++) {
      /* InitializeConditions for RandomNumber: '<S1>/Random Sequence' */
      tmp = std::floor(m_ModelParameters.RandomSequence_Seed[i]);
      if (rtIsNaN(tmp) || rtIsInf(tmp)) {
        tmp = 0.0;
      } else {
        tmp = std::fmod(tmp, 4.294967296E+9);
      }

      tseed = tmp < 0.0 ? static_cast<uint32_T>(-static_cast<int32_T>
        (static_cast<uint32_T>(-tmp))) : static_cast<uint32_T>(tmp);
      r = static_cast<int32_T>((tseed >> 16U));
      t = static_cast<int32_T>((tseed & 32768U));
      tseed = ((((tseed - (static_cast<uint32_T>(r) << 16U)) + t) << 16U) + t) +
        r;
      if (tseed < 1U) {
        tseed = 1144108930U;
      } else {
        if (tseed > 2147483646U) {
          tseed = 2147483646U;
        }
      }

      m_DWork.RandSeed[i] = tseed;
      m_DWork.NextOutput[i] = rt_nrand_Upu32_Yd_f_pw_snf
        (&m_DWork.RandSeed[i]) *
        m_ModelParameters.RandomSequence_StdDev[i] +
        m_ModelParameters.RandomSequence_Mean[i];

      /* End of InitializeConditions for RandomNumber: '<S1>/Random Sequence' */

      /* InitializeConditions for Integrator: '<S33>/Filter' */
      m_ContState.Filter_CSTATE[i] =
        m_ModelParameters.DPController_InitialConditionFo;
    }

    /* InitializeConditions for UnitDelay: '<S53>/Delay Input1' */
    m_DWork.DelayInput1_DSTATE =
      m_ModelParameters.DetectRisePositive_vinit;
  }
}

void CWaveSimulatorNew::WaveSimulatorNew_terminate (void)
{
  /* (no terminate code required) */
}

void CWaveSimulatorNew::rt_ODECreateIntegrationData (RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void CWaveSimulatorNew::rt_ODEDestroyIntegrationData (RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void CWaveSimulatorNew::rt_ODEUpdateContinuousStates (RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void CWaveSimulatorNew::MdlOutputs (int_T tid)
{
  WaveSimulatorNew_output();
  UNUSED_PARAMETER(tid);
}

void CWaveSimulatorNew::MdlUpdate (int_T tid)
{
  WaveSimulatorNew_update();
  UNUSED_PARAMETER(tid);
}

void CWaveSimulatorNew::MdlInitializeSizes (void)
{
}

void CWaveSimulatorNew::MdlInitializeSampleTimes (void)
{
}

void CWaveSimulatorNew::MdlInitialize (void)
{
}

void CWaveSimulatorNew::MdlStart (void)
{
  WaveSimulatorNew_initialize();
}

void CWaveSimulatorNew::MdlTerminate (void)
{
  WaveSimulatorNew_terminate();
}

RT_MODEL_WaveSimulatorNew_T* CWaveSimulatorNew::WaveSimulatorNew (void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  m_ModelParameters.Saturation_UpperSat = rtInf;

  /* initialize real-time model */



  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&m_SimStruct)->solverInfo,
                          &(&m_SimStruct)->Timing.simTimeStep);
    rtsiSetTPtr(&(&m_SimStruct)->solverInfo, &rtmGetTPtr((&m_SimStruct)));
    rtsiSetStepSizePtr(&(&m_SimStruct)->solverInfo,
                       &(&m_SimStruct)->Timing.stepSize0);
    rtsiSetdXPtr(&(&m_SimStruct)->solverInfo, &(&m_SimStruct)->derivs);
    rtsiSetContStatesPtr(&(&m_SimStruct)->solverInfo, (real_T **)
                         &(&m_SimStruct)->contStates);
    rtsiSetNumContStatesPtr(&(&m_SimStruct)->solverInfo,
      &(&m_SimStruct)->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&m_SimStruct)->solverInfo,
      &(&m_SimStruct)->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&m_SimStruct)->solverInfo,
      &(&m_SimStruct)->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&m_SimStruct)->solverInfo,
      &(&m_SimStruct)->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&(&m_SimStruct)->solverInfo, (&rtmGetErrorStatus
      ((&m_SimStruct))));
    rtsiSetRTModelPtr(&(&m_SimStruct)->solverInfo, (&m_SimStruct));
  }

  rtsiSetSimTimeStep(&(&m_SimStruct)->solverInfo, MAJOR_TIME_STEP);
  (&m_SimStruct)->intgData.y = (&m_SimStruct)->odeY;
  (&m_SimStruct)->intgData.f[0] = (&m_SimStruct)->odeF[0];
  (&m_SimStruct)->intgData.f[1] = (&m_SimStruct)->odeF[1];
  (&m_SimStruct)->intgData.f[2] = (&m_SimStruct)->odeF[2];
  (&m_SimStruct)->contStates = ((real_T *) &m_ContState);
  rtsiSetSolverData(&(&m_SimStruct)->solverInfo, (void *)
                    &(&m_SimStruct)->intgData);
  rtsiSetSolverName(&(&m_SimStruct)->solverInfo,"ode3");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = (&m_SimStruct)->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    mdlTsMap[2] = 2;
    mdlTsMap[3] = 3;
    (&m_SimStruct)->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    (&m_SimStruct)->Timing.sampleTimes =
      (&(&m_SimStruct)->Timing.sampleTimesArray[0]);
    (&m_SimStruct)->Timing.offsetTimes =
      (&(&m_SimStruct)->Timing.offsetTimesArray[0]);

    /* task periods */
    (&m_SimStruct)->Timing.sampleTimes[0] = (0.0);
    (&m_SimStruct)->Timing.sampleTimes[1] = (0.005);
    (&m_SimStruct)->Timing.sampleTimes[2] = (0.01);
    (&m_SimStruct)->Timing.sampleTimes[3] = (1.0);

    /* task offsets */
    (&m_SimStruct)->Timing.offsetTimes[0] = (0.0);
    (&m_SimStruct)->Timing.offsetTimes[1] = (0.0);
    (&m_SimStruct)->Timing.offsetTimes[2] = (0.0);
    (&m_SimStruct)->Timing.offsetTimes[3] = (0.0);
  }

  rtmSetTPtr((&m_SimStruct), &(&m_SimStruct)->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = (&m_SimStruct)->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    mdlSampleHits[2] = 1;
    mdlSampleHits[3] = 1;
    (&m_SimStruct)->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal((&m_SimStruct), 240.0);
  (&m_SimStruct)->Timing.stepSize0 = 0.005;
  (&m_SimStruct)->Timing.stepSize1 = 0.005;
  (&m_SimStruct)->Timing.stepSize2 = 0.01;
  (&m_SimStruct)->Timing.stepSize3 = 1.0;
  (&m_SimStruct)->solverInfoPtr = (&(&m_SimStruct)->solverInfo);
  (&m_SimStruct)->Timing.stepSize = (0.005);
  rtsiSetFixedStepSize(&(&m_SimStruct)->solverInfo, 0.005);
  rtsiSetSolverMode(&(&m_SimStruct)->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  (&m_SimStruct)->blockIO = ((void *) &m_BlockIO);



  /* parameters */
  (&m_SimStruct)->defaultParam = ((real_T *)&m_ModelParameters);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &m_ContState;
    (&m_SimStruct)->contStates = (x);


  }

  /* states (dwork) */
  (&m_SimStruct)->dwork = ((void *) &m_DWork);



  /* external inputs */
  (&m_SimStruct)->inputs = (((void*)&m_Input));


  /* external outputs */
  (&m_SimStruct)->outputs = (&m_Output);



  /* Initialize Sizes */
  (&m_SimStruct)->Sizes.numContStates = (30);/* Number of continuous states */
  (&m_SimStruct)->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  (&m_SimStruct)->Sizes.numY = (24);/* Number of model outputs */
  (&m_SimStruct)->Sizes.numU = (10);/* Number of model inputs */
  (&m_SimStruct)->Sizes.sysDirFeedThru = (1);/* The model is direct feedthrough */
  (&m_SimStruct)->Sizes.numSampTimes = (4);/* Number of sample times */
  (&m_SimStruct)->Sizes.numBlocks = (57);/* Number of blocks */
  (&m_SimStruct)->Sizes.numBlockIO = (39);/* Number of block outputs */
  (&m_SimStruct)->Sizes.numBlockPrms = (83);/* Sum of parameter "widths" */
  return (&m_SimStruct);
}
