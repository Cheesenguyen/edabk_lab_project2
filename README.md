**Mô tả phần mềm to do list**

Các chức năng chính
1. Thêm task mới
2. Sửa task
3. Hiển thị
4. Xóa
5. Tìm kiếm
6. Sắp xếp

Chi tiết các chức năng:
1. Tạo task mới: Có 2 tính năng tạo task lớn / bé
  - task lớn:
    + phù hợp để ghi chú những dự án hoặc những công việc dài hạn, cần theo dõi về tiến độ
    + Các ghi chú bao gồm: tiêu đề, mô tả, priority, phần trăm hoàn thành và deadline.
    + Người dùng cần nhập ID cho mỗi task thêm vào. Khuyến khích để những ID có ý nghĩa hoặc sắp xếp khoa học.
  - task bé: phù hợp để ghi chú những note hằng ngày

2. Sửa nội dung task: Có 2 tính năng dành cho task lớn / bé
  - Người dùng không thể sửa ID
  - Muốn chọn task cần sửa, người dùng hãy nhập ID của task (áp dụng cho cả task lớn và bé)
  - Chọn mục muốn sửa và thay bằng thông tin mới. Lưu ý hãy nhập đúng định dạng thông tin.

3. Hiển thị: toàn bộ task
  Tính năng hiển thị theo bảng.
  - Trong phần mức độ ưu tiên, các công việc có mức độ ưu tiên cao nhất được hiển thị bằng màu nóng, đấm như đỏ cam. Và giảm dần xuống vàng, xanh lá
  - Tương tự với phần mức độ hoàn thành. Task hoàn thành càng nhiều thì màu càng nhạt
  - Lưu ý: người dùng không thể thay đổi màu chữ

4. Xóa task: có 2 tính năng dành cho task lớn / bé
   - Task lớn: Chọn ID file muốn xóa -> xóa
   - Task bé: Chọn ID task lớn -> chọn ID task bé -> xóa
   - Lưu ý hãy chọn chính xác file muốn xóa vì bạn không thể khôi phục nếu thực hiện sai!
5. Tìm kiếm
   - Chọn ID
   - Task tự động dược hiển thị nếu thấy ID trùng khớp
   - Nếu không chương trình sẽ báo lỗi
6. Sắp xếp
   - Sắp xếp theo thứ tự ưu tiên công việc
   - Với mức độ hoàn thành: sắp xếp tăng dần
   - Với mức độ ưu tiên: sắp xếp giảm dần
  
   **Hướng dẫn sử dụng file README**
   1. compile.sh: biên dịch file. Chạy file bằng lệnh ./compile.sh
   2. run.sh: chạy file vừa biên dịch. Chạy file bằng lệnh ./run.sh
   3. clean.sh: xóa file sinh ra trong quá trình thực thi
   4. clone.sh: clone folder từ gituhb về. Chạy file bằng lệnh source ./clone.sh đường link
