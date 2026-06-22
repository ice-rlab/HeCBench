# CUDA Compute Sanitizer — Essential Commands (No External Input)

Minimal run commands for CUDA benchmarks that do **not** require external data files, derived from [`CUDA_BENCHMARK_INPUT_SPLIT.md`](CUDA_BENCHMARK_INPUT_SPLIT.md). Benchmarking repeat/iteration counts from `make run` are reduced to **1** so NVIDIA Compute Sanitizer runs finish in reasonable time while still exercising the GPU kernels.

## Usage

```bash
cd <benchmark-directory>   # see tables below
make
compute-sanitizer [tool-options] <essential-command>
```

Common tool options: `--tool memcheck` (default), `--tool synccheck`, `--tool initcheck`. For MPI benchmarks (`allreduce`, `ccl`, `pingpong`, `sparkler`), wrap the binary inside `mpirun`:

```bash
mpirun -n 2 compute-sanitizer ./main
```

**Notes:**
- Problem sizes (matrix dimensions, buffer lengths, etc.) are kept at `make run` defaults unless noted; only repeat/iteration/warmup-style parameters are minimized.
- Benchmarks with multiple `make run` variants list one representative variant.
- Python benchmarks (`convolutionDeformable`, `dwconv1d`, `braycurtis`) use `compute-sanitizer python ...`.
- Output paths (`hungarian`, `bn`, `streamcluster`) are preserved; they write results but do not require pre-existing input files.

**Total benchmarks:** 419

---

## Bandwidth

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| allreduce | `src/allreduce-cuda` | `./main` | `./main` | `mpirun -n 2 compute-sanitizer ./main` |
| cmembench | `src/cmembench-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| babelstream | `src/babelstream-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| ccl | `src/ccl-cuda` | `./main 50` | `./main 1` | `mpirun -n 2 compute-sanitizer ./main 1` |
| memcpy | `src/memcpy-cuda` | `./main 8192` | `./main 8192` | `compute-sanitizer ./main 8192` |
| memtest | `src/memtest-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| p2p | `src/p2p-cuda` | `./main 2000` | `./main 2000` | `compute-sanitizer ./main 2000` |
| pingpong | `src/pingpong-cuda` | `./mpi-program` / `./nccl-program` | `./mpi-program` | `mpirun -n 2 compute-sanitizer ./mpi-program  # also: mpirun -n 2 compute-sanitizer ./nccl-program` |
| randomAccess | `src/randomAccess-cuda` | `./main 10` | `./main 1` | `compute-sanitizer ./main 1` |
| reshapeKVCache | `src/reshapeKVCache-cuda` | `./main 1024 32 128 1000` | `./main 1024 32 128 1` | `compute-sanitizer ./main 1024 32 128 1` |
| shmembench | `src/shmembench-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| storeKVCache | `src/storeKVCache-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| threadcpy | `src/threadcpy-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| triad | `src/triad-cuda` | `./main --passes 100 -v` | `./main --passes 1 -v` | `compute-sanitizer ./main --passes 1 -v` |
| vadd | `src/vadd-cuda` | `./main 16384 8192 100` | `./main 16384 8192 1` | `compute-sanitizer ./main 16384 8192 1` |

---

## Bioinformatics

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| all-pairs-distance | `src/all-pairs-distance-cuda` | `./main 10000` | `./main 10000` | `compute-sanitizer ./main 10000` |
| cm | `src/cm-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| epistasis | `src/epistasis-cuda` | `./main 146 31339 100` | `./main 146 31339 1` | `compute-sanitizer ./main 146 31339 1` |
| extend2 | `src/extend2-cuda` | `./main 2000` | `./main 1` | `compute-sanitizer ./main 1` |
| fsm | `src/fsm-cuda` | `./main 65536` | `./main 65536` | `compute-sanitizer ./main 65536` |
| ga | `src/ga-cuda` | `./main 1000000 1000 11 1` | `./main 1000000 1000 11 1` | `compute-sanitizer ./main 1000000 1000 11 1` |
| nbnxm | `src/nbnxm-cuda` | `./main 10000` | `./main 10000` | `compute-sanitizer ./main 10000` |
| nw | `src/nw-cuda` | `./main 16384 10 100` | `./main 16384 10 1` | `compute-sanitizer ./main 16384 10 1` |
| pcc | `src/pcc-cuda` | `./main 90112 165` | `./main 90112 165` | `compute-sanitizer ./main 90112 165` |

---

