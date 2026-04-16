#!/usr/bin/env bash
set -euo pipefail

src_dir="src"

arch="${ARCH:-sm_80}"

graph=(
  cc
  floydwarshall
  floydwarshall2
  gc
  hbc
  hungarian
  mis
  sssp
  rsmt
)

search=(
  bfs
  bsearch
  b+tree
  grep
  keogh
  s8n
  ss
  sss
  tsp
)

benchmark=()
benchmark+=(
    "${graph[@]}"
    "${search[@]}"
)

build_benchmark() {
  local name="$1"
  local benchmark_dir="${src_dir}/${name}-cuda"

  if [[ ! -d "${benchmark_dir}" ]]; then
    echo "Skipping ${name}: missing ${benchmark_dir}" >&2
    return 1
  fi

  echo "==> Building ${name}-cuda (ARCH=${arch})"

  (
    cd "${benchmark_dir}"
    make ARCH="${arch}"
  )
}

failures=()

for name in "${benchmark[@]}"; do
  if ! build_benchmark "${name}"; then
    failures+=("${name}")
  fi
done

if (( ${#failures[@]} > 0 )); then
  printf 'Failed benchmarks: %s\n' "${failures[*]}" >&2
  exit 1
fi

echo "Built ${#benchmark[@]} benchmark(s) with ARCH=${arch}."
