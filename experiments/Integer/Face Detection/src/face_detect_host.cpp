/*===============================================================*/
/*                                                               */
/*                       face_detect.cpp                         */
/*                                                               */
/*     Main host function for the Face Detection application.    */
/*                                                               */
/*===============================================================*/

// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <time.h>
#include <sys/time.h>
#include "commons.h"

#ifdef SW
  # include "face_detect_sw.h"
#endif

// other headers
//#include "utils.h"
#include "typedefs.h"
//#include "check_result.h"

// data
#include "image_16_16.h"

int main(int argc, char ** argv) 
{
  printf("Face Detection Application\n");

  std::string outFile("");


  // for this benchmark, input data is included in array Data
  // these are outputs
  int result_x[RESULT_SIZE];
  int result_y[RESULT_SIZE];
  int result_w[RESULT_SIZE];
  int result_h[RESULT_SIZE];
  int res_size = 0;

  // timers
  struct timeval start, end;

  // opencl version host code


  // sdsoc host code


  // software version host code
  #ifdef SW
// to be implemented
//    gettimeofday(&start, 0);
    face_detect_sw(Data, result_x, result_y, result_w, result_h, &res_size);
//    gettimeofday(&end, 0);
  #endif
 
//  check results
//  printf("Checking results:\n");
//  check_results(res_size, result_x, result_y, result_w, result_h, Data, outFile);
    
  // print time
  long long elapsed = (end.tv_sec - start.tv_sec) * 1000000LL + end.tv_usec - start.tv_usec;   
  printf("elapsed time: %lld us\n", elapsed);

  // cleanup
  #ifdef OCL
    facedetect_world.releaseWorld();
  #endif

  return EXIT_SUCCESS;

}
