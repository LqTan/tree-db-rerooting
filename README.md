\# Chuyên Đề 4: Tree DP \& Re-rooting - Cài Đặt và Đo Đạc Thực Nghiệm

Dự án cài đặt thuật toán Quy hoạch động trên cây (Tree DP) kết hợp kỹ thuật Đổi Gốc (Re-routing) để giải quyết bài toán tính tổng khoảng cách từ mỗi đỉnh đến tất cả các đỉnh khác trong thời gian tối ưu $O(N)$ bằng ngôn ngữ C++17.

**1. Cấu Trúc Thư Mục Mã Nguồn**

Toàn bộ dự án được tổ chức theo mô hình chuẩn, tách biệt giữa mã nguồn (src/), công cụ kiểm thử (tests/), dữ liệu đầu vào (data/), kết quả thực nghiệm (results/) và tài liệu báo cáo:



project/

├── README.md                # Tài liệu hướng dẫn sử dụng dự án

├── src/                     # Mã nguồn chính

│   ├── Tree.hpp             # Biểu diễn danh sách kề tối ưu bộ nhớ

│   ├── IndexMapper.hpp      # Nén và ánh xạ nhãn đỉnh (chuỗi/rời rạc) sang 1-based index

│   ├── InputValidator.hpp   # Dùng DSU kiểm soát lỗi định dạng và xác thực cấu trúc cây

│   ├── TreeDPSolver.hpp     # Thuật toán tối ưu Tree DP \& Re-rooting O(N) với 2 lượt DFS

│   ├── BaselineSolver.hpp   # Thuật toán đối chiếu BFS từ từng đỉnh với độ phức tạp O(N^2)

│   ├── generator.cpp        # Bộ sinh dữ liệu cây đa dạng (single, line, star, balanced, random)

│   └── main.cpp             # Điểm khởi động hệ thống, hỗ trợ chọn bộ giải qua cờ dòng lệnh

├── tests/                   # Test cố định, bộ sinh test và script tự động

│   ├── benchmark.py         # Kịch bản Python tự động chạy đo đạc thực nghiệm và xuất báo cáo

│   ├── benchmark.sh         # Kịch bản Bash tự động hóa biên dịch và gọi benchmark

│   └── stress\_test.sh       # Script Bash kiểm thử đối kháng tự động giữa O(N) và O(N^2)

├── data/                    # Dữ liệu đầu vào dùng trong thực nghiệm

├── results/                 # Bảng kết quả (.md, .csv) và log chạy (.log)\[

├── report/                  # Báo cáo và hình minh họa

└── slides/                  # Tệp trình bày (slide báo cáo)

**2. Hướng Dẫn Dành Cho Người Mới Bắt Đầu (Cài Đặt Git Bash \& G++)**

Nếu máy tính của bạn sử dụng hệ điều hành Linux hoặc macOS, trình biên dịch C++ (g++) và môi trường Terminal thường đã có sẵn, bạn có thể bỏ qua Mục 2 để chuyển thẳng đến Mục 3.



Hướng dẫn dưới đây dành riêng cho người dùng Windows lần đầu chạy code C/C++ trên dòng lệnh:



***Bước 2.1: Tải và cài đặt môi trường dòng lệnh Git Bash***

Git Bash là công cụ giúp chạy các câu lệnh chuẩn Linux (mkdir, ./, <, >) trên hệ điều hành Windows một cách tiện lợi.



Cách cài đặt:



Truy cập trang chủ git-scm.com và tải bản cài đặt mới nhất cho Windows.



Chạy file .exe vừa tải về, bấm Next liên tục theo các lựa chọn mặc định để hoàn tất.



Cách mở nhanh Git Bash tại thư mục dự án:



Mở File Explorer, tìm đến thư mục gốc của dự án (project/).



Nhấp chuột phải vào khoảng trống trong thư mục

→ Chọn “Git Bash Here”. Cửa sổ dòng lệnh màu đen sẽ xuất hiện ngay tại đường dẫn dự án.



***Bước 2.2: Tải và cấu hình trình biên dịch C++ (g++) bằng w64devkit***

Để máy tính hiểu và chạy được mã nguồn C++17, bạn cần cài đặt bộ biên dịch GCC/g++.



Cách cài đặt ăn liền bằng w64devkit:



Truy cập trang phát hành chính thức: w64devkit Releases on GitHub.



Tải về file có tên dạng w64devkit-x.x.x.zip (chọn bản mới nhất, không cần tải bản -mini).



Giải nén toàn bộ file zip đó vào một ổ đĩa cố định trên máy tính, ví dụ: C:\\w64devkit. (Đảm bảo khi mở thư mục C:\\w64devkit\\bin bạn nhìn thấy file g++.exe bên trong).



Thêm đường dẫn vào Environment Variables (PATH) của Windows:



