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
 *     (c) Copyright 2012-2013 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

/**
 * @file hls_hotbm.h
 * An implementation of sin/cos, based on the HOTBM method published by
 * Detrey and deDinechin, "Floating-point Trigonometric Functions for FPGAs"
 */

#ifndef X_HLS_HOTBM_H
#define X_HLS_HOTBM_H

#include "utils/x_hls_defines.h"
#include "utils/x_hls_utils.h"
#include "hls_range_redux.h"

// FIXME: should be in namespace hotbm
#include "hls_hotbm_tables.h"

namespace hotbm
{

#define parts ((W+15)/16)
#define rem (W - (parts-1)*16)
template <int W, int I>
void my_to_float(ap_fixed<W, I> in, float &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<float> out;
    out.sign[0] = in[in.wl()-1];

    // Note that when we chop off the sign bit, we don't loose any
    // bits to allow for -MAX to be converted correctly.
    ap_ufixed<W,I> ain = in;

    if(out.__signbit()) {
        ain = -in;
    }

    //const int parts = (W+15)/16;
    //const int rem = W - (parts-1)*16; // The size of the last part
    assert(rem > 0);
    assert(rem <= 16);

    // Chop into 16 bit segments. out_bits[0] includes the MSBs.
    ap_uint<32> out_bits[parts] = {};
    for(int i = 0; i < parts-1; i++) {
        out_bits[i](31, 16) = ain(ain.wl()-16*i-1,ain.wl()-16*i-16);
        out_bits[i][15] = 1;
    }
    out_bits[parts-1](31,31-rem) = ain(ain.wl()-16*(parts-1)-rem);
    out_bits[parts-1][31-rem-1] = 1;

    // Count the leading zeros in each segment.
    int c[parts];
    for(int i = 0; i < parts; i++) {
        c[i] = __builtin_clz(out_bits[i].to_uint());
    }

    int shift = 0;
    ap_ufixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 16) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.
    result_mantissa(W-1,0) = in_shift(W-1,0);

    // Truncate the implicit one and zero extend, if necessary.
    out.sig = result_mantissa; // FIXME: Round here?
    assert(((int)fp_struct<float>::EXP_BIAS+in_shift.iwl()-prescale-shift)<255);
    assert(((int)fp_struct<float>::EXP_BIAS+in_shift.iwl()-prescale-shift)>0);
    out.exp = (in == 0) ? 0 : (fp_struct<float>::EXP_BIAS+in_shift.iwl())-prescale-shift;

    result = out.to_ieee();
}

template <int W, int I>
void my_to_float(ap_ufixed<W, I> in, float &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<float> out;
    out.sign[0] = 0;
    ap_ufixed<W,I> ain = in;

    //    std::cout << "ain = " << ain.to_string(2) << "\n";

    assert(rem > 0);
    assert(rem <= 16);

    // Chop into 16 bit segments. out_bits[0] includes the MSBs.
    //    std::cout << W << " " << parts << " " << rem << "\n";
    ap_uint<32> out_bits[parts] = {};
    for(int i = 0; i < parts-1; i++) {
        out_bits[i](31, 16) = ain(ain.wl()-16*i-1,
                                  ain.wl()-16*i-16);
        out_bits[i][15] = 1;
        //std::cout << out_bits[i] << "\n";
    }
    out_bits[parts-1](32-1,32-rem) = ain(ain.wl()-16*(parts-1)-1,
                                         ain.wl()-16*(parts-1)-rem);
    out_bits[parts-1][32-rem-1] = 1;
    //std::cout << out_bits[parts-1] << "\n";

    // Count the leading zeros in each segment.
    int c[parts];
    for(int i = 0; i < parts; i++) {
        c[i] = __builtin_clz(out_bits[i].to_uint());
        //std::cout << "c[" << i << "] = " << c[i] << "\n";
    }

    int shift = 0;
    ap_fixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 16) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.
    result_mantissa(W-1,0) = in_shift(W-1,0);

    // std::cout << "result_mantissa = " << result_mantissa.to_string(2) << "\n";

    // Truncate the implicit one and zero extend, if necessary.
    out.set_mantissa(result_mantissa);
    // std::cout << "sig = " << out.sig.to_string(2) << "\n";
    out.exp = (in == 0) ? 0 : (fp_struct<float>::EXP_BIAS+in_shift.iwl()-1)-prescale-shift;

    result = out.to_ieee();
}
#undef parts
#undef rem

