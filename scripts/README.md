# HeCBench Sanitizer Scripts

Run NVIDIA [Compute Sanitizer](https://developer.nvidia.com/compute-sanitizer) on CUDA benchmarks that need no external input files. Command definitions live in [`CUDA_COMPUTE_SANITIZER_COMMANDS.md`](../CUDA_COMPUTE_SANITIZER_COMMANDS.md).

## Quick start

```bash
# Regenerate manifest after editing the markdown
python3 scripts/parse_sanitizer_commands.py

# Run sanitizer (407 standard benchmarks by default)
python3 scripts/run_sanitizer.py --category Bandwidth --tool memcheck
```

**Requires:** Python 3.8+, `nvcc`, `compute-sanitizer` on `PATH`. Set `ARCH` (default `sm_80`) for your GPU. MPI benchmarks need `mpirun`.

## Scripts

### `parse_sanitizer_commands.py`

> `CUDA_COMPUTE_SANITIZER_COMMANDS.md` => `sanitizer_commands.json`

| Flag | Default |
|---|---|
| `-i` / `--input` | `CUDA_COMPUTE_SANITIZER_COMMANDS.md` |
| `-o` / `--output` | `sanitizer_commands.json` |

### `run_sanitizer.py`

> build each benchmark, run sanitizer, write reports

Build step: `make ARCH=<arch> EXTRA_CFLAGS="-lineinfo"`

| Flag | Default | Notes |
|---|---|---|
| `-m` / `--manifest` | `sanitizer_commands.json` | |
| `-o` / `--output-dir` | `san-reports/` | |
| `--tool` | `memcheck` | `memcheck`, `racecheck`, `synccheck`, `initcheck` |
| `--all` | off | Run all benchmarks (excludes special-handling unless `--include-special`) |
| `--category`, `--project` | — | Repeatable filters (required unless `--all`) |
| `--include-special` | off | MPI, Python, and other special cases (12 benchmarks) |
| `--arch` | `$ARCH` or `sm_80` | |
| `--skip-build` | off | |
| `--dry-run` | off | Print planned runs only |

## Output

```text
san-reports/
  summary.json
  summary.tsv
  <Category>/<project>/   # only when a run fails
    <tool>.log
    <tool>.stdout.log
```

Logs are kept only for non-`PASS` runs (`SANITIZER_ERROR`, `TIMEOUT`, `RUN_FAILED`). Summaries always list every run.

## Examples

```bash
python3 scripts/run_sanitizer.py --all --tool memcheck
python3 scripts/run_sanitizer.py --project triad --tool memcheck,racecheck
python3 scripts/run_sanitizer.py --include-special --project allreduce
python3 scripts/run_sanitizer.py --dry-run --project cm
```

Use `--help` on either script for all options.
