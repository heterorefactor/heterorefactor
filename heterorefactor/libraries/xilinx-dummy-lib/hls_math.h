/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2008-2012 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

/**
 * @file hls_math.h
 * This file contains math.h related functions such as sin, cos, sqrt, ln, etc.
 */

#ifndef X_HLS_MATH_H
#define X_HLS_MATH_H

#ifndef __cplusplus
#error C++ is required to include this header file
#endif

#include <assert.h>
#include <stdint.h>
#include "ap_int.h"
#include "hls/utils/x_hls_defines.h"
#include "hls/utils/x_hls_utils.h"

namespace hls
{

/**
 * The Xilinx math.h library implements standard math.h functions
 * optimized for FPGA implementation.
 *
 */

/* @{ */

static int __signbit(float a_re)
{
    fp_struct<float> s(a_re);
    return s.__signbit();
}

static int __signbit(double a_re)
{
    fp_struct<double> s(a_re);
    return s.__signbit();
}

static double nan(const char *tagp) {
    fp_struct<double> nan;
    nan.sig = -1; // all ones
    nan.exp = -1; // all ones
    nan.sign = 0;
    return nan.to_ieee();
}

static float nanf(const char *tagp) {
    fp_struct<float> nan;
    nan.sig = -1; // all ones
    nan.exp = -1; // all ones
    nan.sign = 0;
    return nan.to_ieee();
}


/**
 * Determines if given floating point number has a finite value.
 * i.e. normal, subnormal or zero, but not infinite or NAN
 *
 * @param t_in floating point vlaue.
 * @return nonzero value if arg has finite value, 0 otherwise.
 */
static
int
__isfinite(float t_in)
{
    fp_struct<float> fs = t_in;
    int ret = (fs.exp == 0xFF) ? 0 : 1;
    return ret;
}

static
int
__isfinite(double t_in)
{
    fp_struct<double> fs = t_in;
    int ret = (fs.exp == 0x7FF) ? 0 : 1;
    return ret;
}

/**
 * Determines if given floating point number is positive or negative infinity.
 *
 * @param t_in floating point vlaue.
 * @return nonzero value if arg is infinite, 0 otherwise.
 */
static
int
__isinf(float t_in)
{
    fp_struct<float> fs = t_in;
    int ret = ((fs.exp == 0xFF) && (fs.sig == 0x0)) ? 1 : 0;
    return ret;
}

static
int
__isinf(double t_in)
{
    fp_struct<double> fs = t_in;
    int ret = ((fs.exp == 0x7FF) && (fs.sig == 0x0)) ? 1 : 0;
    return ret;
}

/**
 * Determines if given floating point number is not-a-number (NaN)
 *
 * @param t_in floating point vlaue.
 * @return nonzero value if arg is not-a-number (NaN), 0 otherwise.
 */
static
int
__isnan(float t_in)
{
    fp_struct<float> fs = t_in;
    int ret = ((fs.exp == 0xFF) && (fs.sig != 0x0)) ? 1 : 0;
    return ret;
}

static
int
__isnan(double t_in)
{
    fp_struct<double> fs = t_in;
    int ret = ((fs.exp == 0x7FF) && (fs.sig != 0x0)) ? 1 : 0;
    return ret;
}


/**
 * Determines if given floating point number is normal
 * i.e. is neither zero, subnormal, infinite or NaN.
 *
 * @param t_in floating point vlaue.
 * @return nonzero value if arg is normal, 0 otherwise.
 */

static
int
__isnormal(float t_in)
{
    fp_struct<float> fs = t_in;
    int ret = ((fs.exp != 0xFF) && (fs.exp != 0x0)) ? 1 : 0;
    return ret;
}

static
int
__isnormal(double t_in)
{
    fp_struct<double> fs = t_in;
    int ret = ((fs.exp != 0x7FF) && (fs.exp != 0x0)) ? 1 : 0;
    return ret;
}

/**
 * Categorizes floating point value t_in into the following categories:
 * zero, subnormal, normal, infinite, NaN, or implementation-defined category.
 *
 * @param t_in floating point vlaue.
 * @return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO
 */
#ifndef FP_NORMAL
#define FP_NORMAL    4
#endif

#ifndef FP_SUBNORMAL
#define FP_SUBNORMAL 3
#endif

#ifndef FP_ZERO
#define FP_ZERO      2
#endif

#ifndef FP_INFINITE
#define FP_INFINITE  1
#endif

#ifndef FP_NAN
#define FP_NAN       0
#endif

static
int
__fpclassifyf(float t_in)
{
    fp_struct<float> fs = t_in;
    int ret;
    if(fs.exp == 0x0) {
        ret = (fs.sig == 0x0) ? FP_ZERO : FP_SUBNORMAL;
    } else if(fs.exp == 0xFF) {
        ret = (fs.sig == 0x0) ? FP_INFINITE : FP_NAN;
    } else {
        ret = FP_NORMAL;
    }
    return ret;
}

static
int
__fpclassify(double t_in)
{
    fp_struct<double> fs = t_in;
    int ret;
    if(fs.exp == 0x0) {
        ret = (fs.sig == 0x0) ? FP_ZERO : FP_SUBNORMAL;
    } else if(fs.exp == 0x7FF) {
        ret = (fs.sig == 0x0) ? FP_INFINITE : FP_NAN;
    } else {
        ret = FP_NORMAL;
    }
    return ret;
}

static
double copysign(double x, double y)
{
    fp_struct<double> xs(x), ys(y);
    xs.sign = ys.sign;
    return xs.to_ieee();
}

static
float copysignf(float x, float y)
{
    fp_struct<float> xs(x), ys(y);
    xs.sign = ys.sign;
    return xs.to_ieee();
}

#ifdef __cplusplus
static
float copysign(float x, float y)
{
    return copysignf(x, y);
}
#endif

static
double fabs(double x)
{
    return copysign(x, 0.0);
}

static
float fabsf(float x)
{
    return copysignf(x, 0.0f);
}

#ifdef __cplusplus
static
float fabs(float x)
{
    return fabsf(x);
}
#endif

#include "hls/hls_cordic.h"
#include "hls/hls_normalize.h"
#include "hls/hls_round.h"

/**
 * fmax returns the maximum of two arguments. 
 * If both arguments are 0/ -0, fmax always returns the 2nd argument
 * If both argumetns are nan, fmax always returns the 1st argument but casted 
 * to be of type QNaN meaning bit 23 is a 1.
 * If only one argument is nan, fmax always returns the other argument.
 */
static
double fmax(double x, double y)
{
	fp_struct<double> x_fp(x);
	fp_struct<double> y_fp(y);
	double res; 
	if(x_fp.exp == 0 && x_fp.sig == 0 && y_fp.exp == 0 && y_fp.sig == 0) { // 0, -0: always return 2nd arg
		res = y;
	} else if(x_fp.exp == 0x7FF && x_fp.sig != 0) { // 1st arg: nan 
		if(y_fp.exp == 0x7FF && y_fp.sig != 0) { // 1st and 2nd arg: nan, cast to QNaN and choose 1st arg 
			x_fp.sig = 0x8000000000000ULL | x_fp.sig;
			res = x_fp.to_ieee();
		} else { // 1st arg: nan, 2nd: ~nan
			res = y;
		}
	} else if(y_fp.exp == 0x7FF && y_fp.sig != 0) { // 2nd arg: nan, 1st arg: ~nan
		res = x;
	} else {
		res = (x > y) ? x : y;
	}
	return res;
}

static
float fmaxf(float x, float y)
{
	fp_struct<float> x_fp(x);
	fp_struct<float> y_fp(y);
	float res; 
	if(x_fp.exp == 0 && x_fp.sig == 0 && y_fp.exp == 0 && y_fp.sig == 0) { // 0, -0: always return 2nd arg
		res = y;
	} else if(x_fp.exp == 0xFF && x_fp.sig != 0) { // 1st arg: nan 
		if(y_fp.exp == 0xFF && y_fp.sig != 0) { // 1st and 2nd arg: nan, cast to QNaN and choose 1st arg 
			x_fp.sig = 0x400000 | x_fp.sig;
			res = x_fp.to_ieee();
		} else { // 1st arg: nan, 2nd: ~nan
			res = y;
		}
	} else if(y_fp.exp == 0xFF && y_fp.sig != 0) { // 2nd arg: nan, 1st arg: ~nan
		res = x;
	} else {
		res = (x > y) ? x : y;
	}
	return res;
}

#ifdef __cplusplus
static
float fmax(float x, float y)
{
    return fmaxf(x,y);
}
#endif

/**
 * fmax returns the maximum of two arguments. 
 * If both arguments are 0/ -0, fmax always returns the 2nd argument
 * If both argumetns are nan, fmax always returns the 1st argument but casted 
 * to be of type QNaN meaning bit 23 is a 1.
 * If only one argument is nan, fmax always returns the other argument.
 */
static
double fmin(double x, double y)
{
	fp_struct<double> x_fp(x);
	fp_struct<double> y_fp(y);
	double res; 
	if(x_fp.exp == 0 && x_fp.sig == 0 && y_fp.exp == 0 && y_fp.sig == 0) { // 0, -0: always return 2nd arg
		res = y;
	} else if(x_fp.exp == 0x7FF && x_fp.sig != 0) { // 1st arg: nan 
		if(y_fp.exp == 0x7FF && y_fp.sig != 0) { // 1st and 2nd arg: nan, cast to QNaN and choose 1st arg 
			x_fp.sig = 0x8000000000000ULL | x_fp.sig;
			res = x_fp.to_ieee();
		} else { // 1st arg: nan, 2nd: ~nan
			res = y;
		}
	} else if(y_fp.exp == 0x7FF && y_fp.sig != 0) { // 2nd arg: nan, 1st arg: ~nan
		res = x;
	} else {
		res = (x < y) ? x : y;
	}
	return res;
}

static
float fminf(float x, float y)
{
	fp_struct<float> x_fp(x);
	fp_struct<float> y_fp(y);
	float res; 
	if(x_fp.exp == 0 && x_fp.sig == 0 && y_fp.exp == 0 && y_fp.sig == 0) { // 0, -0: always return 2nd arg
		res = y;
	} else if(x_fp.exp == 0xFF && x_fp.sig != 0) { // 1st arg: nan 
		if(y_fp.exp == 0xFF && y_fp.sig != 0) { // 1st and 2nd arg: nan, cast to QNaN and choose 1st arg 
			x_fp.sig = 0x400000 | x_fp.sig;
			res = x_fp.to_ieee();
		} else { // 1st arg: nan, 2nd: ~nan
			res = y;
		}
	} else if(y_fp.exp == 0xFF && y_fp.sig != 0) { // 2nd arg: nan, 1st arg: ~nan
		res = x;
	} else {
		res = (x < y) ? x : y;
	}
	return res;
}

#ifdef __cplusplus
static
float fmin(float x, float y)
{
    return fminf(x,y);
}
#endif



#include "hls/hls_log.h"
#include "hls/hls_hotbm.h"

static
float
sinf(float t_in)
{
    return hotbm::sinf(t_in);
}

static
float
sinpif(float t_in)
{
    return hotbm::sinpif(t_in);
}

static
float
sin(float t_in)
{
    return sinf(t_in);
}

static
double
sin(double t_in)
{
    return cordic::sin(t_in);
}

static
float
cosf(float t_in)
{
    return hotbm::cosf(t_in);
}

static
float
cospif(float t_in)
{
    return hotbm::cospif(t_in);
}

static
float
cos(float t_in)
{
    return cosf(t_in);
}

static
double
cos(double t_in)
{
    return cordic::cos(t_in);
}

template<int WI>
ap_fixed<32,2>
    cosf(ap_fixed<32, WI> t_in)
{
    return hotbm::cosf(t_in);
}
template<int WI>
ap_fixed<32,2>
    sinf(ap_fixed<32, WI> t_in)
{
    return hotbm::sinf(t_in);
}

template<int WI>
ap_fixed<32,2>
    cospif(ap_fixed<32, WI> t_in)
{
    return hotbm::cospif(t_in);
}
template<int WI>
ap_fixed<32,2>
    sinpif(ap_fixed<32, WI> t_in)
{
    return hotbm::sinpif(t_in);
}

template<int WI>
ap_fixed<32,2>
    cos(ap_fixed<32, WI> t_in)
{
    return hotbm::cosf(t_in);
}
template<int WI>
ap_fixed<32,2>
    sin(ap_fixed<32, WI> t_in)
{
    return hotbm::sinf(t_in);
}

template<int WI>
ap_fixed<32,2>
    cospi(ap_fixed<32, WI> t_in)
{
    return hotbm::cospif(t_in);
}
template<int WI>
ap_fixed<32,2>
    sinpi(ap_fixed<32, WI> t_in)
{
    return hotbm::sinpif(t_in);
}


static
void sincos(double x, double *sin, double *cos)
{
    cordic::sincos(x, sin, cos);
}

static
void sincosf(float x, float *sin, float *cos)
{
    hotbm::sincosf(x, sin, cos);
}

static
float
atanf(float t_in)
{
	return cordic::atanf(t_in);
}

static
double
atan2(double y_in, double x_in)
{
	return cordic::atan2(y_in, x_in);
}

static
float
atan2f(float y_in, float x_in)
{
	return cordic::atan2f(y_in, x_in);
}

static
float
sinhf(float t_in)
{
	return cordic::sinhf(t_in);
}

static
float
coshf(float t_in)
{
	return cordic::coshf(t_in);
}

}

