# HeCBench Sanitizer Results

Analysis of compute-sanitizer reports (memcheck + racecheck) across HeCBench CUDA benchmarks.

**Scope:** 157 benchmarks across **18 categories** (61 from earlier batches + 81 from Simulation + 5 from Math + 10 from Machine learning)  
**Date analyzed:** 2026-06-25  
**Latest batch:** Machine learning (10 benchmarks, partial — 15 of 20 memcheck + racecheck runs)

## Executive Summary

Across all reports, **2 confirmed real memory bugs** and **1 confirmed real data race** were found.

### Real bugs (confirmed)

| Benchmark | Sanitizer | Bug Type | Severity |
|-----------|-----------|----------|----------|
| **`histogram`** | memcheck | Out-of-bounds global write | **Real bug** |
| **`perlin`** | memcheck | Out-of-bounds global read | **Real bug** |
| **`tridiagonal`** | racecheck | Missing `__syncthreads()` — shared-memory race | **Real bug** |

### Latest batch — Machine learning: no new bugs

The most recent sanitizer run covers **10 benchmarks** in the Machine learning category (`addBiasQKV`, `addBiasResidualLayerNorm`, `attentionMultiHeadKVCache`, `bn`, `channelShuffle`, `clink`, `concat`, `dense-embedding`, `matern`, `overlay`). **No memory bugs or data races were found.**

| Latest-batch result | Count | Details |
|---------------------|------:|---------|
| Clean (PASS, completed runs) | 5 | `addBiasQKV` (racecheck), `attentionMultiHeadKVCache` (both tools), `bn` (both tools), `channelShuffle` (racecheck), `concat` (racecheck) — all completed runs report `0 errors` / `0 hazards` |
| Incomplete, no summary (not a code bug) | 1 | `addBiasResidualLayerNorm` (racecheck) — killed mid-sweep over hidden dimensions; no `RACECHECK SUMMARY` line; all configurations completed so far report PASS |
| Timeout (300s), no findings | 2 | `matern` (racecheck), `overlay` (memcheck + racecheck) — confirmed `ERROR: timed out after 300s`, no sanitizer findings before the limit |
| Sanitizer error (not a code bug) | 2 | `clink` (both tools) — missing `input.hpp` input file; `dense-embedding` (both tools) — harness passes `nrows=256, batch_size=256` but app asserts `nrows > batch_size * batch_size` and exits before any kernel launches |

Notable: `bn` previously timed out at 600s under sanitizer in an ad-hoc run; it now completes clean on both tools in this batch.

The `SANITIZER_ERROR` results in the Machine learning batch are harness/environment issues, not GPU memory-safety defects:

| Benchmark | Cause |
|-----------|-------|
| `clink` | Harness runs `./main 1`; app cannot open `input.hpp` (full run uses `./main 10`) |
| `dense-embedding` | Harness passes `./main 256 256 1`; assertion `nrows > batch_size * batch_size` fails (`256 > 65536`) before any CUDA kernels execute |

### Previous batch — Math: no new bugs

The Math-category run covered **5 benchmarks** (`blas-fp8gemm`, `blas-gemmBatched`, `blas-gemmStridedBatched`, `f16max`, `f16sp`). **No memory bugs or data races were found.**

| Math-batch result | Count | Details |
|---------------------|------:|---------|
| Incomplete, no summary (not a code bug) | 4 | `blas-gemmBatched` (both tools), `blas-gemmStridedBatched` (both tools) — killed mid-sweep over GEMM sizes; no `ERROR: timed out` marker and no `ERROR SUMMARY`/`RACECHECK SUMMARY` line, so the run ended before either completing or hitting the script's 300s timeout handler; no sanitizer findings in the partial output that does exist |
| Timeout (300s), no findings | 2 | `f16max` (racecheck), `f16sp` (memcheck) — confirmed `ERROR: timed out after 300s`, no output at all before the limit |
| App crash, 0 errors/0 hazards (not a code bug) | 2 | `blas-fp8gemm` (memcheck + racecheck) — cuBLAS rejects the FP8 `cublasLtMatmul` config; this GPU is an A30 (sm_80), and FP8 GEMM via cuBLASLt requires Hopper (sm_90+) |

The `blas-gemmBatched`/`blas-gemmStridedBatched` logs have no `f16max`/`f16sp`-style timeout marker, so the run was likely interrupted externally (e.g. session/process killed) rather than hitting the harness's own timeout — either way, no memory errors or hazards were reported in the output captured so far.

