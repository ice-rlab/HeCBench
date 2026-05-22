#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <chrono>
#include <sycl/sycl.hpp>
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

#ifdef USE_GPU
  sycl::queue q(sycl::gpu_selector_v, sycl::property::queue::in_order());
#else
  sycl::queue q(sycl::cpu_selector_v, sycl::property::queue::in_order());
#endif

  int *d_pk_x = sycl::malloc_device<int>(num_pk, q);
  int *d_pk_y = sycl::malloc_device<int>(num_pk, q);
  
  sycl::range<1> gws ((num_pk + 255) / 256 * 256);
  sycl::range<1> lws (256);

  auto start = std::chrono::steady_clock::now();

  for (int i = 0; i < repeat; i++) {
    q.submit([&] (sycl::handler &cgh) {
      cgh.parallel_for<class ecdh_slow>(
        sycl::nd_range<1>(gws, lws), [=] (sycl::nd_item<1> item) {
        k_slow(item, 18, P_x, P_y,
               d_pk_x, d_pk_y,
               MODULUS, a, num_pk);  
      });
    });
  }

  q.wait();
  auto end = std::chrono::steady_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  printf("Average time (slow kernel): %f s\n", time * 1e-9 / repeat);

  q.memcpy(pk_slow_x, d_pk_x, pk_x_size);
  q.memcpy(pk_slow_y, d_pk_y, pk_y_size);

  q.wait();

  start = std::chrono::steady_clock::now();

  for (int i = 0; i < repeat; i++) {
    q.submit([&] (sycl::handler &cgh) {
      cgh.parallel_for<class ecdh_fast>(
        sycl::nd_range<1>(gws, lws), [=] (sycl::nd_item<1> item) {
        k_fast(item, 18, P_x, P_y,
               d_pk_x, d_pk_y,
               MODULUS, a, num_pk);  
      });
    });
  }

  q.wait();
  end = std::chrono::steady_clock::now();
  time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  printf("Average time (fast kernel): %f s\n", time * 1e-9 / repeat);

  q.memcpy(pk_fast_x, d_pk_x, pk_x_size);
  q.memcpy(pk_fast_y, d_pk_y, pk_y_size);

  q.wait();
  
  bool fail_pk_x = memcmp(pk_slow_x, pk_fast_x, pk_x_size);
  bool fail_pk_y = memcmp(pk_slow_y, pk_fast_y, pk_x_size);
  printf("%s\n", (fail_pk_x || fail_pk_y) ? "FAIL" : "PASS");

  sycl::free(d_pk_x, q);
  sycl::free(d_pk_y, q);
  free(pk_slow_x);
  free(pk_slow_y);
  free(pk_fast_x);
  free(pk_fast_y);
  return 0;
}