#include "hls_fpo.h"

namespace hls {

static
float
tanf(float t_in)
{
	float s_out, c_out;
    fp_struct<float> fs = t_in;
    if(fs.exp == 0x0) {
        //subnormal or zero
        return t_in;
    } else {
        sincosf(t_in, &s_out, &c_out);
        return HLS_FPO_DIVF(s_out, c_out);
    }
}

static
double
tan(double t_in)
{
	double s_out, c_out;
    fp_struct<double> fs = t_in;
    if(fs.exp == 0x0) {
        //subnormal or zero
        return t_in;
    } else {
        sincos(t_in, &s_out, &c_out);
        return HLS_FPO_DIV(s_out, c_out);
    }
}

static
float logf(float x)
{
    return ::HLS_FPO_LOGF(x);
}


static
float log10f(float x)
{
    return HLS_FPO_MULF(logf(x), 0.434294481903251827651128918916605082294397005f);
}

static
double log(double x)
{
    return ::HLS_FPO_LOG(x);
}


static
double log10(double x)
{
    return HLS_FPO_MUL(log(x), 0.434294481903251827651128918916605082294397005);
}


static
double recip(double x)
{
    return HLS_FPO_RECIP(x);
}

static
float recipf(float x)
{
    return HLS_FPO_RECIPF(x);
}

static
double rsqrt(double x)
{
    return HLS_FPO_RSQRT(x);
}

static
float rsqrtf(float x)
{
    return HLS_FPO_RSQRTF(x);
}

#include "hls/hls_sqrt.h"

static
double sqrt(double x)
{
    return ::HLS_FPO_SQRT(x);
}

static
float sqrtf(float x)
{
    return ::HLS_FPO_SQRTF(x);
}

static
double exp(double x)
{
    return ::HLS_FPO_EXP(x);
}

static
float expf(float x)
{
    return ::HLS_FPO_EXPF(x);
}

// Do nothing with the result of the filter.
template <typename T>
struct hls_filter_traits {
    typedef T T_data;
    typedef T T_accum;
    static T_data shift(T_accum in) {
        return in;
    }
};

struct hls_filter_traits_Q15 {
    typedef short T_data;
    typedef int T_accum;
    static T_data shift(T_accum in) {
        return (T_data)(in >> 15);
    }
};

/** Compute the dot product of x and y, returning the result.
 * @param x the first vector, with length n
 * @param y the second vector, with length n
 * @param n the size of the vectors.
 * @param T the type of the data
 * @param T_accum the type used for the accumulation
 */
template <typename T_accum, typename T>
T_accum dot(T *x, T *y, int n)
{
    // This volatile prevents the first tap from being constant-optimized.
    volatile T_accum r = 0;
    T_accum result = r;
dot_foreach_tap:
    for(int i = 0; i < n; i++) {
        result += x[i] * y[i];
    }
    return result;
}

/**
 * Block FIR filter, with data type T.  This function supports traits
 * which can be used to control the type used to perform the filter
 * accumultation.
 *
 * @param in the input data, with size incout.
 * @param state the filter state (size = numtaps), with the newest
 * sample in state[0].
 * @param out the filtered data, with size incout.
 * @param taps the filter taps (size = numtaps), stored in 'normal' order.
 * in and out are size incount.
 * @param incount
 * @param numtaps
 */
template <typename T, typename TRAITS>
void fir_blocksrl(T *in, T *taps, T *out, T *state, int incount, int numtaps)
{
#pragma HLS inline self
fir_blocksrl_foreach_data:
    for(int n = 0; n < incount; n++) {
#pragma HLS expression_balance off
        for(int i = numtaps-1; i > 0; i--) {
            state[i] = state[i-1];
        }
        state[0] = in[n];
        out[n] = TRAITS::shift( dot<typename TRAITS::T_accum>(state, taps, numtaps));
    }
}

/**
 * Block FIR filter, with data type T.  This function should be used
 * when the type used for accumulation is the same as the type of the
 * output (e.g. 'float').
 *
 * @param in the input data, with size incout.
 * @param state the filter state (size = numtaps), with the newest
 * sample in state[0].
 * @param out the filtered data, with size incout.
 * @param taps the filter taps (size = numtaps), stored in 'normal' order.
 * in and out are size incount.
 * @param incount
 * @param numtaps
 */
template <typename T>
void fir_blocksrl(T *in, T *taps, T *out, T *state, int incount, int numtaps)
{
    fir_blocksrl<T, hls_filter_traits<T> >(in, taps, out, state, incount, numtaps);
}

/**
 * Block FIR filter, with Q15 datatypes modeled as 'short'.  The
 * result always includes the required right shift by 15.
 *
 * @param in the input data, with size incout.
 * @param state the filter state (size = numtaps), with the newest
 * sample in state[0].
 * @param out the filtered data, with size incout.
 * @param taps the filter taps (size = numtaps), stored in 'normal' order.
 * in and out are size incount.
 * @param incount
 * @param numtaps
 */
static void fir_blocksrl_Q15(short *in, short *taps, short *out, short *state, int incount, int numtaps)
{
    fir_blocksrl<short, hls_filter_traits_Q15>(in, taps, out, state, incount, numtaps);
}

} // namespace hls

#endif
