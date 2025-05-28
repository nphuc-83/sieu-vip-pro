# Hệ thống đăng nhập và đăng ký tài khoản và quản lý ví điểm thưởng
tên nhóm PTKL gồm các thành viên:

Nguyễn Hoàng Phúc    
+ Tạo mới tài khoản khi đăng ký
+ Lưu trữ dữ liệu
  
Nguyễn Gia Kiệt
+ Quản lý đăng nhập
+ Sinh mật khẩu tự động
+ OTP: bảo mật hai lớp để xác thực giao dịch.

Nguyễn Thanh Tâm 
+ Quản lý hoạt động ví
  
Tạ Hoàng Long 
+ Phân chia người dùng và chức năng


*Phân tích & Đặc tả chức năng
Người dùng thông thường (USER):
Đăng ký tài khoản
Đăng nhập hệ thống
Cập nhật thông tin cá nhân (email, số điện thoại, tên)
Thay đổi mật khẩu
Xem số dư ví và lịch sử giao dịch
Gửi điểm đến người dùng khác (có xác thực OTP)
Quản trị viên (ADMIN):
Đăng nhập hệ thống
Tạo tài khoản người dùng mới
Thực hiện thay đổi thông tin người dùng khác (có OTP)
Xoá tài khoản người dùng
Xem danh sách người dùng
Phục hồi dữ liệu từ bản sao lưu

*Hướng dẫn cài đặt và biên dịch
Yêu cầu:
Trình biên dịch C++ (g++ hoặc MSVC)
Tập tin mã nguồn: PTKL.cpp
Không cần thư viện ngoài

*Cách chạy chương trình
  Khởi động chương trình:
  ./PTKL
  Giao diện dòng lệnh chính:
  === HE THONG QUAN LY VI DIEM ===
  1. Dang nhap
  2. Dang ky
  3. Thoat
     *Các thao tác trong chương trình:
     Nhập tài khoản và mật khẩu để đăng nhập
     Với người dùng mới tạo và mật khẩu tự động, nhập đúng mật khẩu tạm → hệ thống sẽ yêu cầu đổi mật khẩu ở lần đầu đăng nhập
     Các thao tác như xem ví, chuyển điểm, cập nhật thông tin sẽ hiển thị menu cụ thể
