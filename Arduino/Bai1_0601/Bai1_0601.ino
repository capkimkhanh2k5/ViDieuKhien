int LED_IN = 13;

void setup() {
  Serial.begin(9600);
  pinMode(LED_IN, INPUT_PULLUP);
}

void loop() {
  // Bật điện chân 13
  digitalWrite(LED_IN, HIGH); 
  Serial.println("Dien ap HIGH! (Dang cap 5V)");
  delay(1000);

  // Tắt điện chân 13
  digitalWrite(LED_IN, LOW);
  Serial.println("Dien ap LOW! (Dang ngat dien)");
  delay(1000);

  int status = digitalRead(LED_IN);
  
  if(status == HIGH){
    Serial.println("Co dong dien (HIGH)");
  }
  else {
    Serial.println("Khong co dien (LOW)");
  }

  delay(1000);
}