## Computer vision and image processing

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| aobench | `src/aobench-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| asmooth | `src/asmooth-cuda` | `./main 8192 2000 9 100` | `./main 8192 2000 9 1` | `compute-sanitizer ./main 8192 2000 9 1` |
| background-subtract | `src/background-subtract-cuda` | `./main 4096 2048 0 102` | `./main 4096 2048 0 1` | `compute-sanitizer ./main 4096 2048 0 1` |
| bezier-surface | `src/bezier-surface-cuda` | `./main -n 8192` | `./main -n 8192` | `compute-sanitizer ./main -n 8192` |
| bilateral | `src/bilateral-cuda` | `./main 2960 1440 0.5 0.5 1000` | `./main 2960 1440 0.5 0.5 1` | `compute-sanitizer ./main 2960 1440 0.5 0.5 1` |
| cbsfil | `src/cbsfil-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| car | `src/car-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| ced | `src/ced-cuda` | `./main -a 0` | `./main -a 0` | `compute-sanitizer ./main -a 0` |
| colorwheel | `src/colorwheel-cuda` | `./main 10 4096 100` | `./main 10 4096 1` | `compute-sanitizer ./main 10 4096 1` |
| convolution1D | `src/convolution1D-cuda` | `./main 134217728 1000` | `./main 134217728 1` | `compute-sanitizer ./main 134217728 1` |
| convolution3D | `src/convolution3D-cuda` | `./main 32 1 6 32 32 5 100` | `./main 32 1 6 32 32 5 1` | `compute-sanitizer ./main 32 1 6 32 32 5 1` |
| convolutionDeformable | `src/convolutionDeformable-cuda` | `python main.py` | `python main.py` | `compute-sanitizer python main.py` |
| convolutionSeperable | `src/convolutionSeparable-cuda` | `./main 8192 8192 1000` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| dct8x8 | `src/dct8x8-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| debayer | `src/debayer-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| depixel | `src/depixel-cuda` | `./main 2048 2048 10` | `./main 2048 2048 1` | `compute-sanitizer ./main 2048 2048 1` |
| degrid | `src/degrid-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| doh | `src/doh-cuda` | `./main 480 640 100` | `./main 480 640 1` | `compute-sanitizer ./main 480 640 1` |
| dpid | `src/dpid-cuda` | `./main 640 480 0.5 100` | `./main 640 480 0.5 1` | `compute-sanitizer ./main 640 480 0.5 1` |
| egs | `src/egs-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| flame | `src/flame-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| gabor | `src/gabor-cuda` | `./main 8640 15360 1000` | `./main 8640 15360 1` | `compute-sanitizer ./main 8640 15360 1` |
| gamma-correction | `src/gamma-correction-cuda` | `./main 7680 4320 256 100` | `./main 7680 4320 256 1` | `compute-sanitizer ./main 7680 4320 256 1` |
| groupnorm | `src/groupnorm-cuda` | `./main 32 32 16 16 32 100` | `./main 32 32 16 16 32 1` | `compute-sanitizer ./main 32 32 16 16 32 1` |
| mandelbrot | `src/mandelbrot-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| marchCubes | `src/marchingCubes-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| match | `src/match-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| morphology | `src/morphology-cuda` | `./main 32 32 8192 8192 100` | `./main 32 32 8192 8192 1` | `compute-sanitizer ./main 32 32 8192 8192 1` |
| ne | `src/ne-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| perlin | `src/perlin-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| resize | `src/resize-cuda` | `./main 1920 1080 256 256 512 100` | `./main 1920 1080 256 256 512 1` | `compute-sanitizer ./main 1920 1080 256 256 512 1` |
| spm | `src/spm-cuda` | `./main 256 1000` | `./main 256 1` | `compute-sanitizer ./main 256 1` |
| srad | `src/srad-cuda` | `./main 1000 0.5 502 458` | `./main 1000 0.5 502 1` | `compute-sanitizer ./main 1000 0.5 502 1` |
| ssim | `src/ssim-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| stencil1d | `src/stencil1d-cuda` | `./main 134217728 1000` | `./main 134217728 1` | `compute-sanitizer ./main 134217728 1` |
| stencil3d | `src/stencil3d-cuda` | `./main 512 100` | `./main 512 1` | `compute-sanitizer ./main 512 1` |
| surfel | `src/surfel-cuda` | `./main 1024 640 480 100` | `./main 1024 640 480 1` | `compute-sanitizer ./main 1024 640 480 1` |
| tgvnn | `src/tgvnn-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| upsample | `src/upsample-cuda` | `./main 32 64 64 64 100` | `./main 32 64 64 64 1` | `compute-sanitizer ./main 32 64 64 64 1` |
| zoom | `src/zoom-cuda` | `./main 1 3 2160 4096 1000` | `./main 1 3 2160 4096 1` | `compute-sanitizer ./main 1 3 2160 4096 1` |

---

## Cryptography

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| bitpermute | `src/bitpermute-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| chacha20 | `src/chacha20-cuda` | `./main 100000` | `./main 1` | `compute-sanitizer ./main 1` |
| ecdh | `src/ecdh-cuda` | `./main 50000000 100` | `./main 50000000 1` | `compute-sanitizer ./main 50000000 1` |
| keccaktreehash | `src/keccaktreehash-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| merkle | `src/merkle-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| present | `src/present-cuda` | `./main 100000 100` | `./main 100000 1` | `compute-sanitizer ./main 100000 1` |

---

