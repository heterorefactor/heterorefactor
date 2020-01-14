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

// This code is partially derived from OpenCV:

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef ___HLS__VIDEO__
#error hls_video_undistort.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef HLS_VIDEO_UNDISTORT_H_
#define HLS_VIDEO_UNDISTORT_H_

namespace hls {

template <int N, typename T,typename PT>
void LU( T A[N][N], PT pos[N])
{
    int  imax = 0;
    T lu[N] ;
    for (int i = 0; i < N; i++)
    {
        lu[i]=0;
    }
    for (int i = 0; i < N; i++) {
        T big = 0.0;
        for (int j = 0; j < N; j++) {
            T temp = ::hls::abs((double)A[i][j]);
            if (temp > big) big = temp;
        }
        lu[i] = ((T)1.0)/big;
    }
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < j; i++) {
            T sum = A[i][j];
            for (int k = 0; k < i; k++) sum -= A[i][k] * A[k][j];
            A[i][j] = sum;
        }
        T big = 0.0;
        for (int i = j; i < N; i++) {
            T sum = A[i][j];
            for (int k = 0; k < j; k++) sum -= A[i][k] * A[k][j];
            A[i][j] = sum;
            T dum = lu[i] *((T) ::hls::abs((double)sum));
            if (dum >= big) {
                big = dum;
                imax = i;
            }
        }
        if (j != imax&&imax<N&&imax>=0) {
            for (int i = 0; i < N; i++) {
                T mid = A[imax][i];
                A[imax][i] = A[j][i];
                A[j][i] = mid;
            }
            T mid = lu[j];
            lu[j] = lu[imax];
            lu[imax] = mid;
        }
        pos[j] = imax;
        if (j != N - 1) {
            T dum = ((T)1.0)/(A[j][j]);
            for (int i = j + 1; i < N; i++) A[i][j] *= dum;
        }
    }
}
template <int N, typename T, typename PT,typename BT>
void LUex(T A[N][N], PT pos[N],BT b[N])
{
    int ii = 0;
    for (int i = 0; i < N; i++) {
        int ip = pos[i];
        T sum=0;
        if(ip<N&& ip>=0)
        {
            sum = b[ip];
            b[ip] = b[i];
        }
        if (ii != 0)
            for (int j = ii - 1; j < i; j++) sum -= A[i][j] * b[j];
        else
            ii = i + 1;
        b[i] = sum;
    }
    for (int i = N - 1; i >= 0; i--) {
        T sum = b[i];
        for (int j = i + 1; j < N; j++) sum -= A[i][j]*b[j];
        b[i] = (sum)*((T) 1.0/A[i][i]);
    }
}


template <int N,typename T>
void Invert(T A[N][N],T Ar[N][N], int flags=0)
{
    T b[N];
    int pos[N];
    for (int i = 0; i < N; i++) {
        b[i]=0;
        pos[i]=0;
    }
    LU<N>(A,pos);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) b[j] = 0;
        b[i] = 1.0;
        LUex<N>(A,pos,b);
        for (int j = 0; j < N; j++) Ar[j][i] = b[j];
    }
}
template<int AROW, int ACOL, int BCOL, typename AT, typename BT, typename CT>
void mul(
               AT A [AROW][ ACOL],
               BT B [AROW][ BCOL],
               CT C [AROW][ BCOL]
)
{
 loop_i: for(int i=0; i<AROW; i++)
    {
    loop_j: for(int j=0; j<BCOL; j++)
        {
            CT sum=0;
        loop_k: for(int k=0; k<ACOL; k++)
                {
                sum=sum+A[i][k]*B[k][j];

            }
            C[i][j]=sum;
        }
    }
}

template <int WIN_ROW, int ROW, int COL, int SRC_T, int DST_T, int MAP1_T, int MAP2_T>
void Remap(
           Mat<ROW, COL, SRC_T>    &src,
           Mat<ROW, COL, DST_T>    &dst,
           Mat<ROW, COL, MAP1_T>   &map1,
           Mat<ROW, COL, MAP2_T>   &map2
        //int bordertype=BORDER_CONSTANT
        //const Scalar<HLS_MAT_CN(SRC_T),HLS_NAME_MAT(SRC_T)> borderValue
        )
{
    HLS_TNAME(SRC_T) buf[WIN_ROW][COL][HLS_MAT_CN(SRC_T)];
    Scalar<HLS_MAT_CN(SRC_T),HLS_TNAME(SRC_T)>      s;
    Scalar<HLS_MAT_CN(DST_T),HLS_TNAME(DST_T)>      d;
    Scalar<HLS_MAT_CN(MAP1_T),HLS_TNAME(MAP1_T)>    m1;
    Scalar<HLS_MAT_CN(MAP1_T),HLS_TNAME(MAP2_T)>    m2;
    int rows=src.rows;
    int cols=src.cols;
    int ishift=WIN_ROW/2;
    int r[WIN_ROW] = {};

 loop_height: for( int i=0; i< rows+ishift; i++)
    {
#pragma HLS LOOP_FLATTEN OFF
    loop_width: for( int j=0; j< cols; j++)
        {
#pragma HLS PIPELINE II=1
#pragma HLS dependence array inter false
            if(i<rows&& j<cols)
            {
                src >> s;
            }
            for(int k=0;k<HLS_MAT_CN(SRC_T);k++) {
                buf[i % WIN_ROW][j][k] = s.val[k];
            }
            r[i % WIN_ROW] = i;

            if(i>=ishift)
            {
                map1 >> m1;
                map2 >> m2;
                int x=m1.val[0];
                int y=m1.val[1];

                if(y>=0 && y<rows && r[y%WIN_ROW] == y && x>=0 && x<cols) {
// Don't read and write a row at the same time.
//                     assert(y%WIN_ROW != i%WIN_ROW);
                    for(int k=0;k<HLS_MAT_CN(SRC_T);k++) {
                        d.val[k]=buf[y%WIN_ROW][x][k];
                    }
                } else {
                    for(int k=0;k<HLS_MAT_CN(SRC_T);k++) {
                        d.val[k]=0;
                    }
                }
                dst<< d;
            }
        }
    }
}
#define HLS_INTER_TAB_SIZE 32
#define HLS_INTER_BITS     5

