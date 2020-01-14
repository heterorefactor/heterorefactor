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

template< class out, class in>
out reference_log(in x)
{
    double x_MS = x;
    in x_MS2 = x_MS;
    in x_diff = (x-(in)x_MS);
    long double x_ld = (long double)x_MS + (long double)(double)x_diff;
    long double ll = logl(x_ld);
    //    double x_LS = (x-(in)x_MS);
// (long double) x_MS + (long double) x_LS;
//     std::cout << "x = " << x << " " << x.to_string(10,true) << "\n";
//     std::cout << "x_MS = " << x_MS << "\n";
//     std::cout << "x_MS2 = " << x_MS2 << " " << x_MS2.to_string(2, true) << "\n";
//     std::cout << "x_diff = " << x_diff << " " << x_diff.to_string(2, true) << "\n";
//     std::cout << "x_LS = " << x_LS << "\n";
//     std::cout << "x_ld = " << x_ld << "\n";
//     std::cout << "ll  = " << ll << "\n";
    double ll_MS = ll;
    out y = ll_MS;
    double ll_LS = ll - (double)y;
    y += (out)ll_LS;
//     std::cout << "ll_MS = " << ll_MS << "\n";
//     std::cout << "ll_LS = " << ll_LS << "\n";
    return y;
}

template<int p, int alpha, class T>
class log_lut_initializer
{
public:
    typedef T TableType;
    static T f(int i, int n) {
        ap_uint<alpha> t = i;
        ap_ufixed<alpha, -p>   a = 0;
        a(alpha-1,0) = t(alpha-1,0); // set alpha1 LSBs to the bit pattern of the index.

        int e_shift = a[a.wl()-1] ? 2*p : 2*p+1; // Stage dependent?
        ap_fixed<1+2*p+alpha, 2> one = 1;
        ap_fixed<1+2*p+alpha, 2> y_tilde_inverse = one - a + (one >> e_shift);

        return -reference_log<T>(y_tilde_inverse);
    }
    static T apply(int i, int n) {
        ap_uint<alpha> t = i;
        ap_ufixed<alpha, -p>   a = 0;
        a(alpha-1,0) = t(alpha-1,0); // set alpha1 LSBs to the bit pattern of the index.

        int e_shift = a[a.wl()-1] ? 2*p : 2*p+1; // Stage dependent?
        ap_fixed<1+2*p+alpha, 2> one = 1;
        ap_fixed<1+2*p+alpha, 2> y_tilde_inverse = one - a + (one >> e_shift);
        T tmp;
#ifdef NO_OFFSET
        tmp = f(i,n);
#else
        tmp = f(i,n)-f(0,n);
#endif
#ifdef DUMP_TABLES
        std::cout.precision(30);
        std::cout << "y_tilde_inverse<" << p << "," << alpha << ">(" << a <<") => " << i << " = " << y_tilde_inverse << " " << y_tilde_inverse.to_string(2, true) << "\n";
        std::cout << "    log_inverse<" << p << "," << alpha << ">(" << a <<") => " << i << " = " << tmp << " " << tmp.to_string(2, true) << "\n";
#endif
        return tmp;
    }
};

template<int p0, int alpha0, class T>
class log0_inverse_lut_initializer
{
public:
    typedef T TableType;
    static T apply(int i, int n) {
        ap_uint<1+alpha0> t = i;
        ap_ufixed<1+alpha0,1> a;
        a(1+alpha0-1,0) = t(1+alpha0-1,0);

        double d = ::ceil((1 << alpha0)/a.to_double())/(1 << alpha0); // Equation in paper is wrong.
        T tmp = d;
        return tmp;
    }
};

template<class LOG_TYPE, class T>
class log0_table_traits {};

