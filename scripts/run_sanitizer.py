#!/usr/bin/env python3
"""Build and run CUDA benchmarks under NVIDIA Compute Sanitizer using a JSON manifest."""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any


VALID_TOOLS = ("memcheck", "racecheck", "initcheck", "synccheck")


@dataclass
class RunResult:
    category: str
    project: str
    tool: str
    status: str
    exit_code: int | None
    duration_sec: float
    log_file: str
    stdout_log: str
    build_ok: bool
    special_handling: bool
    command: str
    variant: str | None = None
    message: str | None = None


def repo_root() -> Path:
    return Path(__file__).resolve().parent.parent


def safe_path_component(name: str) -> str:
    return re.sub(r"[^\w.\-+]+", "_", name.strip())


def load_manifest(path: Path) -> dict[str, Any]:
    with path.open(encoding="utf-8") as fh:
        return json.load(fh)


def filter_benchmarks(
    benchmarks: list[dict[str, Any]],
    categories: list[str] | None,
    projects: list[str] | None,
    exclude_special: bool,
) -> list[dict[str, Any]]:
    selected: list[dict[str, Any]] = []
    for entry in benchmarks:
        if exclude_special and entry.get("special_handling"):
            continue
        if categories and entry["category"] not in categories:
            continue
        if projects and entry["project"] not in projects:
            continue
        selected.append(entry)
    return selected


def inject_sanitizer_flags(command: str, tool: str, log_file: Path) -> str:
    marker = "compute-sanitizer"
    idx = command.find(marker)
    if idx == -1:
        raise ValueError(f"command does not contain compute-sanitizer: {command!r}")
    before = command[: idx + len(marker)]
    after = command[idx + len(marker) :]
    return f"{before} --tool {tool} --log-file {log_file}{after}"


def expand_sanitizer_commands(command: str) -> list[tuple[str, str | None]]:
    """Return (command, variant_label) pairs. pingpong has two variants."""
    if "# also:" in command:
        parts = [part.strip() for part in command.split("# also:") if part.strip()]
        labels = ["mpi-program", "nccl-program"]
        out: list[tuple[str, str | None]] = []
        for i, part in enumerate(parts):
            label = labels[i] if i < len(labels) else f"variant{i + 1}"
            out.append((part, label))
        return out
    return [(command, None)]


def build_benchmark(bench_dir: Path, arch: str, dry_run: bool) -> tuple[bool, str | None]:
    cmd = ["make", f"ARCH={arch}", "EXTRA_CFLAGS=-lineinfo"]
    if dry_run:
        return True, None
    try:
        proc = subprocess.run(
            cmd,
            cwd=bench_dir,
            capture_output=True,
            text=True,
            check=False,
        )
    except OSError as exc:
        return False, str(exc)
    if proc.returncode != 0:
        detail = (proc.stderr or proc.stdout or "").strip()
        return False, detail or f"make failed with exit code {proc.returncode}"
    return True, None


def run_shell_command(
    command: str,
    cwd: Path,
    timeout: int | None,
    stdout_log: Path,
    dry_run: bool,
) -> tuple[int | None, float, str | None]:
    if dry_run:
        return 0, 0.0, None

    start = time.monotonic()
    try:
        with stdout_log.open("w", encoding="utf-8") as out_fh:
            out_fh.write(f"$ cd {cwd}\n")
            out_fh.write(f"$ {command}\n\n")
            proc = subprocess.run(
                command,
                cwd=cwd,
                shell=True,
                stdout=out_fh,
                stderr=subprocess.STDOUT,
                text=True,
                timeout=timeout,
                check=False,
            )
        duration = time.monotonic() - start
        return proc.returncode, duration, None
    except subprocess.TimeoutExpired:
        duration = time.monotonic() - start
        with stdout_log.open("a", encoding="utf-8") as out_fh:
            out_fh.write(f"\n\nERROR: timed out after {timeout}s\n")
        return None, duration, f"timed out after {timeout}s"
    except OSError as exc:
        duration = time.monotonic() - start
        return None, duration, str(exc)


def classify_status(exit_code: int | None, message: str | None) -> str:
    if message and "timed out" in message:
        return "TIMEOUT"
    if exit_code is None:
        return "RUN_FAILED"
    if exit_code == 0:
        return "PASS"
    return "SANITIZER_ERROR"


def should_keep_logs(status: str) -> bool:
    return status != "PASS"


def finalize_logs(
    status: str,
    temp_log_file: Path,
    temp_stdout_log: Path,
    final_log_file: Path,
    final_stdout_log: Path,
) -> tuple[str, str]:
    """Keep sanitizer logs only when an error was detected."""
    if not should_keep_logs(status):
        return "", ""

    final_log_file.parent.mkdir(parents=True, exist_ok=True)
    if temp_log_file.exists():
        shutil.move(str(temp_log_file), str(final_log_file))
    if temp_stdout_log.exists():
        shutil.move(str(temp_stdout_log), str(final_stdout_log))
    return str(final_log_file), str(final_stdout_log)


