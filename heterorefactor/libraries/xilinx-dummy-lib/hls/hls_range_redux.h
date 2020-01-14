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
 *     (c) Copyright 2012 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#ifndef X_HLS_RANGE_RED_H
#define X_HLS_RANGE_RED_H

#include "utils/x_hls_float_utils.h"
#include "hls_big_mult.h"
#include <iostream>


/**
 * This function converts ap_uint to float being sure to
 * truncate unnecessary bits off the end and returns the float.
 * Input format is Q1.x
 *
 * @param W The input data width
 */
template<int W>
float
convToFloat(
    ap_uint<W> din)
{
    ap_ufixed<W,1> d;
    d(W-1,0) = din(W-1,0);
    int prescale = 0;
    fp_struct<float> out;
    out.sign[0] = 0; // unsigned

    ap_ufixed<W,1> ain = d;

    ap_uint<32> out_bits_1 = 0, out_bits_2 = 0, out_bits_3 = 0;
    out_bits_1(31, 16) = ain(ain.wl()-1,ain.wl()-16); out_bits_1[15] = 1;
    out_bits_2(31, 16) = ain(ain.wl()-16-1,ain.wl()-32); out_bits_2[15] = 1;
    if(ain.wl() > 48) {
        out_bits_3(31, 16) = ain(ain.wl()-32-1,ain.wl()-48);
    } else {
        out_bits_3(31, 16) = 0xFFFF;
    }
    out_bits_3[15] = 1;

    int c1 = __builtin_clz(out_bits_1.to_uint());
    int c2 = __builtin_clz(out_bits_2.to_uint());
    int c3 = __builtin_clz(out_bits_3.to_uint());
    int shift = c1;

    ap_ufixed<W,1> in_shift = ain << c1;
    if(c1 == 16) {
        shift += c2;
        in_shift = in_shift << c2;
        if(c2 == 16) {
            // FIXME: All these stages may not be necessary
            shift += c3;
            in_shift = in_shift << c3;
        }
    }
    out.sig(fp_struct<float>::SIG_BITS-1,0) =
        in_shift(in_shift.wl()-1-1,
                 in_shift.wl()-fp_struct<float>::SIG_BITS-1);
    out.exp = (din == 0) ? 0 : (fp_struct<float>::EXP_BIAS+in_shift.iwl())-prescale-shift-1;

    return out.to_ieee();
}

/**
 * This function converts ap_ufixed to float being sure to
 * truncate unnecessary bits off the end and returns the float.
 * Input format is Q1.x so ufixed values larger than this will 
 * have the integer bits truncated as well.
 *
 * @param W The input data width
 */
template<int W, int I>
float
convToFloat(
	ap_ufixed<W,I> din)
{
    ap_ufixed<W,1> d;
	d = din;
	ap_uint<W> di;
	di(W-1,0) = d(W-1,0);
	return convToFloat<W>(di);
}


/**
 * This function converts float to ap_uint.
 * Assume W > 53.
 *
 * @param W The input data width
 */
template<int W>
ap_uint<W>
convToInt(
    float din)
{
    fp_struct<float> fps(din);

    ap_uint<W> dout = ((ap_uint<W>)0x1 << 23) | fps.sig;
    dout = dout << (W-24);
    dout = dout >> (-1-fps.expv());
#ifdef DEBUG
    std::cout << "convToInt: din: " << std::scientific << din
              << "(" << std::hex << *(int *)(&din)
              << "), dout: " << dout.to_string(16).c_str()
              << ", expv: " << std::dec << fps.expv() << std::endl;
#endif
    return dout;
}

/**
 * This function converts ap_uint to double being sure to
 * truncate unnecessary bits off the end and returns the double.
 * Input format is Q1.x
 *
 * @param W The input data width
 */
template<int W>
double
convToDouble(
    ap_uint<W> din)
{
    ap_ufixed<W,1> d;
    d(W-1,0) = din(W-1,0);
    int prescale = 0;
    fp_struct<double> out;
    out.sign[0] = 0;

    ap_ufixed<W,1> ain = d;

    ap_uint<32> out_bits_1 = 0, out_bits_2 = 0, out_bits_3 = 0, out_bits_4 = 0;
    out_bits_1(31, 16) = ain(ain.wl()-1,ain.wl()-16); out_bits_1[15] = 1;
    out_bits_2(31, 16) = ain(ain.wl()-16-1,ain.wl()-32); out_bits_2[15] = 1;
    out_bits_3(31, 16) = ain(ain.wl()-32-1,ain.wl()-48); out_bits_3[15] = 1;
    out_bits_4(31, 16) = ain(ain.wl()-48-1,ain.wl()-64); out_bits_4[15] = 1;

    int c1 = __builtin_clz(out_bits_1.to_uint());
    int c2 = __builtin_clz(out_bits_2.to_uint());
    int c3 = __builtin_clz(out_bits_3.to_uint());
    int c4 = __builtin_clz(out_bits_4.to_uint());
    int shift = c1;

    ap_ufixed<W,1> in_shift = ain << c1;
    if(c1 == 16) {
        shift += c2;
        in_shift = in_shift << c2;
        if(c2 == 16) {
            // FIXME: All these stages may not be necessary
            shift += c3;
            in_shift = in_shift << c3;
            if(c3 == 16) {
                shift += c4;
                in_shift = in_shift << c4; // leading one is now in the sign bit.
            }
        }
    }
    out.sig(fp_struct<double>::SIG_BITS-1,0) =
        in_shift(in_shift.wl()-1-1,
                 in_shift.wl()-fp_struct<double>::SIG_BITS-1);
    out.exp = (din == 0) ? 0 : (fp_struct<double>::EXP_BIAS+in_shift.iwl())-prescale-shift-1;

    return out.to_ieee();
}

