# Hướng dẫn chạy từng bước — Bài toán Tổng khoảng cách trên cây (Tree DP & Re-rooting)

## Cấu trúc thư mục dự án

```
project/
├── README.md
├── src/                        <- Mã nguồn (chưa build)
│   ├── Tree.hpp
│   ├── IndexMapper.hpp
│   ├── InputValidator.hpp
│   ├── InputReader.hpp
│   ├── TreeDPSolver.hpp
│   ├── BaselineSolver.hpp
│   ├── main.cpp
│   └── generator.cpp
├── tests/                      <- Test cố định + script kiểm thử + file thực thi (sau khi build)
│   ├── basic/                  <- 5 test cơ bản
│   ├── edge/                   <- 6 test biên (N=1, N=2, N=200.000...)
│   ├── adversarial/            <- 8 test đối kháng (dữ liệu không hợp lệ)
│   ├── stress_test.sh          <- Script kiểm thử tự động 100 testcase
│   ├── benchmark.py            <- Script đo hiệu năng & tính speedup
│   ├── solver                  <- (sinh ra sau Bước 3.2)
│   └── generator               <- (sinh ra sau Bước 3.2)
├── data/                       <- Dữ liệu đầu vào dùng trong thực nghiệm (sinh động, ví dụ input.txt)
└── results/                    <- Kết quả/log sau khi chạy stress_test.sh và benchmark.py
    ├── stress_test.log
    ├── random_stress_log.csv
    ├── benchmark_report.md
    └── benchmark_results.csv
```

---

### Bước 3.2: Biên dịch mã nguồn C++ (Compile)

Sử dụng cờ `-O3` để tối ưu hóa tốc độ chạy mức cao nhất và `-std=c++17` để sử dụng tiêu chuẩn C++17:

```bash
# 1. Biên dịch chương trình chính (solver)
g++ -O3 -std=c++17 src/main.cpp -o tests/solver

# 2. Biên dịch bộ sinh dữ liệu cây (generator)
g++ -O3 -std=c++17 src/generator.cpp -o tests/generator
```

(Trên hệ điều hành Windows, hệ thống sẽ tạo ra tệp thực thi `tests/solver.exe` và `tests/generator.exe`.)

> Đã kiểm tra: biên dịch sạch tuyệt đối, kể cả khi bật thêm `-Wall -Wextra` (không có bất kỳ cảnh báo nào).

---

### Bước 3.3: Sinh dữ liệu kiểm thử tự động

Cú pháp lệnh sinh dữ liệu cây:

```
./tests/generator <N> <Loại_Cây> > <Đường_dẫn_file_lưu>
```

- `<N>`: Số lượng đỉnh của cây (N≥1).
- `<Loại_Cây>`: Chọn 1 trong 5 cấu trúc đồ thị cây: `single`, `line`, `star`, `balanced`, `random`.

Ví dụ: Sinh cấu trúc cây ngẫu nhiên gồm 100 đỉnh và lưu vào tệp `data/input.txt`:

```bash
./tests/generator 100 random > data/input.txt
```

---

### Bước 3.4: Chạy bộ giải và đối chiếu kết quả

**Chạy mô hình tối ưu Tree DP (O(N) — mặc định):**

```bash
./tests/solver < data/input.txt
```

Chương trình đọc dữ liệu từ tệp `data/input.txt`, thực thi thuật toán Re-rooting O(N) và in tổng khoảng cách của từng đỉnh lên terminal.

**Chạy mô hình đối chiếu Baseline BFS (O(N²)):**

```bash
./tests/solver --baseline < data/input.txt
```

Sử dụng cờ `--baseline` để kích hoạt thuật toán duyệt BFS độc lập từ từng đỉnh. Kết quả xuất ra bắt buộc phải khớp 100% với chế độ mặc định (đã kiểm chứng: `diff` giữa hai đầu ra cho kết quả rỗng).

---

### Bước 3.5: Chạy kiểm thử đối kháng tự động (Stress Test)

Để tự động tạo cây ngẫu nhiên và kiểm tra chéo tính chính xác giữa bộ giải O(N) và O(N²) trên **100 testcase** (25 kích thước × 4 loại cây: line, star, balanced, random), đồng thời chạy lại cả 3 nhóm test cơ bản/biên/đối kháng, sử dụng script Bash:

```bash
chmod +x tests/stress_test.sh
./tests/stress_test.sh
```

Toàn bộ nhật ký kiểm thử sẽ được lưu tự động tại tệp `results/stress_test.log`, và bảng số liệu chi tiết của 100 testcase tại `results/random_stress_log.csv`.

> Đã chạy thực tế: **121/121 test PASS** (100 testcase ngẫu nhiên + 5 test cơ bản + 6 test biên + 8 test đối kháng + 2 kiểm tra bổ sung), tổng thời gian chạy khoảng **30 giây**.

---

### Bước 3.6: Đo đạc thực nghiệm hiệu năng (Benchmark)

Dự án cung cấp kịch bản tự động đo thời gian thực thi (ms) và tính gia tốc tối ưu hóa (Speedup) trên nhiều quy mô đỉnh:

```bash
python3 tests/benchmark.py
```

