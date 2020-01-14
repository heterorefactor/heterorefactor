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
 *     (c) Copyright 2008-2010 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

/**
 * @file cordic.h
 * This file contains the CORDIC function.
 */

#ifndef X_HLS_CORDIC_H
#define X_HLS_CORDIC_H

#include "utils/x_hls_defines.h"
#include "utils/x_hls_utils.h"
#include "hls_big_mult.h"
#include "hls_range_redux.h"
#include "hls_cordic_tables.h"

/**
 * The CORDIC function implements a configurable optimized CORDIC block.
 * The block naturally operates on fixed point data but the input data width and
 * associated number of iterations can be arbitrarily extended.
 *
 *
 */

/* @{ */

namespace cordic
{

template <typename T>
class _const
{
};

template <>
class _const<float>
{
public:
    static const uint32_t zero   = 0x00000000;
    static const uint32_t pi_4   = 0x3F490FDB;
    static const uint32_t pi_2   = 0x3FC90FDB;
    static const uint32_t _3pi_4 = 0x4016CBE4;
    static const uint32_t pi     = 0x40490FDB;
};

template <>
class _const<double>
{
public:
    static const uint64_t zero   = 0x0000000000000000;
    static const uint64_t pi_4   = 0x3FE921FB54442D18;
    static const uint64_t pi_2   = 0x3FF921FB54442D18;
    static const uint64_t _3pi_4 = 0x4002D97C7F3321D2;
    static const uint64_t pi     = 0x400921FB54442D18;
};

/**
 * Performs selectable add/sub operation.
 *
 * @param a First input variable.
 * @param b Second input variable.
 * @param add Add/sub selector. 1 = add. 0 = subtract.
 *
 * @return Result of addsub operation.
 */
template<int II1, int LAT1, class T1, class T2, class T3, class T4>
T1 addsub(
    T2 a,
    T3 b,
    T4 add)
{
    if(II1 == 0) {
        if(LAT1 == 1) {
#pragma HLS INTERFACE ap_none port=return register
        }
    }
#pragma HLS LATENCY MAX=LAT1

    T1 tmp;
    if(add) {
        tmp = a + b;
    } else {
        tmp = a - b;
    }
    return tmp;
}

/**
 * Performs selectable add/sub operation on float/ doubles.
 *
 * @param a First input variable.
 * @param b Second input variable.
 * @param add Add/sub selector. 1 = add. 0 = subtract.
 *
 * @return Result of addsub operation.
 */
template<class T1, class T2, class T3, class T4>
T1 addsub_floatdouble(
	T2 a,
	T3 b,
	T4 add)
{
	T1 tmp;
	fp_struct<T3> b_mod(b);
	b_mod.sign = !(add^b_mod.sign);

	tmp = a + b_mod.to_ieee();
	return tmp;
}

/**
 * Shift a by b bits depending on direction value dir.
 * Shifting is accomplished via exponent addition/ subtraction and overflows/
 * underflows are handled as appropriate. 
 * 
 * SHIFT_RIGHT: shift right is true or left if false
 * 
 */
template<bool SHIFT_RIGHT, class T1, class T2>
T1 shift_floatdouble(
	T1 a,
	T2 b)
{
        assert( (b>=0) && "number of shift bits must be non-negative, use bool value SHIFT_RIGHT for left-shift or right-shift");
        if (a == 0)
            return a;
	fp_struct<T1> tmp(a);
	if(SHIFT_RIGHT) {
            if (tmp.exp == 0) { // subnormal;
                tmp.sig = (tmp.sig >> b);
            } else {
		int new_exp = (int)(tmp.exp) - b;
		int min_bias = 1;
		if(new_exp < min_bias) { // underflow
			tmp.exp = 0;
                        tmp.sig = tmp.sig >> 1;
                        tmp.sig[tmp.SIG_BITS-1] = 1;
			tmp.sig = (tmp.sig >> (-new_exp));
		} else {
			tmp.exp = new_exp;
		}
            }
	} else { // shifting left, non-subnormal
		int new_exp = (int)(tmp.exp) + b;
		int max_bias = 2*tmp.EXP_BIAS;
		if(new_exp > max_bias) { // overflow, round to INF
			tmp.exp = 2*tmp.EXP_BIAS+1; // set all 1's on exp
			tmp.sig = 0;
		} else {
			tmp.exp = new_exp;
                        // FIXME: uncomment below to handle subnormal shifting left (may fail co-sim)
                        /*
                        if (tmp.exp != 0)
			        tmp.exp = new_exp;
                        else { // subnormal
                                int k = 0, flag = 0;
                                for (int i = 0; i < tmp.SIG_BITS; i++) {
                                    if (tmp.sig[tmp.SIG_BITS-1-i] == 1 && flag == 0) {
                                        k = i;
                                        flag = 1;
                                    }
                                }
                                if (b <= k) {
                                    tmp.sig = (tmp.sig << b);
                                } else {
                                    tmp.sig = tmp.sig << (k+1); // hidden bit = 1;
                                    tmp.exp = b-k;
                                }
                        }
                        */
		}
	}
	return tmp.to_ieee();
}


/**
 * Main CORDIC sub-function (version 1) which performs the shift/addsub operations.
 *
 * @param theta Input angle (in radians).
 * @param s Output result of sin(theta) in radians.
 * @param c Output result of cos(theta) in radians.
 */
template<int W, int N, int MODE, int II1, int LAT1, class T>
void cordic_circ_v1(
	T &x,
	T &y,
	T &z)
{
#pragma HLS INLINE SELF

    const int n = (N>W) ? W : N;
    const int LAT2 = LAT1/N;

	int k;
	T tx, ty, tz;
	ap_uint<1> d;

	for (k=0; k<n; ++k) {
#ifdef DEBUG
		std::cout << "k: " << std::dec << k;
#endif
		if(MODE == 1) {
			d = ~y[n-1];
#ifdef DEBUG
			std::cout << ", y: " << std::hex << y.to_string(16).c_str()  << "(" << y[n-1] << "," << d << ")";
#endif
		} else { // == 0
			d = z[n-1];
#ifdef DEBUG
			std::cout << ", z: " << std::hex << z.to_string(16).c_str() << "(" << z[n-1] << "," << d << ")";
#endif
		}
#ifdef DEBUG
		std::cout << ", z: " << std::hex << z.to_string(16).c_str() 
			<< "(" << std::dec << z.to_string(10).c_str() << ")\n";
#endif
		T y_s = y >> k;
		tx = addsub<II1,LAT2,T>(x, y_s, d);
		T x_s = x >> k;
		ty = addsub<II1,LAT2,T>(y, x_s, !d);
		T z_s; z_s  = cordic_ctab_table_128[k];
		tz = addsub<II1,LAT2,T>(z, z_s, d);
		x = tx; y = ty; z = tz;
	}
}

/**
 * Main CORDIC sub-function (version 1) which performs the shift/addsub operations.
 * intending to operate on floats/ doubles directly.
 *
 * @param x (in radians).
 * @param y 
 * @param z 
 */
template<int N, int MODE, class T>
void cordic_circ_floatdouble_v1(
	T &x,
	T &y,
	T &z)
{
#pragma HLS INLINE SELF

	int k;
	T tx, ty, tz;
	ap_uint<1> d;

	for (k=0; k<N; ++k) {
#ifdef DEBUG
		std::cout << "k: " << std::dec << k;
#endif
		fp_struct<T> y_tmp(y);
		fp_struct<T> z_tmp(z);

		if(MODE == 1) {
			d = ~y_tmp.sign;
#ifdef DEBUG
			std::cout << ", y: " << std::hex << y << "(" << d << ")";
#endif
		} else { // == 0
			d = z_tmp.sign;
#ifdef DEBUG
			std::cout << ", z: " << std::hex << z << "(" << d << ")";
#endif
		}
#ifdef DEBUG
		std::cout << ", z: " << std::hex << z << std::endl;
#endif
		T y_s   = shift_floatdouble<true,T>(y,k);
		tx      = addsub_floatdouble<T>(x, y_s, d);
		T x_s   = shift_floatdouble<true,T>(x,k);
		ty      = addsub_floatdouble<T>(y, x_s, !d);
		T z_s   = cordic_ctab_table_floatdouble_128[k];
		tz      = addsub_floatdouble<T>(z, z_s, d);

		x = tx; y = ty; z = tz;
	}
}

template<int W, int N, int MODE, int II1, int LAT1, class T>
void cordic_hyperb_v1(
	T &x,
	T &y, 
	T &z) 
{ 
#pragma HLS INLINE SELF 

	const int n = (N>W) ? W : N; 
	const int n1 = n + n/4; 
	const int LAT2 = LAT1/N; 

//	int k, k1; 
	int k1; 
	T tx, ty, tz; 
	ap_uint<1> d; 

//	k=1;
	for (k1=0; k1<n1; ++k1) {
		int k = k1 - (k1/4) + 1;
#ifdef DEBUG
		std::cout << "k: " << std::dec << k << ", k1: " << std::dec << k1;
		std::cout << ", x: " << std::scientific << x.to_float() //x.to_string(10)
			<< ", y: " << std::scientific << y.to_float() //y.to_string(10)
			<< ", z: " << std::scientific << z.to_float(); //z.to_string(10);

#endif
		if(MODE == 1) {
			d = ~y[n-1];
#ifdef DEBUG
			std::cout << " (y: " << y.to_string(16) << ", d: " << d << ")\n";
#endif
		} else { // == 0
			d = z[n-1];
#ifdef DEBUG
			std::cout << " (z: " << z.to_string(16) << ", d: " << d << ")\n";
#endif
		}
		T y_s = y >> k;
		tx = addsub<II1,LAT2,T>(x, y_s, !d);
#ifdef DEBUG
		std::cout << "y_s: " << std::scientific << y_s.to_float() << ", tx: " << std::scientific << tx.to_float();
#endif
		T x_s = x >> k;
		ty = addsub<II1,LAT2,T>(y, x_s, !d);
//		T z_s = cordic_hyperb_table_128[k-1];
		T z_s = cordic_hyperb_table_128_160[k1];
#ifdef DEBUG
		std::cout << ", z_s: " << std::scientific << z_s.to_float() << std::endl;
#endif
		tz = addsub<II1,LAT2,T>(z, z_s, d);

		x = tx; y = ty; z = tz;
	}
}

template<int N, int MODE, class T>
void cordic_hyperb_floatdouble_v1(
	T &x,
	T &y,
	T &z)
{
#pragma HLS INLINE SELF

	int k, k1;
	T tx, ty, tz;
	ap_uint<1> d;

	k=1;
	int cnt = 0;
	const int n1 = N + N/4;
	for (k1=0; k1<n1; ++k1) {
		fp_struct<T> y_tmp(y);
		fp_struct<T> z_tmp(z);
		
#ifdef DEBUG
		std::cout << "k: " << std::dec << k << ", k1: " << std::dec << k1 << ", cnt: " << cnt;
		std::cout << ", x: " << std::scientific << x 
			<< ", y: " << std::scientific << y
			<< ", z: " << std::scientific << z;
#endif
		if(MODE == 1) {
			d = ~y_tmp.sign;
#ifdef DEBUG
			std::cout << " (y: " << y_tmp.sign << ", d: " << d << ")\n";
#endif
		} else { // == 0
			d = z_tmp.sign;
#ifdef DEBUG
			std::cout << " (z: " << z_tmp.sign << ", d: " << d << ")\n";
#endif
		}
		T y_s = shift_floatdouble<true,T>(y,k);
		tx    = addsub_floatdouble<T>(x, y_s, !d);
		T x_s = shift_floatdouble<true,T>(x,k);
		ty    = addsub_floatdouble<T>(y, x_s, !d);
		T z_s = cordic_hyperb_table_floatdouble_128[k-1];
		tz    = addsub_floatdouble<T>(z, z_s, d);
#ifdef DEBUG
		std::cout << "y_s: " << y_s << ", tx: " << tx;
		std::cout << ", z_s: " << z_s << std::endl;
#endif
		x = tx; y = ty; z = tz;

		if(cnt == 3) {
			cnt = 0;
		} else {
			cnt = cnt + 1;
			k++;
 		}
	}
}



/*
* CORDIC-based sin/cos using payne-hanek range-reduction algorithm
* single-precision float
*/
static
int
sin_cos_range_redux_cordic(
    float t_in,
    float &s_out,
    float &c_out)
{
    const int p1 = 20;
    const int CW = 38;

    fp_struct<float> din(t_in);

    ap_uint<3> k;
    ap_int<CW> dina1, souta, couta;

    ap_uint<CW> dout;
    range_redux_payne_hanek<p1,CW,float>(t_in,k,dout);
	dina1 = dout >> 2;

#ifdef DEBUG
    printf("dout: %s, dina1: %s, k: %d\n",dout.to_string(16).c_str(),dina1.to_string(16).c_str(),k.to_uint());
#endif
	ap_fixed<CW+1,3> x, y, z;
	x.range() = ref_cordic(99,100-CW) >> 2;
	y = 0;
	z.range() = dina1;
	cordic_circ_v1<CW,CW,0,0,0>(x,y,z);
	couta = x.range();
	souta = y.range();
#ifdef DEBUG
    ap_fixed<CW,2> dina1_fxd; dina1_fxd.range() = dina1; double dina1_d = dina1_fxd;
    ap_fixed<CW,2> souta_fxd; souta_fxd.range() = souta; double souta_d = souta_fxd;
    ap_fixed<CW,2> couta_fxd; couta_fxd.range() = couta; double couta_d = couta_fxd;

    std::cout << "dina1: " << std::scientific << dina1_d
              << "(" << dina1_fxd.to_string(16).c_str() << "), "
              << "souta: " << std::scientific << souta_d
              << "(" << souta_fxd.to_string(16).c_str() << "), "
              << "couta: " << std::scientific << couta_d
              << "(" << couta_fxd.to_string(16).c_str() << ")\n";

    float jdina1 = (float)dina1/(float)pow(2.0,CW-2);
    float jsouta = (float)souta/(float)pow(2.0,CW-2);
    float jcouta = (float)couta/(float)pow(2.0,CW-2);
    std::cout << "dina1: " << std::scientific << jdina1
              << "(" << std::hex << *(int*)(&jdina1)
              << "/" << dina1.to_string(16).c_str() << "), "
              << "souta: " << std::scientific << jsouta
              << "(" << std::hex << *(int*)(&jsouta)
              << "/" << souta.to_string(16).c_str() << "), "
              << "couta: " << std::scientific << jcouta
              << "(" << std::hex << *(int*)(&jcouta)
              << "/" << couta.to_string(16).c_str() << ")\n";

    ap_uint<CW+5> j0 = dout((2*24)+p1-1,(2*24)+p1-(CW+5));
    j0 = j0 >> 1;
    float j0_f = convToFloat<CW+5>(j0);
    std::cout << "j0_f: " << std::scientific << j0_f << ", " << std::hex << *(int*)(&j0_f) << std::endl;

#endif

    if(dina1 != 0) {
        souta = souta << 1;
        couta = couta << 1;
    } else {
#ifdef DEBUG
        std::cout << "dina1 is all 0s\n";
#endif
        souta = 0;
        couta = ((ap_uint<CW>)0x1 << (CW-1)) - dina1;
    }
    float tmp_s_out, tmp_c_out;
    tmp_s_out = convToFloat<CW>(souta);
    tmp_c_out = convToFloat<CW>(couta);

#ifdef DEBUG
		std::cout << "tmp_s_out: " << std::scientific << tmp_s_out 
			<< "(" << std::hex << *(int*)(&tmp_s_out) << "), "
			<< "tmp_c_out: " << std::scientific << tmp_c_out 
			<< "(" << std::hex << *(int*)(&tmp_c_out) << ")\n";

#endif

        int sel = (din.__signbit()) ? 7 - k.to_uint() : k.to_uint();
	switch(sel) {
		case 0:
			s_out =  tmp_s_out;
			c_out =  tmp_c_out;
			break;
		case 1:
			s_out =  tmp_c_out;
			c_out =  tmp_s_out;
			break;
		case 2:
			s_out =  tmp_c_out;
			c_out = -tmp_s_out;
			break;
		case 3:
			s_out =  tmp_s_out;
			c_out = -tmp_c_out;
			break;
		case 4:
			s_out = -tmp_s_out;
			c_out = -tmp_c_out;
			break;
		case 5:
			s_out = -tmp_c_out;
			c_out = -tmp_s_out;
			break;
		case 6:
			s_out = -tmp_c_out;
			c_out =  tmp_s_out;
			break;
		case 7:
			s_out = -tmp_s_out;
			c_out =  tmp_c_out;
			break;
		default: // case 0
			s_out = tmp_s_out;
			c_out = tmp_c_out;
	}

	if(din.exp == 0xFF) { // +inf, -inf, nan
        s_out = hls::nan("");
        c_out = hls::nan("");
    } else if(din.exp < (127-11)) {  // exp=-10 -> ulp=6, exp=-11 -> ulp=1
        s_out = t_in;
    }

    return 1;
}



/*
* CORDIC-based sin/cos using payne-hanek range-reduction algorithm
*/
static
int
sin_cos_range_redux_cordic(
    double t_in,
    double &s_out,
    double &c_out)
{
    const int p1 = 16;
    const int CW = 78;

    fp_struct<double> din(t_in);

    ap_uint<3> k;
	ap_uint<CW> dout;

    range_redux_payne_hanek<p1,CW,double>(t_in,k,dout);

    ap_int<CW> dina1, souta, couta;
    dina1 = dout >> 2;
#ifdef DEBUG
    printf("dout: %s, dina1: %s, k: %d\n",dout.to_string(16).c_str(),dina1.to_string(16).c_str(),k.to_uint());
#endif
	ap_fixed<CW+1,3> x, y, z;
	x.range() = ref_cordic(99,100-CW) >> 2;
	y = 0;
	z.range() = dina1;
	cordic_circ_v1<CW,CW,0,0,0>(x,y,z);
	couta = x.range();
	souta = y.range();
#ifdef DEBUG
    ap_fixed<CW,2> dina1_fxd; dina1_fxd.range() = dina1; double dina1_d = dina1_fxd;
    ap_fixed<CW,2> souta_fxd; souta_fxd.range() = souta; double souta_d = souta_fxd;
    ap_fixed<CW,2> couta_fxd; couta_fxd.range() = couta; double couta_d = couta_fxd;

    std::cout << "dina1: " << std::scientific << dina1_d
              << "(" << dina1_fxd.to_string(16).c_str() << "), "
              << "souta: " << std::scientific << souta_d
              << "(" << souta_fxd.to_string(16).c_str() << "), "
              << "couta: " << std::scientific << couta_d
              << "(" << couta_fxd.to_string(16).c_str() << ")\n";
#endif

    if(dina1 != 0) {
        souta = souta << 1;
        couta = couta << 1;
    } else {
#ifdef DEBUG
        std::cout << "dina1 is all 0s\n";
#endif
        souta = 0;
        couta = ((ap_uint<CW>)0x1 << (CW-1)) - dina1;
    }
    double tmp_s_out, tmp_c_out;
    tmp_s_out = convToDouble<CW>(souta);
    tmp_c_out = convToDouble<CW>(couta);

    int sel = (din.__signbit()) ? 7 - k.to_uint() : k.to_uint();
    switch(sel) {
        case 0:
            s_out =  tmp_s_out;
            c_out =  tmp_c_out;
            break;
        case 1:
            s_out =  tmp_c_out;
            c_out =  tmp_s_out;
            break;
        case 2:
            s_out =  tmp_c_out;
            c_out = -tmp_s_out;
            break;
        case 3:
            s_out =  tmp_s_out;
            c_out = -tmp_c_out;
            break;
        case 4:
            s_out = -tmp_s_out;
            c_out = -tmp_c_out;
            break;
        case 5:
            s_out = -tmp_c_out;
            c_out = -tmp_s_out;
            break;
        case 6:
            s_out = -tmp_c_out;
            c_out =  tmp_s_out;
            break;
        case 7:
            s_out = -tmp_s_out;
            c_out =  tmp_c_out;
            break;
        default: // case 0
            s_out = tmp_s_out;
            c_out = tmp_c_out;
    }

    if(din.exp == 0x7FF) { // +inf, -inf, nan
        s_out = hls::nan("");
        c_out = hls::nan("");
    } else if(din.exp < (1023-24)) {  // exp=-24 -> ulp=3, exp=-25 -> ulp=1

        s_out = t_in;
    }
    return 1;
}

static
void 
sincos(
	double t_in,
	double *s_out,
	double *c_out)
{
	sin_cos_range_redux_cordic(t_in,*s_out,*c_out);
}

static float
sinf(float t_in)
{
    float s_out, c_out;
    sin_cos_range_redux_cordic(t_in, s_out, c_out);
    return s_out;
}

static double
sin(double t_in)
{
    double s_out, c_out;
    sin_cos_range_redux_cordic(t_in, s_out, c_out);
    return s_out;
}

static float
cosf(float t_in)
{
    float s_out, c_out;
    sin_cos_range_redux_cordic(t_in, s_out, c_out);
    return c_out;
}

static double
cos(double t_in)
{
    double s_out, c_out;
    sin_cos_range_redux_cordic(t_in, s_out, c_out);
    return c_out;
}

/*
* CORDIC-based atan using payne-hanek range-reduction algorithm
* single-precision float
*/
static
void
atan_range_redux_cordic(
	float t_in,
	float &t_out)
{
	const int p1 = 16;
	const int CW = 38;

	ap_uint<3> k;
	ap_uint<2*24+p1> dout;

	fp_struct<float> fps1(t_in);
	int exp = fp_struct<float>::EXP_BIAS - fps1.exp;
	const int SB = fp_struct<float>::SIG_BITS;
	ap_uint<SB+1> X = ((ap_uint<SB+1>)0x1 << SB) | (fps1.sig);
	dout = 0;
	dout((2*24)+p1-1,(2*24)+p1-(SB+1)) = X;
	dout = dout >> (exp-1);
#ifdef DEBUG
	std::cout << "t_in: " << std::scientific << t_in
		<< ", exp: " << std::dec << exp << ", X: " << X.to_string(16).c_str() << std::endl;
#endif
	ap_uint<CW> dtmp;
	ap_fixed<CW+1,3> dina1, x, y, z;
	dtmp = dout((2*24)+p1-1,(2*24)+p1-CW); // Q0.x
	dina1.range() = (dtmp >> 2); // for CW+1,3

#ifdef DEBUG

	std::cout << "dout: " << dout.to_string(16).c_str()
		<< ", dina1: " << dina1.to_string(16).c_str() << std::endl;


	std::cout << "dina1: " << dina1.to_string(16).c_str()
		<< ", k: " << std::dec << k.to_uint() << std::endl;
#endif

	x = 1;
	y = dina1;
	z = 0;
#ifdef DEBUG
	std::cout << "x: " << x.to_string(16).c_str()
		<< ", y: " << y.to_string(16).c_str()
		<< ", z: " << z.to_string(16).c_str() << std::endl;
#endif
	cordic_circ_v1<CW,CW,1,0,0>(x,y,z);

#ifdef DEBUG
	std::cout << "x: " << x.to_string(16).c_str()
		<< ", y: " << y.to_string(16).c_str()
		<< ", z: " << z.to_string(16).c_str() << std::endl;
#endif

	t_out = convToFloat<CW,3>(z);
}

/*
* CORDIC-based atan restricted to range 0 to 1
* single-precision float
*/
static
void sincosf(
    float t_in,
    float *s,
    float *c)
{
    float s_out, c_out;
    sin_cos_range_redux_cordic(t_in, s_out, c_out);
    *s = s_out;
    *c = c_out;
}


/*
*/
static
float
atan_cordic_floatdouble(
	float t_in)
{
	const int N = 23;
	float x = 1.0;
	float y = t_in;
	float z = 0.0;
#ifdef DEBUG
	std::cout << "x: " << x
		<< ", y: " << y
		<< ", z: " << z << std::endl;
#endif
	cordic_circ_floatdouble_v1<N,1>(x,y,z);
#ifdef DEBUG
	std::cout << "x: " << x
		<< ", y: " << y
		<< ", z: " << z << std::endl;
#endif
	return z;
}

/**
 * atan(x) = pi/2 - atan(1/x)
 */
static
float
atan_cordic(
	float t_in)
{
	fp_struct<float> tmpin(t_in);
	ap_uint<1> neg = tmpin.sign;
	tmpin.sign = 0;
	float pos = tmpin.to_ieee();

	float one_over_x = (float)1.0/pos;
	float din = (pos > (float)1.0) ? one_over_x : pos;
	float res; 
	atan_range_redux_cordic(din, res);
	res = (din < (float)4e-4) ? din : res;
    float pi_over_2 = fp_struct<float>(_const<float>::pi_2).to_ieee();
    float pi_over_4 = fp_struct<float>(_const<float>::pi_4).to_ieee();
	res = (pos > (float)1.0) ? pi_over_2 - res : res;
	res = (pos == (float)1.0) ? pi_over_4 : res;
	fp_struct<float> tmpout(res);
	tmpout.sign = neg;

    if(tmpin.exp == 0xFF) { // +inf, -inf, nan
		if(tmpin.sig == 0) { // +inf, - inf
#ifdef DEBUG
			std::cout << "INF/-INF found!\n";
#endif
			fp_struct<float> res_tmp(_const<float>::pi_2);
			res_tmp.sign = neg;
			res = res_tmp.to_ieee();	
		} else {
#ifdef DEBUG
			std::cout << "NAN found!\n";
#endif
			res = hls::nan("");
		}
	} else {
		res = tmpout.to_ieee();
	}

	return res;
}

static float
atanf(float t_in) {
	return atan_cordic(t_in);	
}

/*
* x_in and y_in are both positive and y_in < x_in
*/
static
float
atan2_range_redux_cordic(
	float y_in,
        float x_in)
{
    if ( y_in < shift_floatdouble<true, float>(x_in, 12) ) { // atan(y/x) = y/x
        return y_in/x_in;
    }

    // normalize y_in and x_in
    fp_struct<float> fps_x(x_in);
    fp_struct<float> fps_y(y_in);
    ap_uint<1> hidden_bit_x = (fps_x.exp == 0) ? 0 : 1;
    ap_uint<1> hidden_bit_y = (fps_y.exp == 0) ? 0 : 1;
    
    int d_exp = fps_x.exp - fps_y.exp;
    if (fps_x.exp > 0 && fps_y.exp == 0)
        d_exp--;

    const int p1 = 16;
    const int CW = 38;

    const int SB = fp_struct<float>::SIG_BITS;
    ap_uint<2*(SB+1)+p1> dout_y, dout_x;
    ap_uint<CW> dtmp_y, dtmp_x;
    ap_fixed<CW+1,3> x, y, z;

    ap_uint<SB+1> Y = ((ap_uint<SB+1>)hidden_bit_y << SB) | (fps_y.sig);
    dout_y = 0;
    dout_y((2*(SB+1))+p1-1,(2*(SB+1))+p1-(SB+1)) = Y;
    dout_y = dout_y >> d_exp;
    dtmp_y = dout_y((2*(SB+1))+p1-1,(2*(SB+1))+p1-CW);
    y.range() = (dtmp_y >> 2); // for CW+1,3
    
    ap_uint<SB+1> X = ((ap_uint<SB+1>)hidden_bit_x << SB) | (fps_x.sig);
    dout_x = 0;
    dout_x((2*(SB+1))+p1-1,(2*(SB+1))+p1-(SB+1)) = X;
    dtmp_x = dout_x((2*(SB+1))+p1-1,(2*(SB+1))+p1-CW);
    x.range() = (dtmp_x >> 2); // for CW+1,3

    z = 0;
#ifdef DEBUG
	std::cout << "x: " << x.to_string(16).c_str()
		<< ", y: " << y.to_string(16).c_str()
		<< ", z: " << z.to_string(16).c_str() << std::endl;
#endif
    cordic_circ_v1<CW,CW,1,0,0>(x,y,z);

#ifdef DEBUG
	std::cout << "x: " << x.to_string(16).c_str()
		<< ", y: " << y.to_string(16).c_str()
		<< ", z: " << z.to_string(16).c_str() << std::endl;
#endif
    return convToFloat<CW,3>(z);
}

static
double
atan2_range_redux_cordic(
	double y_in,
        double x_in)
{
    if ( y_in < shift_floatdouble<true, double>(x_in, 28) ) { // atan(y/x) = y/x
        return y_in/x_in;
    }

    // normalize y_in and x_in
    fp_struct<double> fps_x(x_in);
    fp_struct<double> fps_y(y_in);
    ap_uint<1> hidden_bit_x = (fps_x.exp == 0) ? 0 : 1;
    ap_uint<1> hidden_bit_y = (fps_y.exp == 0) ? 0 : 1;
    
    int d_exp = fps_x.exp - fps_y.exp;
    if (fps_x.exp > 0 && fps_y.exp == 0)
        d_exp--;

    const int p1 = 16;
    const int CW = 78;

    const int SB = fp_struct<double>::SIG_BITS;
    ap_uint<2*(SB+1)+p1> dout_y, dout_x;
    ap_uint<CW> dtmp_y, dtmp_x;
    ap_fixed<CW+1,3> x, y, z;

    ap_uint<SB+1> Y = ((ap_uint<SB+1>)hidden_bit_y << SB) | (fps_y.sig);
    dout_y = 0;
    dout_y((2*(SB+1))+p1-1,(2*(SB+1))+p1-(SB+1)) = Y;
    dout_y = dout_y >> d_exp;
    dtmp_y = dout_y((2*(SB+1))+p1-1,(2*(SB+1))+p1-CW);
    y.range() = (dtmp_y >> 2); // for CW+1,3
    
    ap_uint<SB+1> X = ((ap_uint<SB+1>)hidden_bit_x << SB) | (fps_x.sig);
    dout_x = 0;
    dout_x((2*(SB+1))+p1-1,(2*(SB+1))+p1-(SB+1)) = X;
    dtmp_x = dout_x((2*(SB+1))+p1-1,(2*(SB+1))+p1-CW);
    x.range() = (dtmp_x >> 2); // for CW+1,3

    z = 0;
#ifdef DEBUG
	std::cout << "x: " << x.to_string(16).c_str()
		<< ", y: " << y.to_string(16).c_str()
		<< ", z: " << z.to_string(16).c_str() << std::endl;
#endif
    cordic_circ_v1<CW,CW,1,0,0>(x,y,z);

#ifdef DEBUG
	std::cout << "x: " << x.to_string(16).c_str()
		<< ", y: " << y.to_string(16).c_str()
		<< ", z: " << z.to_string(16).c_str() << std::endl;
#endif
    return convToDouble<CW,3>(z);
}


template<class T>
static T
atan2_cordic_floatdouble(
	T y_in,
        T x_in)
{
#pragma HLS ALLOCATION instances=atan2_range_redux_cordic limit=1 function
    fp_struct<T> fpsx(x_in);
    fp_struct<T> fpsy(y_in);
    if (fpsx.data() == fpsy.data()) 
        return fp_struct<T>(_const<T>::pi_4).to_ieee();
    else if (y_in > x_in)
        return fp_struct<T>(_const<T>::pi_2).to_ieee() - atan2_range_redux_cordic(x_in, y_in);
    else
        return atan2_range_redux_cordic(y_in, x_in);
}


template<class T>
static T
atan2_cordic(
        T y_in,
        T x_in)
{
    fp_struct<T> fpsx(x_in);
    fp_struct<T> fpsy(y_in);
    if ( hls::__isnan(y_in) || hls::__isnan(x_in) ) { // y or x is NaN
        return hls::nan("");
    }

    int m = ( (fpsx.sign.to_int() << 1) | fpsy.sign.to_int() ); // m: bit_1: sign of x; bit_0: sign of y

    if ( (fpsy.data() << 1) == 0 ) { // y = +-0
        switch(m) {
            case 0: return fp_struct<T>(_const<T>::zero).to_ieee();                         // atan2(+0, +x) = +0
            case 1: return -fp_struct<T>(_const<T>::zero).to_ieee();                        // atan2(-0, +x) = -0
            case 2: return fp_struct<T>(_const<T>::pi).to_ieee();                           // atan2(+0, -x) = pi
            case 3: return -fp_struct<T>(_const<T>::pi).to_ieee();                          // atan2(-0, -x) = -pi
        }
    } else if ( (fpsx.data() << 1) == 0 ) { // x = +-0, y != 0
        switch(m&1) {
            case 0: return fp_struct<T>(_const<T>::pi_2).to_ieee();                         // atan2(+y, +-0) = pi/2
            case 1: return -fp_struct<T>(_const<T>::pi_2).to_ieee();                        // atan2(-y, +-0) = -pi/2
        }
    } else if ( hls::__isinf(x_in) && hls::__isinf(y_in) ) { // x, y = +-INF
        switch(m) {
            case 0: return fp_struct<T>(_const<T>::pi_4).to_ieee();                         // atan2(+INF, +INF) = pi/4
            case 1: return -fp_struct<T>(_const<T>::pi_4).to_ieee();                        // atan2(-INF, +INF) = -pi/4
            case 2: return fp_struct<T>(_const<T>::_3pi_4).to_ieee();                       // atan2(+INF, -INF) = 3*pi/4
            case 3: return -fp_struct<T>(_const<T>::_3pi_4).to_ieee();                      // atan2(-INF, -INF) = -3*pi/4
        }
    } else if ( hls::__isinf(x_in) ) { // x = +- INF, y != +- INF
        switch(m) {
            case 0: return fp_struct<T>(_const<T>::zero).to_ieee();                         // atan2(+y, +INF) = +0
            case 1: return -fp_struct<T>(_const<T>::zero).to_ieee();                        // atan2(-y, +INF) = -0
            case 2: return fp_struct<T>(_const<T>::pi).to_ieee();                           // atan2(+y, -INF) = pi
            case 3: return -fp_struct<T>(_const<T>::pi).to_ieee();                          // atan2(-y, -INF) = -pi
        }
    } else if ( hls::__isinf(y_in) ) { // y = +- INF, x != +- INF
        switch(m&1) {
            case 0: return fp_struct<T>(_const<T>::pi_2).to_ieee();                         // atan2(+INF, +-x) = pi/2
            case 1: return -fp_struct<T>(_const<T>::pi_2).to_ieee();                        // atan2(-INF, +-x) = -pi/2
        }
    } else { // x, y != +-INF
        switch(m) {
            case 0:  // quadrant I
                return atan2_cordic_floatdouble(y_in, x_in);
            case 1:  // quadrant IV
                return -atan2_cordic_floatdouble(-y_in, x_in);
            case 2:  // quadrant II
                return fp_struct<T>(_const<T>::pi).to_ieee() - atan2_cordic_floatdouble(y_in, -x_in);
            case 3:  // quadrant III
                return atan2_cordic_floatdouble(-y_in, -x_in) - fp_struct<T>(_const<T>::pi).to_ieee();
        }
    }
    return 0;
}


static float
atan2f(float y_in, float x_in) {
    return atan2_cordic(y_in, x_in);
}

static double
atan2(double y_in, double x_in) {
    return atan2_cordic(y_in, x_in);
}



/**
 * Range reduces input to between -ln2/2 and ln2/2 by 
 * solving for k and y in x = k*ln2 + y, where x = t_in.
 * 
 * Theoretical max value of x is 710.48 where any larger value gives sinh(x) = inf
 * In practcie, for sinhf(float), x,max < 2^7 and sinh(double), x,max < 2^9
 * 
 * k = (int)(x/ln2); k,max = 1025 (11 bits)
 * y = (dec)(x/ln2)*ln2
 *
 * If t_in > X, the final sinh/cosh result will be inf so we set k = 130 to indicate
 * that downstream.
 * 
 * H_INT_MAX: maximum value for which sinh(x) = inf. float(7), double(9), theoretical(11)
 * H_DEC_MIN: minimum value fo rwhich sinh(x) = x.   float(-12), double(-28)
 * k,max = x,max/ln2 or approx x,width+1
 */
template<int H_INT_MAX, int H_DEC_MIN, typename T>
void hyperb_range_redux(
	T t_in, 
	ap_uint<H_INT_MAX+1>   &k, 
	ap_ufixed<fp_struct<T>::SIG_BITS+H_DEC_MIN,0> &t_out)
{
	fp_struct<T> fp_in(t_in);
	// Q7.36 = 6+1.23+13, value ranges from Q7.17 to Q0.36(.12/24)
	const int W1 = H_INT_MAX+fp_struct<T>::SIG_BITS+H_DEC_MIN;
	const int I1 = H_INT_MAX;

	// <43,7>
	ap_ufixed<W1,I1> t_in_f = 0;

#ifdef DEBUG
	std::cout << "W1: " << W1 << ", I1: " << I1 << ", t_in_f: " << t_in_f.to_string(16) << std::endl;
#endif

	// t_in stored by effectively shifted right by H_DEC_MIN
	t_in_f(fp_struct<T>::SIG_BITS-1,0) = fp_in.sig;
	t_in_f[fp_struct<T>::SIG_BITS] = 1;

#ifdef DEBUG
	std::cout << "fp_in: " << std::hex << fp_in.data() 
		<< ", t_in_f: " << t_in_f.to_string(16)
		<< std::endl;
#endif

	// max shift is H_DEC_MIN+H_INT_MAX-1
	int left_shift = (fp_in.expv() > H_INT_MAX-1) ? H_DEC_MIN+H_INT_MAX-1 :
						((fp_in.expv() < -H_DEC_MIN) ? 0 : (fp_in.expv() + H_DEC_MIN));
	t_in_f = t_in_f << left_shift;

#ifdef DEBUG
	std::cout << "left_shift: " << std::dec << left_shift	
		<< ", exp: " << fp_in.exp << ", expv: " << fp_in.expv() << std::endl;
#endif

	ap_ufixed<W1-I1,1> inv_ln2 = 1.4426950408889634073599246810019;
	ap_ufixed<W1-I1,0> ln2 = 0.69314718055994530941723212145818;

#ifdef DEBUG
	std::cout << "t_in_f: " << t_in_f.to_string(10)
		<< ", inv_ln2: " << inv_ln2.to_string(10) << std::endl;
#endif

	// t_in / ln2
	ap_ufixed<2*W1-I1,I1+1> prod;
	big_mult<W1,I1,W1-I1,1>(t_in_f, inv_ln2, prod);

	k     = (fp_in.expv() > H_INT_MAX-1) ? pow(2.0,H_INT_MAX) : (int)prod;

#ifdef DEBUG
	int tpp1 = (int)pow(2.0,(double)H_INT_MAX)-1;
	int tpp2 = (int)prod;
	std::cout << "tpp1: " << tpp1 << ", tpp2: " << tpp2 << std::endl;
#endif

	ap_ufixed<W1-I1,0> prod_dec = prod;

	ap_ufixed<2*(W1-I1),0> tmp_t_out;
	big_mult<W1-I1,0,W1-I1,0>(prod_dec, ln2, tmp_t_out);
	t_out = tmp_t_out;

#ifdef DEBUG
	std::cout << "range redux: t_in: " << std::scientific << t_in 
		<< ", ln2: " << ln2.to_string(10)
		<< ", inv_ln2: " << inv_ln2.to_string(10)
		<< ", k: " << std::dec << k
		<< ", prod: " << prod.to_string(10)
		<< ", prod_dec: " << prod_dec.to_string(10)
		<< ", t_out: " << std::scientific << t_out << std::endl;
#endif
}


static
void
sinh_cosh_range_redux_cordic(
	float t_in,
	float &s_out,
	float &c_out)
{
    fp_struct<float> din(t_in);

	ap_uint<7+1> k;
	ap_ufixed<23+12,0> r_tmp;
	hyperb_range_redux<7,12,float>(t_in, k, r_tmp);

	const int N = 34; // same as N = 40
	float x = 1.0/8.28133692173427360395e-01;
	float y = 0.0;
#ifdef DEBUG
	std::cout << "range_redux_k: k: " << std::dec << k << std::endl;
	std::cout << "before calc: x: " << x
		<< ", y: " << y << std::endl;
#endif

	ap_fixed<N,2> tmp_x = x;
	ap_fixed<N,2> tmp_y = 0;
	ap_fixed<N,2> tmp_z = r_tmp;
	cordic_hyperb_v1<N,N,0,0,0>(tmp_x,tmp_y,tmp_z);
	x = tmp_x; y = tmp_y;

#ifdef DEBUG
	std::cout << "after calc: x: " << x << "(" << std::hex << *((int*)(&x)) << ")"
		<< ", y: " << y << "(" << std::hex << *((int*)(&y)) << ")"
		<< std::endl;
	float z_test = r_tmp;

	float x_test = cosh(z_test); float y_test = sinh(z_test);
	std::cout << "after calc: x_test: " << x_test << "(" << std::hex << *((int*)(&x_test)) << ")"
		<< ", y_test: " << y_test << "(" << std::hex << *((int*)(&y_test)) << ")"
		<< ", z_test: " << z_test << "(" << std::hex << *((int*)(&z_test)) << ")" << std::endl;

	float x_test_d = cosh((double)z_test); float y_test_d = sinh((double)z_test);
	std::cout << "after calc: x_test_d: " << x_test_d << "(" << std::hex << *((int*)(&x_test_d)) << ")"
		<< ", y_test_d: " << y_test_d << "(" << std::hex << *((int*)(&y_test_d)) << ")"
		<< ", z_test: " << (double)z_test << std::endl;

	float x_test_f = coshf(z_test); float y_test_f = sinhf(z_test);
	std::cout << "after calc: x_test_f: " << x_test_f << "(" << std::hex << *((int*)(&x_test_f)) << ")"
		<< ", y_test_f: " << y_test_f << "(" << std::hex << *((int*)(&y_test_f)) << ")"
		<< ", z_test: " << z_test << "(" << std::hex << *((int*)(&z_test)) << ")" << std::endl;

	std::cout << "delta calc: x_test-x: " << x_test - x 
		<< ", y_test-y: " << y_test -y << std::endl;

#endif

#ifdef DEBUG
	std::cout << "x: " << x
		<< ", y: " << y << std::endl;
#endif

	float two_p_plus_k, two_p_minus_k;
    //	int p1 = k-1+127;
	unsigned int pos_exp = ((int)k > pow(2.0,7)-1) ? 255 : (int)(k-1+127); 
	unsigned int neg_exp = ((int)k < -126) ? 255 : (int)(-k-1+127); 
    fp_struct<float> two_p_plus_s(0u);
    fp_struct<float> two_p_minus_s(0u);
    two_p_plus_s.exp = pos_exp;
    two_p_minus_s.exp = neg_exp;
	two_p_plus_k = two_p_plus_s.to_ieee();
	two_p_minus_k = two_p_minus_s.to_ieee();

#ifdef DEBUG
	std::cout << "k: " << std::dec << k << ", pos_exp: " << pos_exp << ", neg_exp: " << neg_exp << std::endl;
	std::cout << std::hex << "two_p_plus_k: " << two_p_plus_k 
              << ", two_p_minus_k: " << two_p_minus_k << std::dec << std::endl;
#endif

	float sinhkln2 = two_p_plus_k - two_p_minus_k;
	float coshkln2 = two_p_plus_k + two_p_minus_k;

	float s_out_tmp = (k > 0) ? coshkln2*y + sinhkln2*x : y;
	float c_out_tmp = (k > 0) ? coshkln2*x + sinhkln2*y : x;

    fp_struct<float> s_fp(s_out_tmp);
    fp_struct<float> c_fp(c_out_tmp);
	s_fp.sign = din.sign;
	c_fp.sign = 0;

	s_out = s_fp.to_ieee();
	c_out = c_fp.to_ieee();

    if(din.exp == 0xFF) { // +inf, -inf, nan
        s_out = t_in;
		din.sign = 0;
        c_out = din.to_ieee();
	} else if(din.exp < (127-11)) {  // exp=-10 -> ulp=6, exp=-11 -> ulp=1
        s_out = t_in;
		c_out = 1.0f;
    } 
}


static 
float
sinhf(
	float t_in) 
{
	float s_out, c_out;
	sinh_cosh_range_redux_cordic(t_in,s_out,c_out);
	return s_out;
}

static 
float
coshf(
	float t_in) 
{
	float s_out, c_out;
	sinh_cosh_range_redux_cordic(t_in,s_out,c_out);
	return c_out;
}

/*
static 
double
sinh(
	double t_in) 
{
	double s_out, c_out;
	sinh_cosh_range_redux_cordic(t_in,s_out,c_out);
	return s_out;
}

static 
double
cosh(
	double t_in) 
{
	double s_out, c_out;
	sinh_cosh_range_redux_cordic(t_in,s_out,c_out);
	return c_out;
}
*/

} // namespace


/* @}*/

#endif

