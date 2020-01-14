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
 * @file hls_round.h
 * This file contains rounding related functions such as round, trunc, floor, ceil.
 */


static
void init_mask_table_double(uint64_t mask_table[64])
{
    // Note that this is a little wierd since we are shifted by BIAS, so
    // mask_table[i] = mask(exp-BIAS+1);
    const uint64_t t = (1LL << fp_struct<double>::SIG_BITS)-1;
    for(int i = 0; i < 62; i++) {
        mask_table[i] = t >> (i+1);
    }
    mask_table[62] = t; // Special case for round()
    mask_table[63] = t; // mask(fp_struct<double>::EXP_BIAS), stuffed into index N-1.
}

static
void init_mask_table_float(uint32_t mask_table[32])
{
    // Note that this is a little wierd since we are shifted by BIAS, so
    // mask_table[i] = mask(exp-BIAS+1);
    const uint32_t t = (1 << fp_struct<float>::SIG_BITS)-1;
    for(int i = 0; i < 30; i++) {
        mask_table[i] = t >> (i+1);
    }
    mask_table[30] = t; // Special case for round()
    mask_table[31] = t; // mask(fp_struct<float>::EXP_BIAS), stuffed into index 31.
}

static
void init_one_half_table_double(uint64_t mask_table[64])
{
    // Note that this is a little wierd since we are shifted by BIAS, so
    // mask_table[i] = mask(exp-BIAS+1);
    const uint64_t t = 1LL << (fp_struct<double>::SIG_BITS-1);
    for(int i = 0; i < 62; i++) {
        mask_table[i] = t >> (i+1);
    }
    mask_table[62] = t << 1; // Special case for round()
    mask_table[63] = t; // one_half(fp_struct<double>::EXP_BIAS), stuffed into index N-1.
}

static
void init_one_half_table_float(uint32_t mask_table[32])
{
    // Note that this is a little wierd since we are shifted by BIAS, so
    // mask_table[i] = mask(exp-BIAS+1);
    const uint32_t t = 1 << (fp_struct<float>::SIG_BITS-1);
    for(int i = 0; i < 30; i++) {
        mask_table[i] = t >> (i+1);
    }
    mask_table[30] = t << 1; // Special case for round()
    mask_table[31] = t; // one_half(fp_struct<float>::EXP_BIAS), stuffed into index 31.
}