## Data compression and reduction

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| atomicAggregate | `src/atomicAggregate-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| atomicCAS | `src/atomicCAS-cuda` | `./main 10000` | `./main 1` | `compute-sanitizer ./main 1` |
| atomicCost | `src/atomicCost-cuda` | `./main 1 10` | `./main 1 1` | `compute-sanitizer ./main 1 1` |
| atomicIntrinsics | `src/atomicIntrinsics-cuda` | `./main 27 100` | `./main 27 1` | `compute-sanitizer ./main 27 1` |
| atomicPerf | `src/atomicPerf-cuda` | `./main 1024` | `./main 1024` | `compute-sanitizer ./main 1024` |
| atomicSystemWide | `src/atomicSystemWide-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| bitpacking | `src/bitpacking-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| bscan | `src/bscan-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| bwt | `src/bwt-cuda` | `./main 10000000` | `./main 10000000` | `compute-sanitizer ./main 10000000` |
| compute-score | `src/compute-score-cuda` | `./main -p=1000` | `./main -p=1` | `compute-sanitizer ./main -p=1` |
| contract | `src/contract-cuda` | `./main 64 5` | `./main 64 5` | `compute-sanitizer ./main 64 5` |
| filter | `src/filter-cuda` | `./main 100000000 256 100` | `./main 100000000 256 1` | `compute-sanitizer ./main 100000000 256 1` |
| fma | `src/fma-cuda` | `./main 8192 8192 1031 8192 1000` | `./main 8192 8192 1031 8192 1` | `compute-sanitizer ./main 8192 8192 1031 8192 1` |
| fpc | `src/fpc-cuda` | `./main 256 100` | `./main 256 1` | `compute-sanitizer ./main 256 1` |
| histogram | `src/histogram-cuda` | `./main --i=100` | `./main --i=1` | `compute-sanitizer ./main --i=1` |
| minmax | `src/minmax-cuda` | `./main 100000 1000` | `./main 100000 1` | `compute-sanitizer ./main 100000 1` |
| mtf | `src/mtf-cuda` | `./main 1024 100` | `./main 1024 1` | `compute-sanitizer ./main 1024 1` |
| quantAQLM | `src/quantAQLM-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| quantBnB | `src/quantBnB-cuda` | `./main 16777216 100` | `./main 16777216 1` | `compute-sanitizer ./main 16777216 1` |
| quantVLLM | `src/quantVLLM-cuda` | `./main 4096 5137 1000` | `./main 4096 5137 1` | `compute-sanitizer ./main 4096 5137 1` |
| rle | `src/rle-cuda` | `./main --i=1000` | `./main --i=1` | `compute-sanitizer ./main --i=1` |
| sc | `src/sc-cuda` | `./main -a 0.1` | `./main -a 0.1` | `compute-sanitizer ./main -a 0.1` |
| scan | `src/scan-cuda` | `./main 268435456 100` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| scan2 | `src/scan2-cuda` | `./main 1000 33554432 256` | `./main 1 33554432 256` | `compute-sanitizer ./main 1 33554432 256` |
| scan3 | `src/scan3-cuda` | `./main 1000 33554432` | `./main 1 33554432` | `compute-sanitizer ./main 1 33554432` |
| scatter | `src/scatter-cuda` | `./main 10000000 100` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| scatterAdd | `src/scatterAdd-cuda` | `./main 10000000 32 8 1000` | `./main 10000000 32 8 1` | `compute-sanitizer ./main 10000000 32 8 1` |
| scatterThrust | `src/scatterThrust-cuda` | `./main 10000000 100` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| segment-reduce | `src/segment-reduce-cuda` | `./main 16384 100` | `./main 16384 1` | `compute-sanitizer ./main 16384 1` |

---

## Data encoding, decoding, or verification

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| ans | `src/ans-cuda` | `./main 10` | `./main 10` | `compute-sanitizer ./main 10` |
| base64e | `src/base64e-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| crc64 | `src/crc64-cuda` | `./main 10 5 33554432` | `./main 1 5 33554432` | `compute-sanitizer ./main 1 5 33554432` |
| crs | `src/crs-cuda` | `./main 10 10` | `./main 10 10` | `compute-sanitizer ./main 10 10` |
| entropy | `src/entropy-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| jenkins-hash | `src/jenkins-hash-cuda` | `./main 256 16777216 100` | `./main 256 16777216 1` | `compute-sanitizer ./main 256 16777216 1` |
| kiss | `src/kiss-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| ldpc | `src/ldpc-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| md5hash | `src/md5hash-cuda` | `./main 1 4` | `./main 1 4` | `compute-sanitizer ./main 1 4` |
| murmurhash3 | `src/murmurhash3-cuda` | `./main 100000 100` | `./main 100000 1` | `compute-sanitizer ./main 100000 1` |

---

## Finance

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| aop | `src/aop-cuda` | `./main -runs 100` | `./main -runs 1` | `compute-sanitizer ./main -runs 1` |
| black-scholes | `src/black-scholes-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| binomial | `src/binomial-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| bonds | `src/bonds-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| libor | `src/libor-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |

---

## Geoscience

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| aidw | `src/aidw-cuda` | `./main 100 1 100` | `./main 100 1 1` | `compute-sanitizer ./main 100 1 1` |
| coordinates | `src/coordinates-cuda` | `./main 10000000 1000` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| hausdorff | `src/hausdorff-cuda` | `./main 100000 100000 100` | `./main 100000 100000 1` | `compute-sanitizer ./main 100000 100000 1` |

---

## Graph and Tree

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| floydwarshall | `src/floydwarshall-cuda` | `./main 1024 100 16` | `./main 1024 1 16` | `compute-sanitizer ./main 1024 1 16` |
| hungarian | `src/hungarian-cuda` | `./main output.txt` | `./main sanitizer_output.txt` | `compute-sanitizer ./main sanitizer_output.txt` |
| sssp | `src/sssp-cuda` | `./main -g 120 -t 1 -w 10 -r 100` | `./main -g 120 -t 1 -w 10 -r 1` | `compute-sanitizer ./main -g 120 -t 1 -w 10 -r 1` |

---

## Language and kernel features

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| adjacent | `src/adjacent-cuda` | `./main 100000000 1000` | `./main 100000000 1` | `compute-sanitizer ./main 100000000 1` |
| aligned-types | `src/aligned-types-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| asta | `src/asta-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| blockAccess | `src/blockAccess-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| blockexchange | `src/blockexchange-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| blockScan | `src/blockScan-cuda` | `./main 2227104 100` | `./main 2227104 1` | `compute-sanitizer ./main 2227104 1` |
| collision | `src/collision-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| concurrentKernels | `src/concurrentKernels-cuda` | `./main 4` | `./main 4` | `compute-sanitizer ./main 4` |
| conversion | `src/conversion-cuda` | `./main 268435456 10` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| dispatch | `src/dispatch-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| dp4a | `src/dp4a-cuda` | `./main 268435456 1000` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| graphExecution | `src/graphExecution-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| ert | `src/ert-cuda` | `./main 1024 256` | `./main 1024 256` | `compute-sanitizer ./main 1024 256` |
| interleave | `src/interleave-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| intrinsics-cast | `src/intrinsics-cast-cuda` | `./main 1000000 100` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |
| kernelLaunch | `src/kernelLaunch-cuda` | `./main 1000000` | `./main 1000000` | `compute-sanitizer ./main 1000000` |
| layout | `src/layout-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| mallocFree | `src/mallocFree-cuda` | `./main 536870912` | `./main 536870912` | `compute-sanitizer ./main 536870912` |
| maxFlops | `src/maxFlops-cuda` | `./main 5000` | `./main 1` | `compute-sanitizer ./main 1` |
| mixbench | `src/mixbench-cuda` | `./main 8192 1000` | `./main 8192 1` | `compute-sanitizer ./main 8192 1` |
| nosync | `src/nosync-cuda` | `./main 268435456 100` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| openmp | `src/openmp-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| overlap | `src/overlap-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| pad | `src/pad-cuda` | `./main -a 0.1` | `./main -a 0.1` | `compute-sanitizer ./main -a 0.1` |
| pitch | `src/pitch-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| popcount | `src/popcount-cuda` | `./main 16777216 1000` | `./main 16777216 1` | `compute-sanitizer ./main 16777216 1` |
| pointerchase | `src/pointerchase-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| prefetch | `src/prefetch-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| reverse | `src/reverse-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| ring | `src/ring-cuda` | `./main 1 67108864 100` | `./main 1 67108864 1` | `compute-sanitizer ./main 1 67108864 1` |
| saxpy-ompt | `src/saxpy-ompt-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| shuffle | `src/shuffle-cuda` | `./main 200000 100` | `./main 200000 1` | `compute-sanitizer ./main 200000 1` |
| simpleMultiDevice | `src/simpleMultiDevice-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| streamCreateCopyDestroy | `src/streamCreateCopyDestroy-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| streamOrderedAllocation | `src/streamOrderedAllocation-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| streamPriority | `src/streamPriority-cuda` | `./main 300` | `./main 300` | `compute-sanitizer ./main 300` |
| streamUM | `src/streamUM-cuda` | `./main 4 1000 1` | `./main 4 1000 1` | `compute-sanitizer ./main 4 1000 1` |
| tensorAccessor | `src/tensorAccessor-cuda` | `./main 8192 8192 1000` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| threadfence | `src/threadfence-cuda` | `./main 100 100000000` | `./main 1 100000000` | `compute-sanitizer ./main 1 100000000` |
| warpexchange | `src/warpexchange-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| vote | `src/vote-cuda` | `./main 10000000` | `./main 10000000` | `compute-sanitizer ./main 10000000` |
| wmma | `src/wmma-cuda` | `./main 0 16 16 64 100 1` | `./main 0 16 16 64 1 1` | `compute-sanitizer ./main 0 16 16 64 1 1` |
| wordcount | `src/wordcount-cuda` | `./main 10` | `./main 10` | `compute-sanitizer ./main 10` |
| zerocopy | `src/zerocopy-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |

---

