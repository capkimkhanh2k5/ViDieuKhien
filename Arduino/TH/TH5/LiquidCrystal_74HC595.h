/*
 * LiquidCrystal_74HC595.h
 * Thư viện LCD 1602/2004 sử dụng IC 74HC595 (Shift Register)
 * 
 * Mapping IC 74HC595:
 * Q0 -> RS (Register Select)
 * Q1 -> RW (Read/Write) 
 * Q2 -> E  (Enable)
 * Q3 -> NC (Không dùng)
 * Q4 -> D4 (Data bit 4)
 * Q5 -> D5 (Data bit 5)
 * Q6 -> D6 (Data bit 6)
 * Q7 -> D7 (Data bit 7)
 * 
 * Tác giả: Chuyển đổi từ LiquidCrystal.h
 * Phiên bản: 1.0
 */

#ifndef LiquidCrystal_74HC595_h
#define LiquidCrystal_74HC595_h

#include <Arduino.h>
#include <inttypes.h>
#include <Print.h>

// Các bit điều khiển trong byte gửi đến 74HC595
#define LCD_RS 0x01  // Bit 0: Register Select
#define LCD_RW 0x02  // Bit 1: Read/Write
#define LCD_E  0x04  // Bit 2: Enable

// Các lệnh LCD
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// Flags cho display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags cho display on/off control
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// Flags cho display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// Flags cho function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00

class LiquidCrystal_74HC595 : public Print {
public:
  // Constructor: chỉ cần 3 chân Arduino
  LiquidCrystal_74HC595(uint8_t data, uint8_t clock, uint8_t latch);
  
  // Khởi tạo màn hình với số cột và hàng
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  
  // Các lệnh điều khiển cơ bản
  void clear();
  void home();
  void setCursor(uint8_t col, uint8_t row);
  
  // Bật/tắt hiển thị
  void noDisplay();
  void display();
  
  // Bật/tắt con trỏ nhấp nháy
  void noBlink();
  void blink();
  
  // Bật/tắt con trỏ gạch dưới
  void noCursor();
  void cursor();
  
  // Cuộn màn hình
  void scrollDisplayLeft();
  void scrollDisplayRight();
  
  // Chế độ nhập liệu
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();
  
  // Tạo ký tự tùy chỉnh
  void createChar(uint8_t location, uint8_t charmap[]);
  
  // Ghi một byte dữ liệu (Override từ Print)
  virtual size_t write(uint8_t value);
  
  // Gửi lệnh trực tiếp
  void command(uint8_t value);
  
  using Print::write;

private:
  // Chân kết nối với 74HC595
  uint8_t _data_pin;
  uint8_t _clock_pin;
  uint8_t _latch_pin;
  
  // Cấu hình màn hình
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  
  uint8_t _numlines;
  uint8_t _row_offsets[4];
  
  // Hàm cấp thấp
  void send(uint8_t value, uint8_t mode);
  void write4bits(uint8_t value, uint8_t mode);
  void pulseEnable(uint8_t data);
  void shiftOut595(uint8_t data);
};

#endif