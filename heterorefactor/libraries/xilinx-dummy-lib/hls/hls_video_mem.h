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

/*
 * HLS Video Memory Partition Header File
 */

#ifndef ___HLS__VIDEO_MEM__
#define ___HLS__VIDEO_MEM__

namespace hls {

/* Template class of Window */
template<int ROWS, int COLS, typename T>
class Window {
public:
    Window() {
#pragma HLS ARRAY_PARTITION variable=val dim=1 complete
#pragma HLS ARRAY_PARTITION variable=val dim=2 complete
    };
    void shift_right();
    void shift_left();
    void shift_up();
    void shift_down();
    void insert(T value, int row, int col);
    void insert_bottom(T value[COLS]);
    void insert_top(T value[COLS]);
    void insert_left(T value[ROWS]);
    void insert_right(T value[ROWS]);
    T& getval(int row, int col);
    T& operator ()(int row, int col);

    T val[ROWS][COLS];
};

/* Member functions of Window class */

/* +---+---+-...-+---+---+
 * |   |   |     |   |   | R-1
 * +---+---+-...-+---+---+  
 * |   |   |     |   |   | R-2
 * +---+---+-...-+---+---+
 *   ...     ...    ...
 * +---+---+-...-+---+---+
 * |   |   |     |   |   | 1
 * +---+---+-...-+---+---+  
 * |   |   |     |   |   | 0
 * +---+---+-...-+---+---+
 *  C-1 C-2        1   0  (origin is at bottom-right point)
 */


/* Window shift left
 * Moves all the contents of the window horizontally (contents shift right)
 * Assumes new values will be placed in column = COLS-1
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::shift_left() {
#pragma HLS inline
    HLS_SIZE_T i, j;
    for(i = 0; i < ROWS; i++) {
#pragma HLS unroll
        for(j = 0; j < COLS-1; j++) {
#pragma HLS unroll
            val[i][j] = val[i][j+1];
        }
    }
}

/* Window shift right
 * Moves all the contents of the window horizontally (contents shift left)
 * Assumes new values will be placed in column = 0
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::shift_right() {
#pragma HLS inline
    HLS_SIZE_T i, j;
    for(i = 0; i < ROWS; i++) {
#pragma HLS unroll
        for(j = COLS-1; j > 0; j--) {
#pragma HLS unroll
            val[i][j] = val[i][j-1];
        }
    }
}

/* Window shift up
 * Moves all the contents of the window vertically (contents shift down)
 * Assumes new values will be placed in row = ROWS-1
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::shift_up() {
#pragma HLS inline
    HLS_SIZE_T i, j;
    for(i = 0; i < ROWS-1; i++) {
#pragma HLS unroll
        for(j = 0; j < COLS; j++) {
#pragma HLS unroll
            val[i][j] = val[i+1][j];
        }
    }
}

/* Window shift down
 * Moves all the contents of the window vertically (contents shift up)
 * Assumes new values will be placed in row = 0
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::shift_down() {
#pragma HLS inline
    HLS_SIZE_T i, j;
    for(i = ROWS-1; i > 0; i--) {
#pragma HLS unroll
        for(j = 0; j < COLS; j++) {
#pragma HLS unroll
            val[i][j] = val[i-1][j];
        }
    }
}

/* Window insert
 * Inserts a new value at any location of the window
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::insert(T value, int row, int col) {
#pragma HLS inline
    assert(row >= 0 && row < ROWS && col >= 0 && col < COLS);
    val[row][col] = value;
}

/* Window insert top
 * Inserts a set of values in row ROWS-1 of the window
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::insert_top(T value[COLS]) {
#pragma HLS inline
    HLS_SIZE_T j;
    for(j = 0; j < COLS; j++) {
#pragma HLS unroll
        val[ROWS-1][j] = value[j];
    }
}

/* Window insert bottom
 * Inserts a set of values in row 0 of the window
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::insert_bottom(T value[COLS]) {
#pragma HLS inline
    HLS_SIZE_T j;
    for(j = 0; j < COLS; j++) {
#pragma HLS unroll
        val[0][j] = value[j];
    }
}

/* Window insert left
 * Inserts a set of values in column COLS-1 of the window
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::insert_left(T value[ROWS]) {
#pragma HLS inline
    HLS_SIZE_T i;
    for(i = 0; i < ROWS; i++) {
#pragma HLS unroll
        val[i][COLS-1] = value[i];
    }
}

/* Window insert right
 * Inserts a set of values in column 0 of the window
 */
template<int ROWS, int COLS, typename T> void Window<ROWS, COLS, T>::insert_right(T value[ROWS]) {
#pragma HLS inline
    HLS_SIZE_T i;
    for(i = 0; i < ROWS; i++) {
#pragma HLS unroll
        val[i][0] = value[i];
    }
}

/* Window getval
 * Returns the data value in the window at position row, col
 */
template<int ROWS, int COLS, typename T> T& Window<ROWS, COLS, T>::getval(int row, int col) {
#pragma HLS inline
    assert(row >= 0 && row < ROWS && col >= 0 && col < COLS);
    return val[row][col];
}

/* Window getval
 * Returns the data value in the window at position row, col
 */
template<int ROWS, int COLS, typename T> T& Window<ROWS, COLS, T>::operator ()(int row, int col) {
#pragma HLS inline
    return getval(row, col);
}

/* Template class of Line Buffer */
template<int ROWS, int COLS, typename T, int RESHAPE=0>
class LineBuffer;

template<int ROWS, int COLS, typename T>
class LineBuffer<ROWS, COLS, T, 0> {
public:
    LineBuffer() {
#pragma HLS array_partition variable=val dim=1 complete
#pragma HLS dependence variable=val inter false
#pragma HLS dependence variable=val intra false
    };
    void shift_up(int col);
    void shift_down(int col);
    void insert_bottom(T value, int col);
    void insert_top(T value, int col);
    T& getval(int row, int col);
    T& operator ()(int row, int col);