## Machine learning

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| accuracy | `src/accuracy-cuda` | `./main 8192 10000 10 100` | `./main 8192 10000 10 1` | `compute-sanitizer ./main 8192 10000 10 1` |
| adam | `src/adam-cuda` | `./main 10000 200 100` | `./main 10000 200 1` | `compute-sanitizer ./main 10000 200 1` |
| adamw | `src/adamw-cuda` | `./main 1000000 100` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |
| addBiasQKV | `src/addBiasQKV-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| addBiasResidualLayerNorm | `src/addBiasResidualLayerNorm-cuda` | `./main 10000` | `./main 1` | `compute-sanitizer ./main 1` |
| attention | `src/attention-cuda` | `./main 65536 2048 0 1000` | `./main 65536 2048 0 1` | `compute-sanitizer ./main 65536 2048 0 1` |
| attention-paged | `src/attention-paged-cuda` | `./main 8 32 128 4096 128 100` | `./main 8 32 128 4096 128 1` | `compute-sanitizer ./main 8 32 128 4096 128 1` |
| attentionMergeState | `src/attentionMergeState-cuda` | `./main 1024 32 128 1000` | `./main 1024 32 128 1` | `compute-sanitizer ./main 1024 32 128 1` |
| attentionMultiHead | `src/attentionMultiHead-cuda` | `./main 100000` | `./main 1` | `compute-sanitizer ./main 1` |
| attentionMultiHeadKVCache | `src/attentionMultiHeadKVCache-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| backprop | `src/backprop-cuda` | `./main 65536` | `./main 65536` | `compute-sanitizer ./main 65536` |
| bincount | `src/bincount-cuda` | `./main 10000000 100` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| bn | `src/bn-cuda` | `./main result 10` | `./main result 1` | `compute-sanitizer ./main result 1` |
| channelShuffle | `src/channelShuffle-cuda` | `./main 2 224 224 100` | `./main 2 224 224 1` | `compute-sanitizer ./main 2 224 224 1` |
| channelSum | `src/channelSum-cuda` | `./main 224 224 100` | `./main 224 224 1` | `compute-sanitizer ./main 224 224 1` |
| clink | `src/clink-cuda` | `./main 10` | `./main 1` | `compute-sanitizer ./main 1` |
| concat | `src/concat-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| crossEntropy | `src/crossEntropy-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| dense-embedding | `src/dense-embedding-cuda` | `./main 100000 256 1000` | `./main 100000 256 1` | `compute-sanitizer ./main 100000 256 1` |
| dropout | `src/dropout-cuda` | `./main 16777216 1000` | `./main 16777216 1` | `compute-sanitizer ./main 16777216 1` |
| dwconv | `src/dwconv-cuda` | `./main 128 16 224 224 100` | `./main 128 16 224 224 1` | `compute-sanitizer ./main 128 16 224 224 1` |
| dwconv1d | `src/dwconv1d-cuda` | `python run.py` | `python run.py` | `compute-sanitizer python run.py` |
| expdist | `src/expdist-cuda` | `./main 16384 1000` | `./main 16384 1` | `compute-sanitizer ./main 16384 1` |
| flip | `src/flip-cuda` | `./main 3 1024 100` | `./main 3 1024 1` | `compute-sanitizer ./main 3 1024 1` |
| gelu | `src/gelu-cuda` | `./main 8 4096 512 100` | `./main 8 4096 512 1` | `compute-sanitizer ./main 8 4096 512 1` |
| geglu | `src/geglu-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| glu | `src/glu-cuda` | `./main 3 1024 100` | `./main 3 1024 1` | `compute-sanitizer ./main 3 1024 1` |
| gru | `src/gru-cuda` | `./main 102400 1024 100` | `./main 102400 1024 1` | `compute-sanitizer ./main 102400 1024 1` |
| kalman | `src/kalman-cuda` | `./main 1000 100 3 100` | `./main 1000 1 3 1` | `compute-sanitizer ./main 1000 1 3 1` |
| knn | `src/knn-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| layernorm | `src/layernorm-cuda` | `./main 8 1024 768 2000` | `./main 8 1024 768 1` | `compute-sanitizer ./main 8 1024 768 1` |
| lda | `src/lda-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| lif | `src/lif-cuda` | `./main 10000 32 30000` | `./main 10000 32 1` | `compute-sanitizer ./main 10000 32 1` |
| logprob | `src/logprob-cuda` | `./main 1024 8 50257 100` | `./main 1024 8 50257 1` | `compute-sanitizer ./main 1024 8 50257 1` |
| lrn | `src/lrn-cuda` | `./main 10` | `./main 1` | `compute-sanitizer ./main 1` |
| mask | `src/mask-cuda` | `./main 512 512 512 100` | `./main 512 512 512 1` | `compute-sanitizer ./main 512 512 512 1` |
| matern | `src/matern-cuda` | `./main 300 100` | `./main 300 1` | `compute-sanitizer ./main 300 1` |
| maxpool3d | `src/maxpool3d-cuda` | `./main 2048 2048 96 100` | `./main 2048 2048 96 1` | `compute-sanitizer ./main 2048 2048 96 1` |
| mergeVS | `src/mergeVS-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| mlp | `src/mlp-cuda` | `./main 3 128 784 512 10 0 0 100` | `./main 3 128 784 512 10 0 0 1` | `compute-sanitizer ./main 3 128 784 512 10 0 0 1` |
| moe | `src/moe-cuda` | `./main 32768 384 1 1000` | `./main 32768 384 1 1` | `compute-sanitizer ./main 32768 384 1 1` |
| moe-align | `src/moe-align-cuda` | `./main 100000` | `./main 1` | `compute-sanitizer ./main 1` |
| moe-sum | `src/moe-sum-cuda` | `./main 256 4096 100` | `./main 256 4096 1` | `compute-sanitizer ./main 256 4096 1` |
| mrc | `src/mrc-cuda` | `./main 10000000 1000` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| multinomial | `src/multinomial-cuda` | `./main 65536 2048 100` | `./main 65536 2048 1` | `compute-sanitizer ./main 65536 2048 1` |
| nlll | `src/nlll-cuda` | `./main 2048 1000 1000` | `./main 2048 1000 1` | `compute-sanitizer ./main 2048 1000 1` |
| nonzero | `src/nonzero-cuda` | `./main 2048 2048 100` | `./main 2048 2048 1` | `compute-sanitizer ./main 2048 2048 1` |
| overlay | `src/overlay-cuda` | `./main 640 480` | `./main 640 480` | `compute-sanitizer ./main 640 480` |
| p4 | `src/p4-cuda` | `./main 10000` | `./main 1` | `compute-sanitizer ./main 1` |
| page-rank | `src/page-rank-cuda` | `./main -n 20000 -i 100` | `./main -n 20000 -i 1` | `compute-sanitizer ./main -n 20000 -i 1` |
| permute | `src/permute-cuda` | `./main 8 100` | `./main 8 1` | `compute-sanitizer ./main 8 1` |
| perplexity | `src/perplexity-cuda` | `./main 10000 50 100` | `./main 10000 50 1` | `compute-sanitizer ./main 10000 50 1` |
| pointwise | `src/pointwise-cuda` | `./main 100 8 512 64 10` | `./main 100 8 512 64 1` | `compute-sanitizer ./main 100 8 512 64 1` |
| pool | `src/pool-cuda` | `./main 128 48 224 224 54 54 100` | `./main 128 48 224 224 54 54 1` | `compute-sanitizer ./main 128 48 224 224 54 54 1` |
| qkv | `src/qkv-cuda` | `./main 1` | `./main 1` | `compute-sanitizer ./main 1` |
| qtclustering | `src/qtclustering-cuda` | `./main --Verbose` | `./main --Verbose` | `compute-sanitizer ./main --Verbose` |
| remap | `src/remap-cuda` | `./main 10000000 1000` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| relu | `src/relu-cuda` | `./main 10000000 100000` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| resnet-kernels | `src/resnet-kernels-cuda` | `./main 0 100` | `./main 0 1` | `compute-sanitizer ./main 0 1` |
| rmsnorm | `src/rmsnorm-cuda` | `./main 1 12288 1000` | `./main 1 12288 1` | `compute-sanitizer ./main 1 12288 1` |
| rowwiseMoments | `src/rowwiseMoments-cuda` | `./main 128 128 128 128 4 100` | `./main 128 128 128 128 4 1` | `compute-sanitizer ./main 128 128 128 128 4 1` |
| rotary | `src/rotary-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| sampling | `src/sampling-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| scel | `src/scel-cuda` | `./main 2048 50176 100` | `./main 2048 50176 1` | `compute-sanitizer ./main 2048 50176 1` |
| silu | `src/silu-cuda` | `./main 32 32 256 256 100` | `./main 32 32 256 256 1` | `compute-sanitizer ./main 32 32 256 256 1` |
| softmax | `src/softmax-cuda` | `./main 100000 784 0 100` | `./main 100000 784 0 1` | `compute-sanitizer ./main 100000 784 0 1` |
| softmax-fused | `src/softmax-fused-cuda` | `./main 4 12 256 256 1000` | `./main 4 12 256 256 1` | `compute-sanitizer ./main 4 12 256 256 1` |
| softmax-online | `src/softmax-online-cuda` | `./main 1` | `./main 1` | `compute-sanitizer ./main 1` |
| ssm | `src/ssm-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| stddev | `src/stddev-cuda` | `./main 65536 16384 100` | `./main 65536 16384 1` | `compute-sanitizer ./main 65536 16384 1` |
| streamcluster | `src/streamcluster-cuda` | `./streamcluster 10 20 256 65536 65536 1000 none output.txt 1` | `./streamcluster 10 20 256 65536 65536 1 none output.txt 1` | `compute-sanitizer ./streamcluster 10 20 256 65536 65536 1 none output.txt 1` |
| tsne | `src/tsne-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| unfold | `src/unfold-cuda` | `./main 10000000 1000` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| vol2col | `src/vol2col-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| wedford | `src/wedford-cuda` | `./main 512 512 8192 100` | `./main 512 512 8192 1` | `compute-sanitizer ./main 512 512 8192 1` |
| winograd | `src/winograd-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |

