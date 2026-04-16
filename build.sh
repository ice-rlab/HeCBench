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

bio=(
  all-pairs-distance
  bsw
  ccs
  # cm
  deredundancy
  diamond
  epistasis
  extend2
  frna
  fsm
  ga
  local-ht
  logan
  minibude
  minimap2
  nbnxm
  nw
  pcc
  prna
  sa
  snake
)

crypto=(
  aes
  bitcracker
  bitpermute
  chacha20
  columnarSolver
  ecdh
  keccaktreehash
  merkle
  present  
)

benchmark=()
benchmark+=(
    "${graph[@]}"
    "${search[@]}"
    "${bio[@]}"
    "${crypto[@]}"
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
    if [[ "${name}" == "local-ht" ]]; then
      cd src
    fi
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
