# CUDA Benchmark Input Requirements — Split by External File Dependency

Per-benchmark analysis of `make run` invocations across all 19 README categories, split into two sub-tables per category: benchmarks that **require an external data file** (YES) and benchmarks that run with only **numeric/string CLI arguments and synthetic/internally-generated data** (NO).

---

## Automotive

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| daphne | `./points2image -p 1` (run from `daphne-cuda/src/points2image/`; top-level dir has no Makefile) | Binary testcase data (`p2i_input.dat`, `p2i_output.dat`), read via `ifstream` in binary mode | Hardcoded relative paths `../../data/p2i_input.dat`/`p2i_output.dat`; data files are NOT bundled in this checkout — `daphne-cuda/data/README` points to https://github.com/esa-tu-darmstadt/daphne-benchmark for the actual data; `-p 1` is a benchmark/precision selector flag |

### External File Input: NO

*No entries in this category.*

---

## Bandwidth

### External File Input: YES

*No entries in this category.*

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| allreduce | `./main` | No arguments; runs with internally generated buffers |
| cmembench | `./main 1000` | Repeat count |
| babelstream | `./main` | No arguments; default array size used internally |
| ccl | `./main 50` | Repeat count (NCCL collective communication benchmark) |
| memcpy | `./main 8192` | Transfer size argument |
| memtest | `./main 100` | Repeat count |
| p2p | `./main 2000` | Size/repeat argument |
| pingpong | `./mpi-program` and `./nccl-program` (two binaries) | MPI/NCCL ping-pong between ranks; no file args, requires an `mpirun` environment rather than a file |
| randomAccess | `./main 10` | Repeat count |
| reshapeKVCache | `./main 1024 32 128 1000` | Numeric KV-cache dimensions + repeat count; synthetic data |
| shmembench | `./main 1000` | Repeat count |
| storeKVCache | `./main 1000` | Repeat count |
| threadcpy | `./main 100` | Repeat count |
| triad | `./main --passes 100 -v` | `--passes` count, `-v` verbose flag |
| vadd | `./main 16384 8192 100` | Vector size, block size, repeat count |

---

## Bioinformatics

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| bsw | `./main ./test-data/protein-reference.fasta ./test-data/protein-query.fasta test_regression_protein_out` (+ `diff` against expected output) | FASTA protein sequence files | Two input FASTA files + an output path; data not present in this checkout (DVC/download required) |
| ccs | `./main -t 0.9 -i Data_Constant_100_1_bicluster.txt -o ./Output.txt -m 50 -p 1 -g 100.0 -r 100` | Plain-text bicluster data matrix | `-i <input>`, `-o <output>`, plus numeric threshold/iteration flags; input file not present in this checkout |
| deredundancy | `./main i ../deredundancy-sycl/testData.fasta` | FASTA gene sequence file | `i <path>` selects input mode + path to FASTA file (sibling dir, missing in this checkout) |
| diamond | `./main blastx -q $(DMND_PATH)/long.fastq.gz -d $(DMND_PATH)/ARDB.dmnd -o long_cur.m8 --tmpdir /dev/shm -p1` | Gzipped FASTQ query + DIAMOND binary database (`.dmnd`); output BLAST tabular (`.m8`) | `-q` query file, `-d` database file, `-o` output path |
| frna | `./main ../prna-cuda/HIV1-NL43.seq hiv1-nl43.out` | Plain-text RNA/DNA sequence (FASTA-like `.seq`) | Reuses `prna-cuda`'s sequence file + output path |
| local-ht | `bash test_script.sh` (wraps `./main ../locassm_data/<file> <kmer_size> test-out.dat`) | Custom binary `.dat` local-assembly extension files (per k-mer size: 21/33/55/77) | Script iterates over 4 bundled-path `.dat` files under `../locassm_data/`, validating against `res_*` reference outputs |
| logan | `./main inputs/100k.txt 17 100 1` | Plain-text DNA sequence-pair file | Path to seed-and-extend input file + seed length, repeat, mode |
| minibude | `./main --deck ../minibude-sycl/data/bm1 --wgsize 256 --iterations 100` | BUDE "deck" binary dataset (protein docking poses + forcefield params) | `--deck <path>` (sibling dir, missing in this checkout), `--wgsize`, `--iterations` |
| minimap2 | `./main ../minimap2-sycl/in-1k.txt kernel-out.txt` | Plain-text long-read sequence input | Path to input reads + output path (sibling dir, missing in this checkout) |
| prna | `./main ./HIV1-NL43.seq` | Plain-text RNA sequence (`.seq`) | Shipped compressed as `HIV1-NL43.tar.gz` (DVC-tracked) — must be extracted first |
| sa | `./main genome.txt 1000000 100` | Plain-text genome sequence file | Shipped compressed as `genome.tar.bz` (DVC-tracked) — must be extracted first |
| snake | `./main 100 ./Datasets/ERR240727_1_E2_30000Pairs.txt 30000 1000` | Plain-text paired-read sequence dataset | Path to dataset file + repeat/size args |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| all-pairs-distance | `./main 10000` | Numeric point count; synthetic data |
| cm | `./main` | No arguments |
| epistasis | `./main 146 31339 100` | Numeric SNP count, sample count, repeat count; synthetic data |
| extend2 | `./main 2000` | Repeat count; synthetic data |
| fsm | `./main 65536` | Numeric size; synthetic data |
| ga | `./main 1000000 1000 11 1` | Numeric gene-alignment size args; synthetic data |
| nbnxm | `./main 10000` | Numeric size; synthetic data |
| nw | `./main 16384 10 100` | Matrix size, penalty, repeat count; synthetic data |
| pcc | `./main 90112 165` | Numeric dims; synthetic data |

---

