/* 
Sử dụng IC 74HC595 để điều khiển hiển thị 2 led 7 thanh đếm từ 0 đến 20
74HC595:
SDI   pin 2
SRCLK pin 3
SCLK  pin 4
CLR   pin 5
led1  pin 6
led2  pin 7
*/

#define SDI   2
#define RCLK  3
#define SRCLK 4
#define CLR   5
#define led1  6
#define led2  7

byte deCodeArr[10] = {
  B01000000, // 0
  B01111001, // 1
  B00100100, // 2
  B00110000, // 3
  B00011001, // 4
  B00010010, // 5
  B00000010, // 6
  B01111000, // 7
  B00000000, // 8
  B00010000  // 9
};

void setup() {
  pinMode(SDI, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(CLR, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(CLR, LOW);
  digitalWrite(CLR, HIGH);
  digitalWrite(SRCLK, LOW);
  digitalWrite(RCLK, LOW);
}

void loop() {
  byte count;
  unsigned int changeTime;
  
  while (1) {
    // Đếm biến count từ 0 đến 20 và lặp lại
    for (count = 0; count <= 20; count++) {
      // Quét 2 led 7 thanh 25 lần x (10ms + 10ms) = 500ms, là thời gian thay đổi biến đếm
      for (changeTime = 0; changeTime <= 25; changeTime++) {
        // Hiển thị hàng đơn vị
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        writeTo595(deCodeArr[count % 10]);
        delay(10);
        
        // Hiển thị hàng chục
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        writeTo595(deCodeArr[count / 10]);
        delay(10);
      }
    }
  }
}

//================================================
// Hàm ghi 8 bit dữ liệu ra IC 74HC595
void writeTo595(byte dataIn) {
  byte shiftNum;
  
  // Ghi 8 bit của dataIn ra 74HC595
  for (shiftNum = 0; shiftNum < 8; shiftNum++) {
    digitalWrite(SDI, (dataIn >> shiftNum) & B00000001);
    
    // Tạo xung clock trên chân SRCLK để nhập dữ liệu từ SDI vào thanh ghi dịch (Shift Register)
    digitalWrite(SRCLK, HIGH);
    digitalWrite(SRCLK, LOW);
  }
  
  // Tạo xung clock trên chân RCLK để xuất dữ liệu từ Shift Register ra ngoài
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}
