#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <chrono>

#include "main.h"                // (in the main program folder)  needed to recognized input parameters
#include "file.h"

//  UTILITIES
#include "./util/avi/avilib.h"          // (in directory)              needed by avi functions
#include "./util/avi/avimod.h"          // (in directory)              needed by avi functions

//  KERNEL
uint64_t 
kernel_gpu_wrapper(params_common common,
                   int* endoRow,
                   int* endoCol,
                   int* tEndoRowLoc,
                   int* tEndoColLoc,
                   int* epiRow,
                   int* epiCol,
                   int* tEpiRowLoc,
                   int* tEpiColLoc,
                   avi_t* frames);

int 
main(int argc, char* argv []) {

  printf("Workgroup size of kernel = %d \n", NUMBER_THREADS);

  // time
  std::chrono::steady_clock::time_point time0,
                                        time1,
                                        time2,
                                        time3,
                                        time4,
                                        time5;

  // other
  avi_t* frames;

  time0 = std::chrono::steady_clock::now();

  //  STRUCTURES, GLOBAL STRUCTURE VARIABLES

  params_common common;
  common.common_mem = sizeof(params_common);

  //   FRAME INFO

  // variables
  char* video_file_name;

  // open movie file
  video_file_name = (char *) "../data/heartwall/test.avi";
  frames = (avi_t*)AVI_open_input_file(video_file_name, 1);                            // added casting
  if (frames == NULL)  {
    AVI_print_error((char *) "Error with AVI_open_input_file");
    return -1;
  }

  // dimensions
  common.no_frames = AVI_video_frames(frames);
  common.frame_rows = AVI_video_height(frames);
  common.frame_cols = AVI_video_width(frames);
  common.frame_elem = common.frame_rows * common.frame_cols;
  common.frame_mem = sizeof(FP) * common.frame_elem;

  time1 = std::chrono::steady_clock::now();

  //   CHECK INPUT ARGUMENTS

  if(argc!=2){
    printf("ERROR: missing argument (number of frames to processed) or too many arguments\n");
    return 0;
  }
  else{
    common.frames_processed = atoi(argv[1]);
    if(common.frames_processed<0 || common.frames_processed>common.no_frames){
      printf("ERROR: %d is an incorrect number of frames specified, select in the range of 0-%d\n", common.frames_processed, common.no_frames);
      return 0;
    }
  }

  time2 = std::chrono::steady_clock::now();

  //  INPUTS

  //  READ PARAMETERS FROM FILE

  char* param_file_name = (char *) "../data/heartwall/input.txt";
  read_parameters(param_file_name,  
                  &common.tSize,
                  &common.sSize,
                  &common.maxMove,
                  &common.alpha);

  //  READ SIZE OF INPUTS FROM FILE

  read_header(param_file_name,
              &common.endoPoints,
              &common.epiPoints);

  common.allPoints = common.endoPoints + common.epiPoints;

  //  READ DATA FROM FILE

  //  ENDO POINTS MEMORY ALLOCATION

  common.endo_mem = sizeof(int) * common.endoPoints;

  int* endoRow;
  endoRow = (int*)malloc(common.endo_mem);
  int* endoCol;
  endoCol = (int*)malloc(common.endo_mem);
  int* tEndoRowLoc;
  tEndoRowLoc = (int*)malloc(common.endo_mem * common.no_frames);
  int* tEndoColLoc;
  tEndoColLoc = (int*)malloc(common.endo_mem * common.no_frames);

  //  EPI POINTS MEMORY ALLOCATION

  common.epi_mem = sizeof(int) * common.epiPoints;

  int* epiRow;
  epiRow = (int *)malloc(common.epi_mem);
  int* epiCol;
  epiCol = (int *)malloc(common.epi_mem);
  int* tEpiRowLoc;
  tEpiRowLoc = (int *)malloc(common.epi_mem * common.no_frames);
  int* tEpiColLoc;
  tEpiColLoc = (int *)malloc(common.epi_mem * common.no_frames);

  //  READ DATA FROM FILE

  read_data(param_file_name,
            common.endoPoints,
            endoRow,
            endoCol,
            common.epiPoints,
            epiRow,
            epiCol);

  time3 = std::chrono::steady_clock::now();

  //  KERNELL WRAPPER CALL

  uint64_t kernelTime = kernel_gpu_wrapper(common,
                     endoRow,
                     endoCol,
                     tEndoRowLoc,
                     tEndoColLoc,
                     epiRow,
                     epiCol,
                     tEpiRowLoc,
                     tEpiColLoc,
                     frames);

  time4 = std::chrono::steady_clock::now();

  //  DUMP DATA TO FILE
#ifdef OUTPUT
  write_data("result.txt",
             common.no_frames,
             common.frames_processed,    
             common.endoPoints,
             tEndoRowLoc,
             tEndoColLoc,
             common.epiPoints,
             tEpiRowLoc,
             tEpiColLoc);

#endif
  //  DEALLOCATION

  // endo points

  free(endoRow);
  free(endoCol);
  free(tEndoRowLoc);
  free(tEndoColLoc);

  // epi points

  free(epiRow);
  free(epiCol);
  free(tEpiRowLoc);
  free(tEpiColLoc);

  time5 = std::chrono::steady_clock::now();

  //  DISPLAY TIMING

  printf("Time spent in different stages of the application:\n");
  auto etime1 = std::chrono::duration_cast<std::chrono::nanoseconds>(time1 - time0).count();
  auto etime2 = std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1).count();
  auto etime3 = std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2).count();
  auto etime4 = std::chrono::duration_cast<std::chrono::nanoseconds>(time4 - time3).count();
  auto etime5 = std::chrono::duration_cast<std::chrono::nanoseconds>(time5 - time4).count();
  auto etime  = std::chrono::duration_cast<std::chrono::nanoseconds>(time5 - time0).count();

  printf("%15.12f s, %15.12f : READ INITIAL VIDEO FRAME\n",
         etime1 * 1e-9, etime1 * 100.0 / etime);
  printf("%15.12f s, %15.12f : READ COMMAND LINE PARAMETERS\n",
         etime2 * 1e-9, etime2 * 100.0 / etime);
  printf("%15.12f s, %15.12f : READ INPUTS FROM FILE\n",
         etime3 * 1e-9, etime3 * 100.0 / etime);
  printf("%15.12f s, %15.12f : GPU ALLOCATION, COPYING, COMPUTATION\n",
         etime4 * 1e-9, etime4 * 100.0 / etime);
  printf("%15.12f s, %15.12f : GPU KERNELS\n",
         kernelTime * 1e-9, kernelTime * 100.0  / etime);
  printf("%15.12f s, %15.12f : FREE MEMORY\n",
         etime5 * 1e-9, etime5 * 100.0 / etime);
  printf("Total time:\n");
  printf("%15.12f s\n", etime * 1e-9);

  //======================================================================================================================================================150
  //  End
  //======================================================================================================================================================150

  //========================================================================================================================================================================================================200
  //  End
  //========================================================================================================================================================================================================200

}
