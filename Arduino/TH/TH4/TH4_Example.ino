/* Bài tập: Điều khiển đèn LED bằng Quang trở (LDR)
  Nguyên lý: 
  - Khi che tay (trời tối) -> Giá trị A0 > 800 -> Bật đèn.
  - Khi bỏ tay (trời sáng) -> Giá trị A0 <= 800 -> Tắt đèn ngay lập tức.
*/

#define led1 9       // Chân đèn LED
#define sensorPin A0 // Chân quang trở

void setup() {
  Serial.begin(9600);    // Bật cổng Serial để theo dõi giá trị
  pinMode(led1, OUTPUT); // Cấu hình chân LED là đầu ra
}

void loop() {
  // 1. Đọc giá trị từ quang trở
  int sensorValue = analogRead(sensorPin);
  
  // In giá trị lên màn hình để kiểm tra (nếu cần chỉnh ngưỡng)
  Serial.print("Gia tri quang tro: ");
  Serial.println(sensorValue);

  // 2. Kiểm tra điều kiện và điều khiển đèn
  if (sensorValue > 800) { 
    // Nếu giá trị lớn hơn 800 (Trời tối) -> Bật đèn
    digitalWrite(led1, HIGH);
  } 
  else {
    // Nếu giá trị nhỏ hơn hoặc bằng 800 (Trời sáng) -> Tắt đèn
    digitalWrite(led1, LOW);
  }

  // Đợi một chút để ổn định (0.1 giây)
  delay(100); 
}