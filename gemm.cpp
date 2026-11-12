#include <bits/time.h>
#include <time.h>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <chrono>

// uint64_t nanos() {
//   auto now = std::chrono::system_clock::now();
//   auto duration = now.time_since_epoch();
//   auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
//   return static_cast<uint64_t>(nanoseconds.count());
// }

uint64_t nanos() {
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  return (uint64_t)(start.tv_sec * 1000000000LL) + (uint64_t)(start.tv_nsec);
}

// fuck std::vector
template<typename T>
struct Matrix {
  int row;
  int col;
  std::vector<T> data;
  T& operator()(int i, int j) {
    assert(i < row); assert(j < col);
    return data[i*row+j];
  }
  const T& operator()(int i, int j) const {
    assert(i < row); assert(j < col);
    return data[i*row+j];
  }
};

void matmulNaive(const Matrix<double>& A, const Matrix<double>& B, Matrix<double>& C) {
  int M = A.row;
  int K = A.col;
  int N = B.col;

  for (int y= 0; y < M; ++y) {
    for (int k = 0; k < K; ++k) {
      for (int x = 0; x < N; ++x)  {
        C(y,x) += A(y,k) * B(k, x);
      }
    }
  }
}



void matmulNaive(const Matrix<float>& A, const Matrix<float>& B, Matrix<float>& C) {
  int M = A.row;
  int N = B.col;
  int K = A.col;

  for (int y= 0; y < M; ++y) {
    for (int k = 0; k < K; ++k) {
      for (int x = 0; x < N; ++x)  {
        C(y,x) += A(y,k) * B(k, x);
      }
    }
  }
}

void matmulTile(const Matrix<float>& A, const Matrix<float>& B, Matrix<float>& C) {
  int M = A.row;
  int N = B.col;
  int K = A.col;

  constexpr auto BLOCK = 64;
  for (int ib = 0; ib < M; ib += BLOCK) {
    for (int kb = 0; kb < K; kb += BLOCK) {
      for (int jb = 0; jb < N; jb += BLOCK) {
        const float* a  = &A(ib, kb);
        const float* mb = &B(kb, jb);
        float*       c  = &C(ib, jb);

        for (int i = 0; i < BLOCK; ++i, c += N, a += K) {
          const float* b = mb;
          for (int k = 0; k < BLOCK; ++k, b += N) {
            for (int j = 0; j < BLOCK; ++j) {
              c[j] += a[k] * b[j];
            }
          }
        }
      }
    }
  }
}


template <typename T>
void print_matrix(const Matrix<T>& m) {
  for(int y = 0; y < m.row; ++y) {
    printf("[");
    for (int x = 0; x < m.col; ++x) {
      if (x < m.col - 1) printf("%.2f, ", m(y, x));
      else printf("%.2f", m(y,x));
    }
    printf("]\n");
  }
}

int main() {
  int N = 1024;

  auto a = Matrix<float>{N, N, std::vector<float>(N*N, 2.0f)};
  auto b = Matrix<float>{N, N, std::vector<float>(N*N, 2.0f)};
  auto c = Matrix<float>{N, N, std::vector<float>(N*N, 0.0f)};

  int run = 5;
  for (int i = 0; i < run; ++i) {
    auto start = nanos();
    // matmulNaive(a, b, c);
    matmulTile(a, b, c);
    auto end = nanos();

    double gflop = (2.0*N*N*N)*1e-9;
    double s = (end - start)*1e-9;

    printf("%.2f GFLOP/S -- %.2f ms\n", gflop/s, s*1e3);
  }

  // print_matrix(c);
}