(Nếu trên Windows Git Bash báo lỗi `python3 not found`, hãy thay bằng lệnh `python tests/benchmark.py` hoặc `py tests/benchmark.py`.)

**Kết quả đầu ra tự động:**
- Bảng báo cáo thực nghiệm định dạng Markdown: `results/benchmark_report.md`.
- Tệp số liệu thô CSV để vẽ biểu đồ: `results/benchmark_results.csv`.

**Số liệu đo thực tế** (cây ngẫu nhiên, đo trực tiếp trên máy build):

| N | TreeDPSolver (ms) | BaselineSolver (ms) | Speedup |
|---|---:|---:|---:|
| 100 | 1,70 | 3,43 | ×2,0 |
| 1.000 | 2,24 | 15,57 | ×6,9 |
| 5.000 | 5,62 | 565,38 | ×100,6 |
| 10.000 | 8,00 | 2.323,69 | ×290,6 |
| 20.000 | 15,77 | 9.916,13 | ×628,6 |
| 50.000 | 43,44 | bỏ qua (quá chậm) | — |
| 200.000 | 224,37 | bỏ qua (quá chậm) | — |

Hệ số gia tốc (speedup) tăng gần như tuyến tính theo N — đúng bản chất chênh lệch O(N) so với O(N²): ở N=20.000, TreeDPSolver đã nhanh hơn Baseline **hơn 600 lần**.

---

## Bốn nhóm test (đúng yêu cầu chuyên đề, tham chiếu Mục 6.1–6.2 của báo cáo)

| # | Nhóm | Vị trí | Số lượng | Mục đích |
|---|------|--------|----------|----------|
| 1 | Test cơ bản | `tests/basic/` | 5 test + 1 đối chiếu tay | Xác nhận đúng trên cây nhỏ, dễ kiểm tra |
| 2 | Test biên | `tests/edge/` | 6 test | N=1 (2 biến thể nhãn), N=2, N=200.000 (đường thẳng & hình sao — regression test chống tràn ngăn xếp), kiểm tra tràn số |
| 3 | Test đối kháng | `tests/adversarial/` | 8 test | Tự khuyên, chu trình (DSU), sai số cạnh, N≤0, input rỗng — phải bị từ chối đúng cách, không được crash |
| 4 | Test ngẫu nhiên đối chiếu | sinh động qua `tests/generator` | 100 testcase | 4 loại cây × 25 kích thước, đối chiếu TreeDPSolver với BaselineSolver |
| + | Benchmark hiệu năng | `tests/benchmark.py` | 10 mốc N | Đo ms thực tế và tính hệ số speedup O(N) so với O(N²) |

---

## Kịch bản gợi ý cho video demo (10–15 phút)

1. **(1–2 phút) Giới thiệu bài toán và kiến trúc hệ thống** — mở nhanh các file trong `src/`, giải thích vai trò từng module (Tree, IndexMapper, InputValidator, InputReader, TreeDPSolver, BaselineSolver) — tham chiếu Mục 5.3 của báo cáo.

2. **(1 phút) Biên dịch** — chạy đúng 2 lệnh ở Bước 3.2, chỉ ra không có cảnh báo nào dù dùng `-O3`.

3. **(2 phút) Test cơ bản** — mở `tests/basic/basic_01_tree8.txt`, vẽ tay cây 8 đỉnh, tính `ans[1]=13` bằng tay, sau đó chạy `./tests/solver < tests/basic/basic_01_tree8.txt` để đối chiếu trực tiếp trên màn hình.

4. **(2–3 phút) Test biên** — trình diễn trực tiếp:
   - `echo "1" | ./tests/solver` (N=1, không nhãn)
   - Chạy lại N=200.000 đường thẳng với `ulimit -s 256` để **tái hiện trực quan lỗi tràn ngăn xếp đã từng gặp**, rồi cho xem code cũ (đệ quy) so với code mới (dùng ngăn xếp tường minh) — điểm nhấn kỹ thuật quan trọng nhất nên nhấn mạnh trong video.

5. **(2 phút) Test đối kháng** — chạy lần lượt vài file trong `tests/adversarial/`, cho xem thông báo lỗi rõ ràng, nhấn mạnh `InputValidator` dùng DSU để phát hiện chu trình.

6. **(3–4 phút) Stress test 100 testcase + Benchmark** — chạy `./tests/stress_test.sh` (≈30 giây), giải thích Mục 4, sau đó chạy `python3 tests/benchmark.py` và chỉ vào bảng speedup tăng dần từ ×2 lên ×628 — đây là hình ảnh trực quan, dễ gây ấn tượng nhất cho người xem.

7. **(1 phút) Tổng kết** — chỉ vào dòng "121/121 PASS" trong `results/stress_test.log` và bảng trong `results/benchmark_report.md`, liên hệ lại với số liệu ở Mục 6.4 của báo cáo.

Thời lượng thực tế của các lệnh chỉ khoảng 30–40 giây (đã đo), phần còn lại của 10–15 phút dành cho giải thích lý thuyết, chỉ vào mã nguồn và trả lời câu hỏi giả định — đúng tinh thần "hiểu được mô hình, thuật toán, mã nguồn" mà đề bài yêu cầu.