### Previous batch — Simulation: 1 new real bug (data race)

The Simulation-category run added **81 benchmarks** (162 memcheck + racecheck runs). **`tridiagonal`'s racecheck run found a genuine data race** (see [Real Bugs](#3-tridiagonal--missing-__syncthreads-shared-memory-race) below) — the first race condition confirmed across all batches to date. No other new real memory bugs or races were found.

| Simulation-batch result | Count | Details |
|---------------------|------:|---------|
| Clean (PASS, both tools) | 55 | All completed runs report `0 errors` / `0 hazards` |
| Mixed PASS / TIMEOUT (clean so far) | 8 | `eikonal`, `grrt`, `lavaMD`, `loopback`, `mdh`, `sheath`, `sph` — one tool passed clean, the other timed out with no findings before being killed |
| **Real race found before timeout** | 1 | `tridiagonal` racecheck — see below |
| Timeout (300s), no findings | 10 | `bspline-vgh`, `d3q19_bgk`, `ddbp`, `goulash`, `gpp`, `hexicton`, `laplace`, `lid-driven-cavity`, `metropolis`, `rayleighBenardConvection` (both tools) |
| Sanitizer error (not a code bug) | 5 | `fdtd3d`, `heartwall`, `miniFE`, `nbody`, `xlqc` (both tools) — harness args / missing input files / stale build, not GPU memory-safety defects |
| Build failed (not a code bug) | 3 | `axhelm`, `miniWeather`, `testSNAP` (both tools) — missing build dependency, missing `mpi.h`, missing generated header |

The `SANITIZER_ERROR` results in the Simulation batch are all environment/harness issues, not GPU memory-safety defects:

| Benchmark | Cause |
|-----------|-------|
| `fdtd3d` | Harness passes `--dimx=64 --dimy=64`; app requires `[96, 376]` and exits before any kernel launches |
| `heartwall` | Missing AVI input file (`AVI_open_input_file` fails) |
| `miniFE` | No `main` binary present (stale/incomplete build); compute-sanitizer reports "target application doesn't exist" |
| `nbody` | Harness passes 1 integration step; app requires at least 3 and exits early |
| `xlqc` | Missing `example/geom.xyz` input file |

The `BUILD_FAILED` results are likewise environment gaps, not code bugs: `axhelm` (missing `BlasLapack` subdirectory dependency), `miniWeather` (missing `mpi.h` — MPI not installed), `testSNAP` (missing generated header `refdata_2J14_W.h`).

### Earlier batches — no new real bugs

The batch before this one added **25 benchmarks** (50 memcheck + racecheck runs) across Sorting, Signal processing, Random number generation, Search, and Robotics. **No additional real memory bugs or data races were found.**

| Earlier-batch result | Count | Details |
|---------------------|------:|---------|
| Clean (PASS) | 41 | All completed runs report `0 errors` / `0 hazards` |
| Timeout (300s) | 7 | Slow under sanitizer; no memory findings in logs |
| Sanitizer error (not a code bug) | 2 | `mt` exits before CUDA instrumentation (missing data file) |

The only `SANITIZER_ERROR` results in that batch are **`mt`** (memcheck + racecheck), caused by a missing `./data/MersenneTwister.dat` input file — not a GPU memory-safety defect.

---

## Real Bugs

### 1. `histogram` — Out-of-Bounds Global Write

