# N square matmul, N = 1024, numpy has ~400-500 GFLOPS/S
clang -O2 -march=native gemm.c -o gemm # 28 GFLOPS/S
# clang -O2 gemm.c -o gemm && ./gemm # 20 GFLOPS/S

clang++ -O2 gemm.cpp -o gemmcpp # 18 GFLOPS/S
# clang++ -O2 -march=native gemm.cpp -o gemmcpp # 14 GFLOPS, even worse, dont know why