template<int Z, int W, int I>
unsigned int clz(ap_ufixed<W,I> x)
{
    ap_uint<Z+1> t;
    t(Z,1) = x(x.wl()-1,x.wl()-Z);
    t[0] = 1;

    return t.countLeadingZeros();
}

/**
 * Algorithm for trigonometric range reduction, based on the method
 * published by Payne and Hanek.  Basically, this maps an arbitrary
 * range input onto the unit circle.  The output is expressed as the
 * quadrant k and fixed point number in [0,1).
 *
 * The Quadrants k for positive numbers:
 *  2 1
 * 3   0
 * 4   7
 *  5 6
 * The Quadrants k for negative numbers:
 *  5 6
 * 4   7
 * 3   0
 *  2 1
 * NOTE: p1 = 10 for double, p1 = 16 for single
 *
 * @param p1 Padding bits
 * @param T data type (float, double)
 * @param din Unreduced data input (float, double)
 * @param k quadrant of the circle that we are in.
 * @param dout Reduced data output
 */
template<int p1, class T, int W_dout, int W_Mx>
void
range_redux_payne_hanek_hotbm(
    T din,
    ap_uint<3> &k,
    ap_ufixed<W_dout, 0> &dout,
    ap_ufixed<W_Mx, 0> &Mx,
    ap_int<fp_struct<T>::EXP_BITS> &Ex)
{
    const int SB = fp_struct<T>::SIG_BITS;

    fp_struct<T> fps1(din);
    int expv = fps1.expv();
#ifdef DEBUG
    std::cout << "\nrange_redux_payne_hanek_hotbm: expv: " << std::dec << expv
              << ", p1: " << p1 << std::endl;
#endif

    ap_ufixed<3*(SB+1)+p1+3,SB+1+3>        h_fixed;        // X*Med

    bool closepath = ((fps1.exp) < fp_struct<T>::EXP_BIAS-1);

    if(closepath) {
        Ex = expv+2;
        expv = -2;
    } else {
        Ex = 0;
    }

    {
        ap_uint<SB+1>                 X;        // mantissa (din) in integer format
        ap_uint<2*(SB+1)+p1+3>        Med;      // significant bits of 4oPi
        ap_uint<3*(SB+1)+p1+3>        h;        // X*Med
        // Construct the mantissa, Q1.SIG_BITS
        X(fp_struct<T>::SIG_BITS-1,0) = fps1.sig(fp_struct<T>::SIG_BITS-1,0);
        X[fp_struct<T>::SIG_BITS] = 1;

        Med         = table_lookup_4oPi<SB,p1>(expv,din);
        //h = Med * X;
        big_mult(Med, X, h);
        //reinterpret bits of h.
        h_fixed(h_fixed.wl()-1, 0) = h(h_fixed.wl()-1, 0);
#ifdef DEBUG
        std::cout << "Med = " << Med.to_string(2) << "\n";
        std::cout << "X = " << X.to_string(2) << "\n";
        std::cout << "h = " << h.to_string(2) << " " << h << "\n";
        std::cout << "h_fixed = " << h_fixed.to_string(2) << " " << h_fixed << "\n";
        std::cout << "closepath = " << closepath << "\n";
#endif

    }

    ap_ufixed<W_Mx+18, 0> Mx_bits = h_fixed; // Mx_bits is not normalized !

    if(closepath) {
        k = 0;
    } else {
        k = h_fixed; // Extract the quadrant from the integer bits of h_fixed.
    }

    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        Mx_bits = 1-Mx_bits;
    }

    ap_uint<5> Mx_zeros = clz<18>(Mx_bits);

#ifdef DEBUG
    std::cout << "Mx_bits = " << Mx_bits << " " << Mx_bits.to_string(2) << "\n";
    std::cout << "Mx_zeros = " << Mx_zeros << "\n";
    std::cout << "Mx has " << Mx_bits.countLeadingZeros() << " leading zeros\n";
#endif
    Mx = ap_ufixed<W_Mx, 0,AP_RND>(Mx_bits << Mx_zeros);
    Ex -= Mx_zeros;

    dout = ap_ufixed<W_dout, 0, AP_RND>(Mx << Ex); // A negative shift!


#ifdef DEBUG
    std::cout << "k = " << k << "\n";
    std::cout << "dout = " << dout << " " << dout.to_string(2) << "\n";
    std::cout << "Mx = " << Mx << " " << Mx.to_string(2) << "\n";
    std::cout << "Ex = " << Ex << "\n";
    std::cout << "Mx has " << Mx.countLeadingZeros() << " leading zeros\n";
