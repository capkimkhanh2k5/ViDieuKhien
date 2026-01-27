int ledPin = 9;
int ledPin2 = 10;
int ledPin3 = 8;

volatile int mode = 1;   // 1 = ON, 0 = OFF

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  pinMode(2, INPUT_PULLUP);  // nút ngắt
  pinMode(3, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), setMode, CHANGE);
}

void setMode() {
  // Nhấn nút → LOW → tắt LED
  // Thả nút → HIGH → bật LED
  mode = digitalRead(2);
  digitalWrite(ledPin3, HIGH);
  delay(200);
  digitalWrite(ledPin3, LOW);
}

void loop() {
  // LED 2 đọc trạng thái pin 3
  if (digitalRead(3) == HIGH) {
    digitalWrite(ledPin2, HIGH);
  } else {
    digitalWrite(ledPin2, LOW);
  }

  // LED 1 theo interrupt
  if (mode == HIGH) {
    digitalWrite(ledPin, HIGH);  // thả nút → ON
  } else {
    digitalWrite(ledPin, LOW);   // nhấn nút → OFF
  }
  delay(5000);
}