template< typename CMT, typename DT, typename ICMT, int ROW, int COL, int MAP1_T, int MAP2_T, int N>
void InitUndistortRectifyMapInverse (
                            Window<3,3, CMT> cameraMatrix,
                            DT (&distCoeffs)[N],
                            Window<3,3, ICMT> ir,
                            Mat<ROW, COL, MAP1_T>  &map1,
                            Mat<ROW, COL, MAP2_T>  &map2,
                            int noRotation=false)
{
    int rows= map1.rows;
    int cols= map1.cols;
    Scalar<HLS_MAT_CN(MAP1_T), HLS_TNAME(MAP1_T)> m1;
    Scalar<HLS_MAT_CN(MAP2_T), HLS_TNAME(MAP2_T)> m2;

    DT zo=0;
    DT k1=distCoeffs[0];
    DT k2=distCoeffs[1];
    DT p1=distCoeffs[2];
    DT p2=distCoeffs[3];
    DT k3= N>=5? distCoeffs[4] : zo;
    DT k4= N>=8? distCoeffs[5] : zo;
    DT k5= N>=8? distCoeffs[6] : zo;
    DT k6= N>=8? distCoeffs[7] : zo;
    CMT u0= cameraMatrix.val[0][2];
    CMT v0= cameraMatrix.val[1][2];
    CMT fx= cameraMatrix.val[0][0];
    CMT fy= cameraMatrix.val[1][1];
 loop_height: for(int i=0; i< rows; i++)
    {
#pragma HLS LOOP_TRIPCOUNT max=ROW
    loop_width: for(int j=0; j< cols; j++)
        {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS LOOP_TRIPCOUNT max=COL
            //#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE II=1
            ICMT x,y;
            if(noRotation)
            {
                // A special case if there is no rotation
                x=i*ir.val[0][1] + j * ir.val[0][0] + ir.val[0][2];
                y=i*ir.val[1][1] + j * ir.val[1][0] + ir.val[1][2];
            } else {
                ICMT _x=i*ir.val[0][1] + j * ir.val[0][0] + ir.val[0][2];
                ICMT _y=i*ir.val[1][1] + j * ir.val[1][0] + ir.val[1][2];
                ICMT _w=i*ir.val[2][1] + j * ir.val[2][0] + ir.val[2][2];

                ICMT w = ap_ufixed<1,1>(1.0f)/_w;
                x = (ICMT)(_x*w);
                y = (ICMT)(_y*w);
            }

            typename x_traits<ICMT,ICMT>::MULT_T x2 = x*x, y2 = y*y;
            typename x_traits<ICMT,ICMT>::MADD_T _2xy = 2*x*y;
            typename x_traits<typename x_traits<ICMT,ICMT>::MULT_T, typename x_traits<ICMT,ICMT>::MULT_T>::ADD_T r2 = x2 + y2;
            ICMT kr = (1 + ICMT(ICMT(k3*r2 + k2)*r2 +k1)*r2)/(1 + ICMT(ICMT(k6*r2 + k5)*r2 + k4)*r2);
            CMT u = fx*(ICMT(x*kr) + ICMT(p1*_2xy) + ICMT(p2*(2*x2 + r2))) + u0;
            CMT v = fy*(ICMT(y*kr) + ICMT(p1*(r2 + 2*y2)) + ICMT(p2*_2xy)) + v0;

            assert(MAP1_T==HLS_16SC2);
            {
                int iu = sr_cast<int>(u*HLS_INTER_TAB_SIZE);
                int iv = sr_cast<int>(v*HLS_INTER_TAB_SIZE);

                m1.val[0]=sr_cast<short>(iu >> HLS_INTER_BITS);
                m1.val[1]=sr_cast<short>(iv >> HLS_INTER_BITS);
                m2.val[0]=(unsigned short)((iv &(HLS_INTER_TAB_SIZE-1))*HLS_INTER_TAB_SIZE + (iu & (HLS_INTER_TAB_SIZE-1)));
            }
//             else
//             {
//                 m1.val[0]=sr_cast<HLS_TNAME(MAP1_T)>(u);
//                 m1.val[1]=sr_cast<HLS_TNAME(MAP1_T)>(v);

//             }
            map1<< m1;
            map2<< m2;
        }
    }
}
template< typename CMT, typename RT, typename DT, int ROW, int COL, int MAP1_T, int MAP2_T, int N>
void InitUndistortRectifyMap(
                            Window<3,3, CMT> cameraMatrix,
                            DT (&distCoeffs)[N],
                            Window<3,3, RT> R,
                            Window<3,3, CMT> newcameraMatrix,
                            Mat<ROW, COL, MAP1_T>  &map1,
                            Mat<ROW, COL, MAP2_T>  &map2
                            )
{
    Window<3,3, RT> iR;
    Window<3,3, RT> ir;
    mul<3, 3, 3>(newcameraMatrix.val, R.val, iR.val);
    Invert<3>(iR.val, ir.val);
    InitUndistortRectifyMapInverse(cameraMatrix, distCoeffs,ir,map1,map2);
}
}
#endif