#endif
}

/**
 * Algorithm for trigonometric range reduction, based on the method
 * published by Payne and Hanek.  Basically, this maps an arbitrary
 * range input onto the unit circle.  The output is expressed as the
 * quadrant k and fixed point number in [0,1).
 *
 * The Quadrants k for positive numbers:
 *  2 1
 * 3   0
 * 4   7
 *  5 6
 * The Quadrants k for negative numbers:
 *  5 6
 * 4   7
 * 3   0
 *  2 1
 * NOTE: p1 = 10 for double, p1 = 16 for single
 *
 * @param p1 Padding bits
 * @param T data type (float, double)
 * @param din Unreduced data input (float, double)
 * @param k quadrant of the circle that we are in.
 * @param dout Reduced data output
 */
template<int p1, class T, int W_dout, int W_Mx>
void
range_redux_payne_hanek_hotbm_pi(
    T din,
    ap_uint<3> &k,
    ap_ufixed<W_dout, 0> &dout,
    ap_ufixed<W_Mx, 0> &Mx,
    ap_int<fp_struct<T>::EXP_BITS> &Ex,
    int &exactly_pi_over_2)
{
    const int SB = fp_struct<T>::SIG_BITS;

    fp_struct<T> fps1(din);
    int expv = fps1.expv();
#ifdef DEBUG
    std::cout << "\nrange_redux_payne_hanek_hotbm: expv: " << std::dec << expv
              << ", p1: " << p1 << std::endl;
#endif

    ap_ufixed<3*(SB+1)+p1+3,SB+1+3>        h_fixed;        // X*Med

    bool closepath = ((fps1.exp) < fp_struct<T>::EXP_BIAS-3);

    if(closepath) {
        Ex = expv+4;
        expv = -4;
    } else {
        Ex = 0;
    }

    {
        ap_uint<SB+1>                 X;        // mantissa (din) in integer format
        ap_uint<2*(SB+1)+p1+3>        Med;      // significant bits of 4oPi
        ap_uint<3*(SB+1)+p1+3>        h;        // X*Med
        // Construct the mantissa, Q1.SIG_BITS
        X(fp_struct<T>::SIG_BITS-1,0) = fps1.sig(fp_struct<T>::SIG_BITS-1,0);
        X[fp_struct<T>::SIG_BITS] = 1; // Don't handle subnormals (fps1.exp == 0) ? 0 : 1; // handle subnormals

        h = X;
        h <<= SB+p1+2; //align
        h <<= 2; // Multiply by 4
        h <<= expv;

        //reinterpret bits of h.
        h_fixed(h_fixed.wl()-1, 0) = h(h_fixed.wl()-1, 0);
#ifdef DEBUG
        std::cout << "Med = " << Med.to_string(2) << "\n";
        std::cout << "X = " << X.to_string(2) << "\n";
        std::cout << "h = " << h.to_string(2) << " " << h << "\n";
        std::cout << "h_fixed = " << h_fixed.to_string(2) << " " << h_fixed << "\n";
        std::cout << "closepath = " << closepath << "\n";
#endif

    }

    ap_ufixed<W_Mx+18, 1> Mx_bits_extended = h_fixed; // Mx_bits is not normalized !
    exactly_pi_over_2 = (Mx_bits_extended == 1);

    ap_ufixed<W_Mx+18, 0> Mx_bits = h_fixed; // Mx_bits is not normalized !

    if(closepath) {
        k = 0;
    } else {
        k = h_fixed; // Extract the quadrant from the integer bits of h_fixed.
    }

    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        Mx_bits = 1-Mx_bits;
    }

    ap_uint<5> Mx_zeros = clz<18>(Mx_bits);

#ifdef DEBUG
    std::cout << "Mx_bits = " << Mx_bits << " " << Mx_bits.to_string(2) << "\n";
    std::cout << "Mx_zeros = " << Mx_zeros << "\n";
    std::cout << "Mx has " << Mx_bits.countLeadingZeros() << " leading zeros\n";
#endif
    Mx = ap_ufixed<W_Mx, 0,AP_RND>(Mx_bits << Mx_zeros);
    Ex -= Mx_zeros;

    dout = ap_ufixed<W_dout, 0, AP_RND>(Mx << Ex); // A negative shift!


