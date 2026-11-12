#include <bits/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int N = 1024;

void fill(float* m, bool zeros) {
  if (zeros)
    memset(m, 0, N*N*sizeof(float));
  else {
    for (int i = 0; i < N * N; ++i) {
      m[i] = (float)rand() / (float)RAND_MAX;
    }
  }
}

uint64_t nanos() {
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  return (uint64_t)(start.tv_sec * 1000000000LL) + (uint64_t)(start.tv_nsec);
}

void print_mat(float* m) {
  for (int y = 0; y < N; ++y) {
    printf("\n");
    for (int x = 0; x < N; ++x) {
      printf("%.2f ", m[y*N+x]);
    }
  }
}

void matmul(float* a, float* b, float* c) {
  for (int y = 0; y < N; ++y) {
    for (int k = 0; k < N; ++k) {
      for (int x = 0; x < N; ++x) {
        c[y*N+x] += a[y*N+k] * b[k*N+x];
      }
    }
  }
}

int main() {
  srand(42);
  float* a = (float*)calloc(N*N, sizeof(float));
  float* b = (float*)calloc(N*N, sizeof(float));
  float* c = (float*)calloc(N*N, sizeof(float));
  fill(a, false); fill(b, false); fill(c, false);

  int run = 5;
  for (int i = 0; i < run; ++i) {
    uint64_t start = nanos();
    matmul(a, b, c);
    uint64_t end = nanos();
    double s = (end-start)*1e-9;
    double gflops = (2.0*N*N*N)*1e-9;
    printf("%.3f GFLOP/S -- %.2f ms\n", gflops/s, s*1e3);
  }
}
