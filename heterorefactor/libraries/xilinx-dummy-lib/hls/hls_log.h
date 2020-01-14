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


/**
 * @file hls_log.h
 *
 * Natural log implementation using range reduction + Taylor series
 * see: de Dinechin, "A flexible floating-point logarithm for reconfigurable computers"
 */
namespace log_reduce {

#include "utils/x_hls_utils.h"
#include "hls_log_tables.h"


template <typename LOG_TYPE, int p, int alpha, int size, int sizeout>
void range_reduce(ap_ufixed<size, -p> zN, LOG_TYPE &log, ap_ufixed<sizeout, -(p+alpha-1)> &zN1)
{
    //#pragma HLS inline self off
    const int T1size = 1 << alpha; // The size of the first table
#ifndef DEBUG
#define log_lut log_lut_table<LOG_TYPE, p,alpha,T1size>::array
    //    LOG_TYPE log_lut[T1size] = log_lut_table<LOG_TYPE, p,alpha,T1size>::array;
#ifdef CHECK_TABLES
    volatile LOG_TYPE dummy = Table<log_lut_initializer<p,alpha,LOG_TYPE>,T1size>::dummy;
    std::cout << "Checking log_lut_table<" << p << "," << alpha << "," << T1size << ">\n";
    for(int i = 0; i < T1size; i++) {
        if (log_lut[i] != Table<log_lut_initializer<p,alpha,LOG_TYPE>,T1size>::array[i]) {
            std::cout << p << " " << alpha << "\n";
            std::cout << log_lut[i].to_string(2,false) << "\n" << Table<log_lut_initializer<p,alpha,LOG_TYPE>,T1size>::array[i].to_string(2,false) << "\n";
            assert(false);
        }
    }
#endif
#else
    volatile LOG_TYPE dummy = Table<log_lut_initializer<p,alpha,LOG_TYPE>,T1size>::dummy;
#define log_lut Table<log_lut_initializer<p,alpha,LOG_TYPE>,T1size>::array
    //    LOG_TYPE log_lut[T1size] = Table<log_lut_initializer<p,alpha,LOG_TYPE>,T1size>::array;
#endif

    //#pragma HLS RESOURCE variable=log_lut core=ROM_1P_LUTRAM

    ap_ufixed<alpha,            -p> a = zN; // Extract alpha MSBs from z1.
    ap_ufixed<size-alpha, -p-alpha> b = zN; // Extract the LSBs from z1.
    ap_ufixed<size+p+2*p+1, 1>      zNext = 1+zN; // Extend z1 for shift
    int e_shift = a[a.wl()-1] ? 2*p : 2*p+1; // Stage dependent?
    ap_ufixed<size+p+1, 1-2*p>         eZ = zNext >> e_shift;

    zN1 = ((b + eZ) - a*zN);
    ap_uint<alpha> index = a(alpha-1,0);
    log = log_lut[index];

#ifdef DEBUG
    ap_fixed<size+alpha, 3>          one = 1;
    ap_fixed<size+alpha, 3> y_tilde_inverse = one - a + (one >> e_shift);

    ap_ufixed<size+alpha, 1> yN1 = 1 + ((b + eZ) - a*zN);
    std::cout << "range reduction by " << alpha << "\n";
    std::cout << "  a is " << a.to_string(10, true) << " " << a.to_string(2,true) << "\n";
    std::cout << "  index is " << index << "\n";
    //std::cout << "  b is " << b.to_string(10, true) << " " << b.to_string(2,true) << "\n";
    //std::cout << "  zN is " << zN.to_string(10, true) << " " << zN.to_string(2,true) << "\n";
    std::cout << "  y_tilde_inverse is " << y_tilde_inverse.to_string(10, true) << " " << y_tilde_inverse.to_string(2,true) << "\n";
    std::cout << "  computed is " << ((1+zN)*y_tilde_inverse).to_string(10, true) << " " << ((1+zN)*y_tilde_inverse).to_string(2,true) << "\n";
    //std::cout << "  (b+eZ) is " << (b+eZ).to_string(10, true) << " " << (b+eZ).to_string(2,true) << "\n";
    //std::cout << "  (a*zN) is " << (a*zN).to_string(10, true) << " " << (a*zN).to_string(2,true) << "\n";
    std::cout << "  yN1 is " << yN1.to_string(10, true) << " " << yN1.to_string(2,true) << "\n";
    std::cout << "  zN1 is " << zN1.to_string(10, true) << " " << zN1.to_string(2,true) << "\n";
    std::cout << "  log is " << log << " " << log.to_string(2,true) << "\n";
    //       assert((1+zN1) == ((1+zN)*y_tilde_inverse));

    assert(yN1( yN1.wl()-yN1.iwl()-2, yN1.wl()-yN1.iwl()-(p+alpha-1) ) == 0);  // Check that the second stage zeros out p2 bits.

#endif
#undef log_lut
}

template <typename T>
void my_to_float(T in, float &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<float> out;
    out.sign[0] = in[in.wl()-1];

    T ain = in;

    if(out.__signbit()) {
        ain = -in;
    }

    ap_uint<32> out_bits_1 = 0, out_bits_2 = 0, out_bits_3 = 0;
    out_bits_1(31, 16) = ain(ain.wl()-1,ain.wl()-16); out_bits_1[15] = 1;
    out_bits_2(31, 16) = ain(ain.wl()-16-1,ain.wl()-32); out_bits_2[15] = 1;
    out_bits_3(31, 31-(ain.wl()-32)+1) = ain(ain.wl()-32-1, 0); out_bits_3[15] = 1;

    int c1 = __builtin_clz(out_bits_1.to_uint());
    int c2 = __builtin_clz(out_bits_2.to_uint());
    int c3 = __builtin_clz(out_bits_3.to_uint());
    int shift = c1;

    T in_shift = ain << c1;
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
    out.exp = (in == 0) ? 0 : (fp_struct<float>::EXP_BIAS+in_shift.iwl())-prescale-shift-1;

    result = out.to_ieee();
}

template <typename T>
void my_to_float(T in, double &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<double> out;
    out.sign[0] = in[in.wl()-1];

    T ain = in;

    if(out.__signbit()) {
        ain = -in;
    }

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

    T in_shift = ain << c1;
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
    out.exp = (in == 0) ? 0 : (fp_struct<double>::EXP_BIAS+in_shift.iwl())-prescale-shift-1;

    result = out.to_ieee();
}


const int p0 = 0; // We start with zero zeroed bits
const int alpha0 = 5; // 1+alpha0 = number of bits considered by stage 0.
const int p1 = (alpha0 == 5) ? 4 : (alpha0-2); // The number of bits zeroed in y1.
const int alpha1 = 4;
const int p2 = p1 + alpha1 - 1;
const int alpha2 = 6;
const int p3 = p2 + alpha2 - 1;
const int alpha3 = 6;
const int p4 = p3 + alpha3 - 1;
const int alpha4 = 6;
const int p5 = p4 + alpha4 - 1;
const int alpha5 = 6;
const int p6 = p5 + alpha5 - 1;
const int alpha6 = 6;
const int p7 = p6 + alpha6 - 1;
const int alpha7 = 6;
const int p8 = p7 + alpha7 - 1;
const int alpha8 = 6;
const int p9 = p8 + alpha8 - 1;
const int alpha9 = 6;
const int p10 = p9 + alpha9 - 1;


//typedef float LOG_TYPE;
template<typename T>
class log_traits {};

template <> class log_traits<float>
{
public:
    const static int bypass_threshold = 12; //~Wf/2
    const static int Wf = fp_struct<float>::SIG_BITS;
    const static int We = fp_struct<float>::EXP_BITS;
    const static int guard_bits = 4;
    const static int MaxPrecision = (Wf*3)/2+1+guard_bits;
    const static int SquarerExtraBits = 4;
    typedef ap_fixed<1+We+MaxPrecision,1+We> LOG_TYPE;

    const static int w1 = Wf+1;
    const static int w2 = w1+2*p1+1-(alpha1-1); // MaxWidth of z2, needs to be truncted to MaxPrecision
    const static int w3 = w2+2*p2+1-(alpha2-1);

    template <int size>
    static ap_ufixed<MaxPrecision-p3, -p3> range_reduction(ap_ufixed<size, -p1> z1, LOG_TYPE &log_sum) {
        ap_ufixed<(w2 < MaxPrecision-p2) ? w2  : (MaxPrecision-p2), -p2>        z2;
        ap_ufixed<(w3 < MaxPrecision-p3) ? w3  : (MaxPrecision-p3), -p3>        z3;
        LOG_TYPE logn;
        range_reduce<LOG_TYPE, p1, alpha1> (z1, logn, z2);
        log_sum += logn;
        range_reduce<LOG_TYPE, p2, alpha2> (z2, logn, z3);
        log_sum += logn;
        return z3;
    }
};

template <> class log_traits<double>
{
public:
    const static int bypass_threshold = p6; //~Wf/2
    const static int Wf = fp_struct<double>::SIG_BITS;
    const static int We = fp_struct<double>::EXP_BITS;
    const static int guard_bits = 5 ;
    const static int MaxPrecision = (Wf*3)/2+1+guard_bits;
    const static int SquarerExtraBits = 8 ;
    typedef ap_fixed<1+We+MaxPrecision,1+We> LOG_TYPE;

    const static int w1 = Wf+1;
    const static int w2 = w1+2*p1+1-(alpha1-1); // MaxWidth of z2, needs to be truncted to MaxPrecision
    const static int w3 = w2+2*p2+1-(alpha2-1);
    const static int w4 = w3+2*p3+1-(alpha3-1);
    const static int w5 = w4+2*p4+1-(alpha4-1);
    const static int w6 = w5+2*p5+1-(alpha5-1);

    template <int size>
    static ap_ufixed<MaxPrecision-p6, -p6> range_reduction(ap_ufixed<size, -p1> z1, LOG_TYPE &log_sum) {
        ap_ufixed<(w2 < MaxPrecision-p2) ? w2  : (MaxPrecision-p2), -p2>        z2;
        ap_ufixed<(w3 < MaxPrecision-p3) ? w3  : (MaxPrecision-p3), -p3>        z3;
        ap_ufixed<(w4 < MaxPrecision-p4) ? w4  : (MaxPrecision-p4), -p4>        z4;
        ap_ufixed<(w5 < MaxPrecision-p5) ? w5  : (MaxPrecision-p5), -p5>        z5;
        ap_ufixed<(w6 < MaxPrecision-p6) ? w6  : (MaxPrecision-p6), -p6>        z6;
        LOG_TYPE logn;
        range_reduce<LOG_TYPE, p1, alpha1> (z1, logn, z2);
        log_sum += logn;
        range_reduce<LOG_TYPE, p2, alpha2> (z2, logn, z3);
        log_sum += logn;
        range_reduce<LOG_TYPE, p3, alpha3> (z3, logn, z4);
        log_sum += logn;
        range_reduce<LOG_TYPE, p4, alpha4> (z4, logn, z5);
        log_sum += logn;
        range_reduce<LOG_TYPE, p5, alpha5> (z5, logn, z6);
        log_sum += logn;
        return z6;
    }
};

template<typename T>
T log_generic(T z)
{
    typedef typename log_traits<T>::LOG_TYPE LOG_TYPE;

    const int T0size = 1 << (1+alpha0); // The size of the first table

#define inverse_lut log_inverse_lut_table<p0,alpha0, T0size>::array
    //    ap_ufixed<1+alpha0,1> inverse_lut[T0size] = log_inverse_lut_table<p0,alpha0, T0size>::array;
#ifdef CHECK_TABLES
    std::cout << "Checking log_inverse_lut_table<" << p0 << "," << alpha0 << "," << T0size << ">\n";
    volatile ap_ufixed<1+alpha0,1> d1 = Table<log0_inverse_lut_initializer<p0,alpha0, ap_ufixed<1+alpha0,1> >,T0size>::dummy;
    for(int i = 0; i < T0size; i++) {
        if(inverse_lut[i] != Table<log0_inverse_lut_initializer<p0,alpha0, ap_ufixed<1+alpha0,1> >,T0size>::array[i]) {
            std::cout << inverse_lut[i].to_string(2,false) << "\n" << Table<log0_inverse_lut_initializer<p0,alpha0,ap_ufixed<1+alpha0,1> >,T0size>::array[i].to_string(2,false) << "\n";
            assert( false );
        }
    }
#endif
#ifndef DEBUG
#define log_lut log0_lut_table<LOG_TYPE,T,p0,alpha0,T0size>::array
    //    LOG_TYPE log_lut[T0size] = log0_lut_table<LOG_TYPE,T,p0,alpha0,T0size>::array;
#ifdef CHECK_TABLES
    std::cout << "Checking log0_lut_table<" << p0 << "," << alpha0 << "," << T0size << ">\n";
    volatile LOG_TYPE dummy = Table<log0_lut_initializer<p0,alpha0,LOG_TYPE, T>,T0size>::dummy;
    for(int i = 0; i < T0size; i++) {
        if (log_lut[i] != Table<log0_lut_initializer<p0,alpha0,LOG_TYPE, T>,T0size>::array[i]) {
            std::cout << p0 << " " << alpha0 << "\n";
            std::cout << log_lut[i].to_string(2,false) << "\n" << Table<log0_lut_initializer<p0,alpha0,LOG_TYPE, T>,T0size>::array[i].to_string(2,false) << "\n";
            assert( false );
        }
    }
#endif
#else
    volatile LOG_TYPE dummy = Table<log0_lut_initializer<p0,alpha0,LOG_TYPE, T>,T0size>::dummy;
#define log_lut Table<log0_lut_initializer<p0,alpha0,LOG_TYPE, T>,T0size>::array
    //    LOG_TYPE log_lut[T0size] = Table<log0_lut_initializer<p0,alpha0,LOG_TYPE, T>,T0size>::array;
#endif

    //#pragma HLS RESOURCE variable=inverse_lut core=ROM_1P_LUTRAM
    //#pragma HLS RESOURCE variable=log_inverse_lut_table<p0,alpha0, T0size>::array core=ROM_1P_LUTRAM
    //#pragma HLS RESOURCE variable=log_lut core=ROM_1P_LUTRAM

    //#pragma HLS inline all recursive
#pragma HLS pipeline

    T logz;
    LOG_TYPE sum;
    LOG_TYPE log_sum;

#ifdef DEBUG
    std::cout << " --- Value of z received " << z << "\n";
#endif

    // Check special cases
    fp_struct<T> zs(z);

#ifndef STDSUBNORMALS
    if(zs.exp == 0) { // zero or subnormal
#ifdef DEBUG
        std::cout << "Subnormal detected! " << z << " " << zs.exp << "\n";
#endif
#else
    if(zs.exp == 0 && zs.sig == 0) { // zero
#endif
        return fp_struct<T>::minus_infinity();
    } else if(zs.sign != 0 || ::hls::__isnan(z)) {
        return ::hls::nan("");
    } else if(::hls::__isinf(z)) {
        return fp_struct<T>::infinity();
    }

    int exponent = zs.expv();
    fp_struct<T> ns = zs;

#ifdef STDSUBNORMALS
    if(zs.exp == 0) {
        // subnormal handling.
        ap_uint<fp_struct<T>::SIG_BITS> nsrev = ns.sig;
        nsrev.reverse();
        unsigned int zeros = __builtin_ctzl(nsrev);
#ifdef DEBUG
        std::cout << "is subnormal, sig = " << std::hex << ns.sig << " " << nsrev << std::dec << " zeros = " << zeros << "\n;";
#endif
        exponent -= zeros;
        ns.sig = ns.sig << (zeros + 1); // add one so we shift off the leading one.
    }
#endif

    int FirstBit = ns.sig[log_traits<T>::Wf-1];

    // Y is prenormalized, between 0.75 and 1.5
    ap_ufixed<1+log_traits<T>::Wf+1,1> y = 0;
    y[y.wl()-1] = 1; // The implicit '1' in IEEE format.
    y(y.wl()-1-y.iwl(),1) = ns.sig(log_traits<T>::Wf-1,0);
    if(FirstBit) {
        y = y >> 1;
        exponent += 1;
    }

    // A lookup table for this?
    const ap_ufixed<log_traits<T>::Wf+log_traits<T>::We+2,0>
    LOG2 = 0.69314718055994530941723212145;
    ap_ufixed<log_traits<T>::We+1+log_traits<T>::Wf+log_traits<T>::We+2,log_traits<T>::We+1>
    Elog2 = LOG2 * exponent;

#ifdef DEBUG
    std::cout << "  normalized " << z << " is " << y.to_string(10, true) << " " << y.to_string(2,true) << "\n";
    std::cout << "    approx log(z) from normalization " << " is " << Elog2.to_string(10, true) << "\n";
#endif

    //    int p_next = 0;

    ap_uint<fp_struct<T>::SIG_BITS> t;
    if(FirstBit) {
        t = -ns.sig;
    } else {
        t = ns.sig;
    }

    ap_fixed<1+-log_traits<T>::bypass_threshold+1+log_traits<T>::MaxPrecision, -log_traits<T>::bypass_threshold+1> zk;
    unsigned int prescale;
    int bits = log_traits<T>::bypass_threshold; // For taylor
    if((exponent == 0) && ((t >> (log_traits<T>::Wf-bits)) == 0)) { // Check the leading bits.  We could also write this with __builtin_clz().
        // Close to 1, Skip range reduction
        ap_uint<fp_struct<T>::SIG_BITS> t2 = t << bits;
        t2.reverse();
        prescale = __builtin_ctzl(t2);

        zk = y << prescale; // This also performs some truncation of the high order bits.
        log_sum = 0;
#ifdef DEBUG
        ap_fixed<log_traits<T>::Wf+2+1+alpha0+alpha1+2, 1+2> yk = y;
        std::cout << "Skipping range reduction t = " << t.to_string(2,true) << ". \n";
        std::cout << "  yk is " << yk.to_string(10, true) << " " << yk.to_string(2,true) << "\n";
        std::cout << "  prescale is " << prescale << "\n";
        std::cout << "  zk is " << zk.to_string(10, true) << " " << zk.to_string(2,true) << "\n";
#endif
    } else {
        prescale = 0;
        ap_uint<1+alpha0> index0 = y(y.wl()-1, y.wl()-(1+alpha0));
        ap_ufixed<1+alpha0,1> y_tilde_inverse = inverse_lut[index0];
        log_sum = log_lut[index0];
        // The first bits of y1 are always a 1, followed by p1 zeros, but
        // it gets dropped later
        ap_ufixed<1+log_traits<T>::Wf+1+(1+alpha0),1> y1 = y * y_tilde_inverse;
        // p_next = p1;

#ifdef DEBUG
        std::cout << "range reduction 0\n";
        std::cout << "  index0 is " << index0 << "\n";
        std::cout << "  y_tilde_inverse is " << y_tilde_inverse.to_string(10, true) << " " << y_tilde_inverse.to_string(2,true) << "\n";
        std::cout << "  y1 is " << y1.to_string(10, true) << " " << y1.to_string(2,true) << "\n";
        std::cout << "  log0 is " << log_sum.to_string(10, true) << "\n";

#endif
        assert(y1( y1.wl()-y1.iwl()-1, y1.wl()-y1.iwl()-p1) == 0);  // Check that the first stage zeros out p1 bits.

        const int z1_width = 1+log_traits<T>::Wf+1+(1+alpha0)-p1+1;
        ap_ufixed<z1_width,        -p1>        z1 = y1; // Extract a1+b1 from y1.

        zk = log_traits<T>::range_reduction(z1,log_sum);
    }

    ap_fixed<1+-log_traits<T>::bypass_threshold+1+log_traits<T>::MaxPrecision/2+log_traits<T>::SquarerExtraBits, -log_traits<T>::bypass_threshold+1>   zk_trunc = zk;
    sum = zk-((zk_trunc*zk_trunc/2) >> prescale);
    //sum = zk*2/(zk+2);


#ifdef DEBUG
    std::cout << "  zk = " << zk.to_string(10, true) << " " << zk.to_string(2,true) << "\n";
    std::cout << "  zk_trunc = " << zk_trunc.to_string(10, true) << " " << zk_trunc.to_string(2,true) << "\n";
    std::cout << "  taylor = " << sum.to_string(10, true) << " " << sum.to_string(2,true) << "\n";
    std::cout << "  log_sum = " << log_sum.to_string(10, true) << " " << log_sum.to_string(2,true) << "\n";
    std::cout << "  log(normalized) is " << -(log_sum+sum)  << "\n";
#endif

    LOG_TYPE log_out = Elog2 + log_sum + sum;

#ifdef DEBUG
    std::cout << "Elog2 = " << Elog2 << "\n";
    std::cout << " --- Natural log of z is " << log_out.to_string(10, true) << " " << log_out.to_string(2,true) << "\n";
#endif

    my_to_float(log_out, logz, prescale);
#ifdef DEBUG
    std::cout << " --- Natural log of z (my_to_float) is " << logz << " " << std::hex << fp_struct<T>(logz).data() << std::dec << "\n";
#endif
    return logz;
#undef log_lut
#undef inverse_lut
}

static
float logf(float x)
{
    return log_generic(x);
}


static
float log(float x)
{
    return logf(x);
}

static
float log10f(float x)
{
    return logf(x) * 0.434294481903251827651128918916605082294397005f;
}

static
float log10(float x)
{
    return log10f(x);
}

/*
template<int N>
double log(double z)
{

#pragma HLS pipeline
    double logz;
    double first_apprx, curr_apprx, sq_apprx, apprx;
    double sum, z1, z2;
    int i;
    double nume1, denom1;

#ifdef DEBUG
  std::cout << " --- Value of z received " << z << "\n";
#endif

  // Check special cases
  fp_struct<double> zs(z);

  if(zs.sign != 0 || (zs.exp == 0x7FF && zs.sig != 0)) { // isnan(z)
  return ::hls::nan("");
#ifndef STDSUBNORMALS
  } else if(zs.exp == 0) { // zero or subnormal
#ifdef DEBUG
      std::cout << "Subnormal detected! " << z << "\n";
#endif
#else
  } else if(zs.exp == 0 && zs.sig == 0) { // zero
#endif
      return fp_struct<T>::minus_infinity();
  } else if(zs.exp == 0x7FF && zs.sig == 0) { // isinf(z)
      return fp_struct<T>::infinity();
  }

  int exponent = zs.expv();

  fp_struct<double> ns = zs;
  if(zs.exp == 1022) {
      z2 = 0; // For exponent == -1, don't do normalization, since it results in catastrophic cancellation where the result is close to zero.
  } else {
#ifdef STDSUBNORMALS
      if(zs.exp == 0) {
          // subnormal handling.
          ap_uint<52> nsrev = ns.sig;
          nsrev.reverse();
          unsigned int zeros = __builtin_ctzl(nsrev);
#ifdef DEBUG
          std::cout << "is subnormal, sig = " << std::hex << ns.sig << " " << nsrev << std::dec << " zeros = " << zeros << "\n;";
#endif
          exponent -= zeros;
          ns.sig = ns.sig << (zeros + 1); // add one so we shift off the leading one.
      }
#endif

      // A lookup table for this?
      z2 = 0.69314718055994530941723212145 * exponent;
      ns.exp = 1023; // set the exponent to biased zero.
  }
  z1 = ns.to_ieee();
#ifdef DEBUG
  std::cout << "normalized " << z << " is " << z1 << " " << std::hex << reinterpret_cast<uint64_t&>(z1) << std::dec << "\n";
#endif

  denom1 = z1+1.0;  // z1 + 1
  nume1 = z1-1.0;   // z1 - 1

  first_apprx = nume1/denom1;
  curr_apprx = first_apprx;
  sum = curr_apprx;
  sq_apprx = first_apprx * first_apprx;

  double scale[32];
  scale[0] = 1.0;
  scale[1] = 1.0;
  scale[2] = 1.0/3.0;
  scale[3] = 1.0/5.0;
  scale[4] = 1.0/7.0;
  scale[5] = 1.0/9.0;
  scale[6] = 1.0/11.0;
  scale[7] = 1.0/13.0;
  scale[8] = 1.0/15.0;
  scale[9] = 1.0/17.0;
  scale[10] = 1.0/19.0;
  scale[11] = 1.0/21.0;
  scale[12] = 1.0/23.0;
  scale[13] = 1.0/25.0;
  scale[14] = 1.0/27.0;
  scale[15] = 1.0/29.0;
  scale[16] = 1.0/31.0;
  scale[17] = 1.0/33.0;
  scale[18] = 1.0/35.0;
  scale[19] = 1.0/37.0;
  scale[20] = 1.0/39.0;
  scale[21] = 1.0/41.0;
  scale[22] = 1.0/43.0;
  scale[23] = 1.0/45.0;
  scale[24] = 1.0/47.0;
  scale[25] = 1.0/49.0;
  scale[26] = 1.0/51.0;
  scale[27] = 1.0/53.0;
  scale[28] = 1.0/55.0;
  scale[29] = 1.0/57.0;
  scale[30] = 1.0/59.0;
  scale[31] = 1.0/61.0;

  // Should probably be fixed point instead of floating point.
  for(i=2;i<N;i++) {
    curr_apprx = sq_apprx * curr_apprx;
    apprx = curr_apprx * scale[i];
    sum = sum + apprx;
  }
  sum = sum * 2.0;
  logz = sum + z2;
#ifdef DEBUG
  std::cout << "sum = " << sum << " " << std::hex << reinterpret_cast<uint64_t&>(sum) << std::dec << "\n";
  std::cout << "z2 = " << z2 << " " << std::hex << reinterpret_cast<uint64_t&>(z2) << std::dec << "\n";
  std::cout << " --- Natural log of z is " << logz << "\n";
#endif
  return logz;
}
*/

static
double log(double x)
{
    return log_generic(x);
}

static
double log10(double x)
{
    return log(x) * 0.434294481903251827651128918916605082294397005;
}

}
