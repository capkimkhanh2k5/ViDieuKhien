#include "LiquidCrystal_74HC595.h"

// Khởi tạo: data, clock, latch
LiquidCrystal_74HC595 lcd(2, 4, 3);

void setup() {
  lcd.begin(16, 2);        // 16 cột, 2 hàng
  lcd.print("Hello!");
  lcd.setCursor(0, 1);
  lcd.print("74HC595 OK!");
}