#ifndef _RGB_2_YUV_
#define _RGB_2_YUV_

#include "thls/tops/fp_flopoco.hpp"
#include "thls/tops/policy_flopoco.hpp"

const int E=8;
const int F=17;

//void rgb2yuv(ap_int<512> r, ap_int<512> g, ap_int<512> b, ap_int<512> y, ap_int<512> u, ap_int<512> v);
void rgb2yuv(int r, int g, int b, float& y, float& u, float& v);

#endif
