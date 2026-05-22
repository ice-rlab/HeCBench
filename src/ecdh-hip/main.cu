#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <chrono>
#include <hip/hip_runtime.h>
#include "ecdh.h"

#define P_x 5
#define P_y 1
#define MODULUS 17
#define A 2

int main(int argc, char **argv)
{
  if (argc != 3) {
    printf("Usage: %s <positive number of keys> <repeat>\n", argv[0]);
    return 1;
  }
  const int num_pk = atoi(argv[1]); 
  const int repeat = atoi(argv[2]); 

  const int pk_x_size = num_pk * sizeof(int);
  const int pk_y_size = num_pk * sizeof(int);

  int *pk_slow_x = (int*) malloc (pk_x_size);
  int *pk_slow_y = (int*) malloc (pk_y_size);
  int *pk_fast_x = (int*) malloc (pk_x_size);
  int *pk_fast_y = (int*) malloc (pk_y_size);

  int *d_pk_x, *d_pk_y;
  hipMalloc((void**)&d_pk_x, pk_x_size);
  hipMalloc((void**)&d_pk_y, pk_y_size);
  
  dim3 grids = (num_pk + 255) / 256;

  auto start = std::chrono::steady_clock::now();

  for (int i = 0; i < repeat; i++)
    k_slow <<<grids, 256>>> (18, P_x, P_y, d_pk_x, d_pk_y, MODULUS, A, num_pk);  

  hipDeviceSynchronize();
  auto end = std::chrono::steady_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  printf("Average time (slow kernel): %f s\n", time * 1e-9 / repeat);

  hipMemcpy(pk_slow_x, d_pk_x, pk_x_size, hipMemcpyDeviceToHost);
  hipMemcpy(pk_slow_y, d_pk_y, pk_y_size, hipMemcpyDeviceToHost);

  start = std::chrono::steady_clock::now();

  for (int i = 0; i < repeat; i++)
    k_fast <<<grids, 256>>> (18, P_x, P_y, d_pk_x, d_pk_y, MODULUS, A, num_pk);  

  hipDeviceSynchronize();
  end = std::chrono::steady_clock::now();
  time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  printf("Average time (fast kernel): %f s\n", time * 1e-9 / repeat);

  hipMemcpy(pk_fast_x, d_pk_x, pk_x_size, hipMemcpyDeviceToHost);
  hipMemcpy(pk_fast_y, d_pk_y, pk_y_size, hipMemcpyDeviceToHost);


  bool fail_pk_x = memcmp(pk_slow_x, pk_fast_x, pk_x_size);
  bool fail_pk_y = memcmp(pk_slow_y, pk_fast_y, pk_x_size);
  printf("%s\n", (fail_pk_x || fail_pk_y) ? "FAIL" : "PASS");
  
  hipFree(d_pk_x);
  hipFree(d_pk_y);
  free(pk_slow_x);
  free(pk_slow_y);
  free(pk_fast_x);
  free(pk_fast_y);
  return 0;
}
