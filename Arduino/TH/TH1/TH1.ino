const int ledPins[] = {4, 5, 6, 7, 8, 9, 10, 11}; // 8 LEDs
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

volatile int mode = 1; 
const int interval = 200; 

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  // Ngắt để đổi mode ngay lập tức
  attachInterrupt(digitalPinToInterrupt(2), setMode1, FALLING); 
  attachInterrupt(digitalPinToInterrupt(3), setMode2, FALLING); 
}

void setMode1() {
  mode = 1;
}

void setMode2() {
  mode = 2;
}

void loop(){
    if (mode == 1) {
        threeLedChase();
    }
    else if (mode == 2) {
        runningLed();
    }
}

// Câu a: 3 led chạy nối đuôi xoay vòng [cite: 23]
void threeLedChase(){
    for (int i = 0; i < numLeds; i++) // Chạy đủ 8 bước
    {
        if (mode != 1) return; // Kiểm tra ngắt để thoát ngay lập tức
        
        turnOffAllLeds(); // Xóa trạng thái cũ
        
        // Bật 3 LED liên tiếp, dùng % để xoay vòng về 0 khi vượt quá 7
        digitalWrite(ledPins[i], HIGH); 
        digitalWrite(ledPins[(i + 1) % numLeds], HIGH); 
        digitalWrite(ledPins[(i + 2) % numLeds], HIGH); 
        
        delay(interval);
    }
}

// Câu b: Sáng dần và giảm dần
void runningLed(){
    // Bước 1: Sáng dần (Tích lũy)
    turnOffAllLeds(); // Đảm bảo tắt hết trước khi bắt đầu
    for (int i = 0; i < numLeds; i++)
    {
        if (mode != 2) return; // Kiểm tra ngắt
        digitalWrite(ledPins[i], HIGH); // Bật và GIỮ NGUYÊN trạng thái
        delay(interval);
    }
    
    delay(interval);

    // Bước 2: Tắt dần (Giảm dần từ 8 về 1)
    for (int i = numLeds - 1; i >= 0; i--)
    {
        if (mode != 2) return; // Kiểm tra ngắt
        digitalWrite(ledPins[i], LOW); // Tắt từng đèn một
        delay(interval);
    }
     
    delay(interval);
}

void turnOffAllLeds(){
    for (int i = 0; i < numLeds; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
}