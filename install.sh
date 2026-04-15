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

benchmark=()
benchmark+=(
    "${graph[@]}"
)

install_benchmark_data() {
  local name="$1"
  local benchmark_dir="${src_dir}/${name}-cuda"

  if [[ ! -d "${benchmark_dir}" ]]; then
    echo "Skipping ${name}: missing ${benchmark_dir}" >&2
    return 1
  fi

  if [[ -f "${benchmark_dir}/install.sh" ]]; then
    echo "==> Running install.sh for ${name}-cuda"
    (
      cd "${benchmark_dir}"
      bash ./install.sh
    )
  fi
}

failures=()

for name in "${benchmark[@]}"; do
  if ! install_benchmark_data "${name}"; then
    failures+=("${name}")
  fi
done

if (( ${#failures[@]} > 0 )); then
  printf 'Failed benchmarks: %s\n' "${failures[*]}" >&2
  exit 1
fi

echo "Installed data for ${#benchmark[@]} benchmark(s)."
