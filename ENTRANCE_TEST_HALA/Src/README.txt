README.txt
 Created on: Dec 29, 2024
     Author: Loc

     
------------TẠO DELAY NGẮT TIMER TRÊN STM32F446RE--------------
*Mục tiêu
	- Hiểu cách hoạt động của Timer và ngắt trên STM32F446RE.
	- Cấu hình Timer và quản lý ngắt bằng cách gắn giá trị trực tiếp vào các thanh ghi.
	- Viết API để dễ sử dụng lại các chức năng Timer.
	
1. Tìm hiểu cấu trúc hệ thống.
- Timer 2 sẽ được sử dụng để tạo ngắt mỗi 1ms. Chương trình bao gồm các thành phần chính:
	+ Cấu hình Timer: Gắn giá trị trực tiếp vào các thanh ghi để thiết lập Prescaler, ARR (Auto Reload Register).
	+ Quản lý ngắt: Kích hoạt ngắt Timer trong NVIC và xử lý ngắt để giảm biến đếm.
	+ API Timer: Tạo các API để:
	+ Cấu hình Timer.
	+ Bắt đầu và dừng Timer.
	+ Tạo delay bằng ngắt Timer.
	+ Ứng dụng: Bật/tắt LED với khoảng thời gian delay 1 giây.
	
////Công thức:
////Ft=Fs/((Prescaler+1)(period+1))
////Ft: tần số của timer
////Fs:tần số của hệ thống (ở đây chọn bộ HSI với tần số 16MHz của STM32F446RE page121-RM)
////prescaler: bộ chia tần
////period: độ phân giải của xung (khi đếm đến giá trị cực hạn được cài đặt sẽ xảy ra sự kiên)(ARR)

2. Tìm hiểu địa chỉ thanh ghi
Các thanh ghi cần thiết cho việc cấu hình Timer và NVIC:
	- Chọn nguồn dao động (HSI 16MHz)
	RCC_CR: chọn nguồn dao động
	Địa chỉ: 0x4002 3800 (base) + 0x00(offset)
	*RCC_CR |= (1<<0);// HSI on, default 16MHz
	RCC_CFGR: chọn HSI là nguồn dao động.
	Địa chỉ: 0x4002 3800 (base) + 0x08(offset)
	*RCC_CFGR &= ~(0x3<<21);// 00: HSI clock selected
	
	- TIM2 thuộc bộ APB1 (page59-RM)
	RCC_APB1ENR: Kích hoạt clock cho Timer2.
	Địa chỉ: 0x4000 0000 (base) + 0x40 (offset).
	*RCC_APB1ENR |= (1<<0); //enb TIM2
	----------
	TIM2_CR1: Thanh ghi điều khiển Timer2.
	Địa chỉ: 0x40000000 (base) + 0x00 (offset).
	*TIM2_CR1 |= (1<<0);// Bit[0] :Counter enable
	----------
	TIM2_PSC: Thanh ghi prescaler.
	Địa chỉ: 0x40000000(base)+0x28(offset).
	----------
	TIM2_ARR: Auto Reload Register.
	Địa chỉ: 0x40000000(base)+0x2C(offset).
	----------
	TIM2_DIER: Enable ngắt Timer2.
	Địa chỉ: 0x40000000(base)+0xC(offset).
	*TIM2_DIER |=(1<<0);// UIE 
	----------
	TIM2_SR: Status register để kiểm tra sự kiện ngắt.
	Địa chỉ: 0x40000000(base) + 0x10(offset).
	
	----------
	NVIC_ISER0: Enable ngắt trong NVIC.
	Địa chỉ: 0xE000E100.
	ISER0: quản lí các từ 0-31
	ISER1: quản lí các từ 32-63
	ISER2: quản lí các từ 64-95
	TIMER2 là 37 thuộc ISER1
	Địa chỉ NVIC_ISER1: 0xE000E104 (cách nhau 32bit)

	
	
	
	
	