    T val[ROWS][COLS];
};

/* Member functions of LineBuffer class */

/*     +---+---+-... ...-+---+---+
 * R-1 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+  
 * R-2 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+
 *       ...     ... ...   ...
 *     +---+---+-... ...-+---+---+
 *   1 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+  
 *   0 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+
 *       0   1   ... ...  C-2 C-1   (origin is at bottom-left point)
 */

/* Member functions of LineBuffer class */

/* Line buffer shift down
 * Assumes new data pixel will be entered at the bottom of the line buffer
 * The bottom is row = 0
 */
template<int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T>::shift_down(int col) {
#pragma HLS inline 
    assert(col >= 0 && col < COLS);
    HLS_SIZE_T i;
    for(i = ROWS-1; i > 0; i--) {
#pragma HLS unroll
        val[i][col] = val[i-1][col];
    }
}

/* Line buffer shift up
 * Assumes new data pixel will be entered at the top of the line buffer
 * The top is row = ROWS-1
 */
template<int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T>::shift_up(int col) {
#pragma HLS inline
    assert(col >= 0 && col < COLS);
    HLS_SIZE_T i;
    for(i = 0; i < ROWS-1; i++) {
#pragma HLS unroll
        val[i][col] = val[i+1][col];
    }
}

/* Line buffer insert bottom
 * Inserts a new value in the bottom row of the line buffer at column = col
 * The bottom is row = 0
 */
template<int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T>::insert_bottom(T value, int col) {
#pragma HLS inline
    assert(col >= 0 && col < COLS);
    val[0][col] = value;
}

/* Line buffer insert top
 * Inserts a new value in the top row of the line buffer at column = col
 * The top is row = ROWS - 1
 */
template <int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T>::insert_top(T value, int col) {
#pragma HLS inline
    assert(col >= 0 && col < COLS);
    val[ROWS-1][col] = value;
}

/* Line buffer getval
 * Returns the data value in the line buffer at position row, col
 */
template <int ROWS, int COLS, typename T> T& LineBuffer<ROWS, COLS, T>::getval(int row, int col) {
#pragma HLS inline
    assert(row >= 0 && row < ROWS && col >= 0 && col < COLS);
    return val[row][col];
}

/* Line buffer getval
 * Returns the data value in the line buffer at position row, col
 */
template<int ROWS, int COLS, typename T> T& LineBuffer<ROWS, COLS, T>::operator ()(int row, int col) {
#pragma HLS inline
    return getval(row, col);
}

/* Template class of Line Buffer (array reshaped) */
template<int ROWS, int COLS, typename T>
class LineBuffer<ROWS, COLS, T, 1> {
public:
    LineBuffer() {
#pragma HLS array_reshape variable=val dim=1
#pragma HLS dependence variable=val inter false
#pragma HLS dependence variable=val intra false
    };
    void shift_up(int col);
    void shift_down(int col);
    void insert_bottom(T value, int col);
    void insert_top(T value, int col);
    T& getval(int row, int col);
    T& operator ()(int row, int col);