template<class LOG_TYPE>
class log0_table_traits<LOG_TYPE, float>
{
public:
    // Info below is duplicated from hls_log.h
    const static int p0 = 0; // We start with zero zeroed bits
    const static int alpha0 = 5; // 1+alpha0 = number of bits considered by stage 0.
    const static int p1 = (alpha0 == 5) ? 4 : (alpha0-2); // The number of bits zeroed after stage 0.
    const static int alpha1 = 4;
    const static int p2 = p1 + alpha1 - 1;
    const static int alpha2 = 6;
    const static int p3 = p2 + alpha2 - 1;
    const static int alpha3 = 6;
    const static int p4 = p3 + alpha3 - 1;
    const static int alpha4 = 6;
    const static int p5 = p4 + alpha4 - 1;
    const static int alpha5 = 6;
    static LOG_TYPE log0_offset() {
        return log_lut_initializer<log0_table_traits<LOG_TYPE, float>::p1, log0_table_traits<LOG_TYPE, float>::alpha1, LOG_TYPE>::f(0,0) +
               log_lut_initializer<log0_table_traits<LOG_TYPE, float>::p2, log0_table_traits<LOG_TYPE, float>::alpha2, LOG_TYPE>::f(0,0);
    }
};

template<class LOG_TYPE>
class log0_table_traits<LOG_TYPE, double>
{
public:
    // Info below is duplicated from hls_log.h
    const static int p0 = 0; // We start with zero zeroed bits
    const static int alpha0 = 5; // 1+alpha0 = number of bits considered by stage 0.
    const static int p1 = (alpha0 == 5) ? 4 : (alpha0-2); // The number of bits zeroed after stage 0.
    const static int alpha1 = 4;
    const static int p2 = p1 + alpha1 - 1;
    const static int alpha2 = 6;
    const static int p3 = p2 + alpha2 - 1;
    const static int alpha3 = 6;
    const static int p4 = p3 + alpha3 - 1;
    const static int alpha4 = 6;
    const static int p5 = p4 + alpha4 - 1;
    const static int alpha5 = 6;
    static LOG_TYPE log0_offset() {
        return log_lut_initializer<log0_table_traits<LOG_TYPE, double>::p1, log0_table_traits<LOG_TYPE, double>::alpha1, LOG_TYPE>::f(0,0) +
               log_lut_initializer<log0_table_traits<LOG_TYPE, double>::p2, log0_table_traits<LOG_TYPE, double>::alpha2, LOG_TYPE>::f(0,0) +
               log_lut_initializer<log0_table_traits<LOG_TYPE, double>::p3, log0_table_traits<LOG_TYPE, double>::alpha3, LOG_TYPE>::f(0,0) +
               log_lut_initializer<log0_table_traits<LOG_TYPE, double>::p4, log0_table_traits<LOG_TYPE, double>::alpha4, LOG_TYPE>::f(0,0) +
               log_lut_initializer<log0_table_traits<LOG_TYPE, double>::p5, log0_table_traits<LOG_TYPE, double>::alpha5, LOG_TYPE>::f(0,0);
    };
};

template<int p0, int alpha0, class LOG_TYPE, class T>
class log0_lut_initializer
{
public:
    typedef LOG_TYPE TableType;
    static LOG_TYPE apply(int i, int n) {
        ap_ufixed<1+alpha0,1> y_tilde_inverse = log0_inverse_lut_initializer<p0, alpha0, ap_ufixed<1+alpha0,1> > ::apply(i,n);

#ifdef NO_OFFSET
        LOG_TYPE tmp = -reference_log<LOG_TYPE>(y_tilde_inverse);
#else
        LOG_TYPE tmp = -reference_log<LOG_TYPE>(y_tilde_inverse) + log0_table_traits<LOG_TYPE, T>::log0_offset();
//             log_lut_initializer<log0_table_traits<LOG_TYPE, float>::p1, log0_table_traits<LOG_TYPE, float>::alpha1, LOG_TYPE>::f(0,0) +
//             log_lut_initializer<log0_table_traits<LOG_TYPE, float>::p2, log0_table_traits<LOG_TYPE, float>::alpha2, LOG_TYPE>::f(0,0);
        //log0_table_traits<LOG_TYPE, T>::log0_offset;
#endif
#ifdef DUMP_TABLES
        std::cout.precision(30);
        std::cout << "log0_offset = " << log0_table_traits<LOG_TYPE, T>::log0_offset() << " " << log0_table_traits<LOG_TYPE, T>::log0_offset().to_string(2, true) << "\n";
        std::cout << "y_tilde_inverse<" << p0 << "," << alpha0 << "> => " << i << " = " << y_tilde_inverse << " " << y_tilde_inverse.to_string(2, true) << "\n";
        std::cout << "    log_inverse<" << p0 << "," << alpha0 << "> => " << i << " = " << tmp << " " << tmp.to_string(2, true) << "\n";
#endif
        return tmp;
    }
};
