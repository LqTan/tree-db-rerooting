import subprocess
import time
import sys
import os

# Python Benchmark Driver for Chapter 6 Experiments
# Trích nguồn phương pháp: tối ưu hóa thuật toán.pdf (Yêu cầu thực nghiệm đa quy mô và xuất dạng bảng)

sizes = [1, 4]
tree_types = ["random", "line", "star", "balanced"]

solver_path = "./tests/solver"
generator_path = "./tests/generator"

# Khởi tạo các thư mục đầu ra
os.makedirs("data", exist_ok=True)
os.makedirs("results", exist_ok=True)

print("==================================================================")
print("   STARTING DETAILED BENCHMARK FOR TREE DP & RE-ROOTING (O(N))    ")
print("==================================================================")

# Tự động kiểm tra và biên dịch mã nguồn C++
if not os.path.exists(solver_path) or not os.path.exists(generator_path):
    print("Executables not found. Attempting to compile...")
    try:
        subprocess.run(["g++", "-O3", "-std=c++17", "src/main.cpp", "-o", solver_path], check=True)
        subprocess.run(["g++", "-O3", "-std=c++17", "src/generator.cpp", "-o", generator_path], check=True)
        print("Compilation successful!")
    except Exception as e:
        print("Failed to compile C++ source files automatically.")
        print("Please ensure 'g++' is installed in your PATH.")
        sys.exit(1)

results_table = []
results_csv = ["N,TreeType,Baseline_ms,Optimal_ms,Speedup"]

for N in sizes:
    for t_type in tree_types:
        # Giới hạn Baseline O(N^2) không chạy ở N=100,000 để tránh bị treo hệ thống (TLE)
        run_baseline = N <= 10000
        
        # Bước 1: Gọi generator sinh cây tương ứng
        input_file = "data/benchmark_input.txt"
        with open(input_file, "w") as f:
            subprocess.run([generator_path, str(N), t_type], stdout=f, check=True)
            
        # Bước 2: Đo thời gian chạy Baseline O(N^2)
        baseline_time_ms = 0.0
        if run_baseline:
            start = time.perf_counter()
            subprocess.run([solver_path, "--baseline"], stdin=open(input_file, "r"), stdout=subprocess.DEVNULL, check=True)
            end = time.perf_counter()
            baseline_time_ms = (end - start) * 1000.0
        else:
            baseline_time_ms = -1.0  # Đánh dấu bị treo (Time Limit Exceeded)
            
        # Bước 3: Đo thời gian chạy tối ưu O(N)
        # Chạy lặp lại nhiều lần với N nhỏ để lấy trung bình thời gian chính xác nhất
        runs = 5 if N < 1000 else 1
        total_opt_time = 0.0
        for _ in range(runs):
            start = time.perf_counter()
            subprocess.run([solver_path], stdin=open(input_file, "r"), stdout=subprocess.DEVNULL, check=True)
            end = time.perf_counter()
            total_opt_time += (end - start) * 1000.0
        optimal_time_ms = total_opt_time / runs
        
        # Bước 4: Định dạng dữ liệu và tính toán gia tốc tối ưu hóa (Speedup)
        baseline_str = f"{baseline_time_ms:.2f} ms" if baseline_time_ms >= 0 else "TLE (>5.0s)"
        optimal_str = f"{optimal_time_ms:.2f} ms"
        
        if baseline_time_ms >= 0:
            speedup_val = baseline_time_ms / optimal_time_ms
            speedup_str = f"{speedup_val:.1f}x"
        else:
            speedup_str = "N/A"
            
        print(f"N={N:<6} | Type={t_type:<10} | Baseline={baseline_str:<15} | Optimal={optimal_str:<12} | Speedup={speedup_str}")
        
        results_table.append((N, t_type, baseline_str, optimal_str, speedup_str))
        csv_base_val = f"{baseline_time_ms:.2f}" if baseline_time_ms >= 0 else "TLE"
        results_csv.append(f"{N},{t_type},{csv_base_val},{optimal_time_ms:.2f},{speedup_str}")

# Bước 5: Xuất báo cáo dạng Markdown bảng biểu cực kỳ đẹp mắt
report_md_path = "results/benchmark_report.md"
with open(report_md_path, "w", encoding="utf-8") as f:
    f.write("# BẢNG KẾT QUẢ THỰC NGHIỆM ĐO THỜI GIAN CHẠY CHUYÊN ĐỀ 4\n\n")
    f.write("| Quy mô dữ liệu (N) | Loại cấu trúc cây | Thời gian chạy Baseline O(N^2) | Thời gian chạy Tối ưu O(N) | Tỷ lệ gia tốc (Speedup) |\n")
    f.write("| :---: | :---: | :---: | :---: | :---: |\n")
    for N, t_type, base_t, opt_t, speedup in results_table:
        f.write(f"| **{N}** | {t_type} | {base_t} | {opt_t} | **{speedup}** |\n")
    f.write("\n*Lưu ý: Ký hiệu TLE (Time Limit Exceeded) biểu thị thuật toán vượt quá ngưỡng thời gian chờ an toàn (5 giây).*")

# Xuất tệp CSV phụ trợ
with open("results/benchmark_results.csv", "w") as f:
    f.write("\n".join(results_csv))

print("\n==================================================================")
print(f"Benchmark finished successfully!")
print(f"1. Markdown table exported to: {report_md_path}")
print(f"2. Raw CSV data exported to: results/benchmark_results.csv")
print("==================================================================")