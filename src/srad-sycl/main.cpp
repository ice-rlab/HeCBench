#include <stdio.h>
#include <stdlib.h>
#include "./main.h"
#include "./util/graphics/graphics.h"
#include "./util/graphics/resize.h"

int main(int argc, char* argv []) {

  // times
  std::chrono::steady_clock::time_point time[13];

  time[0] = std::chrono::steady_clock::now();

  // inputs image, input paramenters
  FP* image_ori;                      // originalinput image
  int image_ori_rows;
  int image_ori_cols;
  long image_ori_elem;

  // inputs image, input paramenters
  FP* image;                          // input image
  int Nr,Nc;                          // IMAGE nbr of rows/cols/elements
  long Ne;

  // algorithm parameters
  int niter;                          // nbr of iterations
  FP lambda;                          // update step size

  // size of IMAGE
  int r1,r2,c1,c2;                    // row/col coordinates of uniform ROI
  long NeROI;                         // ROI nbr of elements

  // surrounding pixel indicies
  int* iN, *iS, *jE, *jW;

  // counters
  int iter;   // primary loop
  long i,j;     // image row/col

  // memory sizes
  int mem_size_i;
  int mem_size_j;

  // image size
  int mem_size;

  int blocks_x;
  int blocks_work_size, blocks_work_size2;
  size_t global_work_size, global_work_size2;
  size_t local_work_size;
  int no;
  int mul;
  FP total;
  FP total2;
  FP meanROI;
  FP meanROI2;
  FP varROI;
  FP q0sqr;

  time[1] = std::chrono::steady_clock::now();

  if(argc != 5){
    printf("Usage: %s <repeat> <lambda> <number of rows> <number of columns>\n", argv[0]);
    return 1;
  }
  else{
    niter = atoi(argv[1]);
    lambda = atof(argv[2]);
    Nr = atoi(argv[3]);
    Nc = atoi(argv[4]);
  }

  time[2] = std::chrono::steady_clock::now();

  //================================================================================80
  //   READ IMAGE (SIZE OF IMAGE HAS TO BE KNOWN)
  //================================================================================80

  image_ori_rows = 502;
  image_ori_cols = 458;
  image_ori_elem = image_ori_rows * image_ori_cols;
  image_ori = (FP*)malloc(sizeof(FP) * image_ori_elem);

  const char* input_image_path = "../data/srad/image.pgm";
  if ( !read_graphics( input_image_path, image_ori, image_ori_rows, image_ori_cols, 1) ) {
    printf("ERROR: failed to read input image at %s\n", input_image_path);
    if (image_ori != NULL) free(image_ori);
    return -1; // exit on file i/o error
  }

  time[3] = std::chrono::steady_clock::now();

  //================================================================================80
  //   RESIZE IMAGE (ASSUMING COLUMN MAJOR STORAGE OF image_orig)
  //================================================================================80

  Ne = Nr*Nc;

  image = (FP*)malloc(sizeof(FP) * Ne);

  resize(image_ori, image_ori_rows, image_ori_cols, image, Nr, Nc, 1);

  time[4] = std::chrono::steady_clock::now();

  //   SETUP

  // variables
  r1     = 0;      // top row index of ROI
  r2     = Nr - 1; // bottom row index of ROI
  c1     = 0;      // left column index of ROI
  c2     = Nc - 1; // right column index of ROI

  // ROI image size
  NeROI = (r2-r1+1)*(c2-c1+1);                      // number of elements in ROI, ROI size

  // allocate variables for surrounding pixels
  mem_size_i = sizeof(int) * Nr;                      //
  iN = (int *)malloc(mem_size_i) ;                    // north surrounding element
  iS = (int *)malloc(mem_size_i) ;                    // south surrounding element
  mem_size_j = sizeof(int) * Nc;                      //
  jW = (int *)malloc(mem_size_j) ;                    // west surrounding element
  jE = (int *)malloc(mem_size_j) ;                    // east surrounding element

  // N/S/W/E indices of surrounding pixels (every element of IMAGE)
  for (i=0; i<Nr; i++) {
    iN[i] = i-1;                            // holds index of IMAGE row above
    iS[i] = i+1;                            // holds index of IMAGE row below
  }
  for (j=0; j<Nc; j++) {
    jW[j] = j-1;                            // holds index of IMAGE column on the left
    jE[j] = j+1;                            // holds index of IMAGE column on the right
  }

  // N/S/W/E boundary conditions, fix surrounding indices outside boundary of image
  iN[0]    = 0;                             // changes IMAGE top row index from -1 to 0
  iS[Nr-1] = Nr-1;                          // changes IMAGE bottom row index from Nr to Nr-1
  jW[0]    = 0;                             // changes IMAGE leftmost column index from -1 to 0
  jE[Nc-1] = Nc-1;                          // changes IMAGE rightmost column index from Nc to Nc-1

#ifdef USE_GPU
  sycl::queue q(sycl::gpu_selector_v, sycl::property::queue::in_order());
#else
  sycl::queue q(sycl::cpu_selector_v, sycl::property::queue::in_order());
#endif

  // allocate memory for derivatives
  FP *d_dN = sycl::malloc_device<FP>(Ne, q);
  FP *d_dS = sycl::malloc_device<FP>(Ne, q);
  FP *d_dW = sycl::malloc_device<FP>(Ne, q);
  FP *d_dE = sycl::malloc_device<FP>(Ne, q);

  // allocate memory for entire IMAGE 
  mem_size = sizeof(FP) * Ne; // get the size of float representation of input IMAGE
  FP *d_I = sycl::malloc_device<FP>(Ne, q);

  // allocate memory for partial sums 
  FP *d_sums = sycl::malloc_device<FP>(Ne, q);
  FP *d_sums2 = sycl::malloc_device<FP>(Ne, q);

  // allocate memory for coordinates 
  int *d_iN = sycl::malloc_device<int>(Nr, q);
  q.memcpy(d_iN, iN, mem_size_i);

  int *d_iS = sycl::malloc_device<int>(Nr, q);
  q.memcpy(d_iS, iS, mem_size_i);

  int *d_jE = sycl::malloc_device<int>(Nc, q);
  q.memcpy(d_jE, jE, mem_size_j);

  int *d_jW = sycl::malloc_device<int>(Nc, q);
  q.memcpy(d_jW, jW, mem_size_j);

  // allocate memory for coefficient 
  FP *d_c = sycl::malloc_device<FP>(Ne, q);

  // threads
  local_work_size = NUMBER_THREADS;

  // workgroups
  blocks_x = Ne/(int)local_work_size;
  if (Ne % (int)local_work_size != 0){ // compensate for division remainder above by adding one grid
    blocks_x = blocks_x + 1;
  }
  blocks_work_size = blocks_x;
  global_work_size = blocks_work_size * local_work_size; // define the number of blocks in the grid

  time[5] = std::chrono::steady_clock::now();

  //================================================================================80
  //   COPY INPUT TO GPU
  //================================================================================80
  q.memcpy(d_I, image, mem_size).wait();

  time[6] = std::chrono::steady_clock::now();

  sycl::range<1> gws (global_work_size);
  sycl::range<1> lws (local_work_size);

  q.submit([&](sycl::handler& cgh) {
    cgh.parallel_for<class extract>(
      sycl::nd_range<1>(gws, lws), [=] (sycl::nd_item<1> item) {
      #include "kernel_extract.sycl"
    });
  }).wait();

  time[7] = std::chrono::steady_clock::now();

  for (iter=0; iter<niter; iter++){ // do for the number of iterations input parameter
    // Prepare kernel
    q.submit([&](sycl::handler& cgh) {
      cgh.parallel_for<class prepare>(
        sycl::nd_range<1>(gws, lws), [=] (sycl::nd_item<1> item) {
        #include "kernel_prepare.sycl"
      });
    });

    blocks_work_size2 = blocks_work_size;  // original number of blocks
    global_work_size2 = global_work_size;
    no = Ne;  // original number of sum elements
    mul = 1;  // original multiplier

    while(blocks_work_size2 != 0){

      sycl::range<1> gws2 (global_work_size2);

      q.submit([&](sycl::handler& cgh) {
        sycl::local_accessor<FP, 1> d_psum (lws, cgh);
        sycl::local_accessor<FP, 1> d_psum2 (lws, cgh);
        cgh.parallel_for<class reduce>(
          sycl::nd_range<1>(gws2, lws), [=] (sycl::nd_item<1> item) {
          #include "kernel_reduce.sycl"
        });
      }).wait();

      // update execution parameters
      no = blocks_work_size2;
      if(blocks_work_size2 == 1){
          blocks_work_size2 = 0;
      }
      else{
        mul = mul * NUMBER_THREADS; // update the increment
        blocks_x = blocks_work_size2/(int)local_work_size; // number of blocks
        if (blocks_work_size2 % (int)local_work_size != 0){ // compensate for division remainder above by adding one grid
            blocks_x = blocks_x + 1;
        }
        blocks_work_size2 = blocks_x;
        global_work_size2 = blocks_work_size2 * (int)local_work_size;
      }
    } // while

    q.memcpy(&total, d_sums, sizeof(FP));
    q.memcpy(&total2, d_sums2, sizeof(FP));

    q.wait();

    // calculate statistics

    meanROI  = total / (FP)(NeROI); // gets mean (average) value of element in ROI
    meanROI2 = meanROI * meanROI;
    varROI = (total2 / (FP)(NeROI)) - meanROI2; // gets variance of ROI
    q0sqr = varROI / meanROI2; // gets standard deviation of ROI

    // set arguments that were uptaded in this loop
    q.submit([&](sycl::handler& cgh) {
      cgh.parallel_for<class srad>(
        sycl::nd_range<1>(gws, lws) , [=] (sycl::nd_item<1> item) {
        #include "kernel_srad.sycl"
      });
    });

    q.submit([&](sycl::handler& cgh) {
      cgh.parallel_for<class srad2>(
        sycl::nd_range<1>(gws, lws), [=] (sycl::nd_item<1> item) {
        #include "kernel_srad2.sycl"
      });
    });
  }

  q.wait();

  time[8] = std::chrono::steady_clock::now();

  //   Compress Kernel - SCALE IMAGE UP FROM 0-1 TO 0-255 AND COMPRESS

  q.submit([&](sycl::handler& cgh) {
    cgh.parallel_for<class compress>(
      sycl::nd_range<1>(gws, lws), [=] (sycl::nd_item<1> item) {
      #include "kernel_compress.sycl"
    });
  }).wait();

  time[9] = std::chrono::steady_clock::now();

  q.memcpy(image, d_I, mem_size).wait();

  time[10] = std::chrono::steady_clock::now();

  //   WRITE OUTPUT IMAGE TO FILE

  write_graphics(
      "./image_out.pgm",
      image,
      Nr,
      Nc,
      1,
      255);

  time[11] = std::chrono::steady_clock::now();

  //   FREE MEMORY

  free(image_ori);
  free(image);
  free(iN);
  free(iS);
  free(jW);
  free(jE);

  sycl::free(d_I, q);
  sycl::free(d_c, q);
  sycl::free(d_iN, q);
  sycl::free(d_iS, q);
  sycl::free(d_jE, q);
  sycl::free(d_jW, q);
  sycl::free(d_dN, q);
  sycl::free(d_dS, q);
  sycl::free(d_dE, q);
  sycl::free(d_dW, q);
  sycl::free(d_sums, q);
  sycl::free(d_sums2, q);

  time[12] = std::chrono::steady_clock::now();

  //  DISPLAY TIMING

  printf("Time spent in different stages of the application:\n");
  long etime[13];
  for (int i = 0; i <= 11; i++) {
    etime[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(time[i+1] - time[i]).count();
  }
  etime[12] = std::chrono::duration_cast<std::chrono::nanoseconds>(time[12] - time[0]).count();

  printf("%15.12f s, %15.12f %% : SETUP VARIABLES\n",
         etime[0] * 1e-9, etime[0] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : READ COMMAND LINE PARAMETERS\n",
         etime[1] * 1e-9, etime[1] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : READ IMAGE FROM FILE\n",
         etime[2] * 1e-9, etime[2] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : RESIZE IMAGE\n", 
         etime[3] * 1e-9, etime[3] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : GPU DRIVER INIT, CPU/GPU SETUP, MEMORY ALLOCATION\n",
         etime[4] * 1e-9, etime[4] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : COPY DATA TO CPU->GPU\n",
         etime[5] * 1e-9, etime[5] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : EXTRACT IMAGE\n", 
         etime[6] * 1e-9, etime[6] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : COMPUTE (%d iterations)\n", 
         etime[7] * 1e-9, etime[7] * 100.0 / etime[12], niter);
  printf("%15.12f s, %15.12f %% : COMPRESS IMAGE\n", 
         etime[8] * 1e-9, etime[8] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : COPY DATA TO GPU->CPU\n", 
         etime[9] * 1e-9, etime[9] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : SAVE IMAGE INTO FILE\n", 
         etime[10] * 1e-9, etime[10] * 100.0 / etime[12]);
  printf("%15.12f s, %15.12f %% : FREE MEMORY\n", 
         etime[11] * 1e-9, etime[11] * 100.0 / etime[12]);
  printf("Total time:\n");
  printf("%.12f s\n", etime[12] * 1e-9);

  return 0;
}
