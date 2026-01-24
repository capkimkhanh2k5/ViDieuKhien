/*
 * TEST KẾT NỐI 74HC595 VÀ LCD 1602 (4-BIT MODE)
 * Mapping:
 * Q0 -> RS
 * Q1 -> RW
 * Q2 -> E
 * Q4-Q7 -> D4-D7
 */

// Khai báo chân Arduino nối với IC
const int DATA_PIN = 2;  // Input (DS)
const int LATCH_PIN = 3; // ORC (ST_CP)
const int CLOCK_PIN = 4; // SRC (SH_CP)

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  
  // Khởi tạo màn hình LCD
  lcd_init();
  
  // In thử dòng chữ
  lcd_print("KET NOI DUNG!");
  
  // Xuống dòng (di chuyển con trỏ đến đầu dòng 2)
  lcd_command(0xC0); 
  lcd_print("Test OK 100%");
}

void loop() {
  // Không làm gì cả
}

// --- CÁC HÀM XỬ LÝ DƯỚI CẤP THẤP ---

void shift_595(byte data) {
  // Đẩy dữ liệu ra IC 74HC595
  // LSBFIRST: Bit 0 (RS) đi trước, Bit 7 (D7) đi sau cùng
  // Điều này khớp với mapping: Q0=RS ... Q7=D7
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
  digitalWrite(LATCH_PIN, HIGH);
}

void lcd_send_4bit(byte nibble, bool rs) {
  // Cấu trúc byte gửi đi: [D7 D6 D5 D4] [NC] [E] [RW] [RS]
  // nibble nằm ở 4 bit cao (D7-D4)
  // rs nằm ở bit 0
  // rw luôn là 0 (Ghi)
  
  byte baseData = (nibble & 0xF0) | (rs ? 0x01 : 0x00);
  
  // Tạo xung E (Enable): LOW -> HIGH -> LOW
  // 1. E = 0
  shift_595(baseData); 
  delayMicroseconds(1);
  
  // 2. E = 1 (Bật bit 2)
  shift_595(baseData | 0x04); 
  delayMicroseconds(1);
  
  // 3. E = 0 (Tắt bit 2)
  shift_595(baseData); 
  delayMicroseconds(50); // Đợi lệnh thực thi
}

void lcd_byte(byte value, bool rs) {
  // Gửi 1 byte: Chia làm 2 lần gửi 4 bit (Cao trước, Thấp sau)
  lcd_send_4bit(value & 0xF0, rs);        // 4 bit cao
  lcd_send_4bit((value << 4) & 0xF0, rs); // 4 bit thấp
}

void lcd_command(byte cmd) {
  lcd_byte(cmd, false); // RS = 0 cho lệnh
}

void lcd_print(const char* str) {
  while (*str) {
    lcd_byte(*str, true); // RS = 1 cho dữ liệu
    str++;
  }
}

void lcd_init() {
  delay(50); // Đợi màn hình khởi động
  
  // Chuỗi khởi tạo đặc biệt cho chế độ 4-bit
  lcd_send_4bit(0x30, false); delay(5);
  lcd_send_4bit(0x30, false); delay(1);
  lcd_send_4bit(0x30, false); delay(1);
  lcd_send_4bit(0x20, false); // Chuyển sang 4-bit mode
  
  // Cấu hình màn hình
  lcd_command(0x28); // 4-bit, 2 dòng, font 5x8
  lcd_command(0x0C); // Bật hiển thị, tắt con trỏ
  lcd_command(0x06); // Tự động tăng con trỏ
  lcd_command(0x01); // Xóa màn hình
  delay(2);
}