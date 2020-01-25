/*===============================================================*/
/*                                                               */
/*                       rendering_sw.h                          */
/*                                                               */
/*              Software version for 3D Rendering                */
/*                                                               */
/*===============================================================*/

#ifndef __RENDERING_SW_H__
#define __RENDERING_SW_H__

#include "typedefs.h"

#include "ap_int.h"
void rendering_sw(Triangle_3D triangle_3ds[NUM_3D_TRI], ap_uint<8> output[MAX_X][MAX_Y]);

#endif