def run_one(
    entry: dict[str, Any],
    tool: str,
    output_dir: Path,
    arch: str,
    timeout: int | None,
    skip_build: bool,
    dry_run: bool,
) -> list[RunResult]:
    root = repo_root()
    bench_dir = root / entry["directory"]
    category_dir = safe_path_component(entry["category"])
    project_dir = safe_path_component(entry["project"])
    log_dir = output_dir / category_dir / project_dir

    results: list[RunResult] = []
    build_ok = True
    build_message: str | None = None

    if not bench_dir.is_dir():
        for command, variant in expand_sanitizer_commands(entry["sanitizer_command"]):
            suffix = f"-{variant}" if variant else ""
            results.append(
                RunResult(
                    category=entry["category"],
                    project=entry["project"],
                    tool=tool,
                    status="RUN_FAILED",
                    exit_code=None,
                    duration_sec=0.0,
                    log_file="",
                    stdout_log="",
                    build_ok=False,
                    special_handling=entry.get("special_handling", False),
                    command=command,
                    variant=variant,
                    message=f"benchmark directory not found: {bench_dir}",
                )
            )
        return results

    if not skip_build:
        build_ok, build_message = build_benchmark(bench_dir, arch, dry_run)
        if not build_ok:
            for command, variant in expand_sanitizer_commands(entry["sanitizer_command"]):
                suffix = f"-{variant}" if variant else ""
                results.append(
                    RunResult(
                        category=entry["category"],
                        project=entry["project"],
                        tool=tool,
                        status="BUILD_FAILED",
                        exit_code=None,
                        duration_sec=0.0,
                        log_file="",
                        stdout_log="",
                        build_ok=False,
                        special_handling=entry.get("special_handling", False),
                        command=command,
                        variant=variant,
                        message=build_message,
                    )
                )
            return results

    for command, variant in expand_sanitizer_commands(entry["sanitizer_command"]):
        suffix = f"-{variant}" if variant else ""
        final_log_file = log_dir / f"{tool}{suffix}.log"
        final_stdout_log = log_dir / f"{tool}{suffix}.stdout.log"
        try:
            with tempfile.TemporaryDirectory(prefix="hecbench-sanitizer-") as tmpdir:
                temp_log_file = Path(tmpdir) / f"{tool}{suffix}.log"
                temp_stdout_log = Path(tmpdir) / f"{tool}{suffix}.stdout.log"
                full_command = inject_sanitizer_flags(command, tool, temp_log_file)

                exit_code, duration, message = run_shell_command(
                    full_command,
                    bench_dir,
                    timeout,
                    temp_stdout_log,
                    dry_run,
                )
                status = "PASS" if dry_run else classify_status(exit_code, message)
                kept_log, kept_stdout = finalize_logs(
                    status,
                    temp_log_file,
                    temp_stdout_log,
                    final_log_file,
                    final_stdout_log,
                )
        except ValueError as exc:
            results.append(
                RunResult(
                    category=entry["category"],
                    project=entry["project"],
                    tool=tool,
                    status="RUN_FAILED",
                    exit_code=None,
                    duration_sec=0.0,
                    log_file="",
                    stdout_log="",
                    build_ok=build_ok,
                    special_handling=entry.get("special_handling", False),
                    command=command,
                    variant=variant,
                    message=str(exc),
                )
            )
            continue

        results.append(
            RunResult(
                category=entry["category"],
                project=entry["project"],
                tool=tool,
                status=status,
                exit_code=exit_code,
                duration_sec=round(duration, 3),
                log_file=kept_log,
                stdout_log=kept_stdout,
                build_ok=build_ok,
                special_handling=entry.get("special_handling", False),
                command=full_command,
                variant=variant,
                message=message,
            )
        )

    return results


