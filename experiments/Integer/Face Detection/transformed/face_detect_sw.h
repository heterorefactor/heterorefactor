/*
 * face_detect.h
 *
 *  Created on: Apr 16, 2019
 *      Author: hczhu
 */

#ifndef FACE_DETECT_SW_H_
#define FACE_DETECT_SW_H_


/*===============================================================*/
/*                                                               */
/*                       face_detect_sw.h                        */
/*                                                               */
/*             Software version for face detection.              */
/*                                                               */
/*===============================================================*/


#ifndef __FACE_DETECT_SW_H__
#define __FACE_DETECT_SW_H__

#include "typedefs.h"

#include "ap_int.h"
extern bool guard_error_face_detect_sw;

void face_detect_sw(ap_uint<8> Data[IMAGE_HEIGHT][IMAGE_WIDTH],
                    int _result_x[RESULT_SIZE],
                    int _result_y[RESULT_SIZE],
                    int _result_w[RESULT_SIZE],
                    int _result_h[RESULT_SIZE],
                    int *result_size);

#endif

#endif /* FACE_DETECT_SW_H_ */