## Computer vision and image processing

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| affine | `./main data/CT-MONO2-16-brain.raw result.raw 1000000` | Raw 16-bit grayscale medical image (`.raw`) | Bundled in `data/`; input path, output path, repeat count |
| bm3d | `./main noised-bufferfly-20.png denoised.png 20 color bufferfly.png` | PNG images | Input/output PNG paths bundled in the directory (`noised-bufferfly-20.png`, `bufferfly.png` present) |
| boxfilter | `./main ../boxfilter-sycl/data/lenaRGB.ppm 10000` | `.ppm` image | Sibling-dir path, missing in this checkout; + repeat count |
| face | `./main-cpu Face.pgm info.txt class.txt Output-cpu.pgm` / `./main Face.pgm info.txt class.txt Output-gpu.pgm` | PGM grayscale image + plain-text Haar-cascade-style `info.txt`/`class.txt` config files | All three input files missing in this checkout; output PGM path also given |
| hogbom | `./main data/dirty_4096.img data/psf_4096.img 1000` | Custom binary astronomy image format (`.img`) | `data/` dir absent in this checkout; dirty image + PSF image paths + repeat count |
| medianfilter | `./main ../medianfilter-sycl/data/SierrasRGB.ppm 1000` | `.ppm` image | Sibling-dir path, missing in this checkout; + repeat count |
| mriQ | `./main datasets/128x128x128/input/128x128x128.bin output_128` | Binary MRI k-space dataset | Shipped compressed as `datasets.tar.bz` (DVC-tracked) — must be extracted first |
| opticalFlow | `./main data/frame10.ppm data/frame11.ppm` | `.ppm` image pair | Bundled in `data/` (present in this checkout) |
| sobel | `./main ../sobel-sycl/SobelFilter_Input.bmp 100000` | `.bmp` image | Sibling-dir path, missing in this checkout; + repeat count |
| tonemapping | `./main ../tonemapping-sycl/input.hdr 10000` | Radiance HDR image (`.hdr`) | Sibling-dir path, missing in this checkout; + repeat count |
| recursiveGaussian | `./main StoneRGB.ppm 1000` | `.ppm` image | Bundled in this checkout (`StoneRGB.ppm` present); + repeat count |
| sad | `./main coins.bmp coin.bmp 100` | `.bmp` images | Both files missing in this checkout; + repeat count |
| seam-carving | `./main Broadway_tower.jpg 100 -u` | `.jpg` image | File missing in this checkout; + repeat count, `-u`/`-a` mode flag |
| voxelization | `./main data/test/ 100000` | Binary point-cloud files (`.bin`) | Bundled in `data/test/` (present in this checkout) + repeat count |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| aobench | `./main 100` | Repeat count |
| asmooth | `./main 8192 2000 9 100` | Numeric dims, box size, repeat count; synthetic data |
| background-subtract | `./main 4096 2048 0 102` (and a variant) | Numeric dims, mode flag, repeat count; synthetic data |
| bezier-surface | `./main -n 8192` | `-n <size>` numeric flag; synthetic data |
| bilateral | `./main 2960 1440 0.5 0.5 1000` | Numeric dims, sigma values, repeat count; synthetic data |
| cbsfil | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic data |
| car | `./main 100` | Repeat count; synthetic data |
| ced | `./main -a 0` | `-a <threshold>` numeric flag (Canny edge detection on synthetic/internal image) |
| colorwheel | `./main 10 4096 100` | Numeric dims + repeat count; synthetic data |
| convolution1D | `./main 134217728 1000` | Numeric size + repeat count |
| convolution3D | `./main 32 1 6 32 32 5 100` (3 variants) | Numeric tensor dims + repeat count |
| convolutionDeformable | `python main.py` (via `run: build`) | Python/PyTorch script, no CLI args; synthetic tensors |
| convolutionSeperable | `./main 8192 8192 1000` (dir is actually `convolutionSeparable-cuda`) | Numeric dims + repeat count |
| dct8x8 | `./main 8192 8192 100` | Numeric dims + repeat count |
| debayer | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic Bayer pattern |
| depixel | `./main 2048 2048 10` | Numeric dims + repeat count; synthetic pixel-art image |
| degrid | `./main` | No arguments |
| doh | `./main 480 640 100` | Numeric dims + repeat count; synthetic data |
| dpid | `./main 640 480 0.5 100` | Numeric dims, scale factor, repeat count; synthetic image |
| egs | `./main` | No arguments |
| flame | `./main 1000` | Repeat count; synthetic fractal data |
| gabor | `./main 8640 15360 1000` | Numeric dims + repeat count; synthetic image |
| gamma-correction | `./main 7680 4320 256 100` | Numeric dims, bin count, repeat count; synthetic image |
| groupnorm | `./main 32 32 16 16 32 100` (and a variant) | Numeric tensor dims + repeat count |
| mandelbrot | `./main 1000` | Repeat count |
| marchCubes | `./main 100` (dir is `marchingCubes-cuda`) | Repeat count; synthetic volumetric data |
| match | `./main 100` | Repeat count; synthetic feature points |
| morphology | `./main 32 32 8192 8192 100` (and a variant) | Numeric kernel/image dims + repeat count; synthetic image |
| ne | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic point cloud |
| perlin | `./main` | No arguments |
| resize | `./main 1920 1080 256 256 512 100` | Numeric source/dest dims + repeat count; synthetic image |
| spm | `./main 256 1000` | Numeric size + repeat count; synthetic data |
| srad | `./main 1000 0.5 502 458` | Repeat count, lambda, image dims; synthetic speckle image generated internally |
| ssim | `./main` | No arguments; synthetic images |
| stencil1d | `./main 134217728 1000` | Numeric size + repeat count |
| stencil3d | `./main 512 100` | Numeric size + repeat count |
| surfel | `./main 1024 640 480 100` (and a variant) | Numeric point count, image dims, repeat count; synthetic data |
| tgvnn | `./main` | No arguments |
| upsample | `./main 32 64 64 64 100` | Numeric tensor dims + repeat count; synthetic data |
| zoom | `./main 1 3 2160 4096 1000` | Numeric mode, channels, dims, repeat count; synthetic image |

---

## Cryptography

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| aes | `./aes 100 0 ../urng-sycl/URNG_Input.bmp` / `./aes 100 1 ../urng-sycl/URNG_Input.bmp` | `.bmp` bitmap image | `<iterations> <0=encrypt\|1=decrypt> <path to bitmap image>` |
| bitcracker | `./bitcracker -f ./hash_pass/img_win8_user_hash.txt -d ./hash_pass/user_passwords_60000.txt -b 60000` | Plain-text hash file + plain-text password dictionary | `-f <hash_file>`, `-d <dictionary_file>`, `-b <count>`; files must be generated via bundled `hash_pass/password_generator.py` |
| columnarSolver | `./main data` | Plain-text bigram table (`<bigram> <score>` per line) | Path to bigram-score data file, shipped compressed as `data.tar.gz` |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| bitpermute | `./main 100` | Repeat count |
| chacha20 | `./main 100000` | Repeat count |
| ecdh | `./main 50000000 100` | Number of keys + repeat count |
| keccaktreehash | `./main` | No arguments |
| merkle | `./main` | No arguments |
| present | `./main 100000 100` | Number of plaintexts + repeat count |

---

## Data compression and reduction

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| dxtc2 | `./main ../dxtc2-sycl/data/lena_std.ppm ../dxtc2-sycl/data/lena_ref.dds 100` (and a teapot variant) | `.ppm` input image + `.dds` reference compressed-texture file | Path to source `.ppm`, path to reference `.dds`, repeat count |
| lzss | `./main -i QCLOUDf48.bin.f32.errctrl.1e-3` | Binary scientific dataset (SZ/ZFP-style error-controlled float32 data) | `-i <inputfile>`; sample data NOT bundled in this checkout — README points to https://github.com/hpdps-group/ICS23-GPULZ |
| mpc | `./main msg_sp.trace.out 1` (compression) / `./main compression.txt` (decompression) | Plain-text message trace file / compressed-text intermediate file | Trace/text files bundled compressed as `trace.tar.bz` (tracked via `trace.tar.bz.dvc`) — must be extracted before running |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| atomicAggregate | `./main 1000` | Repeat count |
| atomicCAS | `./main 10000` | Repeat count |
| atomicCost | `./main 1 10` / `./main 16 10` | Numeric mode + repeat count |
| atomicIntrinsics | `./main 27 100` | Numeric size + repeat count |
| atomicPerf | `./main 1024` | Numeric size argument |
| atomicSystemWide | `./main 1000` | Repeat count |
| bitpacking | `./main` | No arguments; synthetic data |
| bscan | `./main 1000` | Repeat count |
| bwt | `./main 10000000` | Input size; synthetic data |
| compute-score | `./main -p=1000` | `-p=<value>` parsed via generic `--name=value` option parser; numeric only |
| contract | `./main 64 5` | Numeric dims; synthetic data |
| filter | `./main 100000000 256 100` | Numeric size, threshold, repeat count |
| fma | `./main 8192 8192 1031 8192 1000` | Numeric gather/scatter dims + repeat count |
| fpc | `./main 256 100` | Numeric size + repeat count |
| histogram | `./main --i=100` | `--i=<iterations>` option; synthetic data |
| minmax | `./main 100000 1000` | Numeric size + repeat count |
| mtf | `./main 1024 100` | Numeric size + repeat count |
| quantAQLM | `./main` | No arguments; synthetic weights |
| quantBnB | `./main 16777216 100` | Numeric size + repeat count |
| quantVLLM | `./main 4096 5137 1000` | Numeric dims + repeat count |
| rle | `./main --i=1000` | `--i=<iterations>` option; synthetic data |
| sc | `./main -a 0.1` | `-a <alpha>` numeric option |
| scan | `./main 268435456 100` | Numeric size + repeat count |
| scan2 | `./main 1000 33554432 256` | Repeat count, size, block size |
| scan3 | `./main 1000 33554432` | Repeat count + size |
| scatter | `./main 10000000 100` | Numeric size + repeat count |
| scatterAdd | `./main 10000000 32 8 1000` (and a variant) | Numeric size, dims, repeat count |
| scatterThrust | `./main 10000000 100` | Numeric size + repeat count |
| segment-reduce | `./main 16384 100` | Numeric size + repeat count |

