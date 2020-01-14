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

#ifndef ___HLS__VIDEO__
#error hls_video_histogram.h is not intended to be included directly.  Please include hls_video.h instead.
#endif

#ifndef _HLS_VIDEO_HISTOGRAM_
#define _HLS_VIDEO_HISTOGRAM_

namespace hls {

template<int SRC_T, int DST_T,int ROW, int COL, int N>
void Equalize(
	Mat<ROW, COL, SRC_T>	&_src,
	Mat<ROW, COL, DST_T>	&_dst,
    ap_uint<8> (&map)[N]
                      )
{
    // Assumed 8 bits per pixel
    ap_uint<8> addr=0;
    // The histogram needs to be able to count up to the number of pixels in the image.
    ap_uint<BitWidth<ROW*COL>::Value> hist_out[N];
    ap_uint<BitWidth<ROW*COL>::Value> data[2];
#pragma HLS ARRAY_PARTITION variable= data complete dim=0
    for(int i=0;i<N;i++)
        hist_out[i]=0;
    int cols=_src.cols;
    int rows=_src.rows;
 loop_height: for(int i=0;i<rows;i++)
    {
    loop_width: for(int j=0;j<cols;j++)
        {
#pragma HLS PIPELINE
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS DEPENDENCE array inter false
            HLS_TNAME(SRC_T) tempsrc=0;
            HLS_TNAME(DST_T) tempdst=0;
            _src.data_stream[0].read(tempsrc);

            if(!(i==0&&j==0))
            {
                data[1]=data[0];
                data[0]=hist_out[tempsrc];
                if(tempsrc==addr)
                    data[0]++;
                hist_out[addr]=data[1]+1;
                addr=tempsrc;

                tempdst=map[tempsrc];
                _dst.data_stream[0]<<tempdst;
            }
            else
            {
                addr=tempsrc;
                data[0]=hist_out[tempsrc]+1;
            }
        }
    }
    hist_out[addr]=data[0]+1;
    _dst.data_stream[0]<<map[addr];

    // floating point probably makes sense here, because cols*rows could be large or small.
    // This could probably be only 8 mantissa bits or so, though.
    float scale = 255.0f/(cols*rows);
    ap_uint<BitWidth<ROW*COL>::Value> sum=0;
 loop_normalize: for(int i=0;i<N;i++)
    {
#pragma HLS PIPELINE
        sum+=hist_out[i];
        ap_uint<8> val=sr_cast< ap_uint<8> > (sum*scale);
        map[i]=val;
    }
    map[0]=0;
}

static  ap_uint<8> lut[256];
template<int SRC_T, int DST_T,int ROW, int COL>
void EqualizeHist(
		Mat<ROW, COL, SRC_T>	&_src,
		Mat<ROW, COL, DST_T>	&_dst)
{ 
#pragma HLS INLINE
    Equalize(_src, _dst, lut);
}
}
#endif
