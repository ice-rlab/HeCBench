#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <chrono>
#include <omp.h>
#include "ecdh.h"

#define P_x 5
#define P_y 1
#define MODULUS 17
#define a 2

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

  #pragma omp target data map(from: pk_slow_x[0:num_pk], pk_slow_y[0:num_pk], \
                                    pk_fast_x[0:num_pk], pk_fast_y[0:num_pk])
  {
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < repeat; i++)
      k_slow (18, P_x, P_y, pk_slow_x, pk_slow_y, MODULUS, a, num_pk);  

    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    printf("Average time (slow kernel): %f s\n", time * 1e-9 / repeat);

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < repeat; i++)
      k_fast(18, P_x, P_y, pk_fast_x, pk_fast_y, MODULUS, a, num_pk);  

    end = std::chrono::steady_clock::now();
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    printf("Average time (fast kernel): %f s\n", time * 1e-9 / repeat);
  }

  bool fail_pk_x = memcmp(pk_slow_x, pk_fast_x, pk_x_size);
  bool fail_pk_y = memcmp(pk_slow_y, pk_fast_y, pk_x_size);
  printf("%s\n", (fail_pk_x || fail_pk_y) ? "FAIL" : "PASS");

  free(pk_slow_x);
  free(pk_slow_y);
  free(pk_fast_x);
  free(pk_fast_y);
  return 0;
}