---

## Data encoding, decoding, or verification

### External File Input: YES

*No entries in this category.*

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| ans | `./main 10` | Single numeric size argument (MB); synthetic data generated internally |
| base64e | `./main` | No arguments; synthetic data generated internally |
| crc64 | `./main 10 5 33554432` | Numeric repeat/thread/size args |
| crs | `./main 10 10` | Numeric data-block / parity-block counts |
| entropy | `./main 8192 8192 100` | Numeric width, height, repeat count |
| jenkins-hash | `./main 256 16777216 100` | Numeric args; synthetic data |
| kiss | `./main 1000` | Repeat count |
| ldpc | `./main` | No arguments; writes `llr_fp.dat` as output internally (not a required input) |
| md5hash | `./main 1 4` | Numeric mode + thread/block arg |
| murmurhash3 | `./main 100000 100` | Numeric size + repeat count |

---

## Finance

### External File Input: YES

*No entries in this category.*

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| aop | `./main -runs 100` | `-timesteps`/`-paths`/`-runs` flags, all numeric; synthetic data |
| black-scholes | `./main 100` | Repeat count |
| binomial | `./main` | No arguments |
| bonds | `./main 100` | Repeat count |
| libor | `./main 100` | Repeat count |

---

## Geoscience

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| geodesic | `./main ../geodesic-sycl/locations.txt 1000` | Plain-text latitude/longitude coordinate list | Path to `locations.txt` + repeat count |
| haversine | `./main ../geodesic-cuda/locations.txt 100` | Plain-text latitude/longitude coordinate list (reuses `geodesic-cuda`'s file) | Path to `locations.txt` + repeat count |
| stsg | `./main example.txt` | Plain-text time-series/parameter config file (comma-separated yearly data) | Path to `example.txt` config/data file |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| aidw | `./main 100 1 100` | Numeric grid size, mode, repeat count; synthetic data |
| coordinates | `./main 10000000 1000` | Number of points + repeat count; synthetic data |
| hausdorff | `./main 100000 100000 100` | Numeric point-set sizes + repeat count; synthetic data |

---

## Graph and Tree

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| cc | `./main delaunay_n24.egr 100` | ECL binary graph format (`.egr`) | Path to `.egr` graph file (not bundled in this checkout — must be obtained separately) + repeat count |
| floydwarshall2 | `./main CollegeMsg.egr 1000` | ECL binary graph format (`.egr`) | Path to bundled `CollegeMsg.egr` + repeat count |
| gc | `./main ../mis-cuda/internet.egr 100` | ECL binary graph format (`.egr`) | Reuses `mis-cuda`'s bundled `internet.egr` + repeat count |
| hbc | `./main -i ./graphs/breast_coexpress.txt -v --printscores=output.txt` | Plain-text edge-list graph file | `-i <path>` input graph, `-v` verbose, `--printscores=<path>` output path |
| mis | `./main internet.egr 100` | ECL binary graph format (`.egr`) | Path to bundled `internet.egr` + repeat count |
| rsmt | `./main newblue7.kraftwerk70.3d.80.20.82.m8.gr` | Plain-text ISPD global-routing benchmark format (`.gr`) | Path to bundled `.gr` routing-grid file |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| floydwarshall | `./main 1024 100 16` | Graph size, repeat count, block size; synthetic graph |
| hungarian | `./main output.txt` | `argv[1]` is an OUTPUT file path (`freopen(argv[1], "w", stdout)`), not a required input file |
| sssp | `./main -g 120 -t 1 -w 10 -r 100` | `-g` graph size, `-t` type, `-w` weight range, `-r` repeat — all numeric, synthetic graph |

---

## Language and kernel features

### External File Input: YES

*No entries in this category.*

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| adjacent | `./main 100000000 1000` | Numeric size + repeat count; synthetic data |
| aligned-types | `./main` | No arguments |
| asta | `./main` | No arguments |
| blockAccess | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic data |
| blockexchange | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic data |
| blockScan | `./main 2227104 100` | Numeric size + repeat count; synthetic data |
| collision | `./main 1000` | Repeat count; synthetic data |
| concurrentKernels | `./main 4` | Numeric stream count |
| conversion | `./main 268435456 10` | Numeric size + repeat count; synthetic data |
| dispatch | `./main` | No arguments |
| dp4a | `./main 268435456 1000` | Numeric size + repeat count; synthetic data |
| graphExecution | `./main` | No arguments |
| ert | `./main 1024 256` | Numeric work-size args; synthetic data |
| interleave | `./main 100` | Repeat count; synthetic data |
| intrinsics-cast | `./main 1000000 100` | Numeric size + repeat count; synthetic data |
| kernelLaunch | `./main 1000000` | Numeric repeat count |
| layout | `./main 1000` | Repeat count; synthetic data |
| mallocFree | `./main 536870912` | Numeric allocation size |
| maxFlops | `./main 5000` | Repeat count; synthetic data |
| mixbench | `./main 8192 1000` | Numeric size + repeat count; synthetic data |
| nosync | `./main 268435456 100` | Numeric size + repeat count; synthetic data |
| openmp | `./main 100` | Repeat count; synthetic data |
| overlap | `./main` | No arguments |
| pad | `./main -a 0.1` | `-a <alpha>` numeric option (memory-padding microbenchmark) |
| pitch | `./main 1000` | Repeat count; synthetic data |
| popcount | `./main 16777216 1000` | Numeric size + repeat count; synthetic data |
| pointerchase | `./main` | No arguments; synthetic pointer-chase buffer generated internally |
| prefetch | `./main 100` | Repeat count; synthetic data |
| reverse | `./main 100` | Repeat count; synthetic data |
| ring | `./main 1 67108864 100` | Numeric mode, size, repeat count; synthetic data |
| saxpy-ompt | `./main` | No arguments; synthetic vectors |
| shuffle | `./main 200000 100` | Numeric size + repeat count; synthetic data |
| simpleMultiDevice | `./main 1000` | Repeat count; synthetic data |
| streamCreateCopyDestroy | `./main` | No arguments |
| streamOrderedAllocation | `./main` | No arguments |
| streamPriority | `./main 300` | Numeric size; synthetic data |
| streamUM | `./main 4 1000 1` / `./main 4 4000 0` | Numeric stream count, size, mode flag; synthetic data |
| tensorAccessor | `./main 8192 8192 1000` | Numeric dims + repeat count; synthetic data |
| threadfence | `./main 100 100000000` | Numeric repeat count + size; synthetic data |
| warpexchange | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic data |
| vote | `./main 10000000` | Numeric size; synthetic data |
| wmma | `./main 0 16 16 64 100 1` (and other dim variants) | Numeric mode, matrix dims, repeat count, flag; synthetic matrices |
| wordcount | `./main 10` | Numeric arg (repeat/size); synthetic text buffer generated internally |
| zerocopy | `./main 100` | Repeat count; synthetic data |

---

## Machine learning

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| gd | `./main gisette_scale 0.0001 10 100` | LibSVM text format (`label feature_index:value ...` per line) | First arg is a path to a LibSVM-formatted training data file (e.g. gisette_scale dataset), parsed via ifstream/getline; other args are lambda, alpha, repeat |
| ge-spmm | `./main data/snap/amazon0302/amazon0302.mtx 8 1000` | Matrix Market (.mtx) sparse matrix file | First arg is a path to a Matrix Market (.mtx) sparse matrix file parsed by readMtx(); other args are tile row size and repeat count |
| gmm | `./main 500 data result` | CSV (comma/whitespace separated rows of floats) or proprietary binary (.bin) format, auto-detected by file extension | Second arg ("data") is a path to an input dataset file read via readData()/readCSV()/readBIN() (extracted from data.tar.gz); third arg ("result") is an output file prefix; first arg is number of clusters |
| kmc | `./main -v 10 gisette_scale` | LibSVM text format (`label feature_index:value ...` per line) | This is svm-train (libsvm-style); final positional arg is a path to a LibSVM-formatted training file (e.g. gisette_scale); `-v 10` requests 10-fold cross validation |
| kmeans | `./main -r -n 5 -m 15 -l 10 -o -i ../data/kmeans/kdd_cup` | Plain-text whitespace/tab-delimited rows of an id followed by numeric features (ASCII), or optionally a custom binary format with `-b` flag | `-i` flag specifies path to input data file (kdd_cup dataset) read via fopen/fgets in read_input.cpp; other flags set cluster range, loop count, and output options |
| lr | `./main 100000 0` | Tab-separated text file with two numeric columns (x value, y value) per line | Program reads a hardcoded dataset file path `../lr-sycl/assets/temperature.txt` (TEMP_FILENAME) via fopen/fgets/strtok in create_dataset(); CLI args are repeat count and cpu offset, not the file path itself |
| mcpr | `./main alphas.csv 10` | CSV (numeric text, one value per line; bundled as alphas.csv.bz2) | path to alphas.csv data file, repeat count |
| meanshift | `./main dataset/data.csv dataset/centroids.csv` | CSV (rows of comma-separated floats) | path to data points CSV, path to centroids CSV |
| mf-sgd | `./main [options] train_file [model_file]` | Binary matrix-factorization rating records (user_id:int32, item_id:int32, rating:float32, repeated) | options (-l/-k/-t/-a/-b/-s/-u/-v/-x/-y), required path to training ratings file, optional output model path; note: this benchmark's Makefile (src/Makefile) has no `run:` target |
| mmcsf | `./main -i output.tns -m 0 -R 32 -f 128 -w 4` | .tns sparse tensor text file (COO-style: indices per mode + value per line) | -i input tensor file (.tns, generated via generate_tensor.py or e.g. toy.tns), -m mode, -R rank, -f fiber threshold, -w warp count |
| mnist | `./main ./data 2` | MNIST IDX binary format (train-images.idx3-ubyte, train-labels.idx1-ubyte, t10k-images.idx3-ubyte, t10k-labels.idx1-ubyte, unpacked from data.zip) | path to data directory containing MNIST idx files, number of iterations |
| snicit | `./run.sh` (runs `./main -k A/B/C/D`) | MNIST IDX binary images/labels plus TSV weight/bias matrices | -k benchmark key (A/B/C/D) selecting bundled dataset under dataset/beyond (MNIST idx3/idx1 files, networks/tsv_weights, networks/tsv_biases) |
| word2vec | `./main -train text8 -output vectors.bin -cbow 1 -size 200 -window 8 -negative 25 -hs 0 -sample 1e-4 -threads 20 -binary 1 -iter 15` | Plain text corpus (raw whitespace-separated words, e.g. the "text8" Wikipedia dump) | -train path to text corpus file, -output path to output vectors, plus numeric hyperparameters (cbow, size, window, negative, hs, sample, threads, binary, iter) |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| accuracy | `./main 8192 10000 10 100` | Numeric args: number of samples, number of classes, top-k, repeat count; synthetic data generated internally |
| adam | `./main 10000 200 100` | Numeric args: vector length, number of steps, repeat count; synthetic data generated internally |
| adamw | `./main 1000000 100` | Numeric args: vector length, repeat count; synthetic data generated internally |
| addBiasQKV | `./main` | No arguments; sizes are hardcoded/synthetic in source |
| addBiasResidualLayerNorm | `./main 10000` | Numeric arg: repeat count; synthetic data generated internally |
| attention | `./main 65536 2048 0 1000` | Numeric args: sequence length/dim, mode, repeat count; synthetic data generated internally |
| attention-paged | `./main 8 32 128 4096 128 100` | Numeric args: batch, heads, head dim, context len, block size, repeat; synthetic KV cache generated internally |
| attentionMergeState | `./main 1024 32 128 1000` | Numeric args: batch/sequence size, heads, dim, repeat count; synthetic data generated internally |
| attentionMultiHead | `./main 100000` | Numeric arg: repeat count; synthetic data generated internally |
| attentionMultiHeadKVCache | `./main 100` | Numeric arg: repeat count; synthetic data generated internally |
| backprop | `./main 65536` | Numeric arg: number of input nodes/layer size; synthetic data generated internally |
| bincount | `./main 10000000 100` | Numeric args: number of elements, repeat count; synthetic data generated internally |
| bn | `./main result 10` | First arg is an OUTPUT file path to write results (not a required pre-existing input file); second is repeat count |
| channelShuffle | `./main 2 224 224 100` | Numeric args: batch, height, width, repeat count; synthetic data generated internally |
| channelSum | `./main 224 224 100` | Numeric args: height, width, repeat count; synthetic data generated internally |
| clink | `./main 10` | Numeric arg: repeat count; synthetic data generated internally |
| concat | `./main 1000` | Numeric arg: repeat count; synthetic data generated internally |
| crossEntropy | `./main 100` | Numeric arg: repeat count; synthetic data generated internally |
| dense-embedding | `./main 100000 256 1000` | Numeric args: number of rows, batch size, repeat count; synthetic embedding table generated internally |
| dropout | `./main 16777216 1000` | Numeric args: number of elements, repeat count; synthetic data generated internally |
| dwconv | `./main 128 16 224 224 100` | Numeric args: batch, channels, height, width, repeat count; synthetic data generated internally |
| dwconv1d | `python run.py` | PyTorch-based benchmark invoked via `python run.py` with no CLI args; generates synthetic tensors internally |
| expdist | `./main 16384 1000` | Numeric args: number of points, repeat count; synthetic data generated internally |
| flip | `./main 3 1024 100` | Numeric args: channels, size, repeat count; synthetic data generated internally |
| gelu | `./main 8 4096 512 100` | Numeric args: batch, sequence length, hidden size, repeat count; synthetic data generated internally |
| geglu | `./main 100` | Numeric arg: repeat count; synthetic data generated internally |
| glu | `./main 3 1024 100` | Numeric args: channels, size, repeat count; synthetic data generated internally |
| gru | `./main 102400 1024 100` | Numeric args: number of sequences, hidden size, repeat count; synthetic data generated internally |
| kalman | `./main 1000 100 3 100` | Numeric args: number of points, iterations, state dim, repeat count; synthetic data generated internally |
| knn | `./main 100` | Numeric arg: repeat count; synthetic data generated internally |
| layernorm | `./main 8 1024 768 2000` | Numeric args: batch, sequence length, hidden size, repeat count; synthetic data generated internally |
| lda | `./main 100` | Numeric arg: repeat count; synthetic data generated internally |
| lif | `./main 10000 32 30000` | Numeric args: number of neurons, batch size, repeat count; synthetic data generated internally |
| logprob | `./main 1024 8 50257 100` | Numeric args: sequence length, batch size, vocab size, repeat count; synthetic data generated internally |
| lrn | `./main 10` | Numeric arg: repeat count; synthetic data generated internally |
| mask | `./main 512 512 512 100` | Numeric args: dimensions, repeat count; synthetic data generated internally |
| matern | `./main 300 100` | Numeric args: size, repeat count; synthetic data generated internally |
| maxpool3d | `./main 2048 2048 96 100` | image width, image height, image count, repeat count (synthetic data) |
| mergeVS | `./main 1000` | repeat count (synthetic data) |
| mlp | `./main 3 128 784 512 10 0 0 100` | number of MLP layers, batch size, input feature size, hidden dimension, number of outputs, use relu flag, use bias flag, repeat count |
| moe | `./main 32768 384 1 1000` | number of tokens, number of experts, top K, repeat count (synthetic data) |
| moe-align | `./main 100000` | repeat count (synthetic data) |
| moe-sum | `./main 256 4096 100` | number of tokens, hidden size, repeat count (synthetic data) |
| mrc | `./main 10000000 1000` | number of elements, repeat count (synthetic data) |
| multinomial | `./main 65536 2048 100` | number of distributions, number of categories, repeat count (synthetic data) |
| nlll | `./main 2048 1000 1000` | minibatch size, number of classes, repeat count (synthetic data) |
| nonzero | `./main 2048 2048 100` | number of rows, number of columns, repeat count (synthetic data) |
| overlay | `./main 640 480` | image width, image height (synthetic data) |
| p4 | `./main 10000` | repeat count (synthetic data) |
| page-rank | `./main -n 20000 -i 100` | -n number of pages, -i max iterations (synthetic graph generated internally) |
| permute | `./main 8 100` | batch size, repeat count (synthetic data) |
| perplexity | `./main 10000 50 100` | number of points, perplexity, repeat count (synthetic data) |
| pointwise | `./main 100 8 512 64 10` | seqLength, numLayers, hiddenSize, miniBatch, repeat count (synthetic data) |
| pool | `./main 128 48 224 224 54 54 100` | batch, input channels, input height, input width, output height, output width, repeat count (synthetic data) |
| qkv | `./main 1` | kernel selector (1/2/3) choosing which QKV projection kernel variant to run (synthetic data) |
| qtclustering | `./main --Verbose` | --Verbose flag; problem size/passes options default and generate synthetic data internally |
| remap | `./main 10000000 1000` | number of elements, repeat count (synthetic data) |
| relu | `./main 10000000 100000` | element count, repeat count (synthetic data) |
| resnet-kernels | `./main 0 100` | mode selector, repeat count (synthetic data) |
| rmsnorm | `./main 1 12288 1000` | number of rows, number of columns, repeat count (synthetic data) |
| rowwiseMoments | `./main 128 128 128 128 4 100` | batch, channel, width, height, group, repeat count (synthetic data) |
| rotary | `./main 1000` | repeat count (synthetic data) |
| sampling | `./main 100` | repeat count (synthetic data) |
| scel | `./main 2048 50176 100` | outer size, inner size, repeat count (synthetic data) |
| silu | `./main 32 32 256 256 100` | batch size, number of channels, height, width, repeat count (synthetic data) |
| softmax | `./main 100000 784 0 100` | number of rows, number of columns, mode flag, repeat count (synthetic data) |
| softmax-fused | `./main 4 12 256 256 1000` | batch, heads, seq length, seq length, repeat count (synthetic data) |
| softmax-online | `./main 1` | kernel/mode selector (1-4) (synthetic data) |
| ssm | `./main 1000` | repeat count (synthetic data) |
| stddev | `./main 65536 16384 100` | D dimension, N count, repeat count (synthetic data) |
| streamcluster | `./streamcluster 10 20 256 65536 65536 1000 none output.txt 1` | dist, kmin, kmax, n_points-related params, point dimension, chunk size, nproc, "none" (no input file flag), output.txt (output, not input), num threads — "none" indicates synthetic/no external input file |
| tsne | `./main` | no arguments; fixed/synthetic data generated internally |
| unfold | `./main 10000000 1000` | number of elements, repeat count (synthetic data) |
| vol2col | `./main 1000` | repeat count (synthetic data) |
| wedford | `./main 512 512 8192 100` | batch_size, spatial_size, feature_size, repeat count (synthetic data) |
| winograd | `./main` | no arguments; fixed/synthetic data generated internally |

---

## Math

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| divergence | `./main input.txt 10000` | Plain-text numeric grid/field file | Path to bundled `input.txt` (present in this checkout) + repeat count |
| lanczos | `./main -g data/social-large-800k.txt -n 800000 -k 3 -d float` | Plain-text graph/sparse-matrix file | `-g <path>` graph file shipped compressed as `data.tar.bz` (DVC-tracked) — must be extracted first; `-n` nodes, `-k` iterations, `-d` data type |
| leukocyte | `./main ../data/leukocyte/testfile.avi 100` | AVI video file (cell-motion microscopy footage) | Path to `testfile.avi` + repeat count; file is NOT present in this checkout (`data/leukocyte/` dir absent) |
| log2 | `./main log2_parameters.config` | Plain-text parameter config file | Path to bundled `log2_parameters.config` (present in this checkout) |
| rsc | `./main -f input/vectors.csv -g 512` | CSV (comma-separated numeric vectors) | `-f <path>` bundled `input/vectors.csv` (present in this checkout); `-g` grid size |
| slu | `./main -i ./nicslu/demo/ASIC_100k.mtx` | Matrix Market (`.mtx`) sparse matrix file | `-i <path>` via `SLU_PATH` (defaults to `.`); demo `.mtx` dataset bundled under NICSLU third-party sources |
| sptrsv | `./main ../sptrsv-sycl/lp1.mtx 2000` | Matrix Market (`.mtx`) sparse triangular matrix | Sibling `lp1.mtx` shipped compressed as `lp1.mtx.tgz` in `sptrsv-sycl/` — not extracted in this checkout; + repeat count |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| atan2 | `./main 10000000 100` | Numeric size + repeat count; synthetic data |
| axpby | `./main 1000` | Repeat count; synthetic data |
| bgmv | `./main --op bgmv_shrink --tokens 1 --hidden 4096 --rank 16 --loras 4 --repeat 200` (and variants) | `--op`/`--tokens`/`--hidden`/`--rank`/`--loras`/`--repeat`/`--vectorize` numeric/string flags; synthetic LoRA weights |
| blas-dot | `./main 268435456 1000` | Numeric size + repeat count; synthetic vectors |
| blas-fp8gemm | `./main 100` | Repeat count; synthetic matrices |
| blas-gemm | `./main 79 91 83 1000` (and a 4096^3 variant) | Numeric matrix dims + repeat count; synthetic matrices |
| blas-gemmBatched | `./main` | No arguments; synthetic matrices |
| blas-gemmStridedBatched | `./main` | No arguments; synthetic matrices |
| blas-gemmEx | `./main 4096 4096 4096 100` (and an 8192^3 variant) | Numeric matrix dims + repeat count; synthetic matrices |
| blas-gemmEx2 | `./main 4096 4096 4096 100` (and an 8192^3 variant) | Numeric matrix dims + repeat count; synthetic matrices |
| braycurtis | `python tests/main.py` (via `run: build`) | Python script, no CLI args; synthetic tensors |
| complex | `./main 10000000 1000` | Numeric size + repeat count; synthetic data |
| cross | `./main 10000000 100` | Numeric size + repeat count; synthetic vectors |
| determinant | `./main 1000` | Repeat count; synthetic matrices |
| dp | `./main 268435456 1000` | Numeric size + repeat count; synthetic vectors |
| eigenvalue | `./main 2048 10000` | Numeric matrix size + repeat count; synthetic matrix |
| f16max | `./main 100` | Repeat count; synthetic data |
| f16sp | `./main 1000` | Repeat count; synthetic data |
| f8cast | `./main 268435456 100` | Numeric size + repeat count; synthetic data |
| fresnel | `./main 100` | Repeat count; synthetic data |
| fwt | `./main 100` | Repeat count; synthetic data |
| gaussian | `./main -q -t -s 4096` | `-s <size>` generates a random matrix internally (an alternate `-i <path>` flag exists but is not used by `run`); `-q`/`-t` are quiet/timing flags |
| geam | `./main 16384 16384 200` | Numeric matrix dims + repeat count; synthetic matrices |
| gels | `./main 1000` | Repeat count; synthetic least-squares system |
| gemv | `./main -s 16384 -i 100 -x 512 -y 2` (and other tile variants) | `-s` size, `-i` iterations, `-x`/`-y` tile dims; synthetic matrix/vector |
| hadamard | `./main 1024 100` (and a 32768 variant) | Numeric size + repeat count; synthetic vectors |
| hellinger | `./main 100` | Repeat count; synthetic distributions |
| hmm | `./main` | No arguments; synthetic hidden-Markov model data |
| idivide | `./main 100` | Repeat count; synthetic data |
| interval | `./main 0 100000` (and a mode-1 variant) | Mode flag + repeat count; synthetic interval data |
| jaccard | `./main 1024 512 1000` | Numeric dims + repeat count; synthetic sets |
| jacobi | `./main` | No arguments; synthetic linear system |
| kurtosis | `./main 100000000 100` | Numeric size + repeat count; synthetic data |
| langford | `./main` | No arguments; combinatorial search over fixed/internal sequence lengths |
| lci | `./main` | No arguments; synthetic data |
| lebesgue | `./main 1000000 2` | Numeric size + degree/order; synthetic data |
| lfib4 | `./main 2000000000` | Numeric size; synthetic lagged-Fibonacci RNG state |
| lud | `./main -s 8192` | `-s <size>` generates a random matrix internally |
| ludb | `./main 1000` | Repeat count; synthetic matrix |
| lut-gemm | `./main 1 100` | Numeric mode + repeat count; synthetic quantized weights |
| michalewicz | `./main 100000000 100` | Numeric size + repeat count; synthetic data (Michalewicz function benchmark) |
| matrix-rotate | `./main 5000 100` | Numeric matrix size + repeat count; synthetic matrix |
| matrixT | `./main 16384 16384 200` | Numeric matrix dims + repeat count; synthetic matrix |
| minkowski | `./main 100` | Repeat count; synthetic vectors |
| mr | `./main 100` | Repeat count; synthetic candidates (Miller-Rabin primality test) |
| mrg32k3a | `./main 1000000 10` | Numeric size + repeat count; synthetic RNG state |
| norm2 | `./main 100` | Repeat count; synthetic vector |
| nqueen | `./main 15 7 100` | Board size, thread param, repeat count; synthetic search |
| ntt | `./main 100000` | Numeric size; synthetic polynomial coefficients |
| phmm | `./main 1000` | Repeat count; synthetic profile-HMM data |
| pnpoly | `./main 100` | Repeat count; synthetic polygon/points |
| quant3MatMul | `./main 1000` | Repeat count; synthetic quantized matrices |
| reverse2D | `./main 8192 8192 100` | Numeric dims + repeat count; synthetic matrix |
| rfs | `./main 100 5000000` | Repeat count + size; synthetic data |
| romberg | `./main 128 64 1000` | Numeric grid dims + repeat count; synthetic data |
| sddmm-batch | `./main 32 1024 1024 1024 1024 1000 1` (and a 4096-dim variant) | Numeric batch/matrix dims, repeat count, flag; synthetic sparse matrices |
| secp256k1 | `./main 100` | Repeat count; synthetic elliptic-curve keys |
| simpleSpmv | `./main 16777216 10240 100` | Numeric size, nnz, repeat count; synthetic sparse matrix |
| spd2s | `./main 20082 20082 150616 1000` (and a larger variant) | Numeric rows, cols, nnz, repeat count; synthetic sparse matrix |
| spgeam | `./main 20082 20082 150616 1000 1` | Numeric rows, cols, nnz, repeat count, flag; synthetic sparse matrices |
| spgemm | `./main 1024 1024 1024 1024 1000 1` (and a 4096-dim variant) | Numeric matrix dims, repeat count, flag; synthetic sparse matrices |
| spmm | `./main 1024 1024 1024 1024 1024 1000 1` (and a 4096-dim variant) | Numeric matrix dims, repeat count, flag; synthetic sparse matrices |
| spmv | `./main 16777216 10240 100` | Numeric size, nnz, repeat count; synthetic sparse matrix |
| spnnz | `./main 20082 20082 150616 1000` (and a larger variant) | Numeric rows, cols, nnz, repeat count; synthetic sparse matrix |
| sps2d | `./main 20082 20082 150616 1000` (and a larger variant) | Numeric rows, cols, nnz, repeat count; synthetic sparse matrix |
| spsort | `./main 20082 20082 150616 1000` (and a larger variant) | Numeric rows, cols, nnz, repeat count; synthetic sparse matrix |
| thomas | `./main 1024 16384 64 100` | Numeric system count, size, block size, repeat count; synthetic tridiagonal system |
| wyllie | `./main 8000000 1 100` | Numeric size, mode, repeat count; synthetic linked list |
| zeropoint | `./main 10000000 100` | Numeric size + repeat count; synthetic quantization data |

---

## Random number generation

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| permutate | `./main test_data/truerand_1bit.bin` | Binary bitstream file (NIST SP 800-90B test data) | Path to `truerand_1bit.bin` |
| urng | `./main ../urng-sycl/URNG_Input.bmp 1000` | `.bmp` bitmap image | Path to bitmap image + repeat count |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| mt | `./main 1000` | Repeat count |
| qrg | `./main 100000` | Number of quasirandom points |
| rng-wallace | `./main 1000` | Repeat count |
| sobol | `./main 1000000 1000 100` | Numeric dimensions, path count, repeat count |

---

## Search

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| bfs | `./main ../data/bfs/graph1MW_6.txt` | Plain-text graph adjacency-list file (Rodinia BFS format) | Path to graph file |
| b+tree | `./main file ../data/b+tree/mil.txt command ../data/b+tree/command.txt` | Plain-text B+tree record file + plain-text command script file | `file <path>` and `command <path>` positional args pointing at two bundled text files |
| grep | `./runtests.sh` | Shell script wrapping text pattern-matching test cases | Runs `runtests.sh`, which internally exercises bundled text inputs against the regex-matching binary |
| ss | `./main ../ss-sycl/StringSearch_Input.txt clEnqueueNDRangeKernel 20000` | Plain-text string-search corpus | Path to input text file, kernel-name string, repeat count |
| sss | `GSL_RNG_SEED=123 ./main f9_n150_p50` | Plain-text dataset files referenced by a name prefix (`DATA/f9_n150_p50.txt` + `f9_n150_p50_init{1,2,3}.txt`) | Dataset-name-prefix positional arg; `GSL_RNG_SEED` env var sets the RNG seed |
| tsp | `./main d493.tsp 24 100` | TSPLIB plain-text city-coordinate format (`.tsp`) | Path to `.tsp` file, thread/city count, repeat count |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| bsearch | `./main 8388608 100` | Array size + repeat count; synthetic data |
| keogh | `./main 256 20000000 100` | Query length, data length, repeat count; synthetic data |
| s8n | `./main 1513 2048 100` | Numeric size args; synthetic data |

---

## Signal processing

### External File Input: YES

*No entries in this category.*

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| extrema | `./main 100` | Repeat count |
| fft | `./main 3 100` | log2(size) + repeat count |
| lombscargle | `./main 100` | Repeat count |
| sosfil | `./main 100` | Repeat count |
| zmddft | `./main 100` | Repeat count |

---

## Simulation

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| cmp | `./main -c0 1.98e-7 -c1 1.77e-6 -nc 5 -aph 600 -tau 0.002 -v 2 -i ./data/simple-synthetic.su` | Seismic Unix (`.su`) seismic trace format | `-i <path>` to `simple-synthetic.su`; file NOT present in this checkout (`data/` dir absent) |
| d2q9_bgk | `./main Inputs/input_256x256.params Obstacles/obstacles_256x256.dat` | Plain-text params file + plain-text obstacle-grid data file | Two bundled input file paths (lattice-Boltzmann D2Q9 solver) |
| easyWave | `./main -grid ../easyWave-omp/data/grids/e2Asean.grd -source ../easyWave-omp/data/faults/BengkuluSept2007.flt -time 120` | Custom binary tsunami-simulation grid (`.grd`) + fault-source file (`.flt`) | `-grid`/`-source` sibling-dir paths (both present in this checkout under `easyWave-omp/data/`); `-time` simulation duration |
| gibbs | `./main data.csv 5000` | CSV (numeric sample data) | Path to `data.csv`; file NOT present in this checkout |
| henry | `./main IRMOF-1.cssr 1000` | CSSR crystal-structure format | Path to `IRMOF-1.cssr`; file NOT present in this checkout |
| hotspot | `./main 512 2 200 ../data/hotspot/temp_512 ../data/hotspot/power_512 output_512.out` | Plain-text temperature/power grid files | Shipped compressed as `hotspot.tar.bz` (DVC-tracked) — must be extracted first |
| hotspot3D | `./main 512 8 5000 ../data/hotspot3D/power_512x8 ../data/hotspot3D/temp_512x8 output.out` | Plain-text temperature/power grid files | Shipped compressed as `hotspot3D.tar.bz` (DVC-tracked) — must be extracted first |
| hpl | `mpirun -n 1 ./xhpl` (after copying `datafiles/HPL_small_gpu.dat` to `HPL.dat`) | HPL benchmark config (`HPL.dat`) | Bundled `datafiles/HPL_small_gpu.dat`, copied into place before running; requires `mpirun` |
| logic-resim | `./main toy/intermediate.file toy/NV_NVDLA_partition_o_dc_24x33x55_5x5x55x25_int8_input.vcd 0 2972036001 toy-result.saif` | Custom intermediate netlist file + VCD (Value Change Dump) waveform file | Bundled paths under `toy/` (present in this checkout); numeric time-window args + output `.saif` path |
| logic-rewrite | `./main -c "read ./benchmarks/arithmetic/hyp.aig; resyn2; write hyp_resyn2.aig"` | AIGER binary And-Inverter-Graph format (`.aig`) | `-c "<script>"` ABC-style command script referencing bundled `hyp.aig` |
| lsqt | `./main examples/input.txt` | Plain-text simulation parameter file | Path to `examples/input.txt`; shipped compressed as `examples.tar.gz` (DVC-tracked) — must be extracted first |
| mcmd | `cd dataset && ../main mcmd.inp` | Plain-text Monte Carlo input/config file | Bundled `dataset/mcmd.inp` (present in this checkout) |
| sw4ck | `./main sw4ck.in 100` | Plain-text seismic-wave config file | Bundled `sw4ck.in` (present in this checkout) + repeat count |
| tpacf | `./main -d ./data/small/Datapnts.1 -p <POINTS> -r ./data/small/Randompnts -n 100 -q <POINTS> -b 5 -l 1 -u 10000 -a -j 10 -o ./small_results_test.dat` | Custom binary/text point-distribution data files | `-d`/`-r` paths to `Datapnts.1`/`Randompnts`; entire `data/` directory is absent in this checkout |
| tqs | `./main -f input/patternsNP100NB512FB25.txt` | Plain-text pattern file | `-f <path>` to bundled `patternsNP100NB512FB25.txt` (present in this checkout) |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| ace | `./main 100` | Repeat count; synthetic data |
| adv | `./main 7 15 8000 100` | Numeric dims/iteration args + repeat count; synthetic advection field |
| amgmk | `./main` | No arguments; synthetic AMG matrix |
| axhelm | `./main 1 8000 100` (and a mode-3 variant) | Numeric mode, elements, repeat count; synthetic mesh |
| bh | `./main 100000 100` | Number of bodies + repeat count; synthetic N-body data (Barnes-Hut) |
| bspline-vgh | `./main` | No arguments; synthetic spline data |
| burger | `./main 8200 8100 100` | Numeric grid dims + repeat count; synthetic Burgers'-equation field |
| cooling | `./main 1000000 1000` | Numeric size + repeat count; synthetic gas-cooling data |
| ccsd-trpdrv | `./main 160 400 100` | Numeric occupied/virtual orbital counts + repeat count; synthetic CCSD tensors |
| che | `./main 1000` (preceded by `rm -rf out; mkdir out`) | Repeat count; writes outputs to `out/` (not a required input) |
| chemv | `./main` | No arguments; synthetic chemical kinetics data |
| chi2 | `./main 4000 400000 2000 2000 256 1000` | Numeric size/dim args + repeat count; synthetic histogram data |
| clenergy | `./main` | No arguments; synthetic electrostatic grid |
| cobahh | `./main 1000000 10` | Numeric neuron count + repeat count; synthetic Hodgkin-Huxley network |
| d3q19_bgk | `./main 128` | Numeric grid size; synthetic lattice-Boltzmann D3Q19 field |
| damage | `./main 50000000 1000` | Numeric size + repeat count; synthetic material-damage field |
| ddbp | `./main` | No arguments; synthetic density-functional data |
| dslash | `./main 256` | Numeric lattice size; synthetic lattice-QCD field |
| eikonal | `./main -s 512` | `-s <size>` generates a synthetic speed field internally |
| fdtd3d | `./main --dimx=192 --dimy=184 --timesteps=90` | Numeric grid dims + timestep count; synthetic FDTD field |
| feynman-kac | `./main 10` | Repeat count; synthetic Monte Carlo paths |
| fhd | `./main 100000 2097152 0` | Numeric size args + mode flag; synthetic MRI field-map data |
| fluidSim | `./main 10000` | Repeat count; synthetic lattice-Boltzmann fluid field |
| goulash | `./main 101 5` | Numeric size args; synthetic data |
| gpp | `./main benchmark` | `benchmark`/`test` mode-selector string (not a file path); synthetic GW (general plasmon pole) data |
| grrt | `./main` | No arguments; synthetic general-relativistic ray-tracing data |
| haccmk | `./main 1000` | Repeat count; synthetic N-body particle data |
| halo-finder | `./cuda/ForceTreeTest 0.5 0.1 10000 0.1 10 N 12 rcb` | Numeric physics/tree parameters + decomposition mode; synthetic particle data |
| heartwall | `./main 104` | Repeat count (frame count); synthetic/internal cardiac image sequence |
| heat | `./main 4096 1000` | Numeric grid size + repeat count; synthetic heat field |
| heat2d | `./main 4096 4096 1000` | Numeric grid dims + repeat count; synthetic heat field |
| hexicton | `./main` | No arguments; synthetic vibronic-coupling data (dir is `hexciton-cuda`, README has a transposed-letter typo) |
| hwt1d | `./main 8388608 100` | Numeric size + repeat count; synthetic 1D signal |
| hypterm | `./main 100` | Repeat count; synthetic hyperbolic-term field |
| ising | `./main -x 5120 -y 5120 -w 10 -n 1000` | Numeric lattice dims, warmup, iteration count; synthetic spin lattice |
| iso2dfd | `./main 2048 2048 1000` | Numeric grid dims + repeat count; synthetic seismic wavefield |
| laplace | `./main` | No arguments; synthetic Laplace-equation grid |
| laplace3d | `./main 128 128 128 100 1` (and a 512^3 variant) | Numeric grid dims, repeat count, flag; synthetic 3D field |
| lavaMD | `./main -boxes1d 30` | `-boxes1d <n>` synthetic molecular-dynamics box grid |
| lid-driven-cavity | `./main` | No arguments; synthetic cavity-flow grid |
| loopback | `./main 0 100` | Mode flag + repeat count; synthetic options-pricing data |
| lulesh | `./main -i 100 -s 128 -r 11 -b 1 -c 1` (run twice) | `-i` iterations, `-s` mesh size, `-r` regions, `-b` balance, `-c` cost; synthetic Lagrangian hydrodynamics mesh |
| md | `./main 3 1000` | Numeric mode/dim args + repeat count; synthetic molecular-dynamics data |
| mdh | `./main -itmax 100` | `-itmax <n>` iteration count; synthetic data |
| metropolis | `./main -l 32 11 -t 4.7 0.1 -a 10 2 2000 10 -h 1.0 -z 7919` | Numeric lattice/temperature/annealing/seed flags; synthetic Ising lattice (Metropolis Monte Carlo) |
| miniFE | `./main -nx 128 -ny 128 -nz 128` | Numeric grid dims; synthetic finite-element mesh |
| minimod | `./main --warm-up --niters 1 --grid 256 --nsteps 1000` | Numeric grid/step args + warm-up flag; synthetic seismic wavefield |
| minisweep | `./main --niterations 100` (and a detailed-params variant) | Numeric iteration/cell/angle args; synthetic neutral-particle-transport data |
| miniWeather | `./main` | No arguments; synthetic atmospheric weather field |
| multimaterial | `./multimat_FL 2000 2000` (and `multimat_F`/`multimat` variants) | Numeric grid dims (+ optional fraction params); synthetic multi-material field |
| mxfp4 | `./main 536870912 100` | Numeric size + repeat count; synthetic quantized data |
| myocte | `./main 100` (dir is `myocyte-cuda`) | Repeat count; synthetic cardiac myocyte model |
| nbody | `./main 16000 10` | Number of bodies + repeat count; synthetic N-body data |
| particle-diffusion | `./main 2000 100` | Numeric size + repeat count; synthetic particle field |
| particlefilter | `./main -x 128 -y 128 -z 10 -np 400000` | Numeric grid dims + particle count; synthetic video frames generated internally |
| particles | `./main 10000` | Repeat count; synthetic particle system |
| pathfinder | `./main 100000 1000 5` | Numeric width, height/rows, block size; synthetic elevation grid |
| pns | `./main 22 100000 1000000` | Numeric size/iteration args; synthetic data |
| projectile | `./main 1000` | Repeat count; synthetic projectile-motion data |
| pso | `./main 30 10000` | Numeric particle count + iteration count; synthetic particle-swarm-optimization data |
| qem | `./main 100` | Repeat count; synthetic mesh (quadric error metrics) |
| rainflow | `./main 100000 100` | Numeric size + repeat count; synthetic load/stress signal |
| rayleighBenardConvection | `./main 1000` | Repeat count; synthetic convection field |
| reaction | `./main 10000` | Repeat count; synthetic reaction-diffusion field |
| rsbench | `./main -s large -m event` | `-s`/`-m` size and mode flags; synthetic nuclear cross-section data |
| rtm8 | `./main 100` | Repeat count; synthetic seismic reverse-time-migration field |
| rushlarsen | `./main 100 100000` | Repeat count + size; synthetic cardiac-cell ODE data |
| s3d | `./main -q -n 100 -s 1` | `-q` quiet, `-n` iterations, `-s` size selector; synthetic combustion chemistry data |
| su3 | `./main -i 1000 -l 32 -t 128 -v 3 -w 1` | Numeric iteration/lattice/threading flags; synthetic SU(3) lattice-QCD field |
| sheath | `./main` | No arguments; synthetic plasma-sheath data |
| simplemoc | `./main -s 5000000 -e 128 -n 10` | Numeric segments, energy groups, iterations; synthetic method-of-characteristics data |
| slit | `./main 1024 1000` | Numeric size + repeat count; synthetic diffraction field |
| sparkler | `mpirun -n 4 ./main --num_vector 4000 --num_field 90000 --num_iterations 10` | Numeric vector/field/iteration counts; requires `mpirun` but no external data file; synthetic data |
| sph | `./main` | No arguments; synthetic smoothed-particle-hydrodynamics field |
| tensorT | `./main 100` | Repeat count; synthetic tensor data |
| testSNAP | `./main --nsteps 100` | `--nsteps <n>`; synthetic SNAP (spectral neighbor analysis potential) data |
| tissue | `./main 32 100` | Numeric size + repeat count; synthetic tissue-simulation data |
| tridiagonal | `./main -num_systems=524288` (and `--lmem`/`--vec4` variants) | `-num_systems=<n>` + optional memory-mode flags; synthetic tridiagonal systems |
| tsa | `./main 1024 1024 100` | Numeric dims + repeat count; synthetic time-series data |
| vanGenuchten | `./main 256 256 256 1000` | Numeric grid dims + repeat count; synthetic soil-moisture field (van Genuchten model) |
| vmc | `./main 100` | Repeat count; synthetic variational-Monte-Carlo wavefunction |
| wlcpow | `./main 1000` | Repeat count; synthetic worm-like-chain polymer data |
| wsm5 | `./main 10` | Repeat count; synthetic WSM5 microphysics atmospheric data |
| xlqc | `./main sp` (and a `dp` variant) | Precision-mode flag (single/double); synthetic quantum-chemistry (Hartree-Fock) molecule data |
| xsbench | `./main -s large -m event -r 10` | `-s`/`-m`/`-r` size, mode, repeat flags; synthetic nuclear cross-section data (XSBench) |

---

## Sorting

### External File Input: YES

*No entries in this category.*

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| bitonic-sort | `./main 25 2` | log2(size) + repeat count |
| hybridsort | `./main r` | `argv[1]=="r"` selects internally-generated random data path (the alternative branch would `fopen(argv[1])` to read a file, but `r` is not a path) |
| is | `./main 256 256 256` | Numeric problem-size class args; synthetic data |
| merge | `./main 100000 100` | Numeric size + repeat count |
| quicksort | `./main 10 2048 2048` | Numeric repeat/array-size args; synthetic data |
| radixsort | `./main 1000` | Repeat count |
| segsort | `./main` | No arguments; synthetic data |
| sort | `./main 3 100` | log2(size) + repeat count |
| sortKV | `./main 1000000 100` | Numeric size + repeat count |
| split | `./main 33554432 1000` | Numeric size + repeat count |
| topk | `./main 3072 1000` | Numeric size + repeat count |
| warpsort | `./main 1000` | Repeat count |

---

## Robotics

### External File Input: YES

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| inversek2j | `./main ../inversek2j-sycl/coord_in.txt 100000` | Plain-text list of 2D (x,y) coordinates | Path to `coord_in.txt` + repeat count |

### External File Input: NO

| Project Name | `make run` Command | Notes/Arguments |
|---|---|---|
| rodrigues | `./main 1000000 10000` | Numeric size + repeat count; synthetic data |

---

## Summary

### Totals

| Section | Count |
|---|---:|
| **YES** (requires external file) | **86** |
| **NO** (no external file) | **419** |
| **Total** | **505** |

### Per Category

| Category | YES | NO | Subtotal |
|---|---:|---:|---:|
| Automotive | 1 | 0 | 1 |
| Bandwidth | 0 | 15 | 15 |
| Bioinformatics | 12 | 9 | 21 |
| Computer vision and image processing | 14 | 40 | 54 |
| Cryptography | 3 | 6 | 9 |
| Data compression and reduction | 3 | 29 | 32 |
| Data encoding, decoding, or verification | 0 | 10 | 10 |
| Finance | 0 | 5 | 5 |
| Geoscience | 3 | 3 | 6 |
| Graph and Tree | 6 | 3 | 9 |
| Language and kernel features | 0 | 44 | 44 |
| Machine learning | 13 | 76 | 89 |
| Math | 7 | 69 | 76 |
| Random number generation | 2 | 4 | 6 |
| Search | 6 | 3 | 9 |
| Signal processing | 0 | 5 | 5 |
| Simulation | 15 | 85 | 100 |
| Sorting | 0 | 12 | 12 |
| Robotics | 1 | 1 | 2 |
| **Total** | **86** | **419** | **505** |
