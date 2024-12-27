**Mô tả phần mềm to do list**

Các chức năng chính
1. Tạo task mới
2. Thêm task mới trong file
3. Thay đổi nội dung task
4. Xóa task
5. Phân loại/ tìm task

Chi tiết các chức năng:
1. Tạo task mới: Có 2 tính năng tạo task lớn / bé
  - task lớn:
    + phù hợp để ghi chú những dự án hoặc những công việc dài hạn, cần theo dõi về tiến độ
    + Các ghi chú bao gồm: priority, phần trăm hoàn thành, note và deadline sẽ tự động hiện lên
  - task bé: phù hợp để ghi chú những note hằng ngày

2. Thêm task mới trong file (dành cho task lớn)
  - Lựa chọn file muốn thêm task
  - Các ghi chú tự động hiện lên

3. Thay đổi nội dung task: Có 2 tính năng dành cho task lớn / bé
Xóa và thay đổi trực tiếp trên file

4. Xóa task: có 2 tính năng dành cho task lớn / bé
   - Task lớn: Chọn ID file muốn xóa -> xóa
   - Task bé: xóa trực tiếp trên file
5. Phân loại/ tìm task: dành cho task lớn
   - phân loại theo: priority, phần trăm hoàn thành và deadline
   - tìm: theo tên
  
   **Hướng dẫn sử dụng file README**
   1. compile.sh: biên dịch file. Chạy file bằng lệnh ./compile.sh
   2. run.sh: chạy file vừa biên dịch. Chạy file bằng lệnh ./run.sh
   3. clean.sh: xóa file sinh ra trong quá trình thực thi
   4. clone.sh: clone folder từ gituhb về. Chạy file bằng lệnh source ./clone.sh đường link
