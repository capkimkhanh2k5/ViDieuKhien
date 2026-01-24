/*
 * LiquidCrystal_74HC595.cpp
 * Implementation của thư viện LCD 1602/2004 với IC 74HC595
 */

#include "LiquidCrystal_74HC595.h"

// Constructor
LiquidCrystal_74HC595::LiquidCrystal_74HC595(uint8_t data, uint8_t clock, uint8_t latch) {
  _data_pin = data;
  _clock_pin = clock;
  _latch_pin = latch;
  
  pinMode(_data_pin, OUTPUT);
  pinMode(_clock_pin, OUTPUT);
  pinMode(_latch_pin, OUTPUT);
}

// Khởi tạo màn hình
void LiquidCrystal_74HC595::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  // Thiết lập địa chỉ dòng cho các loại màn hình
  if (lines > 1) {
    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
  } else {
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  }
  
  _numlines = lines;
  
  // Địa chỉ bắt đầu mỗi dòng
  _row_offsets[0] = 0x00;
  _row_offsets[1] = 0x40;
  _row_offsets[2] = 0x00 + cols;
  _row_offsets[3] = 0x40 + cols;
  
  // Font size 5x10 chỉ có trên màn hình 1 dòng
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }
  
  // Chờ màn hình khởi động (>40ms sau khi nguồn ổn định)
  delay(50);
  
  // Đưa tất cả các chân về LOW
  shiftOut595(0x00);
  delay(10);
  
  // Chuỗi khởi tạo đặc biệt cho 4-bit mode
  // Theo datasheet HD44780
  write4bits(0x03, 0);
  delayMicroseconds(4500);
  
  write4bits(0x03, 0);
  delayMicroseconds(4500);
  
  write4bits(0x03, 0);
  delayMicroseconds(150);
  
  // Chuyển sang 4-bit mode
  write4bits(0x02, 0);
  
  // Thiết lập số dòng, font size
  command(LCD_FUNCTIONSET | _displayfunction);
  
  // Bật hiển thị, tắt cursor và blink
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();
  
  // Xóa màn hình
  clear();
  
  // Thiết lập chế độ nhập liệu
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Xóa màn hình
void LiquidCrystal_74HC595::clear() {
  command(LCD_CLEARDISPLAY);
  delayMicroseconds(2000); // Lệnh này mất thời gian
}

// Đưa con trỏ về đầu
void LiquidCrystal_74HC595::home() {
  command(LCD_RETURNHOME);
  delayMicroseconds(2000);
}

// Đặt vị trí con trỏ
void LiquidCrystal_74HC595::setCursor(uint8_t col, uint8_t row) {
  if (row >= _numlines) {
    row = _numlines - 1;
  }
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Tắt hiển thị
void LiquidCrystal_74HC595::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Bật hiển thị
void LiquidCrystal_74HC595::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Tắt con trỏ nhấp nháy
void LiquidCrystal_74HC595::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Bật con trỏ nhấp nháy
void LiquidCrystal_74HC595::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Tắt con trỏ gạch dưới
void LiquidCrystal_74HC595::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Bật con trỏ gạch dưới
void LiquidCrystal_74HC595::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Cuộn màn hình sang trái
void LiquidCrystal_74HC595::scrollDisplayLeft() {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

// Cuộn màn hình sang phải
void LiquidCrystal_74HC595::scrollDisplayRight() {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// Chế độ ghi từ trái sang phải
void LiquidCrystal_74HC595::leftToRight() {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Chế độ ghi từ phải sang trái
void LiquidCrystal_74HC595::rightToLeft() {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Bật tự động cuộn
void LiquidCrystal_74HC595::autoscroll() {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Tắt tự động cuộn
void LiquidCrystal_74HC595::noAutoscroll() {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Tạo ký tự tùy chỉnh
void LiquidCrystal_74HC595::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // Chỉ có 8 vị trí (0-7)
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(charmap[i]);
  }
}

// Ghi một byte dữ liệu (Override từ Print)
size_t LiquidCrystal_74HC595::write(uint8_t value) {
  send(value, LCD_RS);
  return 1;
}

// Gửi lệnh
void LiquidCrystal_74HC595::command(uint8_t value) {
  send(value, 0);
}

// ==================== HÀM CẤP THẤP ====================

// Gửi byte dữ liệu hoặc lệnh
void LiquidCrystal_74HC595::send(uint8_t value, uint8_t mode) {
  // Gửi 4 bit cao trước
  write4bits((value >> 4) & 0x0F, mode);
  // Gửi 4 bit thấp
  write4bits(value & 0x0F, mode);
}

// Gửi 4 bit dữ liệu
void LiquidCrystal_74HC595::write4bits(uint8_t value, uint8_t mode) {
  // Cấu trúc byte: [D7 D6 D5 D4] [NC] [E] [RW] [RS]
  // value: 4 bit dữ liệu (sẽ được dịch lên vị trí D7-D4)
  // mode: LCD_RS (0x01) nếu là dữ liệu, 0 nếu là lệnh
  
  uint8_t data = (value << 4) | mode; // RW luôn = 0 (ghi)
  pulseEnable(data);
}

// Tạo xung Enable
void LiquidCrystal_74HC595::pulseEnable(uint8_t data) {
  // E = 0
  shiftOut595(data & ~LCD_E);
  delayMicroseconds(1);
  
  // E = 1
  shiftOut595(data | LCD_E);
  delayMicroseconds(1);
  
  // E = 0
  shiftOut595(data & ~LCD_E);
  delayMicroseconds(50); // Thời gian thực thi lệnh
}

// Gửi dữ liệu đến IC 74HC595
void LiquidCrystal_74HC595::shiftOut595(uint8_t data) {
  digitalWrite(_latch_pin, LOW);
  shiftOut(_data_pin, _clock_pin, MSBFIRST, data);
  digitalWrite(_latch_pin, HIGH);
}