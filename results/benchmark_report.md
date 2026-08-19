# Báo cáo thực nghiệm hiệu năng

So sánh **TreeDPSolver O(N)** với **BaselineSolver O(N²)** trên cây loại `random`, đo trực tiếp trên máy chạy benchmark.

| N | TreeDPSolver (ms) | BaselineSolver (ms) | Speedup |
|---|---:|---:|---:|
| 100 | 52.13 | 62.81 | ×1.2 |
| 500 | 54.89 | 50.30 | ×0.9 |
| 1000 | 50.86 | 46.69 | ×0.9 |
| 2000 | 50.15 | 74.31 | ×1.5 |
| 5000 | 50.46 | 294.23 | ×5.8 |
| 10000 | 57.57 | 1200.70 | ×20.9 |
| 20000 | 64.58 | 5091.14 | ×78.8 |
| 50000 | 74.49 | bỏ qua (quá chậm / vượt timeout) | N/A |
| 100000 | 91.33 | bỏ qua (quá chậm / vượt timeout) | N/A |
| 200000 | 161.73 | bỏ qua (quá chậm / vượt timeout) | N/A |

*Ghi chú: BaselineSolver bị bỏ qua khi N ≥ 50000 hoặc khi vượt quá thời gian chờ 30s, vì độ phức tạp O(N²) khiến thời gian chạy tăng quá nhanh để đo trực tiếp ở quy mô lớn.*