#ifdef DEBUG
    std::cout << "k = " << k << "\n";
    std::cout << "dout = " << dout << " " << dout.to_string(2) << "\n";
    std::cout << "Mx = " << Mx << " " << Mx.to_string(2) << "\n";
    std::cout << "Ex = " << Ex << "\n";
    std::cout << "Mx has " << Mx.countLeadingZeros() << " leading zeros\n";
#endif
}

static
void sincosf(
    float t_in,
    float *s,
    float *c)
{
    const int g = 7;
    const int p1 = 20;//+g;
    // Alpha should come from tables.
    const int alpha = 7;
    const int beta1 = 7; // The number of bits of B that are not used in the K1 computation
    const int beta2 = 15; // The number of bits of B that are not used in the K2 computation

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=cos_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=cos_K2 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_K2 core=ROM_1P_LUTRAM

    fp_struct<float> din(t_in);

#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 0> x;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 0> Mx;
    ap_int<fp_struct<float>::EXP_BITS> Ex;

    range_redux_payne_hanek_hotbm<p1,float>(t_in,k,x,Mx,Ex);

    ap_uint<alpha> A = x(x.wl()-x.iwl()-1,
                         x.wl()-x.iwl()-alpha);
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g-alpha-beta1, -alpha> B = x;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g-alpha-beta2, -alpha> B_trunc = B;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;

#ifdef DEBUG
    std::cout << "flipped_x         =" << x << " " << x.to_string(2) << "\n";
    std::cout << "A = " << A.to_string(2) << "\n";
    std::cout << "B         = " << B.to_string(2) << "\n";
    std::cout << "B_trunc         = " << B_trunc.to_string(2) << "\n";
    std::cout << "B*B         = " << (B*B).to_string(2) << "\n";
    std::cout << "B_squared         = " << B_squared.to_string(2) << "\n";
#endif

    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 1> cos_t1 = cos_K0[A];
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 1> cos_t2 = B*cos_K1[A];
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 1> cos_t3 = B_squared*cos_K2[A];
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 1> cos_result =
        1-(cos_t1+cos_t2+cos_t3);
    float cos_resultf;
    my_to_float(cos_result, cos_resultf, 0);
    fp_struct<float> cos_results(cos_resultf);

    ap_fixed<fp_struct<float>::SIG_BITS+1+g, 1> sin_t1 = sin_K0[A];
    ap_fixed<fp_struct<float>::SIG_BITS+1+g, 1> sin_t2 = B*sin_K1[A];
    ap_fixed<fp_struct<float>::SIG_BITS+1+g, 1> sin_t3 = B_squared*sin_K2[A];
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 1> sin_result =
        (sin_t1 + sin_t2 + sin_t3) * Mx;
    float sin_resultf;
    my_to_float(sin_result, sin_resultf, -Ex);
    fp_struct<float> sin_results(sin_resultf);

#ifdef DEBUG
    std::cout << "cos_t1 =" << cos_t1.to_string(2) << "\n";
    std::cout << "cos_t2 = " << cos_t2.to_string(2) << "\n";
    std::cout << "cos_t3 = " << cos_t3.to_string(2) << "\n";
    std::cout << "cos_result = " << cos_result.to_string(2) << "\n";

    std::cout << "sin_t1 =" << sin_t1.to_string(2) << "\n";
    std::cout << "sin_t2 = " << sin_t2.to_string(2) << "\n";
    std::cout << "sin_t3 = " << sin_t3.to_string(2) << "\n";
    std::cout << "sin_result = " << sin_result.to_string(2) << "\n";
#endif

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    ap_uint<4> index = k;
    index[3] = din.sign;

    if(neg_sin_table[index]) { sin_results.sign = 1; }
    if(neg_cos_table[index]) { cos_results.sign = 1; }

    float s_out, c_out;
    if(din.exp == 0 && din.sig == 0) { // t_in == 0.0
        s_out = copysignf(0.0f, t_in);
        c_out = 1.0f;
    } else if(::hls::__isinf(t_in) || ::hls::__isnan(t_in)) {//din.exp == 0x7FF) { // +inf, -inf, nan
        s_out = ::hls::nan("");
        c_out = ::hls::nan("");
    } else {
        if(swap_table[k]) {
            s_out = cos_results.to_ieee();
            c_out = sin_results.to_ieee();
        } else {
            s_out = sin_results.to_ieee();
            c_out = cos_results.to_ieee();
        }
    }
    *s = s_out;
    *c = c_out;
}

