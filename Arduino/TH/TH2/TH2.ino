// C++ code
const int pinLed[] = {3, 5, 6, 9, 10, 11};
const int numLed = sizeof(pinLed) / sizeof(pinLed[0]);
const int tailLength = 3; // Độ dài đuôi sao băng

// Tỉ lệ độ sáng của đuôi (Head -> Tail)
const float tailRatio[] = {1.0, 0.5, 0.25, 0.1}; 

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < numLed; i++) {
    pinMode(pinLed[i], OUTPUT);
  }
}

// Hàm tắt toàn bộ đèn
void allLow() {
  for (int i = 0; i < numLed; i++) {
    digitalWrite(pinLed[i], LOW);
  }
}

void meteorEffect(int headPos, int rawBright, int rawSpeed) {
  allLow(); // Reset trạng thái
  
  // Map giá trị Analog (0-1023)
  int maxBrightness = map(rawBright, 0, 1023, 0, 255);
  int delayTime = map(rawSpeed, 0, 1023, 200, 20);

  // Vẽ sao băng (Đầu + Đuôi)
  for (int i = 0; i < tailLength; i++) {
    int ledIndex = (headPos - i + numLed) % numLed;
    
    // Tính độ sáng
    int brightness = maxBrightness * tailRatio[i];
    analogWrite(pinLed[ledIndex], brightness);
  }
  
  delay(delayTime);
}

void loop() {
  static int currentLed = 0;

  int valBright = analogRead(A0); // Điều chỉnh độ sáng
  int valSpeed = analogRead(A3);  // Điều chỉnh tốc độ

  meteorEffect(currentLed, valBright, valSpeed);

  currentLed = (currentLed + 1) % numLed;

  float volt1 = valBright * (5.0 / 1023.0);
  float volt2 = valSpeed * (5.0 / 1023.0);
  
  Serial.print("Brightness (V): ");
  Serial.print(volt1);
  Serial.print(" | Speed (V): ");
  Serial.println(volt2);
}