def write_summary(output_dir: Path, results: list[RunResult], manifest_path: Path, args: argparse.Namespace) -> None:
    summary = {
        "manifest": str(manifest_path),
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "options": {
            "tools": args.tools,
            "arch": args.arch,
            "timeout": args.timeout,
            "skip_build": args.skip_build,
            "include_special": args.include_special,
            "exclude_special": not args.include_special,
            "all": args.all,
            "categories": args.categories,
            "projects": args.projects,
        },
        "counts": {},
        "results": [asdict(r) for r in results],
    }
    counts: dict[str, int] = {}
    for result in results:
        counts[result.status] = counts.get(result.status, 0) + 1
    summary["counts"] = counts

    summary_json = output_dir / "summary.json"
    summary_json.write_text(json.dumps(summary, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    headers = [
        "category",
        "project",
        "tool",
        "variant",
        "status",
        "exit_code",
        "duration_sec",
        "build_ok",
        "special_handling",
        "log_file",
        "stdout_log",
        "command",
        "message",
    ]
    lines = ["\t".join(headers)]
    for result in results:
        row = [
            result.category,
            result.project,
            result.tool,
            result.variant or "",
            result.status,
            "" if result.exit_code is None else str(result.exit_code),
            str(result.duration_sec),
            str(result.build_ok),
            str(result.special_handling),
            result.log_file,
            result.stdout_log,
            result.command,
            result.message or "",
        ]
        lines.append("\t".join(row))
    (output_dir / "summary.tsv").write_text("\n".join(lines) + "\n", encoding="utf-8")


def parse_args() -> argparse.Namespace:
    root = repo_root()
    default_manifest = root / "sanitizer_commands.json"
    default_arch = os.environ.get("ARCH", "sm_80")

    parser = argparse.ArgumentParser(
        description="Build and run HeCBench CUDA benchmarks under Compute Sanitizer.",
    )
    parser.add_argument(
        "-m",
        "--manifest",
        type=Path,
        default=default_manifest,
        help=f"Sanitizer manifest JSON (default: {default_manifest})",
    )
    parser.add_argument(
        "-o",
        "--output-dir",
        type=Path,
        default=None,
        help="Directory for logs and summaries (default: san-reports/)",
    )
    parser.add_argument(
        "--tool",
        dest="tools",
        default="memcheck",
        help="Comma-separated sanitizer tools: memcheck,racecheck,synccheck,initcheck",
    )
    parser.add_argument(
        "--category",
        dest="categories",
        action="append",
        help="Run only benchmarks in this category (repeatable)",
    )
    parser.add_argument(
        "--project",
        dest="projects",
        action="append",
        help="Run only benchmarks with this project name (repeatable)",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="Run all benchmarks in the manifest (special-handling entries still excluded unless --include-special)",
    )
    parser.add_argument(
        "--include-special",
        action="store_true",
        help="Include benchmarks marked special_handling (excluded by default)",
    )
    parser.add_argument(
        "--arch",
        default=default_arch,
        help=f"CUDA architecture for make ARCH=... (default: {default_arch})",
    )
    parser.add_argument(
        "--skip-build",
        action="store_true",
        help="Skip make; assume binaries are already built",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=600,
        help="Per-run timeout in seconds (default: 600)",
    )
    parser.add_argument(
        "--fail-fast",
        action="store_true",
        help="Stop after the first non-PASS result",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print planned work without building or running",
    )
    parser.add_argument(
        "--jobs",
        type=int,
        default=1,
        help="Concurrent benchmark runs (default: 1; use with care on one GPU)",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    tools = [tool.strip() for tool in args.tools.split(",") if tool.strip()]
    invalid = [tool for tool in tools if tool not in VALID_TOOLS]
    if invalid:
        print(f"error: invalid tool(s): {', '.join(invalid)}", file=sys.stderr)
        return 2

    if not args.manifest.is_file():
        print(f"error: manifest not found: {args.manifest}", file=sys.stderr)
        return 1

    if not args.all and not args.categories and not args.projects:
        print(
            "error: specify --all, --category, and/or --project to select benchmarks",
            file=sys.stderr,
        )
        return 2

    if args.all and (args.categories or args.projects):
        print("error: --all cannot be combined with --category or --project", file=sys.stderr)
        return 2

    output_dir = args.output_dir
    if output_dir is None:
        # stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
        # output_dir = repo_root() / "san-reports" / stamp
        output_dir = repo_root() / "san-reports"
    output_dir.mkdir(parents=True, exist_ok=True)

    manifest = load_manifest(args.manifest)
    categories = None if args.all else args.categories
    projects = None if args.all else args.projects
    selected = filter_benchmarks(
        manifest.get("benchmarks", []),
        categories,
        projects,
        not args.include_special,
    )
    if not selected:
        print("error: no benchmarks matched the given filters", file=sys.stderr)
        return 1

    print(f"Selected {len(selected)} benchmark(s); output -> {output_dir}")

    tasks: list[tuple[dict[str, Any], str]] = []
    for entry in selected:
        for tool in tools:
            tasks.append((entry, tool))

    results: list[RunResult] = []
    stop = False

    def execute(task: tuple[dict[str, Any], str]) -> list[RunResult]:
        entry, tool = task
        label = f"{entry['category']} / {entry['project']} ({tool})"
        print(f"==> {label}")
        return run_one(
            entry,
            tool,
            output_dir,
            args.arch,
            args.timeout,
            args.skip_build,
            args.dry_run,
        )

    if args.jobs <= 1:
        for task in tasks:
            if stop:
                break
            batch = execute(task)
            results.extend(batch)
            if args.fail_fast and any(r.status != "PASS" for r in batch):
                stop = True
    else:
        with ThreadPoolExecutor(max_workers=args.jobs) as pool:
            futures = {pool.submit(execute, task): task for task in tasks}
            for future in as_completed(futures):
                if stop:
                    continue
                batch = future.result()
                results.extend(batch)
                if args.fail_fast and any(r.status != "PASS" for r in batch):
                    stop = True

    results.sort(key=lambda r: (r.category, r.project, r.tool, r.variant or ""))
    write_summary(output_dir, results, args.manifest, args)

    counts: dict[str, int] = {}
    for result in results:
        counts[result.status] = counts.get(result.status, 0) + 1
    print("Summary:", ", ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    print(f"Wrote {output_dir / 'summary.json'}")

    harness_failures = {r.status for r in results} & {"BUILD_FAILED", "TIMEOUT", "RUN_FAILED"}
    sanitizer_failures = {r.status for r in results} & {"SANITIZER_ERROR"}
    if harness_failures or sanitizer_failures:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
