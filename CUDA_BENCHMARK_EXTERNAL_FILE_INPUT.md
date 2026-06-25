# CUDA Benchmarks Requiring External File Input

Benchmarks extracted from `CUDA_BENCHMARK_INPUT_SPLIT.md` that have **External File Input: YES** — i.e. their `make run` invocation requires reading an external data file (not just numeric/string CLI arguments or synthetic/internally-generated data).

**Total: 86 benchmarks across 13 categories.**

---

## Automotive

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| daphne | `./points2image -p 1` (run from `daphne-cuda/src/points2image/`; top-level dir has no Makefile) | Binary testcase data (`p2i_input.dat`, `p2i_output.dat`), read via `ifstream` in binary mode | Hardcoded relative paths `../../data/p2i_input.dat`/`p2i_output.dat`; data files are NOT bundled in this checkout — `daphne-cuda/data/README` points to https://github.com/esa-tu-darmstadt/daphne-benchmark for the actual data; `-p 1` is a benchmark/precision selector flag |

---

## Bioinformatics

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

---

## Computer vision and image processing

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

---

## Cryptography

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| aes | `./aes 100 0 ../urng-sycl/URNG_Input.bmp` / `./aes 100 1 ../urng-sycl/URNG_Input.bmp` | `.bmp` bitmap image | `<iterations> <0=encrypt\|1=decrypt> <path to bitmap image>` |
| bitcracker | `./bitcracker -f ./hash_pass/img_win8_user_hash.txt -d ./hash_pass/user_passwords_60000.txt -b 60000` | Plain-text hash file + plain-text password dictionary | `-f <hash_file>`, `-d <dictionary_file>`, `-b <count>`; files must be generated via bundled `hash_pass/password_generator.py` |
| columnarSolver | `./main data` | Plain-text bigram table (`<bigram> <score>` per line) | Path to bigram-score data file, shipped compressed as `data.tar.gz` |

---

## Data compression and reduction

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| dxtc2 | `./main ../dxtc2-sycl/data/lena_std.ppm ../dxtc2-sycl/data/lena_ref.dds 100` (and a teapot variant) | `.ppm` input image + `.dds` reference compressed-texture file | Path to source `.ppm`, path to reference `.dds`, repeat count |
| lzss | `./main -i QCLOUDf48.bin.f32.errctrl.1e-3` | Binary scientific dataset (SZ/ZFP-style error-controlled float32 data) | `-i <inputfile>`; sample data NOT bundled in this checkout — README points to https://github.com/hpdps-group/ICS23-GPULZ |
| mpc | `./main msg_sp.trace.out 1` (compression) / `./main compression.txt` (decompression) | Plain-text message trace file / compressed-text intermediate file | Trace/text files bundled compressed as `trace.tar.bz` (tracked via `trace.tar.bz.dvc`) — must be extracted before running |

---