static
float
sinf_or_cosf(float t_in, bool do_cos, bool do_pi)
{
    const int g = 7;
    const int p1 = 20;//+g;
    // Alpha should come from tables.
    const int alpha = 7;
    const int beta1 = 7; // The number of bits of B that are not used in the K1 computation
    const int beta2 = 15; // The number of bits of B that are not used in the K2 computation

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_cos_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_cos_K2 core=ROM_1P_LUTRAM

    fp_struct<float> din(t_in);

#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 0> x;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 0> Mx;
    ap_int<fp_struct<float>::EXP_BITS> Ex;

    int exactly_pi_over_2 = 0;
    if(do_pi) {
        range_redux_payne_hanek_hotbm_pi<p1,float>(t_in,k,x,Mx,Ex,exactly_pi_over_2);
    } else {
        range_redux_payne_hanek_hotbm<p1,float>(t_in,k,x,Mx,Ex);
    }
    ap_uint<alpha+1> A = x(x.wl()-x.iwl()-1,
                           x.wl()-x.iwl()-alpha);
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g-alpha-beta1, -alpha> B = x;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g-alpha-beta2, -alpha> B_trunc = B;
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;

#ifdef DEBUG
    std::cout << "flipped_x         =" << x << " " << x.to_string(2) << "\n";
    std::cout << "A = " << A.to_string(2) << "\n";
    std::cout << "B         = " << B.to_string(2) << "\n";
    std::cout << "B_trunc         = " << B_trunc.to_string(2) << "\n";
    std::cout << "B*B         = " << (B*B).to_string(2) << "\n";
    std::cout << "B_squared         = " << B_squared.to_string(2) << "\n";
#endif

    bool cos_basis = do_cos?!swap_table[k]: swap_table[k];
    bool sin_basis = do_cos? swap_table[k]:!swap_table[k];

    A[alpha] = sin_basis; // cos and sin are concatentated.

    if(cos_basis) {
        // In this case, we are using cos() as the basis, so we don't
        // multiply by Mx*2^Ex.
        Mx = ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 0, AP_RND, AP_SAT>(1);
        Ex = 0;
    }

    ap_fixed<fp_struct<float>::SIG_BITS+1+g, 1> t1 = sin_cos_K0[A];
    ap_fixed<fp_struct<float>::SIG_BITS+1+g, 1> t2 = B*sin_cos_K1[A];
    ap_fixed<fp_struct<float>::SIG_BITS+1+g, 1> t3 = B_squared*sin_cos_K2[A];
    ap_ufixed<fp_struct<float>::SIG_BITS+1+g, 1> result =
        (t1+t2+t3) * Mx;
    float resultf;
    if(exactly_pi_over_2) {
        resultf = 0.70710678118;
    } else {
        my_to_float(result, resultf, -Ex);
    }
    fp_struct<float> results(resultf);


#ifdef DEBUG
    std::cout << "t1 =" << t1.to_string(2) << "\n";
    std::cout << "t2 = " << t2.to_string(2) << "\n";
    std::cout << "t3 = " << t3.to_string(2) << "\n";
    std::cout << "result = " << result.to_string(2) << "\n";
    dumpSingle(resultf, results);
#endif

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    ap_uint<4> index = k;
    index[3] = din.sign;
    if(cos_basis ? neg_cos_table[index] : neg_sin_table[index]) {
        results.sign = 1;
    }

    if(din.exp == 0 && (do_pi ? 1 : din.sig == 0)) { // t_in == 0.0
        // Note that handling subnormals correctly for sinpif/cospif is harder
        // than sinf/cosf.  So for cospif and sinpif, we do flush-to-zero
        // for all subnormals.
        return do_cos ? 1.0f : copysignf(0.0f, t_in);
    } else if(::hls::__isinf(t_in) || ::hls::__isnan(t_in)) {//din.exp == 0x7FF) { // +inf, -inf, nan
        return ::hls::nan("");
    } else {
        return results.to_ieee();
    }
}