static
double floor(double x)
{
    uint64_t mask_table[64];
    init_mask_table_double(mask_table);
    fp_struct<double> xs(x);
    if(xs.exp < fp_struct<double>::EXP_BIAS) {
        if(xs.__signbit() && !(xs.sig == 0 && xs.exp == 0)) { // negative and not zero
            return -1.0;
        } else {
            return ::hls::copysign(0.0, x);
        }
    } else if((xs.exp > fp_struct<double>::EXP_BIAS+fp_struct<double>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint64_t mask;
       // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<6> index = xs.exp(5,0);
        mask = mask_table[index];

        // Deal with negative numbers
        if(xs.__signbit()) {
            xs = ap_uint<64>(xs.data() + mask);
        }

        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

static
float floorf(float x)
{
    uint32_t mask_table[32];
    init_mask_table_float(mask_table);
    fp_struct<float> xs(x);
    if(xs.exp < fp_struct<float>::EXP_BIAS) { // fp_struct<float>::EXP_BIAS is biased zero
        if(xs.__signbit() && !(xs.sig == 0 && xs.exp == 0)) { // negative and not zero
            return -1.0f;
        } else {
            return ::hls::copysignf(0.0f, x);
        }
    } else if((xs.exp > fp_struct<float>::EXP_BIAS+fp_struct<float>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint32_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<5> index = xs.exp(4,0);
        mask = mask_table[index];

        // Deal with negative numbers
        if(xs.__signbit()) {
            xs = ap_uint<32>(xs.data() + mask);
        }

        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

#ifdef __cplusplus
static
float floor(float x)
{
    return floorf(x);
}
#endif

static
double ceil(double x)
{
    uint64_t mask_table[64];
    init_mask_table_double(mask_table);
    fp_struct<double> xs(x);

    if(xs.exp < fp_struct<double>::EXP_BIAS) {
        if(!xs.__signbit() && !(xs.sig == 0 && xs.exp == 0)) { // positive and not zero
            return 1.0;
        } else {
            return ::hls::copysign(0.0, x);
        }
    } else if((xs.exp > fp_struct<double>::EXP_BIAS+fp_struct<double>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint64_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<6> index = xs.exp(5,0);
        mask = mask_table[index];

        // Deal with positive numbers
        if(!xs.__signbit()) {
            xs = ap_uint<64>(xs.data() + mask);
        }

        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

static
float ceilf(float x)
{
    uint32_t mask_table[32];
    init_mask_table_float(mask_table);
    fp_struct<float> xs(x);

    if(xs.exp < fp_struct<float>::EXP_BIAS) {
        if(!xs.__signbit() && !(xs.sig == 0 && xs.exp == 0)) { // positive and not zero
            return 1.0f;
        } else {
            return ::hls::copysignf(0.0f, x);
        }
    } else if((xs.exp > fp_struct<float>::EXP_BIAS+fp_struct<float>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint32_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<5> index = xs.exp(4,0);
        mask = mask_table[index];

        // Deal with positive numbers
        if(!xs.__signbit()) {
            xs = ap_uint<32>(xs.data() + mask);
        }

        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

#ifdef __cplusplus
static
float ceil(float x)
{
    return ceilf(x);
}
#endif

static
double trunc(double x)
{
    uint64_t mask_table[64];
    init_mask_table_double(mask_table);
    fp_struct<double> xs(x);

    if(xs.exp < fp_struct<double>::EXP_BIAS) {
        return ::hls::copysign(0.0, x);
    } else if((xs.exp > fp_struct<double>::EXP_BIAS+fp_struct<double>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint64_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<6> index = xs.exp(5,0);
        mask = mask_table[index];
        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

static
float truncf(float x)
{
    uint32_t mask_table[32];
    init_mask_table_float(mask_table);
    fp_struct<float> xs(x);
    if(xs.exp < fp_struct<float>::EXP_BIAS) { // fp_struct<float>::EXP_BIAS is biased zero
        return ::hls::copysignf(0.0f, x);
    } else if((xs.exp > fp_struct<float>::EXP_BIAS+fp_struct<float>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint32_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<5> index = xs.exp(4,0);
        mask = mask_table[index];
        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

#ifdef __cplusplus
static
float trunc(float x)
{
    return truncf(x);
}
#endif

static
double round(double x)
{
    uint64_t mask_table[64];
    init_mask_table_double(mask_table);
    uint64_t one_half_table[64];
    init_one_half_table_double(one_half_table);

    fp_struct<double> xs(x);
    if(xs.exp < fp_struct<double>::EXP_BIAS - 1) {
        // Then we have a number less than 0.5
        return ::hls::copysign(0.0, x);
    } else if((xs.exp > fp_struct<double>::EXP_BIAS+fp_struct<double>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint64_t mask;
        uint64_t one_half;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<6> index = xs.exp(5,0);
        mask = mask_table[index];
        one_half = one_half_table[index];

        xs = ap_uint<64>(xs.data() + one_half);

        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

// For round, we deal with three cases, the first is for small numbers <0.5, which get rounded to 0.0.
// The second case if for large numbers where nothing happens.  The third deals with the 'normal' case
// where we add one half and then drop the fractional bits.  There are two things to observe:
// 1) When we add one half, we sometimes roll over and increment the exponent, by one.  The exponent
// integer bits of the resulting number is correct, but the fractional bits aren't.  This is OK, since
// we drop the fractional bits.
// 2) The 'missing case' of [0.5, 1.0) is handled in the third case, where adding one
// to the exponent (making it zero) and then dropping all of the fractional bits, resulting in 1.0f.
// As a result there is an extra 'dummy' entry in the tables for round, that is not used for the other
// operators.
static
float roundf(float x)
{
    uint32_t mask_table[32];
    init_mask_table_float(mask_table);
    uint32_t one_half_table[32];
    init_one_half_table_float(one_half_table);

    fp_struct<float> xs(x);

    if(xs.exp < fp_struct<float>::EXP_BIAS - 1) {
        return ::hls::copysignf(0.0f, x);
    } else if((xs.exp > fp_struct<float>::EXP_BIAS+fp_struct<float>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        return x;
    } else {
        uint32_t mask;
        uint32_t one_half;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<5> index = xs.exp(4,0);
        mask = mask_table[index];
        one_half = one_half_table[index];

        xs = ap_uint<32>(xs.data() + one_half);

        xs.sig = xs.sig & (~mask);
        return xs.to_ieee();
    }
}

#ifdef __cplusplus
static
float round(float x)
{
    return roundf(x);
}
#endif

// conversion from float or double to integer type with truncation towards zero.  Should be equivalent to int(x).
template <typename T, typename FP>
T cast_IEEE754(FP x, bool detect_overflow = false, typename enable_if<std::numeric_limits<T>::is_signed, bool>::type dummy = true) {
    fp_struct<FP> xs(x);
    ap_ufixed<2*fp_struct<FP>::SIG_BITS + 8 * sizeof(T),
                fp_struct<FP>::SIG_BITS + 8 * sizeof(T)> mantissa = xs.mantissa();

    ap_ufixed<  fp_struct<FP>::SIG_BITS + 8 * sizeof(T),
                fp_struct<FP>::SIG_BITS + 8 * sizeof(T)> smantissa = mantissa << xs.expv();

    ap_ufixed<1            ,1 + 8 * sizeof(T), AP_TRN,AP_SAT> overflow_bits = smantissa;
    ap_ufixed<8 * sizeof(T),    8 * sizeof(T)> val = smantissa;

    bool overflow = xs.expv() > (signed)(2*fp_struct<FP>::SIG_BITS) || overflow_bits != 0;
    ap_int<8*sizeof(T)> minval = 0;
    minval[8*sizeof(T)-1] = 1;
    if(overflow && detect_overflow) return minval;

    // std::cout << std::hex << "val = " << val.to_string(16) << std::dec << "\n";
    ap_fixed<8 * sizeof(T),8 * sizeof(T)> result = val;
    if(xs.__signbit()) result = -val;
    // std::cout << std::hex << "result = " << result.to_string(16) << std::dec << "\n";
    return result;
}

template <typename T, typename FP>
T cast_IEEE754(FP x, bool detect_overflow = false, typename enable_if<!std::numeric_limits<T>::is_signed, bool>::type dummy = true) {
    fp_struct<FP> xs(x);
    ap_ufixed<2*fp_struct<FP>::SIG_BITS + 8 * sizeof(T),
                fp_struct<FP>::SIG_BITS + 8 * sizeof(T)> mantissa = xs.mantissa();

    ap_ufixed<  fp_struct<FP>::SIG_BITS + 8 * sizeof(T),
                fp_struct<FP>::SIG_BITS + 8 * sizeof(T)> smantissa = mantissa << xs.expv();

    ap_ufixed<1            ,1 + 8 * sizeof(T), AP_TRN,AP_SAT> overflow_bits = smantissa;
    ap_ufixed<8 * sizeof(T),    8 * sizeof(T)> val = smantissa;

    bool overflow = xs.expv() > (signed)(2*fp_struct<FP>::SIG_BITS) || overflow_bits != 0;
    ap_uint<8*sizeof(T)> minval = 0;
    if((overflow || xs.__signbit()) && detect_overflow) return minval;

    // std::cout << std::hex << "val = " << val.to_string(16) << std::dec << "\n";
    ap_ufixed<8 * sizeof(T),8 * sizeof(T)> result = val;
    // std::cout << std::hex << "result = " << result.to_string(16) << std::dec << "\n";
    return result;
}


static
long int lrint(double x) {
    // Note: long int may be 32 or 64 bits.
    return cast_IEEE754<long int>(x);
}

static
long long int llrint(double x) {
    return cast_IEEE754<long long int>(x);
}

static
long int lrintf(float x) {
    // Note: long int may be 32 or 64 bits.
    return cast_IEEE754<long int>(x);
}

static
long long int llrintf(float x) {
    return cast_IEEE754<long long int>(x);
}

static
double modf(double x, double *intpart)
{
    uint64_t mask_table[64];
    init_mask_table_double(mask_table);
    fp_struct<double> xs(x);

    if(xs.exp < fp_struct<double>::EXP_BIAS) {
        *intpart = ::hls::copysign(0.0, x);
        return x;
    } else if((xs.exp > fp_struct<double>::EXP_BIAS+fp_struct<double>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        *intpart = x;
        if(::hls::__isnan(x)) {
            fp_struct<double> nan;
            nan.sig = -1; // all ones
            nan.exp = -1; // all ones
            nan.sign = 0;
            return nan.to_ieee();
        } else {
            return ::hls::copysign(0.0, x);
        }
    } else {
        uint64_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<6> index = xs.exp(5,0);
        mask = mask_table[index];

        // normalize the fractional part
        fp_struct<double> xf;
        xf.sig = xs.sig & mask;

        int zeros = xf.sig.countLeadingZeros();;
        // If the significand is exactly zero, then we need to return a floating point zero
        // Otherwise compute the new exponent from the old one.
        xf.exp = (xf.sig == 0) ? 0 : (unsigned short)(xs.exp-zeros-1);
        // Normalize the significand, shifting off the zeros and the leading 1.
        xf.sig = (xf.sig << 1) << zeros;
        xf.sign = xs.sign;

        // mask the integer part
        xs.sig = xs.sig & (~mask);
        *intpart = xs.to_ieee();

        return xf.to_ieee();
    }
}

static
float modff(float x, float *intpart)
{
    uint32_t mask_table[32];
    init_mask_table_float(mask_table);
    fp_struct<float> xs(x);
    if(xs.exp < fp_struct<float>::EXP_BIAS) { // fp_struct<float>::EXP_BIAS is biased zero
        *intpart = ::hls::copysignf(0.0f, x);
        return x;
    } else if((xs.exp > fp_struct<float>::EXP_BIAS+fp_struct<float>::SIG_BITS)) {
        // In this special case, we know there are no fractional bits.
        *intpart = x;
        if(::hls::__isnan(x)) {
            fp_struct<float> nan;
            nan.sig = -1; // all ones
            nan.exp = -1; // all ones
            nan.sign = 0;
            return nan.to_ieee();
        } else {
            return ::hls::copysignf(0.0f, x);
        }
    } else {
        uint32_t mask;
        // Find the mask.  These are 1's where there are fractional bits.
        ap_uint<5> index = xs.exp(4,0);
        mask = mask_table[index];

        // normalize the fractional part
        fp_struct<float> xf;
        xf.sig = xs.sig & mask;

        int zeros = xf.sig.countLeadingZeros();;
        // If the significand is exactly zero, then we need to return a floating point zero
        // Otherwise compute the new exponent from the old one.
        xf.exp = (xf.sig == 0) ? 0 : (unsigned short)(xs.exp-zeros-1);
        // Normalize the significand, shifting off the zeros and the leading 1.
        xf.sig = (xf.sig << 1) << zeros;
        xf.sign = xs.sign;

        // mask the integer part
        xs.sig = xs.sig & (~mask);
        *intpart = xs.to_ieee();

        return xf.to_ieee();
    }
}

#ifdef __cplusplus
static
float modf(float x, float *intpart)
{
    return modff(x, intpart);
}
#endif
