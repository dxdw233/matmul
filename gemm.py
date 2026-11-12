import numpy as np
import time

def nanos(): return time.monotonic_ns()

if __name__ == "__main__":
  N = 256
  A = np.random.randn(N,N).astype(np.float32)
  B = np.random.randn(N,N).astype(np.float32)
  run = 10
  for i in range(run):
    start = nanos()
    C = A @ B.T
    end = nanos()
    gflop = (2*N*N*N)*1e-9
    s = (end-start)*1e-9
    print(f"{gflop/s:.2f} GFLOP/S -- {s*1e3:.2f} ms")