template<int WO, int WI>
ap_fixed<WO,2>
    sinf_or_cosf(ap_fixed<32, WI> t_in, bool do_cos, bool do_pi)
{
    const int g = 7;
    //  const int p1 = 20;//+g;
    // Alpha should come from tables.
    const int alpha = 7;
    const int beta1 = 7; // The number of bits of B that are not used in the K1 computation
    const int beta2 = 15; // The number of bits of B that are not used in the K2 computation

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_cos_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=sin_cos_K2 core=ROM_1P_LUTRAM


#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in.to_string(2) << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<WO+1+g, 0> x;
    ap_ufixed<WO+1+g, 0> Mx;

    int exactly_pi_over_2 = 0;
    bool is_negative = t_in[32-1];
    ap_ufixed<32, WI> din = is_negative ? ap_fixed<32, WI>(-t_in) : t_in;
    if(do_pi) {
        k = din*4;
        x = din*4;
        ap_ufixed<32, 1> Mx_bits_extended = din*4;
#ifdef DEBUG
        std::cout << "sin_cos: Mx_bits_extended = " << Mx_bits_extended.to_string(2) << "\n";
#endif
        exactly_pi_over_2 = (Mx_bits_extended == 1);
    } else {
        k = din * ap_ufixed<32,2>(4/M_PI);
        x = din * ap_ufixed<32,2>(4/M_PI);
    }
    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        x = 1-x;
    }

    ap_uint<alpha+1> A = x(x.wl()-x.iwl()-1,
                           x.wl()-x.iwl()-alpha);
    ap_ufixed<WO+1+g-alpha-beta1, -alpha> B = x;
    ap_ufixed<WO+1+g-alpha-beta2, -alpha> B_trunc = B;
    ap_ufixed<WO+1+g-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;

#ifdef DEBUG
    std::cout << "flipped_x         =" << x << " " << x.to_string(2) << "\n";
    std::cout << "A = " << A.to_string(2) << "\n";
    std::cout << "B         = " << B.to_string(2) << "\n";
    std::cout << "B_trunc         = " << B_trunc.to_string(2) << "\n";
    std::cout << "B*B         = " << (B*B).to_string(2) << "\n";
    std::cout << "B_squared         = " << B_squared.to_string(2) << "\n";
#endif

    bool cos_basis = do_cos?!swap_table[k]: swap_table[k];
    bool sin_basis = do_cos? swap_table[k]:!swap_table[k];

    A[alpha] = sin_basis; // cos and sin are concatentated.

    if(cos_basis) {
        // In this case, we are using cos() as the basis, so we don't
        // multiply by Mx*2^Ex.
        Mx = ap_ufixed<WO+1+g, 0, AP_RND, AP_SAT>(1);
    } else {
        Mx = x;
    }

    ap_ufixed<WO+1+g, 1> t1 = sin_cos_K0[A];
    ap_fixed<WO+1+g, 1> t2 = B*sin_cos_K1[A];
    ap_fixed<WO+1+g, 1> t3 = B_squared*sin_cos_K2[A];
    ap_fixed<WO+1+g, 1> result =
        (t1+t2+t3) * Mx;
    if(exactly_pi_over_2) {
        result = 0.70710678118;
    }

#ifdef DEBUG
    std::cout << "t1 =" << t1.to_string(2) << "\n";
    std::cout << "t2 = " << t2.to_string(2) << "\n";
    std::cout << "t3 = " << t3.to_string(2) << "\n";
    std::cout << "result = " << result.to_string(2) << "\n";
#endif

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    ap_uint<4> index = k;
    index[3] = is_negative;
    if(cos_basis ? neg_cos_table[index] : neg_sin_table[index]) {
        result = -result;
    }
    return result;
}

static
float
cosf(float t_in)
{
    return sinf_or_cosf(t_in, 1, 0);
}

static
float
sinf(float t_in)
{
    return sinf_or_cosf(t_in, 0, 0);
}

static
float
cospif(float t_in)
{
    return sinf_or_cosf(t_in, 1, 1);
}

static
float
sinpif(float t_in)
{
    return sinf_or_cosf(t_in, 0, 1);
}


template<int WI>
ap_fixed<32,2>
    cosf(ap_fixed<32, WI> t_in)
{
    return sinf_or_cosf<32>(t_in, 1, 0);
}
template<int WI>
ap_fixed<32,2>
    sinf(ap_fixed<32, WI> t_in)
{
    return sinf_or_cosf<32>(t_in, 0, 0);
}

template<int WI>
ap_fixed<32,2>
    cospif(ap_fixed<32, WI> t_in)
{
    return sinf_or_cosf<32>(t_in, 1, 1);
}
template<int WI>
ap_fixed<32,2>
    sinpif(ap_fixed<32, WI> t_in)
{
    return sinf_or_cosf<32>(t_in, 0, 1);
}

}



/* @}*/

#endif

