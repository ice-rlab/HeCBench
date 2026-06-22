#!/usr/bin/env python3
"""Parse CUDA_COMPUTE_SANITIZER_COMMANDS.md into a JSON manifest."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any


def strip_backticks(value: str) -> str:
    return value.strip().strip("`").strip()


def parse_total_counts(text: str) -> dict[str, int | None]:
    totals = {"total": None, "standard": None, "special": None}
    match = re.search(
        r"\*\*Total benchmarks:\*\*\s*(\d+)\s*\((\d+)\s+standard\s+\+\s+(\d+)\s+special handling\)",
        text,
    )
    if match:
        totals["total"] = int(match.group(1))
        totals["standard"] = int(match.group(2))
        totals["special"] = int(match.group(3))
    return totals


def is_table_header(line: str) -> bool:
    return line.startswith("|") and "Project" in line


def is_table_separator(line: str) -> bool:
    return bool(re.match(r"^\|\s*[-:]+\s*(\|\s*[-:]+\s*)+\|?\s*$", line))


def parse_table_row(line: str) -> list[str]:
    return [cell.strip() for cell in line.split("|")[1:-1]]


def row_to_benchmark(cells: list[str], category: str, special_handling: bool) -> dict[str, Any]:
    if special_handling:
        if len(cells) != 7:
            raise ValueError(f"Expected 7 columns in special-handling row, got {len(cells)}: {cells}")
        category, project, directory, make_run, essential, sanitizer, reason = cells
    else:
        if len(cells) != 5:
            raise ValueError(f"Expected 5 columns in standard row, got {len(cells)}: {cells}")
        project, directory, make_run, essential, sanitizer = cells
        reason = None

    return {
        "category": category,
        "project": project,
        "directory": strip_backticks(directory),
        "make_run_command": strip_backticks(make_run),
        "essential_command": strip_backticks(essential),
        "sanitizer_command": strip_backticks(sanitizer),
        "special_handling": special_handling,
        "reason": reason,
    }


def parse_markdown(text: str) -> dict[str, Any]:
    benchmarks: list[dict[str, Any]] = []
    categories: list[str] = []

    current_category: str | None = None
    in_special_section = False
    table_kind: str | None = None  # "special" or "standard"

    for line in text.splitlines():
        heading = re.match(r"^## (.+)$", line)
        if heading:
            title = heading.group(1).strip()
            if title == "Special Handling":
                in_special_section = True
                current_category = None
                table_kind = None
                continue
            if title in {"Usage"}:
                in_special_section = False
                current_category = None
                table_kind = None
                continue
            in_special_section = False
            current_category = title
            if current_category not in categories:
                categories.append(current_category)
            table_kind = None
            continue

        if not line.startswith("|"):
            if not line.strip().startswith("---"):
                table_kind = None
            continue

        if is_table_separator(line):
            continue

        if is_table_header(line):
            if in_special_section:
                table_kind = "special"
            elif current_category is not None:
                table_kind = "standard"
            else:
                table_kind = None
            continue

        if table_kind is None:
            continue

        cells = parse_table_row(line)
        if not cells or all(re.match(r"^-+$", cell) for cell in cells):
            continue

        special = table_kind == "special"
        category = cells[0] if special else current_category
        if category is None:
            raise ValueError(f"Row without category context: {line}")

        benchmark = row_to_benchmark(cells, category, special)
        benchmarks.append(benchmark)

    totals = parse_total_counts(text)
    special_count = sum(1 for b in benchmarks if b["special_handling"])
    standard_count = len(benchmarks) - special_count

    return {
        # "source": "CUDA_COMPUTE_SANITIZER_COMMANDS.md",
        "totals": {
            "benchmarks": len(benchmarks),
            "standard": standard_count,
            "special_handling": special_count,
            "declared_total": totals["total"],
            "declared_standard": totals["standard"],
            "declared_special": totals["special"],
        },
        "categories": categories,
        "benchmarks": benchmarks,
    }


def main() -> int:
    repo_root = Path(__file__).resolve().parent.parent
    default_input = repo_root / "CUDA_COMPUTE_SANITIZER_COMMANDS.md"
    default_output = repo_root / "sanitizer_commands.json"

    parser = argparse.ArgumentParser(
        description="Parse CUDA_COMPUTE_SANITIZER_COMMANDS.md into JSON.",
    )
    parser.add_argument(
        "-i",
        "--input",
        type=Path,
        default=default_input,
        help=f"Input markdown file (default: {default_input})",
    )
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=default_output,
        help=f"Output JSON file (default: {default_output})",
    )
    parser.add_argument(
        "--indent",
        type=int,
        default=2,
        help="JSON indentation (default: 2; use 0 for compact output)",
    )
    args = parser.parse_args()

    if not args.input.is_file():
        print(f"error: input file not found: {args.input}", file=sys.stderr)
        return 1

    text = args.input.read_text(encoding="utf-8")
    data = parse_markdown(text)
    # data["source"] = str(args.input)

    indent = None if args.indent == 0 else args.indent
    args.output.write_text(
        json.dumps(data, indent=indent, ensure_ascii=False) + "\n",
        encoding="utf-8",
    )

    totals = data["totals"]
    print(
        f"Wrote {totals['benchmarks']} benchmarks "
        f"({totals['standard']} standard, {totals['special_handling']} special) "
        f"to {args.output}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
