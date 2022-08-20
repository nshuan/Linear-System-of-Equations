# Hệ phương trình đại số tuyến tính
Giải gần đúng hệ phương trình đại số tuyến tính bằng phương pháp lặp đơn và lặp Seidel.

## Các file text
> Các file text được đặt trong thư mục Texts.

`input.txt` là file chứa ma trận đầu vào mặc định. Ngoài ra, trong thư mục còn có các file logs lưu kết quả từng bước lặp.

## Các file mã nguồn
> Chương trình bao gồm 1 file mã nguồn chính và 4 files phụ được đặt trong thư mục Packages.

### `main.cpp`
File chương trình chính

### `Console.cpp`
> File chứa các hàm dùng để xây tùy chỉnh console và vẽ cửa sổ Menu.

Mỗi ký tự trên màn hình console tương đương với một đơn vị, số ký tự tối đa có thể hiện thị sẽ tương ứng với kích thước của console.
Chương trình sẽ có kích thước cửa sổ console như sau:
* Chiều rộng: 185 đơn vị
* Chiều cao: 50 đơn vị
Ngoài ra kích thước tối đa của chương trình là 200x50.

> Vùng input sẽ bắt đầu từ hàng thứ 36 và chiều rộng bằng chiều rộng tối đa.
> Vùng output sẽ bắt đầu từ cột thứ 4 và chiều cao bắt đầu từ hàng thứ 4 đến hàng thứ 46.
> Chương trình sẽ vô hiệu hóa khả năng phóng to, thu nhỏ và thay đổi kích thước cửa sổ để đảm bảo kết quả hiển thị tốt nhất.
> Các kết quả cần hiển thị ở vùng output sẽ được chuyển đổi thành vector với kiểu dữ liệu là string, mỗi phần tử tương ứng với nội dung một hàng. Vector này sẽ được truyền vào hàm `DrawOutputBox(vector<string>*)` để in ra vùng output. Nếu số hàng cần in quá lớn, chương trình sẽ báo lỗi.

Chương trình sử dụng mã ANSI để thay đổi màu và di chuyển con trỏ trên màn hình ứng dụng. Bộ ký tự (code page) được sử dụng là Unicode (utf-8) với ID 65001, bộ ký tự này sẽ hỗ trợ việc hiển thị tiếng Việt.

### `Functions.cpp`
> File chứa các hàm hỗ trợ cho thuật toán.

Chương trình cho phép hệ phương trình đầu vào có số ẩn tối đa là 10.

File chứa các hàm dùng để đọc, ghi các file, tính toán sai số các phương pháp, xử lý ma trận trước khi in ra vùng output.

Mặc định, chương trình sẽ hiển thị 4 chữ số phần thập phân, con số này có thể thay đổi đến tối đa 6 chữ số.

### `MathFuncs.cpp`
> File chứa một số hàm tính toán trên ma trận.

### `ColorCode.h`
> File chứa các định nghĩa mã màu được dùng trong chương trình.
