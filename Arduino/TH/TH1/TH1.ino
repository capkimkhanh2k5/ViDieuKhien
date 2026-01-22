#include <LiquidCrystal.h>

// Tần số nốt nhạc đầy đủ
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_E3  165
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_F5  698

// Setup LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Setup LED + Còi Buzzer
const int leds[] = {9, 8, A4, A5}; 
const int pinCoi = 13;

// Nội dung hiển thị
char txt[] = "HELLO WORLD";
int len_txt = 11;
int currentIndex = -11; 

// Quản lý trạng thái
int mode = 0;
unsigned long lastScrollTime = 0; 
const int scrollInterval = 300;

void setup() {
    lcd.begin(16, 2);
    pinMode(A0, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
    
    for(int i = 0; i < 4; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }
    pinMode(pinCoi, OUTPUT);
}

void playMario() {
    // Giai điệu điệp khúc
    int melody[] = {
        NOTE_E4, NOTE_E4, 0, NOTE_E4, 0, NOTE_C4, NOTE_E4, 0, NOTE_G4, 0, NOTE_G3, 0,
        NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_AS3, NOTE_A3
    };
    int duration[] = {
        125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
        200, 200, 200, 200, 200, 200, 200
    };
    for (int i = 0; i < 19; i++) {
        if (melody[i] == 0) delay(duration[i]);
        else {
            tone(pinCoi, melody[i], duration[i]);
            delay(duration[i] * 1.3);
        }
    }
    noTone(pinCoi);
}

void playStarWars() {
    // Điệp khúc hành khúc đế chế
    int melody[] = {
        NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5,
        NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
        NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5,
        NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4
    };
    int duration[] = {
        500, 500, 500, 350, 150,
        500, 350, 150, 1000,
        500, 500, 500, 350, 150,
        500, 350, 150, 1000
    };
    for (int i = 0; i < 18; i++) {
        tone(pinCoi, melody[i], duration[i]);
        delay(duration[i] * 1.2);
    }
    noTone(pinCoi);
}

void playHappyBirthday() {
    int melody[] = {
        NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, 0,
        NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, 0,
        NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4
    };
    int duration[] = {
        200, 200, 400, 400, 400, 800, 200,
        200, 200, 400, 400, 400, 800, 200,
        200, 200, 400, 400, 400, 400, 400
    };
    for (int i = 0; i < 21; i++) {
        if (melody[i] == 0) delay(duration[i]);
        else {
            tone(pinCoi, melody[i], duration[i]);
            delay(duration[i] * 1.2);
        }
    }
    noTone(pinCoi);
}

void playJingleBells() {
    int melody[] = {
        NOTE_E4, NOTE_E4, NOTE_E4, 0, NOTE_E4, NOTE_E4, NOTE_E4, 0,
        NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4, 0,
        NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
        NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4
    };
    int duration[] = {
        200, 200, 400, 100, 200, 200, 400, 100,
        200, 200, 300, 100, 800, 100,
        200, 200, 200, 200, 200, 200, 200, 100,
        200, 200, 200, 200, 800
    };
    for (int i = 0; i < 27; i++) {
        if (melody[i] == 0) delay(duration[i]);
        else {
            tone(pinCoi, melody[i], duration[i]);
            delay(duration[i] * 1.2);
        }
    }
    noTone(pinCoi);
}

// Chạy hiệu ứng tương ứng với chế độ
void runEffect(int currentMode) {
    for(int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
    
    switch(currentMode) {
        case 1: digitalWrite(leds[0], HIGH); playMario(); break;
        case 2: digitalWrite(leds[1], HIGH); playStarWars(); break;
        case 3: digitalWrite(leds[2], HIGH); playHappyBirthday(); break;
        case 4: digitalWrite(leds[3], HIGH); playJingleBells(); break;
    }
}

// Xoá 1 hàng trên LCD
void clearRow(int row) {
    lcd.setCursor(0, row);
    for(int i = 0; i < 16; i++) lcd.print(" "); 
}

// Hiển thị chữ chạy
void printTxt(){
    if(millis() - lastScrollTime > scrollInterval){
        lastScrollTime = millis();
        clearRow(0);
        for(int i = 0; i < len_txt; i++) {
            int position = currentIndex + i;
            if(position >= 0 && position < 16) {
                lcd.setCursor(position, 0);
                lcd.print(txt[i]);
            }
        }
        currentIndex++;
        if (currentIndex >= 16) currentIndex = -len_txt;
    }
}

void loop() {
    printTxt();

    lcd.setCursor(0, 1);
    lcd.print(">I Love Arduino");

    int pressedMode = 0;
    if(digitalRead(A0) == LOW) pressedMode = 1;
    else if(digitalRead(A1) == LOW) pressedMode = 2;
    else if(digitalRead(A2) == LOW) pressedMode = 3;
    else if(digitalRead(A3) == LOW) pressedMode = 4;

    if (pressedMode != 0) {
        mode = pressedMode;
        clearRow(1);
        lcd.setCursor(0, 1);
        
        // Hiển thị tên bài hát tương ứng
        switch(mode) {
            case 1: lcd.print(">M1: MarioTheme"); break;
            case 2: lcd.print(">M2: StarWars"); break;
            case 3: lcd.print(">M3: HappyB-Day"); break;
            case 4: lcd.print(">M4: JingleBells"); break;
        }

        runEffect(mode);
        
        // Chờ thả nút để không bị lặp
        while(digitalRead(A0) == LOW || digitalRead(A1) == LOW || 
              digitalRead(A2) == LOW || digitalRead(A3) == LOW); 
    }
}

// C++ Optimized Code
const int Start_pin = 5;
const int End_pin = 13;
const int Btn1_pin = 3;
const int Btn2_pin = 2;

int Mode = 0;
int currentStep = 0;
unsigned long lastUpdate = 0;
const long interval = 300;

bool filling = true; 

void setup() {
  for (int i = Start_pin; i <= End_pin; i++) pinMode(i, OUTPUT);
  pinMode(Btn1_pin, INPUT_PULLUP);
  pinMode(Btn2_pin, INPUT_PULLUP);
  allLow();
}

int getCirclePin(int current) {
  if (current > End_pin) return Start_pin + (current - End_pin - 1);
  return current;
}

void allLow() {
  for (int i = Start_pin; i <= End_pin; i++) digitalWrite(i, LOW);
}

// Chạy 3 LED nối tiếp
void runLed3Loop() {
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    allLow();
    
    int p1 = Start_pin + currentStep;
    digitalWrite(p1, HIGH);
    digitalWrite(getCirclePin(p1 + 1), HIGH);
    digitalWrite(getCirclePin(p1 + 2), HIGH);

    currentStep = (currentStep + 1) % (End_pin - Start_pin + 1);
  }
}

// Sáng dần rồi tắt dần
void runAllCircleLoop() {
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    
    if (filling) {
      digitalWrite(Start_pin + currentStep, HIGH);
      currentStep++;
      if (currentStep > (End_pin - Start_pin)) {
        filling = false;
        currentStep = End_pin - Start_pin;
      }
    } else {
      digitalWrite(Start_pin + currentStep, LOW);
      currentStep--;
      if (currentStep < 0) {
        filling = true;
        currentStep = 0;
      }
    }
  }
}

void loop() {
  if (digitalRead(Btn1_pin) == LOW) {
    if (Mode != 1) { Mode = 1; currentStep = 0; allLow(); }
  }
  if (digitalRead(Btn2_pin) == LOW) {
    if (Mode != 2) { Mode = 2; currentStep = 0; filling = true; allLow(); }
  }

  switch (Mode) {
    case 1: runLed3Loop(); break;
    case 2: runAllCircleLoop(); break;
    default: allLow(); break;
  }
}