| Field | Value |
|-------|-------|
| **Sanitizer** | memcheck |
| **Type** | Invalid `__global__` write (4 bytes) |
| **Buggy line (sanitizer)** | [`src/histogram-cuda/histogram_gmem_atomics.h:95`](../src/histogram-cuda/histogram_gmem_atomics.h#L95) — `out[i] = total` |
| **Root-cause line** | [`src/histogram-cuda/histogram_gmem_atomics.h:88`](../src/histogram-cuda/histogram_gmem_atomics.h#L88) — off-by-one bounds check (`>` should be `>=`) |
| **Sanitizer report** | [memcheck.log](Data_compression_and_reduction/histogram/memcheck.log) |
| **Stdout log** | [memcheck.stdout.log](Data_compression_and_reduction/histogram/memcheck.stdout.log) |

**Root cause:** Off-by-one bounds check. The guard `if (i > ACTIVE_CHANNELS * NUM_BINS)` allows `i == ACTIVE_CHANNELS * NUM_BINS` (256 when `ACTIVE_CHANNELS=1`, `NUM_BINS=256`), but valid indices are `[0, 255]`. The write lands 1 byte past a 1024-byte allocation (256 `uint32_t` elements). Thread `(0,0,0)` in block `(2,0,0)`.

**Corroborating evidence:** Application stdout reports incorrect results:

```
Global memory atomics INCORRECT: [0]: 4133502928 != 3  FAIL
```

**Reproduce:**

```bash
cd src/histogram-cuda
make ARCH=sm_80 EXTRA_CFLAGS=-lineinfo    # set ARCH to match your GPU
compute-sanitizer --tool memcheck \
  --log-file memcheck.log \
  ./main --i=1
```

Or via the HeCBench harness:

```bash
python3 scripts/run_sanitizer.py --project histogram --tool memcheck
```

---

### 2. `perlin` — Out-of-Bounds Global Read

| Field | Value |
|-------|-------|
| **Sanitizer** | memcheck |
| **Type** | Invalid `__global__` read (4 bytes), repeated across many threads (160 errors reported, 60 truncated) |
| **Buggy line (sanitizer)** | [`src/perlin-cuda/noise.cu:84`](../src/perlin-cuda/noise.cu#L84) — `gradientY[iBL]` |
| **Root-cause lines** | [`src/perlin-cuda/noise.cu:78-81`](../src/perlin-cuda/noise.cu#L78-L81) — unmasked `_hash[h0 + iy0]` indexing |
| **Sanitizer report** | [memcheck.log](Computer_vision_and_image_processing/perlin/memcheck.log) |
| **Stdout log** | [memcheck.stdout.log](Computer_vision_and_image_processing/perlin/memcheck.stdout.log) |

**Root cause:** Unmasked `_hash` table indexing. Expressions like `_hash[h0 + iy0]` can index beyond the 256-element `_hash` array because `h0`/`h1` and `iy0`/`iy1` are each in `[0, 255]`, so `h0 + iy0` can reach 510. Corrupted indices then cause out-of-bounds reads from the 8-element `gradientX`/`gradientY` arrays (`N_GRADIENTS = 8`, 32 bytes each). Call chain: `perlin` → `sumOctaves` → `noiseAt`.

**Corroborating evidence:** Application stdout reports kernel launch failure:

```
CUDA error at noise.cu:164: unspecified launch failure
```

**Reproduce:**

```bash
cd src/perlin-cuda
make ARCH=sm_80 EXTRA_CFLAGS=-lineinfo    # set ARCH to match your GPU
compute-sanitizer --tool memcheck \
  --log-file memcheck.log \
  ./main
```

Or via the HeCBench harness:

```bash
python3 scripts/run_sanitizer.py --project perlin --tool memcheck
```

---

### 3. `tridiagonal` — Missing `__syncthreads()` (Shared-Memory Race)

| Field | Value |
|-------|-------|
| **Sanitizer** | racecheck |
| **Type** | Write-after-read / read-after-write hazard on `__shared__` memory, 16384 hazards per pair, 6 distinct write/read pairs, repeated every kernel launch |
| **Kernel** | [`src/tridiagonal-cuda/cyclic_kernels.cu`](../src/tridiagonal-cuda/cyclic_kernels.cu) — `cyclic_small_systems_kernel` |
| **Buggy lines (sanitizer)** | Writes at [lines 81–98](../src/tridiagonal-cuda/cyclic_kernels.cu#L81-L98) (last iteration of forward elimination) race with reads at [lines 108 and 113](../src/tridiagonal-cuda/cyclic_kernels.cu#L108-L113) (final 2x2 system solve) |
| **Root-cause line** | [`cyclic_kernels.cu:102-104`](../src/tridiagonal-cuda/cyclic_kernels.cu#L102-L104) — no `__syncthreads()` between the forward-elimination loop and the `if (thid < 2)` block |
| **Sanitizer report** | [racecheck.log](Simulation/tridiagonal/racecheck.log) |
| **Stdout log** | [racecheck.stdout.log](Simulation/tridiagonal/racecheck.stdout.log) |

**Root cause:** The forward-elimination loop (lines 64–102) calls `__syncthreads()` only at the *top* of each iteration (line 66). After the loop's final iteration, threads write `b[i]`, `d[i]`, `a[i]`, `c[i]` (lines 81–98) and the loop exits — with no synchronization barrier before the very next statement, `if (thid < 2) { ... }` (lines 104–116), which reads `b[addr1]`, `b[addr2]`, `c[addr1]`, `a[addr2]`, `d[addr1]`, `d[addr2]` to solve the final 2x2 system. Threads 0/1 can read stale or in-flight shared-memory values written by other threads in the same block, since nothing guarantees those writes have completed and are visible block-wide. The fix is to add a `__syncthreads()` immediately after the forward-elimination loop (after line 102) and before line 104.

**Corroborating evidence:** Application stdout reports a large residual error for both kernel variants that use `cyclic_small_systems_kernel`, consistent with a wrong result from the race:

```
Tridiagonal-cyclicsmall-base, ... err = 1.1549
Tridiagonal-cyclicsmall-optimized, ... err = 1.1549
```

**Reproduce:**

```bash
cd src/tridiagonal-cuda
make ARCH=sm_80 EXTRA_CFLAGS=-lineinfo    # set ARCH to match your GPU
compute-sanitizer --tool racecheck \
  --log-file racecheck.log \
  ./main
```

Or via the HeCBench harness:

```bash
python3 scripts/run_sanitizer.py --project tridiagonal --tool racecheck
```

Note: the full racecheck run times out at 300s on larger problem sizes; the race above is found within the first ~4 minutes of the run, well before the timeout.

---

## Race Conditions

**One confirmed race**, found in the latest batch: `tridiagonal` (see [above](#3-tridiagonal--missing-__syncthreads-shared-memory-race)).

Every other completed racecheck run reports:

```
RACECHECK SUMMARY: 0 hazards displayed (0 errors, 0 warnings)
```

---

## Excluded Findings (Not Real Code Bugs)

### GPU Out-of-Memory

| Benchmark | Report | Notes |
|-----------|--------|-------|
| `threadcpy` | `Bandwidth/threadcpy/memcheck.log` | `cudaMalloc` fails on 1 GB allocation under sanitizer overhead |
| `storeKVCache` | `Bandwidth/storeKVCache/memcheck.log` | OOM during large batch-size sweep (after many passing tests) |
| `atomicCost` | `Data_compression_and_reduction/atomicCost/racecheck.stdout.log` | FP64 path tries ~7 GB allocation (`length=922521600`); subsequent `illegal memory access` errors are stale CUDA state |

### Invalid Harness Arguments / Early Abort

| Benchmark | Report | Notes |
|-----------|--------|-------|
| `vadd` | `Bandwidth/vadd/memcheck.log` | Harness runs `./main 256 128 1`; `N=128` fails assertion `N % TILE_N == 0` (`TILE_N=256`) before any CUDA kernels execute |
| `sssp` | `Graph_and_Tree/sssp/memcheck.log` | Missing `input/NYR_input.dat`; app exits before instrumentation |
| `ced` | `Computer_vision_and_image_processing/ced/memcheck.log` | Missing `input/peppa/0.txt` |
| `cm` | `Bioinformatics/cm/memcheck.log` | Missing `queries/` subdirectory |
| `mt` | `Random_number_generation/mt/memcheck.log` | Missing `./data/MersenneTwister.dat`; app exits before instrumentation |
| `fdtd3d` | `Simulation/fdtd3d/memcheck.log` | Harness passes `--dimx=64 --dimy=64`; app requires `[96, 376]` and exits before any kernel launches |
| `heartwall` | `Simulation/heartwall/memcheck.log` | Missing AVI input file (`AVI_open_input_file` fails) |
| `nbody` | `Simulation/nbody/memcheck.log` | Harness passes 1 integration step; app requires at least 3 and exits early |
| `xlqc` | `Simulation/xlqc/memcheck.log` | Missing `example/geom.xyz` input file |
| `clink` | `Machine_learning/clink/memcheck.log` | Harness runs `./main 1`; app cannot open `input.hpp` |
| `dense-embedding` | `Machine_learning/dense-embedding/memcheck.log` | Harness passes `nrows=256, batch_size=256`; assertion `nrows > batch_size * batch_size` fails before any kernel launches |

### Build / Run Environment Gaps

| Benchmark | Report | Notes |
|-----------|--------|-------|
| `miniFE` | `Simulation/miniFE/racecheck.log` | No `main` binary present (stale/incomplete build); compute-sanitizer reports "target application doesn't exist" |
| `axhelm` | n/a (`BUILD_FAILED`) | Missing `BlasLapack` subdirectory dependency |
| `miniWeather` | n/a (`BUILD_FAILED`) | Missing `mpi.h` — MPI not installed |
| `testSNAP` | n/a (`BUILD_FAILED`) | Missing generated header `refdata_2J14_W.h` |
| `blas-fp8gemm` | `Math/blas-fp8gemm/memcheck.log` | cuBLAS rejects the FP8 `cublasLtMatmul` config (`std::logic_error: cuBLAS API failed`); requires Hopper (sm_90+), this GPU is an A30 (sm_80) |

### Timeouts (No Sanitizer Findings)

**300s (earlier batches):** `floydwarshall`, `aidw`, `binomial`, `ldpc`, `segment-reduce`, `sc`, `contract`, `bitpacking`, `atomicPerf`, `ssim` (memcheck), `threadcpy` (racecheck)

**300s (Sorting/Signal/RNG/Search/Robotics batch):** `sosfil` (memcheck + racecheck), `bitonic-sort` (racecheck), `segsort` (racecheck), `sort` (racecheck), `topk` (memcheck + racecheck)

**300s (Simulation batch, clean before timeout):** `bspline-vgh`, `d3q19_bgk`, `ddbp`, `goulash`, `gpp`, `hexicton`, `laplace`, `lid-driven-cavity`, `metropolis`, `rayleighBenardConvection` (memcheck + racecheck); `eikonal`, `lavaMD`, `loopback`, `mdh`, `sheath` (racecheck only — memcheck passed clean)

**300s (Simulation batch, race found before timeout):** `tridiagonal` (racecheck) — see [Real Bugs §3](#3-tridiagonal--missing-__syncthreads-shared-memory-race); `sph` (memcheck) was clean before timeout, racecheck passed

**300s (Math batch):** `f16max` (racecheck), `f16sp` (memcheck) — confirmed `ERROR: timed out after 300s`, no output produced before the limit

**300s (Machine learning batch):** `matern` (racecheck), `overlay` (memcheck + racecheck) — confirmed `ERROR: timed out after 300s`, no output produced before the limit

**600s (earlier batches):** `aligned-types`, `blockScan`, `streamOrderedAllocation`, `streamPriority` (all racecheck)

`topk` and `sosfil` were making progress with PASS results when killed; `bitonic-sort` racecheck completed one 33M-element sort (~51 min) but timed out before the run finished. In the Simulation batch, `lid-driven-cavity` (both tools) and `d3q19_bgk`/`goulash`/`laplace` (racecheck) timed out without completing even one configuration — no sanitizer summary line was produced at all.

### Incomplete Runs (Killed Mid-Execution, No Summary)

| Benchmark | Report | Notes |
|-----------|--------|-------|
| `match` | `Computer_vision_and_image_processing/match/racecheck.log` | GPU2/GPU3 paths take millions of ms under racecheck; no summary yet |
| `blas-gemmBatched` | `Math/blas-gemmBatched/{memcheck,racecheck}.log` | Killed mid-sweep over GEMM sizes; no timeout marker and no sanitizer summary line in either tool's log — no findings in the partial output |
| `blas-gemmStridedBatched` | `Math/blas-gemmStridedBatched/{memcheck,racecheck}.log` | Same as above — killed mid-sweep, no findings in the partial output |
| `addBiasResidualLayerNorm` | `Machine_learning/addBiasResidualLayerNorm/racecheck.log` | Killed mid-sweep over hidden dimensions under racecheck; no summary yet; all configurations completed so far report PASS |

### Clean Runs (0 Errors / 0 Hazards)

**Earlier batches:** `base64e`, `crs`, `babelstream`, `ssim` (racecheck), `atomicIntrinsics`, `atomicCost` (racecheck), `merkle`, `degrid`, `egs`, `fsm`, `storeKVCache` (racecheck), `binomial` (memcheck), `bitpacking` (memcheck), `atomicPerf` (memcheck), `dispatch` (memcheck + racecheck), `pointerchase` (memcheck + racecheck), `shuffle` (racecheck), `asta` (racecheck)

**Sorting/Signal/RNG/Search/Robotics batch (all clean):** `qrg`, `rng-wallace`, `sobol`, `rodrigues`, `bsearch`, `keogh`, `s8n`, `extrema`, `fft`, `lombscargle`, `zmddft`, `hybridsort`, `is`, `merge`, `quicksort`, `radixsort`, `sortKV`, `split`, `warpsort`, `bitonic-sort` (memcheck), `segsort` (memcheck), `sort` (memcheck)

**Simulation batch (clean on both tools):** `ace`, `adv`, `amgmk`, `bh`, `burger`, `ccsd-trpdrv`, `chemv`, `chi2`, `clenergy`, `cobahh`, `cooling`, `damage`, `dslash`, `feynman-kac`, `fhd`, `fluidSim`, `haccmk`, `heat`, `heat2d`, `hwt1d`, `hypterm`, `ising`, `iso2dfd`, `laplace3d`, `lulesh`, `md`, `minimod`, `minisweep`, `mxfp4`, `myocte`, `particle-diffusion`, `particlefilter`, `particles`, `pathfinder`, `pns`, `projectile`, `pso`, `qem`, `rainflow`, `reaction`, `rsbench`, `rtm8`, `rushlarsen`, `s3d`, `simplemoc`, `slit`, `su3`, `tensorT`, `tissue`, `tsa`, `vanGenuchten`, `vmc`, `wlcpow`, `wsm5`, `xsbench`

**Simulation batch (clean on the tool that completed):** `eikonal`, `grrt`, `lavaMD`, `loopback`, `mdh`, `sheath`, `sph`, `tridiagonal` (memcheck only — racecheck found the race above)

**Machine learning batch (clean on completed runs):** `addBiasQKV` (racecheck), `attentionMultiHeadKVCache` (memcheck + racecheck), `bn` (memcheck + racecheck), `channelShuffle` (racecheck), `concat` (racecheck)

---

## Benchmark Coverage

| Category | Benchmarks Tested |
|----------|-------------------|
| Bandwidth | `babelstream`, `storeKVCache`, `threadcpy`, `vadd` |
| Bioinformatics | `cm`, `fsm` |
| Computer Vision | `ced`, `degrid`, `egs`, `match`, `perlin`, `ssim` |
| Cryptography | `merkle` |
| Data Compression | `atomicCost`, `atomicIntrinsics`, `atomicPerf`, `bitpacking`, `contract`, `histogram`, `sc`, `segment-reduce` |
| Data Encoding | `base64e`, `crs`, `ldpc` |
| Finance | `binomial` |
| Geoscience | `aidw` |
| Graph/Tree | `floydwarshall`, `sssp` |
| Language & Kernel Features | `aligned-types`, `asta`, `blockScan`, `dispatch`, `pointerchase`, `shuffle`, `streamOrderedAllocation`, `streamPriority` |
| Random Number Generation | `mt`, `qrg`, `rng-wallace`, `sobol` |
| Robotics | `rodrigues` |
| Math | `blas-fp8gemm`, `blas-gemmBatched`, `blas-gemmStridedBatched`, `f16max`, `f16sp` |
| Machine Learning | `addBiasQKV`, `addBiasResidualLayerNorm`, `attentionMultiHeadKVCache`, `bn`, `channelShuffle`, `clink`, `concat`, `dense-embedding`, `matern`, `overlay` |
| Search | `bsearch`, `keogh`, `s8n` |
| Signal Processing | `extrema`, `fft`, `lombscargle`, `sosfil`, `zmddft` |
| Simulation | `ace`, `adv`, `amgmk`, `axhelm`, `bh`, `bspline-vgh`, `burger`, `ccsd-trpdrv`, `chemv`, `chi2`, `clenergy`, `cobahh`, `cooling`, `d3q19_bgk`, `damage`, `ddbp`, `dslash`, `eikonal`, `fdtd3d`, `feynman-kac`, `fhd`, `fluidSim`, `goulash`, `gpp`, `grrt`, `haccmk`, `heartwall`, `heat`, `heat2d`, `hexicton`, `hwt1d`, `hypterm`, `ising`, `iso2dfd`, `laplace`, `laplace3d`, `lavaMD`, `lid-driven-cavity`, `loopback`, `lulesh`, `md`, `mdh`, `metropolis`, `miniFE`, `miniWeather`, `minimod`, `minisweep`, `mxfp4`, `myocte`, `nbody`, `particle-diffusion`, `particlefilter`, `particles`, `pathfinder`, `pns`, `projectile`, `pso`, `qem`, `rainflow`, `rayleighBenardConvection`, `reaction`, `rsbench`, `rtm8`, `rushlarsen`, `s3d`, `sheath`, `simplemoc`, `slit`, `sph`, `su3`, `tensorT`, `testSNAP`, `tissue`, `tridiagonal`, `tsa`, `vanGenuchten`, `vmc`, `wlcpow`, `wsm5`, `xlqc`, `xsbench` |
| Sorting | `bitonic-sort`, `hybridsort`, `is`, `merge`, `quicksort`, `radixsort`, `segsort`, `sort`, `sortKV`, `split`, `topk`, `warpsort` |