/**
 * This function converts ap_ufixed to double being sure to
 * truncate unnecessary bits off the end and returns the double.
 * Input format is Q1.x so ufixed values larger than this will 
 * have the integer bits truncated as well.
 *
 * @param W The input data width
 */
template<int W, int I>
double
convToDouble(
	ap_ufixed<W,I> din)
{
    ap_ufixed<W,1> d;
	d = din;
	ap_uint<W> di;
	di(W-1,0) = d(W-1,0);
	return convToDouble<W>(di);
}


/**
 * This function converts double to ap_uint.
 * Assume W > 53.
 *
 * @param W The input data width
 */
template<int W>
ap_uint<W>
convToInt(
    double din)
{
    fp_struct<double> fps(din);

    ap_uint<W> dout = ((ap_uint<W>)0x1 << 52) | fps.sig;
    dout = dout << (W-53);
    dout = dout >> (-1-fps.expv());
#ifdef DEBUG
    std::cout << "convToInt: din: " << std::scientific << din
              << "(" << std::hex << *(long long *)(&din)
              << "), dout: " << dout.to_string(16).c_str()
              << ", expv: " << std::dec << fps.expv() << std::endl;
#endif
    return dout;
}

/**
 * This function performs a table lookup to get the relevant values
 * of 4oPi given an exponent value for floats.
 *
 * @param SB Number of mantissa bits (e.g. 23 for float)
 * @param p1 Padding bits
 * @param exp Exponent value
 * @param dummy dummy value to indicate float type.
 */
template<int SB, int p1>
ap_uint<2*(SB+1)+p1+3>
table_lookup_4oPi(int exp, float dummy)
{
#pragma HLS RESOURCE variable=ref_4oPi_table_100 core=ROM_1P_LUTRAM
    ap_int<9>   addr     = exp+(1+4*16);
    assert(addr >= 0);
    ap_uint<100> table_100 = ref_4oPi_table_100[addr >> 4];
    return (table_100 << (ap_uint<4>)(addr(3,0)))(99,100-((2*(SB+1)+p1+3)));
}

/**
 * This function performs a table lookup to get the relevant values
 * of 4oPi given an exponent value for doubles.
 *
 * @param SB Number of mantissa bits (e.g. 53 for double)
 * @param p1 Padding bits
 * @param exp Exponent value
 * @param dummy dummy value to indicate float type.
 */
template<int SB, int p1>
ap_uint<2*(SB+1)+p1+3>
table_lookup_4oPi(int exp, double dummy)
{
    ap_uint<2*(SB+1)+p1+3> ret;
    ap_int<12>   addr      = exp+76;
    ap_uint<256> table_256 = ref_4oPi_table_256[addr(10,7)];
    ret                    = (table_256 << (ap_uint<7>)(addr(6,0)))(255,256-((2*(SB+1))+p1+3));
    if(addr < 0) {
        ret = 0;
    }
    return ret;
}

/**
 * Payne-hanek algorithm for range reduction (single precision)
 *
 * NOTE: p1 = 10 for double, p1 = 16 for single
 *
 * @param p1 Padding bits
 * @param T data type (float, double)
 * @param din Unreduced data input (float, double)
 * @param k Relevant LSB used to adjust sin/cos identities following reduction
 * @param dout Reduced data output
 */
template<int p1, int CW, class T>
void
range_redux_payne_hanek(
    T din,
    ap_uint<3> &k,
    ap_uint<CW> &dout)
{
    const int EB = fp_struct<T>::EXP_BIAS;
    const int SB = fp_struct<T>::SIG_BITS;

    fp_struct<T> fps1(din);
    int exp = fps1.exp - EB;
#ifdef DEBUG
    std::cout << "\nrange_redux_payne_hanek: exp: " << std::dec << exp
              << ", p1: " << p1 << std::endl;
#endif
    if(exp > -1) {
        ap_uint<CW>                   Pio4_i1;  // Relevant bits from Pio4
        ap_uint<CW+1>                 Pio4_i2;  // Relevant bits from Pio4
        ap_uint<SB+1>                 X;        // mantissa (din) in integer format
        ap_uint<2*(SB+1)+p1+3>        Med;      // significant bits of 4oPi
        ap_uint<3*(SB+1)+p1+3>        h;        // X*Med
        ap_uint<(SB+1)+3>             h_int;    // integer portion of h
        ap_uint<CW>                   h_fract;  // fractional portion of h
        ap_uint<2*CW>                 prod; // h_fract*Pio4
        ap_uint<CW+1>                 prod_adj;

        Pio4_i1    = ref_Pio4_400(400-1-2,400-1-2-CW+1);
        Pio4_i2    = ref_Pio4_400(400-1-2,400-1-2-(CW+1)+1);

        X          = ((ap_uint<SB+1>)0x1 << SB) | (fps1.sig);
        Med         = table_lookup_4oPi<SB,p1>(exp,din);

        big_mult(Med, X, h);

        h_int      = h >> (2*(SB+1)+p1);
        h_fract    = h(2*(SB+1)+p1-1,2*(SB+1)+p1-CW);
        k          = h_int(2,0);

        big_mult(h_fract, Pio4_i1, prod);

		prod_adj   = prod((2*CW)-1,CW-1);
		prod_adj   = (k[0] == 1) ? (ap_uint<CW+1>)(Pio4_i2 - prod_adj) : prod_adj;

		dout       = prod_adj(CW,1);
    } else {
        k = 0;
        dout = convToInt<CW>(din);
    }
}


#endif