---

## Math

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| atan2 | `src/atan2-cuda` | `./main 10000000 100` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| axpby | `src/axpby-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| bgmv | `src/bgmv-cuda` | `./main --op bgmv_shrink --tokens 1 --hidden 4096 --rank 16 --loras 4 --repeat 200` | `./main --op bgmv_shrink --tokens 1 --hidden 4096 --rank 16 --loras 4 --repeat 1` | `compute-sanitizer ./main --op bgmv_shrink --tokens 1 --hidden 4096 --rank 16 --loras 4 --repeat 1` |
| blas-dot | `src/blas-dot-cuda` | `./main 268435456 1000` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| blas-fp8gemm | `src/blas-fp8gemm-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| blas-gemm | `src/blas-gemm-cuda` | `./main 79 91 83 1000` | `./main 79 91 83 1` | `compute-sanitizer ./main 79 91 83 1` |
| blas-gemmBatched | `src/blas-gemmBatched-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| blas-gemmStridedBatched | `src/blas-gemmStridedBatched-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| blas-gemmEx | `src/blas-gemmEx-cuda` | `./main 4096 4096 4096 100` | `./main 4096 4096 4096 1` | `compute-sanitizer ./main 4096 4096 4096 1` |
| blas-gemmEx2 | `src/blas-gemmEx2-cuda` | `./main 4096 4096 4096 100` | `./main 4096 4096 4096 1` | `compute-sanitizer ./main 4096 4096 4096 1` |
| braycurtis | `src/braycurtis-cuda` | `python tests/main.py` | `python tests/main.py` | `compute-sanitizer python tests/main.py` |
| complex | `src/complex-cuda` | `./main 10000000 1000` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| cross | `src/cross-cuda` | `./main 10000000 100` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |
| determinant | `src/determinant-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| dp | `src/dp-cuda` | `./main 268435456 1000` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| eigenvalue | `src/eigenvalue-cuda` | `./main 2048 10000` | `./main 2048 1` | `compute-sanitizer ./main 2048 1` |
| f16max | `src/f16max-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| f16sp | `src/f16sp-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| f8cast | `src/f8cast-cuda` | `./main 268435456 100` | `./main 268435456 1` | `compute-sanitizer ./main 268435456 1` |
| fresnel | `src/fresnel-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| fwt | `src/fwt-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| gaussian | `src/gaussian-cuda` | `./main -q -t -s 4096` | `./main -q -t -s 4096` | `compute-sanitizer ./main -q -t -s 4096` |
| geam | `src/geam-cuda` | `./main 16384 16384 200` | `./main 16384 16384 1` | `compute-sanitizer ./main 16384 16384 1` |
| gels | `src/gels-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| gemv | `src/gemv-cuda` | `./main -s 16384 -i 100 -x 512 -y 2` | `./main -s 16384 -i 1 -x 512 -y 2` | `compute-sanitizer ./main -s 16384 -i 1 -x 512 -y 2` |
| hadamard | `src/hadamard-cuda` | `./main 1024 100` | `./main 1024 1` | `compute-sanitizer ./main 1024 1` |
| hellinger | `src/hellinger-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| hmm | `src/hmm-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| idivide | `src/idivide-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| interval | `src/interval-cuda` | `./main 0 100000` | `./main 0 1` | `compute-sanitizer ./main 0 1` |
| jaccard | `src/jaccard-cuda` | `./main 1024 512 1000` | `./main 1024 512 1` | `compute-sanitizer ./main 1024 512 1` |
| jacobi | `src/jacobi-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| kurtosis | `src/kurtosis-cuda` | `./main 100000000 100` | `./main 100000000 1` | `compute-sanitizer ./main 100000000 1` |
| langford | `src/langford-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| lci | `src/lci-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| lebesgue | `src/lebesgue-cuda` | `./main 1000000 2` | `./main 1000000 2` | `compute-sanitizer ./main 1000000 2` |
| lfib4 | `src/lfib4-cuda` | `./main 2000000000` | `./main 2000000000` | `compute-sanitizer ./main 2000000000` |
| lud | `src/lud-cuda` | `./main -s 8192` | `./main -s 8192` | `compute-sanitizer ./main -s 8192` |
| ludb | `src/ludb-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| lut-gemm | `src/lut-gemm-cuda` | `./main 1 100` | `./main 1 1` | `compute-sanitizer ./main 1 1` |
| michalewicz | `src/michalewicz-cuda` | `./main 100000000 100` | `./main 100000000 1` | `compute-sanitizer ./main 100000000 1` |
| matrix-rotate | `src/matrix-rotate-cuda` | `./main 5000 100` | `./main 5000 1` | `compute-sanitizer ./main 5000 1` |
| matrixT | `src/matrixT-cuda` | `./main 16384 16384 200` | `./main 16384 16384 1` | `compute-sanitizer ./main 16384 16384 1` |
| minkowski | `src/minkowski-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| mr | `src/mr-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| mrg32k3a | `src/mrg32k3a-cuda` | `./main 1000000 10` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |
| norm2 | `src/norm2-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| nqueen | `src/nqueen-cuda` | `./main 15 7 100` | `./main 15 7 1` | `compute-sanitizer ./main 15 7 1` |
| ntt | `src/ntt-cuda` | `./main 100000` | `./main 100000` | `compute-sanitizer ./main 100000` |
| phmm | `src/phmm-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| pnpoly | `src/pnpoly-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| quant3MatMul | `src/quant3MatMul-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| reverse2D | `src/reverse2D-cuda` | `./main 8192 8192 100` | `./main 8192 8192 1` | `compute-sanitizer ./main 8192 8192 1` |
| rfs | `src/rfs-cuda` | `./main 100 5000000` | `./main 1 5000000` | `compute-sanitizer ./main 1 5000000` |
| romberg | `src/romberg-cuda` | `./main 128 64 1000` | `./main 128 64 1` | `compute-sanitizer ./main 128 64 1` |
| sddmm-batch | `src/sddmm-batch-cuda` | `./main 32 1024 1024 1024 1024 1000 1` | `./main 32 1024 1024 1024 1024 1 1` | `compute-sanitizer ./main 32 1024 1024 1024 1024 1 1` |
| secp256k1 | `src/secp256k1-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| simpleSpmv | `src/simpleSpmv-cuda` | `./main 16777216 10240 100` | `./main 16777216 10240 1` | `compute-sanitizer ./main 16777216 10240 1` |
| spd2s | `src/spd2s-cuda` | `./main 20082 20082 150616 1000` | `./main 20082 20082 150616 1` | `compute-sanitizer ./main 20082 20082 150616 1` |
| spgeam | `src/spgeam-cuda` | `./main 20082 20082 150616 1000 1` | `./main 20082 20082 150616 1 1` | `compute-sanitizer ./main 20082 20082 150616 1 1` |
| spgemm | `src/spgemm-cuda` | `./main 1024 1024 1024 1024 1000 1` | `./main 1024 1024 1024 1024 1 1` | `compute-sanitizer ./main 1024 1024 1024 1024 1 1` |
| spmm | `src/spmm-cuda` | `./main 1024 1024 1024 1024 1024 1000 1` | `./main 1024 1024 1024 1024 1024 1 1` | `compute-sanitizer ./main 1024 1024 1024 1024 1024 1 1` |
| spmv | `src/spmv-cuda` | `./main 16777216 10240 100` | `./main 16777216 10240 1` | `compute-sanitizer ./main 16777216 10240 1` |
| spnnz | `src/spnnz-cuda` | `./main 20082 20082 150616 1000` | `./main 20082 20082 150616 1` | `compute-sanitizer ./main 20082 20082 150616 1` |
| sps2d | `src/sps2d-cuda` | `./main 20082 20082 150616 1000` | `./main 20082 20082 150616 1` | `compute-sanitizer ./main 20082 20082 150616 1` |
| spsort | `src/spsort-cuda` | `./main 20082 20082 150616 1000` | `./main 20082 20082 150616 1` | `compute-sanitizer ./main 20082 20082 150616 1` |
| thomas | `src/thomas-cuda` | `./main 1024 16384 64 100` | `./main 1024 16384 64 1` | `compute-sanitizer ./main 1024 16384 64 1` |
| wyllie | `src/wyllie-cuda` | `./main 8000000 1 100` | `./main 8000000 1 1` | `compute-sanitizer ./main 8000000 1 1` |
| zeropoint | `src/zeropoint-cuda` | `./main 10000000 100` | `./main 10000000 1` | `compute-sanitizer ./main 10000000 1` |