Nhấn phím Windows trên bàn phím, gõ từ khóa Edit the system environment variables rồi chọn kết quả tìm kiếm tương ứng.



Bấm vào nút Environment Variables… ở góc dưới bên phải.



Trong mục System variables (bảng phía dưới), cuộn tìm và nhấp chọn dòng Path, sau đó bấm Edit….



Bấm New ở bảng mới hiện ra và dán chính xác đường dẫn:



Plaintext



C:\\w64devkit\\bin



Bấm OK liên tục ở các cửa sổ để lưu cài đặt.



***Bước 2.3: Kiểm tra cài đặt thành công***

Tắt cửa sổ Git Bash hiện tại (nếu đang mở) để hệ thống nhận cấu hình Path mới.



Mở lại Git Bash Here tại thư mục project/.



Gõ lệnh kiểm tra phiên bản:



*g++ --version*



Nếu màn hình hiển thị thông tin phiên bản (ví dụ g++ (GCC) 13.x.0...)

→ Bạn đã cài đặt thành công 100%!



Nếu báo lỗi command not found, hãy kiểm tra lại đường dẫn C:\\w64devkit\\bin ở Bước 2.2.



**3. Hướng Dẫn Từng Bước Tái Hiện Mã Nguồn**

Lưu ý: Tất cả các lệnh dưới đây đều được thực thi trên cửa sổ Git Bash tại thư mục gốc của dự án (project/), không phụ thuộc vào đường dẫn tuyệt đối của máy cá nhân.



***Bước 3.1: Khởi tạo các thư mục phụ trợ (nếu chưa có)***

Chạy lệnh sau để tự động tạo các thư mục chứa file thực thi, tệp dữ liệu vào/ra:



mkdir -p tests data results



***Bước 3.2: Biên dịch mã nguồn C++ (Compile)***

Sử dụng cờ -O3 để tối ưu hóa tốc độ chạy mức cao nhất và -std=c++17 để sử dụng tiêu chuẩn C++17:



*# 1. Biên dịch chương trình chính (solver)*

*g++ -O3 -std=c++17 src/main.cpp -o tests/solver*



*# 2. Biên dịch bộ sinh dữ liệu cây (generator)*

*g++ -O3 -std=c++17 src/generator.cpp -o tests/generator*



(Trên hệ điều hành Windows, hệ thống sẽ tạo ra tệp thực thi tests/solver.exe và tests/generator.exe).



***Bước 3.3: Sinh dữ liệu kiểm thử tự động***

Cú pháp lệnh sinh dữ liệu cây:





*./tests/generator <N> <Loại\_Cây> > <Đường\_dẫn\_file\_lưu>*



<N>: Số lượng đỉnh của cây (N≥1).



<Loại\_Cây>: Chọn 1 trong 5 cấu trúc đồ thị cây: single, line, star, balanced, random.



Ví dụ: Sinh cấu trúc cây ngẫu nhiên gồm 100 đỉnh và lưu vào tệp data/input.txt:



./tests/generator 100 random > data/input.txt



***Bước 3.4: Chạy bộ giải và đối chiếu kết quả***

***Chạy mô hình tối ưu Tree DP (O(N) - Mặc định):***



*./tests/solver < data/input.txt*



Chương trình đọc dữ liệu từ tệp data/input.txt, thực thi thuật toán Re-rooting

O(N) và in tổng khoảng cách của từng đỉnh lên terminal.



***Chạy mô hình đối chiếu Baseline BFS(O(N2))):***



*./tests/solver --baseline < data/input.txt*



Sử dụng cờ --baseline để kích hoạt thuật toán duyệt BFS độc lập từ từng đỉnh. Kết quả xuất ra bắt buộc phải khớp 100% với chế độ mặc định.



***Bước 3.5: Chạy kiểm thử đối kháng tự động (Stress Test)***

Để tự động tạo cây ngẫu nhiên và kiểm tra chéo tính chính xác giữa bộ giải

O(N) và O(N2) trên 100 testcase, sử dụng script Bash:



*chmod +x tests/stress\_test.sh*

*./tests/stress\_test.sh*



Toàn bộ nhật ký kiểm thử sẽ được lưu tự động tại tệp results/stress\_test.log

***Bước 3.6: Đo đạc thực nghiệm hiệu năng (Benchmark)***

Dự án cung cấp kịch bản tự động đo thời gian thực thi (ms) và tính gia tốc tối ưu hóa (Speedup) trên nhiều quy mô đỉnh:



*python3 tests/benchmark.py*



(Nếu trên Windows Git Bash báo lỗi python3 not found, hãy thay bằng lệnh python tests/benchmark.py hoặc py tests/benchmark.py).



***Kết quả đầu ra tự động:***



Bảng báo cáo thực nghiệm định dạng Markdown: results/benchmark\_report.md.



Tệp số liệu thô CSV để vẽ biểu đồ: results/benchmark\_results.csv.