## Geoscience

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| geodesic | `./main ../geodesic-sycl/locations.txt 1000` | Plain-text latitude/longitude coordinate list | Path to `locations.txt` + repeat count |
| haversine | `./main ../geodesic-cuda/locations.txt 100` | Plain-text latitude/longitude coordinate list (reuses `geodesic-cuda`'s file) | Path to `locations.txt` + repeat count |
| stsg | `./main example.txt` | Plain-text time-series/parameter config file (comma-separated yearly data) | Path to `example.txt` config/data file |

---

## Graph and Tree

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| cc | `./main delaunay_n24.egr 100` | ECL binary graph format (`.egr`) | Path to `.egr` graph file (not bundled in this checkout — must be obtained separately) + repeat count |
| floydwarshall2 | `./main CollegeMsg.egr 1000` | ECL binary graph format (`.egr`) | Path to bundled `CollegeMsg.egr` + repeat count |
| gc | `./main ../mis-cuda/internet.egr 100` | ECL binary graph format (`.egr`) | Reuses `mis-cuda`'s bundled `internet.egr` + repeat count |
| hbc | `./main -i ./graphs/breast_coexpress.txt -v --printscores=output.txt` | Plain-text edge-list graph file | `-i <path>` input graph, `-v` verbose, `--printscores=<path>` output path |
| mis | `./main internet.egr 100` | ECL binary graph format (`.egr`) | Path to bundled `internet.egr` + repeat count |
| rsmt | `./main newblue7.kraftwerk70.3d.80.20.82.m8.gr` | Plain-text ISPD global-routing benchmark format (`.gr`) | Path to bundled `.gr` routing-grid file |

---

## Machine learning

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

---

## Math

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| divergence | `./main input.txt 10000` | Plain-text numeric grid/field file | Path to bundled `input.txt` (present in this checkout) + repeat count |
| lanczos | `./main -g data/social-large-800k.txt -n 800000 -k 3 -d float` | Plain-text graph/sparse-matrix file | `-g <path>` graph file shipped compressed as `data.tar.bz` (DVC-tracked) — must be extracted first; `-n` nodes, `-k` iterations, `-d` data type |
| leukocyte | `./main ../data/leukocyte/testfile.avi 100` | AVI video file (cell-motion microscopy footage) | Path to `testfile.avi` + repeat count; file is NOT present in this checkout (`data/leukocyte/` dir absent) |
| log2 | `./main log2_parameters.config` | Plain-text parameter config file | Path to bundled `log2_parameters.config` (present in this checkout) |
| rsc | `./main -f input/vectors.csv -g 512` | CSV (comma-separated numeric vectors) | `-f <path>` bundled `input/vectors.csv` (present in this checkout); `-g` grid size |
| slu | `./main -i ./nicslu/demo/ASIC_100k.mtx` | Matrix Market (`.mtx`) sparse matrix file | `-i <path>` via `SLU_PATH` (defaults to `.`); demo `.mtx` dataset bundled under NICSLU third-party sources |
| sptrsv | `./main ../sptrsv-sycl/lp1.mtx 2000` | Matrix Market (`.mtx`) sparse triangular matrix | Sibling `lp1.mtx` shipped compressed as `lp1.mtx.tgz` in `sptrsv-sycl/` — not extracted in this checkout; + repeat count |

---

## Random number generation

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| permutate | `./main test_data/truerand_1bit.bin` | Binary bitstream file (NIST SP 800-90B test data) | Path to `truerand_1bit.bin` |
| urng | `./main ../urng-sycl/URNG_Input.bmp 1000` | `.bmp` bitmap image | Path to bitmap image + repeat count |

---

## Search

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| bfs | `./main ../data/bfs/graph1MW_6.txt` | Plain-text graph adjacency-list file (Rodinia BFS format) | Path to graph file |
| b+tree | `./main file ../data/b+tree/mil.txt command ../data/b+tree/command.txt` | Plain-text B+tree record file + plain-text command script file | `file <path>` and `command <path>` positional args pointing at two bundled text files |
| grep | `./runtests.sh` | Shell script wrapping text pattern-matching test cases | Runs `runtests.sh`, which internally exercises bundled text inputs against the regex-matching binary |
| ss | `./main ../ss-sycl/StringSearch_Input.txt clEnqueueNDRangeKernel 20000` | Plain-text string-search corpus | Path to input text file, kernel-name string, repeat count |
| sss | `GSL_RNG_SEED=123 ./main f9_n150_p50` | Plain-text dataset files referenced by a name prefix (`DATA/f9_n150_p50.txt` + `f9_n150_p50_init{1,2,3}.txt`) | Dataset-name-prefix positional arg; `GSL_RNG_SEED` env var sets the RNG seed |
| tsp | `./main d493.tsp 24 100` | TSPLIB plain-text city-coordinate format (`.tsp`) | Path to `.tsp` file, thread/city count, repeat count |

---

## Simulation

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

---

## Robotics

| Project Name | `make run` Command | File Format | Notes/Arguments |
|---|---|---|---|
| inversek2j | `./main ../inversek2j-sycl/coord_in.txt 100000` | Plain-text list of 2D (x,y) coordinates | Path to `coord_in.txt` + repeat count |

---

## Summary by Category

| Category | Count |
|---|---:|
| Automotive | 1 |
| Bioinformatics | 12 |
| Computer vision and image processing | 14 |
| Cryptography | 3 |
| Data compression and reduction | 3 |
| Geoscience | 3 |
| Graph and Tree | 6 |
| Machine learning | 13 |
| Math | 7 |
| Random number generation | 2 |
| Search | 6 |
| Simulation | 15 |
| Robotics | 1 |
| **Total** | **86** |