---

## Random number generation

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| mt | `src/mt-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| qrg | `src/qrg-cuda` | `./main 100000` | `./main 100000` | `compute-sanitizer ./main 100000` |
| rng-wallace | `src/rng-wallace-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| sobol | `src/sobol-cuda` | `./main 1000000 1000 100` | `./main 1000000 1000 1` | `compute-sanitizer ./main 1000000 1000 1` |

---

## Search

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| bsearch | `src/bsearch-cuda` | `./main 8388608 100` | `./main 8388608 1` | `compute-sanitizer ./main 8388608 1` |
| keogh | `src/keogh-cuda` | `./main 256 20000000 100` | `./main 256 20000000 1` | `compute-sanitizer ./main 256 20000000 1` |
| s8n | `src/s8n-cuda` | `./main 1513 2048 100` | `./main 1513 2048 1` | `compute-sanitizer ./main 1513 2048 1` |

---

## Signal processing

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| extrema | `src/extrema-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| fft | `src/fft-cuda` | `./main 3 100` | `./main 3 1` | `compute-sanitizer ./main 3 1` |
| lombscargle | `src/lombscargle-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| sosfil | `src/sosfil-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| zmddft | `src/zmddft-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |

---

## Simulation

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| ace | `src/ace-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| adv | `src/adv-cuda` | `./main 7 15 8000 100` | `./main 7 15 8000 1` | `compute-sanitizer ./main 7 15 8000 1` |
| amgmk | `src/amgmk-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| axhelm | `src/axhelm-cuda` | `./main 1 8000 100` | `./main 1 8000 1` | `compute-sanitizer ./main 1 8000 1` |
| bh | `src/bh-cuda` | `./main 100000 100` | `./main 100000 1` | `compute-sanitizer ./main 100000 1` |
| bspline-vgh | `src/bspline-vgh-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| burger | `src/burger-cuda` | `./main 8200 8100 100` | `./main 8200 8100 1` | `compute-sanitizer ./main 8200 8100 1` |
| cooling | `src/cooling-cuda` | `./main 1000000 1000` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |
| ccsd-trpdrv | `src/ccsd-trpdrv-cuda` | `./main 160 400 100` | `./main 160 400 1` | `compute-sanitizer ./main 160 400 1` |
| che | `src/che-cuda` | `./main 1000` | `./main 1` | `rm -rf out && mkdir -p out && compute-sanitizer ./main 1` |
| chemv | `src/chemv-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| chi2 | `src/chi2-cuda` | `./main 4000 400000 2000 2000 256 1000` | `./main 4000 400000 2000 2000 256 1` | `compute-sanitizer ./main 4000 400000 2000 2000 256 1` |
| clenergy | `src/clenergy-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| cobahh | `src/cobahh-cuda` | `./main 1000000 10` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |
| d3q19_bgk | `src/d3q19-bgk-cuda` | `./main 128` | `./main 128` | `compute-sanitizer ./main 128` |
| damage | `src/damage-cuda` | `./main 50000000 1000` | `./main 50000000 1` | `compute-sanitizer ./main 50000000 1` |
| ddbp | `src/ddbp-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| dslash | `src/dslash-cuda` | `./main 256` | `./main 256` | `compute-sanitizer ./main 256` |
| eikonal | `src/eikonal-cuda` | `./main -s 512` | `./main -s 512` | `compute-sanitizer ./main -s 512` |
| fdtd3d | `src/fdtd3d-cuda` | `./main --dimx=192 --dimy=184 --timesteps=90` | `./main --dimx=192 --dimy=184 --timesteps=1` | `compute-sanitizer ./main --dimx=192 --dimy=184 --timesteps=1` |
| feynman-kac | `src/feynman-kac-cuda` | `./main 10` | `./main 1` | `compute-sanitizer ./main 1` |
| fhd | `src/fhd-cuda` | `./main 100000 2097152 0` | `./main 100000 2097152 0` | `compute-sanitizer ./main 100000 2097152 0` |
| fluidSim | `src/fluidSim-cuda` | `./main 10000` | `./main 1` | `compute-sanitizer ./main 1` |
| goulash | `src/goulash-cuda` | `./main 101 5` | `./main 101 5` | `compute-sanitizer ./main 101 5` |
| gpp | `src/gpp-cuda` | `./main benchmark` | `./main benchmark` | `compute-sanitizer ./main benchmark` |
| grrt | `src/grrt-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| haccmk | `src/haccmk-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| halo-finder | `src/halo-finder-cuda` | `./cuda/ForceTreeTest 0.5 0.1 10000 0.1 10 N 12 rcb` | `./cuda/ForceTreeTest 0.5 0.1 1000 0.1 10 N 12 rcb` | `compute-sanitizer ./cuda/ForceTreeTest 0.5 0.1 1000 0.1 10 N 12 rcb` |
| heartwall | `src/heartwall-cuda` | `./main 104` | `./main 1` | `compute-sanitizer ./main 1` |
| heat | `src/heat-cuda` | `./main 4096 1000` | `./main 4096 1` | `compute-sanitizer ./main 4096 1` |
| heat2d | `src/heat2d-cuda` | `./main 4096 4096 1000` | `./main 4096 4096 1` | `compute-sanitizer ./main 4096 4096 1` |
| hexicton | `src/hexciton-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| hwt1d | `src/hwt1d-cuda` | `./main 8388608 100` | `./main 8388608 1` | `compute-sanitizer ./main 8388608 1` |
| hypterm | `src/hypterm-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| ising | `src/ising-cuda` | `./main -x 5120 -y 5120 -w 10 -n 1000` | `./main -x 5120 -y 5120 -w 10 -n 1` | `compute-sanitizer ./main -x 5120 -y 5120 -w 10 -n 1` |
| iso2dfd | `src/iso2dfd-cuda` | `./main 2048 2048 1000` | `./main 2048 2048 1` | `compute-sanitizer ./main 2048 2048 1` |
| laplace | `src/laplace-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| laplace3d | `src/laplace3d-cuda` | `./main 128 128 128 100 1` | `./main 128 128 128 1 1` | `compute-sanitizer ./main 128 128 128 1 1` |
| lavaMD | `src/lavaMD-cuda` | `./main -boxes1d 30` | `./main -boxes1d 30` | `compute-sanitizer ./main -boxes1d 30` |
| lid-driven-cavity | `src/lid-driven-cavity-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| loopback | `src/loopback-cuda` | `./main 0 100` | `./main 0 1` | `compute-sanitizer ./main 0 1` |
| lulesh | `src/lulesh-cuda` | `./main -i 100 -s 128 -r 11 -b 1 -c 1` | `./main -i 1 -s 128 -r 11 -b 1 -c 1` | `compute-sanitizer ./main -i 1 -s 128 -r 11 -b 1 -c 1` |
| md | `src/md-cuda` | `./main 3 1000` | `./main 3 1` | `compute-sanitizer ./main 3 1` |
| mdh | `src/mdh-cuda` | `./main -itmax 100` | `./main -itmax 1` | `compute-sanitizer ./main -itmax 1` |
| metropolis | `src/metropolis-cuda` | `./main -l 32 11 -t 4.7 0.1 -a 10 2 2000 10 -h 1.0 -z 7919` | `./main -l 32 11 -t 4.7 0.1 -a 10 2 2000 1 -h 1.0 -z 7919` | `compute-sanitizer ./main -l 32 11 -t 4.7 0.1 -a 10 2 2000 1 -h 1.0 -z 7919` |
| miniFE | `src/miniFE-cuda` | `./main -nx 128 -ny 128 -nz 128` | `./main -nx 128 -ny 128 -nz 128` | `compute-sanitizer ./main -nx 128 -ny 128 -nz 128` |
| minimod | `src/minimod-cuda` | `./main --warm-up --niters 1 --grid 256 --nsteps 1000` | `./main --warm-up --niters 1 --grid 256 --nsteps 1` | `compute-sanitizer ./main --warm-up --niters 1 --grid 256 --nsteps 1` |
| minisweep | `src/minisweep-cuda` | `./main --niterations 100` | `./main --niterations 1` | `compute-sanitizer ./main --niterations 1` |
| miniWeather | `src/miniWeather-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| multimaterial | `src/multimaterial-cuda` | `./multimat_FL 2000 2000` | `./multimat_FL 2000 2000` | `compute-sanitizer ./multimat_FL 2000 2000` |
| mxfp4 | `src/mxfp4-cuda` | `./main 536870912 100` | `./main 536870912 1` | `compute-sanitizer ./main 536870912 1` |
| myocte | `src/myocyte-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| nbody | `src/nbody-cuda` | `./main 16000 10` | `./main 16000 1` | `compute-sanitizer ./main 16000 1` |
| particle-diffusion | `src/particle-diffusion-cuda` | `./main 2000 100` | `./main 2000 1` | `compute-sanitizer ./main 2000 1` |
| particlefilter | `src/particlefilter-cuda` | `./main -x 128 -y 128 -z 10 -np 400000` | `./main -x 128 -y 128 -z 10 -np 400000` | `compute-sanitizer ./main -x 128 -y 128 -z 10 -np 400000` |
| particles | `src/particles-cuda` | `./main 10000` | `./main 1` | `compute-sanitizer ./main 1` |
| pathfinder | `src/pathfinder-cuda` | `./main 100000 1000 5` | `./main 100000 1000 5` | `compute-sanitizer ./main 100000 1000 5` |
| pns | `src/pns-cuda` | `./main 22 100000 1000000` | `./main 22 100000 1000000` | `compute-sanitizer ./main 22 100000 1000000` |
| projectile | `src/projectile-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| pso | `src/pso-cuda` | `./main 30 10000` | `./main 30 1` | `compute-sanitizer ./main 30 1` |
| qem | `src/qem-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| rainflow | `src/rainflow-cuda` | `./main 100000 100` | `./main 100000 1` | `compute-sanitizer ./main 100000 1` |
| rayleighBenardConvection | `src/rayleighBenardConvection-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| reaction | `src/reaction-cuda` | `./main 10000` | `./main 1` | `compute-sanitizer ./main 1` |
| rsbench | `src/rsbench-cuda` | `./main -s large -m event` | `./main -s large -m event` | `compute-sanitizer ./main -s large -m event` |
| rtm8 | `src/rtm8-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| rushlarsen | `src/rushlarsen-cuda` | `./main 100 100000` | `./main 1 100000` | `compute-sanitizer ./main 1 100000` |
| s3d | `src/s3d-cuda` | `./main -q -n 100 -s 1` | `./main -q -n 1 -s 1` | `compute-sanitizer ./main -q -n 1 -s 1` |
| su3 | `src/su3-cuda` | `./main -i 1000 -l 32 -t 128 -v 3 -w 1` | `./main -i 1 -l 32 -t 128 -v 3 -w 1` | `compute-sanitizer ./main -i 1 -l 32 -t 128 -v 3 -w 1` |
| sheath | `src/sheath-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| simplemoc | `src/simplemoc-cuda` | `./main -s 5000000 -e 128 -n 10` | `./main -s 5000000 -e 128 -n 1` | `compute-sanitizer ./main -s 5000000 -e 128 -n 1` |
| slit | `src/slit-cuda` | `./main 1024 1000` | `./main 1024 1` | `compute-sanitizer ./main 1024 1` |
| sparkler | `src/sparkler-cuda` | `mpirun -n 4 ./main --num_vector 4000 --num_field 90000 --num_iterations 10` | `./main --num_vector 4000 --num_field 90000 --num_iterations 1` | `mpirun -n 2 compute-sanitizer ./main --num_vector 4000 --num_field 90000 --num_iterations 1` |
| sph | `src/sph-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| tensorT | `src/tensorT-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| testSNAP | `src/testSNAP-cuda` | `./main --nsteps 100` | `./main --nsteps 1` | `compute-sanitizer ./main --nsteps 1` |
| tissue | `src/tissue-cuda` | `./main 32 100` | `./main 32 1` | `compute-sanitizer ./main 32 1` |
| tridiagonal | `src/tridiagonal-cuda` | `./main -num_systems=524288` | `./main -num_systems=524288` | `compute-sanitizer ./main -num_systems=524288` |
| tsa | `src/tsa-cuda` | `./main 1024 1024 100` | `./main 1024 1024 1` | `compute-sanitizer ./main 1024 1024 1` |
| vanGenuchten | `src/vanGenuchten-cuda` | `./main 256 256 256 1000` | `./main 256 256 256 1` | `compute-sanitizer ./main 256 256 256 1` |
| vmc | `src/vmc-cuda` | `./main 100` | `./main 1` | `compute-sanitizer ./main 1` |
| wlcpow | `src/wlcpow-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| wsm5 | `src/wsm5-cuda` | `./main 10` | `./main 1` | `compute-sanitizer ./main 1` |
| xlqc | `src/xlqc-cuda` | `./main sp` | `./main sp` | `compute-sanitizer ./main sp` |
| xsbench | `src/xsbench-cuda` | `./main -s large -m event -r 10` | `./main -s large -m event -r 1` | `compute-sanitizer ./main -s large -m event -r 1` |

