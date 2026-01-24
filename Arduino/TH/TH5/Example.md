# Hướng Dẫn Sử Dụng Thư Viện

/*
 * VÍ DỤ SỬ DỤNG THƯ VIỆN LiquidCrystal_74HC595
 * 
 * Kết nối:
 * Arduino -> 74HC595
 * Pin 2   -> DS (Data)
 * Pin 3   -> ST_CP (Latch)
 * Pin 4   -> SH_CP (Clock)
 * 
 * 74HC595 -> LCD 1602
 * Q0 -> RS
 * Q1 -> RW
 * Q2 -> E
 * Q4 -> D4
 * Q5 -> D5
 * Q6 -> D6
 * Q7 -> D7
 */

``` cpp
#include "LiquidCrystal_74HC595.h"

// Khởi tạo LCD: data=2, clock=4, latch=3
LiquidCrystal_74HC595 lcd(2, 4, 3);

void setup() {
  // Khởi tạo LCD 16 cột, 2 hàng
  lcd.begin(16, 2);
  
  // In dòng chữ
  lcd.print("Hello, World!");
  
  // Xuống dòng 2
  lcd.setCursor(0, 1);
  lcd.print("74HC595 LCD!");
}

void loop() {
  // Không làm gì
}
```

``` cpp
// ============================================
// VÍ DỤ 2: HIỂN THỊ ĐẾM SỐ
// ============================================

void setup() {
  lcd.begin(16, 2);
  lcd.print("Counter:");
}

void loop() {
  static int count = 0;
  
  lcd.setCursor(0, 1);
  lcd.print("                "); // Xóa dòng
  lcd.setCursor(0, 1);
  lcd.print(count);
  
  count++;
  delay(1000);
}
```

``` cpp
// ============================================
// VÍ DỤ 3: CON TRỎ NHẤP NHÁY
// ============================================

void setup() {
  lcd.begin(16, 2);
  lcd.cursor();      // Hiện con trỏ gạch dưới
  lcd.blink();       // Nhấp nháy
  lcd.print("Cursor ON!");
}

void loop() {
  // Không làm gì
}
```

``` cpp
// ============================================
// VÍ DỤ 4: CUỘN CHỮ
// ============================================

void setup() {
  lcd.begin(16, 2);
  lcd.print("Scrolling Text Example!");
}

void loop() {
  // Cuộn sang trái
  for (int i = 0; i < 10; i++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }
  
  // Cuộn sang phải
  for (int i = 0; i < 10; i++) {
    lcd.scrollDisplayRight();
    delay(300);
  }
}
```

``` cpp
// ============================================
// VÍ DỤ 5: KÝ TỰ TỰ TẠO
// ============================================

// Mảng bitmap ký tự trái tim
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

// Ký tự mặt cười
byte smile[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

void setup() {
  lcd.begin(16, 2);
  
  // Tạo ký tự tùy chỉnh tại vị trí 0 và 1
  lcd.createChar(0, heart);
  lcd.createChar(1, smile);
  
  // Hiển thị ký tự tùy chỉnh
  lcd.print("Custom char: ");
  lcd.write(byte(0)); // Hiển thị trái tim
  lcd.write(' ');
  lcd.write(byte(1)); // Hiển thị mặt cười
}

void loop() {
  // Không làm gì
}
```

``` cpp
// ============================================
// VÍ DỤ 6: NHIỆT ĐỘ VÀ ĐỘ ẨM
// ============================================

void setup() {
  lcd.begin(16, 2);
  lcd.print("Temp & Humidity");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Giả lập giá trị cảm biến
  float temp = 25.5;
  float humi = 60.3;
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223); // Ký tự độ
  lcd.print("C  ");
  
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humi);
  lcd.print("%  ");
  
  delay(2000);
}
```

``` cpp
// ============================================
// VÍ DỤ 7: MENU ĐƠN GIẢN
// ============================================

const char* menuItems[] = {
  "1. Settings",
  "2. Data Log",
  "3. About",
  "4. Exit"
};

int menuIndex = 0;

void setup() {
  lcd.begin(16, 2);
  displayMenu();
}

void loop() {
  // Giả lập nút bấm (trong thực tế dùng digitalRead)
  // Nhấn nút -> tăng menuIndex
  
  delay(1000);
  menuIndex = (menuIndex + 1) % 4;
  displayMenu();
}

void displayMenu() {
  lcd.clear();
  lcd.print(menuItems[menuIndex]);
  lcd.setCursor(0, 1);
  lcd.print("Press to select");
}
```

``` cpp
// ============================================
// VÍ DỤ 8: ĐỒ HỒ CƯỜNG ĐỘ TÍNH HIỆU (BAR GRAPH)
// ============================================

byte bar1[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
byte bar2[8] = {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000};
byte bar3[8] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100};
byte bar4[8] = {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110};
byte bar5[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};


void setup() {
  lcd.begin(16, 2);
  
  lcd.createChar(0, bar1);
  lcd.createChar(1, bar2);
  lcd.createChar(2, bar3);
  lcd.createChar(3, bar4);
  lcd.createChar(4, bar5);
  
  lcd.print("Signal Level:");
}


void loop() {
  int level = random(0, 81); // Giá trị 0-80 (16 cột * 5 thanh)
  
  lcd.setCursor(0, 1);
  
  // Vẽ thanh
  for (int i = 0; i < 16; i++) {
    int barValue = level - (i * 5);
    if (barValue >= 5) {
      lcd.write(byte(4)); // Đầy
    } else if (barValue > 0) {
      lcd.write(byte(barValue - 1)); // Một phần
    } else {
      lcd.print(" "); // Trống
    }
  }
  
  delay(500);
}
```