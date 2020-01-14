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
 *     (c) Copyright 2013 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/


/**
 * @file hls_sqrt.h
 */


#define Wg 1
template <int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O>
ap_ufixed<W,I> sqrt(ap_ufixed<W,I, _AP_Q, _AP_O> x) {
    assert(I >= 0 && "Number of integer bits for sqrt() must be greater than zero");
    assert(W >= I && "Number of integer bits for sqrt() must be less than or equal to total width");
    ap_ufixed<W+Wg,I> factor = 0;
    int offset;
    // Since input bits are handled in pairs, the
    // start condition for even and odd integer widths
    // are handled slightly differently.
    if(I%2 == 0) {
        offset = 1;
    } else {
        offset = 0;
    }
    factor[W+Wg-1-offset] = 1;
    ap_ufixed<W+Wg+1,I+1> result = 0;
    ap_ufixed<W+Wg+2,I+2> x2 = x;
    for(int i = W+Wg-offset; i > (I-1)/2; i -= 1 ) {

        ap_ufixed<W+2+Wg,I+2> t = (result << 1) + factor;
        //        std::cout << x2.to_string(2) << " " << t.to_string(2) << " " << result.to_string(2) << " " << factor.to_string(2) << "\n";
        ap_ufixed<W+Wg,I> thisfactor = 0;
        if(x2 >= t) {
            x2 -= t;
            thisfactor = factor;
        }
        result = result + thisfactor;
        factor >>= 1;
        x2 <<= 1;
    }
    //    std::cout << W << " " << I << " " << offset << ((I-1) >> 1) << " " << x2.to_string(2) << " " << result.to_string(2) << " " << factor.to_string(2) << "\n";
    return result >> ((I-1) >> 1);
    //I <= 0 ? (I-2)/2 : (I-1)/2);
}

template <int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O>
ap_fixed<W,I> sqrt(ap_fixed<W,I, _AP_Q, _AP_O> x) {
    ap_ufixed<W-1,I-1> x2 = x;
    if(x < 0) return 0; else return ::hls::sqrt(x2);
}

