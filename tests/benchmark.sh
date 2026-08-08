#!/bin/bash
set -e

# Script Bash tự động kích hoạt tiến trình đo đạc hiệu năng chi tiết
# Trích nguồn phương pháp: VNOI Wiki & "tối ưu hóa thuật toán.pdf"

echo "=== KHỞI ĐỘNG HỆ THỐNG ĐO ĐẠC THỰC NGHIỆM CHUYÊN ĐỀ 4 ==="

# Đảm bảo các thư mục tồn tại
mkdir -p tests data results

# Biên dịch mã nguồn C++17 với cờ tối ưu hóa cao nhất -O3
echo "Đang kiểm tra và biên dịch mã nguồn..."
g++ -O3 -std=c++17 src/main.cpp -o tests/solver
g++ -O3 -std=c++17 src/generator.cpp -o tests/generator

echo "Biên dịch thành công!"
echo "Đang kích hoạt chương trình đo thời gian chạy chi tiết và xuất biểu bảng..."

# Chạy driver Python
python3 tests/benchmark.py

echo "=== ĐÃ HOÀN THÀNH TOÀN BỘ QUY TRÌNH ĐO ĐẠC THỰC NGHIỆM ==="
echo "Bảng kết quả đã được ghi nhận tự động tại tệp: results/benchmark_report.md"