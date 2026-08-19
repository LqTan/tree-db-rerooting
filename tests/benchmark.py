#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
tests/benchmark.py

Do dac thuc nghiem hieu nang: thoi gian chay (ms) va he so gia toc (speedup)
cua TreeDPSolver (O(N)) so voi BaselineSolver (O(N^2)) tren nhieu quy mo N.

Cach chay (tu thu muc goc du an, sau khi da build tests/solver va tests/generator):
    python3 tests/benchmark.py
    (Windows Git Bash: python tests/benchmark.py  hoac  py tests/benchmark.py)

Ket qua sinh ra:
    results/benchmark_report.md    - bang bao cao dang Markdown
    results/benchmark_results.csv  - so lieu tho de ve bieu do
"""

import csv
import os
import subprocess
import sys
import tempfile
import time

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SOLVER = os.path.join(ROOT_DIR, "tests", "solver")
GENERATOR = os.path.join(ROOT_DIR, "tests", "generator")
RESULTS_DIR = os.path.join(ROOT_DIR, "results")

if os.name == "nt":
    SOLVER += ".exe"
    GENERATOR += ".exe"

TREE_TYPE = "random"
BENCH_SIZES_FULL = [100, 500, 1000, 2000, 5000, 10000, 20000]
BENCH_SIZES_LARGE_ONLY = [50000, 100000, 200000]
BASELINE_TIMEOUT_SEC = 30


def check_prereqs():
    if not (os.path.isfile(SOLVER) and os.access(SOLVER, os.X_OK)):
        sys.exit(
            "Loi: khong tim thay '{}'.\n"
            "Hay bien dich truoc:\n"
            "  g++ -O3 -std=c++17 src/main.cpp -o tests/solver\n"
            "  g++ -O3 -std=c++17 src/generator.cpp -o tests/generator".format(SOLVER)
        )
    if not (os.path.isfile(GENERATOR) and os.access(GENERATOR, os.X_OK)):
        sys.exit("Loi: khong tim thay '{}'.".format(GENERATOR))


def generate_tree(n, tree_type):
    fd, path = tempfile.mkstemp(suffix=".txt")
    os.close(fd)
    with open(path, "w") as f:
        subprocess.run([GENERATOR, str(n), tree_type], stdout=f, check=True)
    return path


def time_run(args, stdin_path, timeout=None):
    with open(stdin_path, "rb") as fin:
        t0 = time.perf_counter()
        try:
            subprocess.run(
                args, stdin=fin, stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL, timeout=timeout, check=True,
            )
        except subprocess.TimeoutExpired:
            return None
        t1 = time.perf_counter()
    return t1 - t0


def main():
    check_prereqs()
    os.makedirs(RESULTS_DIR, exist_ok=True)
    rows = []

    print("=" * 72)
    print(" BENCHMARK: TreeDPSolver (O(N)) vs BaselineSolver (O(N^2))")
    print("=" * 72)

    all_sizes = [(n, True) for n in BENCH_SIZES_FULL] + [(n, False) for n in BENCH_SIZES_LARGE_ONLY]

    for n, run_baseline in all_sizes:
        tree_path = generate_tree(n, TREE_TYPE)
        try:
            t_dp = time_run([SOLVER], tree_path)
            treedp_ms = t_dp * 1000.0

            baseline_ms = None
            speedup = None
            if run_baseline:
                t_bl = time_run([SOLVER, "--baseline"], tree_path, timeout=BASELINE_TIMEOUT_SEC)
                if t_bl is not None:
                    baseline_ms = t_bl * 1000.0
                    speedup = baseline_ms / treedp_ms if treedp_ms > 0 else None

            rows.append((n, TREE_TYPE, treedp_ms, baseline_ms, speedup))

            bl_str = f"{baseline_ms:10.2f} ms" if baseline_ms is not None else "  (bo qua)"
            sp_str = f"x{speedup:8.1f}" if speedup is not None else "     N/A"
            print(f"  N={n:>7}  TreeDP: {treedp_ms:10.2f} ms   Baseline: {bl_str}   Speedup: {sp_str}")
        finally:
            os.remove(tree_path)

    csv_path = os.path.join(RESULTS_DIR, "benchmark_results.csv")
    with open(csv_path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["N", "loai_cay", "treedp_ms", "baseline_ms", "speedup"])
        for n, t, dp, bl, sp in rows:
            writer.writerow([n, t, f"{dp:.3f}", f"{bl:.3f}" if bl is not None else "", f"{sp:.2f}" if sp is not None else ""])

    md_path = os.path.join(RESULTS_DIR, "benchmark_report.md")
    with open(md_path, "w", encoding="utf-8") as f:
        f.write("# Báo cáo thực nghiệm hiệu năng\n\n")
        f.write("So sánh **TreeDPSolver O(N)** với **BaselineSolver O(N²)** trên cây loại "
                f"`{TREE_TYPE}`, đo trực tiếp trên máy chạy benchmark.\n\n")
        f.write("| N | TreeDPSolver (ms) | BaselineSolver (ms) | Speedup |\n")
        f.write("|---|---:|---:|---:|\n")
        for n, t, dp, bl, sp in rows:
            bl_str = f"{bl:.2f}" if bl is not None else "bỏ qua (quá chậm / vượt timeout)"
            sp_str = f"×{sp:.1f}" if sp is not None else "N/A"
            f.write(f"| {n} | {dp:.2f} | {bl_str} | {sp_str} |\n")
        f.write("\n")
        f.write(
            f"*Ghi chú: BaselineSolver bị bỏ qua khi N ≥ {min(BENCH_SIZES_LARGE_ONLY)} hoặc khi "
            f"vượt quá thời gian chờ {BASELINE_TIMEOUT_SEC}s, vì độ phức tạp O(N²) khiến thời gian "
            "chạy tăng quá nhanh để đo trực tiếp ở quy mô lớn.*\n"
        )

    print()
    print(f"Da ghi: {csv_path}")
    print(f"Da ghi: {md_path}")


if __name__ == "__main__":
    main()