    T val[ROWS][COLS];
};

/* Member functions of LineBuffer_reshape class */

/*     +---+---+-... ...-+---+---+
 * R-1 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+  
 * R-2 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+
 *       ...     ... ...   ...
 *     +---+---+-... ...-+---+---+
 *   1 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+  
 *   0 |   |   |         |   |   |
 *     +---+---+-... ...-+---+---+
 *       0   1   ... ...  C-2 C-1   (origin is at bottom-left point)
 */

/* Member functions of LineBuffer_reshape class */

/* Line buffer shift down
 * Assumes new data pixel will be entered at the bottom of the line buffer
 * The bottom is row = 0
 */
template<int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T, 1>::shift_down(int col) {
#pragma HLS inline 
    assert(col >= 0 && col < COLS);
    HLS_SIZE_T i;
    for(i = ROWS-1; i > 0; i--) {
#pragma HLS unroll
        val[i][col] = val[i-1][col];
    }
}

/* Line buffer shift up
 * Assumes new data pixel will be entered at the top of the line buffer
 * The top is row = ROWS-1
 */
template<int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T, 1>::shift_up(int col) {
#pragma HLS inline
    assert(col >= 0 && col < COLS);
    HLS_SIZE_T i;
    for(i = 0; i < ROWS-1; i++) {
#pragma HLS unroll
        val[i][col] = val[i+1][col];
    }
}

/* Line buffer insert bottom
 * Inserts a new value in the bottom row of the line buffer at column = col
 * The bottom is row = 0
 */
template<int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T, 1>::insert_bottom(T value, int col) {
#pragma HLS inline
    assert(col >= 0 && col < COLS);
    val[0][col] = value;
}

/* Line buffer insert top
 * Inserts a new value in the top row of the line buffer at column = col
 * The top is row = ROWS - 1
 */
template <int ROWS, int COLS, typename T> void LineBuffer<ROWS, COLS, T, 1>::insert_top(T value, int col) {
#pragma HLS inline
    assert(col >= 0 && col < COLS);
    val[ROWS-1][col] = value;
}

/* Line buffer getval
 * Returns the data value in the line buffer at position row, col
 */
template <int ROWS, int COLS, typename T> T& LineBuffer<ROWS, COLS, T, 1>::getval(int row, int col) {
#pragma HLS inline
    assert(row >= 0 && row < ROWS && col >= 0 && col < COLS);
    return val[row][col];
}

/* Line buffer getval
 * Returns the data value in the line buffer at position row, col
 */
template<int ROWS, int COLS, typename T> T& LineBuffer<ROWS, COLS, T, 1>::operator ()(int row, int col) {
#pragma HLS inline
    return getval(row, col);
}

} // namespace hls

#endif