---

## Sorting

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| bitonic-sort | `src/bitonic-sort-cuda` | `./main 25 2` | `./main 25 1` | `compute-sanitizer ./main 25 1` |
| hybridsort | `src/hybridsort-cuda` | `./main r` | `./main r` | `compute-sanitizer ./main r` |
| is | `src/is-cuda` | `./main 256 256 256` | `./main 256 256 256` | `compute-sanitizer ./main 256 256 256` |
| merge | `src/merge-cuda` | `./main 100000 100` | `./main 100000 1` | `compute-sanitizer ./main 100000 1` |
| quicksort | `src/quicksort-cuda` | `./main 10 2048 2048` | `./main 1 2048 2048` | `compute-sanitizer ./main 1 2048 2048` |
| radixsort | `src/radixsort-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |
| segsort | `src/segsort-cuda` | `./main` | `./main` | `compute-sanitizer ./main` |
| sort | `src/sort-cuda` | `./main 3 100` | `./main 3 1` | `compute-sanitizer ./main 3 1` |
| sortKV | `src/sortKV-cuda` | `./main 1000000 100` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |
| split | `src/split-cuda` | `./main 33554432 1000` | `./main 33554432 1` | `compute-sanitizer ./main 33554432 1` |
| topk | `src/topk-cuda` | `./main 3072 1000` | `./main 3072 1` | `compute-sanitizer ./main 3072 1` |
| warpsort | `src/warpsort-cuda` | `./main 1000` | `./main 1` | `compute-sanitizer ./main 1` |

---

## Robotics

| Project | Directory | `make run` Command | Essential Command | Compute Sanitizer Command |
|---|---|---|---|---|
| rodrigues | `src/rodrigues-cuda` | `./main 1000000 10000` | `./main 1000000 1` | `compute-sanitizer ./main 1000000 1` |

---
