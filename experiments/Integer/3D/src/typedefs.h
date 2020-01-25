/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*                     Typedefs for the host                     */
/*                                                               */
/*===============================================================*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__
#include "commons.h"
#include "ap_int.h"

// resolution 256x256
const int MAX_X = 256;
const int MAX_Y = 256;

// number of values in frame buffer: 32 bits
const int NUM_FB = MAX_X * MAX_Y / 4;
// dataset information
const int NUM_3D_TRI = 192;

#ifdef OCL
  #include <string>
  // target device
  // change here to map to a different device
  const std::string TARGET_DEVICE = "xilinx:aws-vu9p-f1:4ddr-xpr-2pr:4.0";
#endif

#ifndef SW
  // hls header
  #include "ap_int.h"
  // specialized datatypes
  typedef ap_uint<1> bit1;
  typedef ap_uint<2> bit2;
  typedef ap_uint<8> bit8;
  typedef ap_uint<16> bit16;
  typedef ap_uint<32> bit32;
#else
  typedef unsigned int bit8;
  typedef unsigned int bit32;
#endif

// struct: 3D triangle
typedef struct
{
  int   x0;
  int   y0;
  int   z0;
  int   x1;
  int   y1;
  int   z1;
  int   x2;
  int   y2;
  int   z2;
} Triangle_3D;

// struct: 2D triangle
typedef struct
{
  ap_uint<8>   x0;
  ap_uint<8>   y0;
  ap_uint<8>   x1;
  ap_uint<8>   y1;
  ap_uint<8>   x2;
  ap_uint<8>   y2;
  ap_uint<8>   z;
} Triangle_2D;

// struct: candidate pixels
typedef struct
{
  ap_uint<8>   x;
  ap_uint<8>   y;
  ap_uint<8>   z;
  ap_uint<8>   color;
} CandidatePixel;

// struct: colored pixel
typedef struct
{
  ap_uint<8>   x;
  ap_uint<8>   y;
  ap_uint<8>   color;
} Pixel;

// dataflow switch
#define ENABLE_DATAFLOW
#endif
