## Ramdom-Number-Linux
Tạo ngẫu nhiên số trong Kernel Linux

## Nội dung đồ án:
Mục tiêu hiểu về Linux kernel module và hệ thống quản lý file và device trong linux, giao tiếp giữa tiến trình ở user space và code kernel space 
+ Viết một module dùng để tạo ra số ngẫu nhiên. 
+ Module này sẽ tạo một character device để cho phép các tiến trình ở userspace có thể open và read các số ngẫu nhiên.


## Cách sử dụng
Chọn Folder chứa mã nguồn, click phải chọn Open In Terminal
Gõ make để tạo file RandomModule.ko  
+ sudo insmod Random.ko -> sudo ./test (Để chạy thử) 
+ gcc testRandom.c -o test (Biên dịch file testRandom.c để đọc và hiển thị nội dung từ file thiết bị)
+ dmesg (Để kiểm tra quá trình hoạt động của việc cài đặt module driver) 
+ sudo rmmod Random.ko (Remove Module)

