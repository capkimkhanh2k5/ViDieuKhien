//Check lại logic vì thao tác trên simulator khác với thực tế

const int sensorPins[] = {A0, A1, A2, A3}; 
const int ledPins[]    = {13, 12, 11, 10}; 
const int n = 4;
const int THRESHOLD = 600; 

bool ledState[] = {0, 0, 0, 0}; 
bool wasTouched[] = {0, 0, 0, 0}; // Trạng thái chạm ở vòng lặp trước
unsigned long lastToggleTime[] = {0, 0, 0, 0}; 
unsigned long lastSwipeTime = 0;
unsigned long firstTouchTime[] = {0, 0, 0, 0}; // Thời điểm bắt đầu chạm
int lrStage = 0; 
int rlStage = 0; 

const int DEBOUNCE_TIME = 300;      // Thời gian chống rung
const int SWIPE_TIMEOUT = 400;      // Timeout giữa các bước vuốt
const int EDGE_HOLD_TIME = 200;     // Thời gian giữ để phân biệt chạm đơn/vuốt

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < n; i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void controlAllLeds(int state) {
  for(int i = 0; i < n; i++){
    digitalWrite(ledPins[i], state);
    ledState[i] = state;
    lastToggleTime[i] = millis(); 
  }
  lastSwipeTime = millis();
  lrStage = 0;
  rlStage = 0;
}

void loop() {
  bool isTouched[4]; 
  for(int i = 0; i < n; i++){
    isTouched[i] = analogRead(sensorPins[i]) > THRESHOLD;
  }
  
  unsigned long currentMillis = millis();
  
  // Đếm số cảm biến đang được chạm
  int touchCount = 0;
  for(int i = 0; i < n; i++){
    if(isTouched[i]) touchCount++;
  }
  
  // ----------------------------------------------------
  // RESET STAGE KHI KHÔNG CÒN CHẠM
  // ----------------------------------------------------
  if(touchCount == 0) {
    if(lrStage > 0 && currentMillis - firstTouchTime[lrStage - 1] > SWIPE_TIMEOUT) {
      lrStage = 0;
    }
    if(rlStage > 0 && currentMillis - firstTouchTime[3 - rlStage + 1] > SWIPE_TIMEOUT) {
      rlStage = 0;
    }
  }
  
  // ----------------------------------------------------
  // XỬ LÝ VUỐT TRÁI -> PHẢI
  // ----------------------------------------------------
  if (isTouched[0] && !wasTouched[0] && lrStage == 0 && currentMillis - lastSwipeTime > DEBOUNCE_TIME) {
    lrStage = 1; 
    firstTouchTime[0] = currentMillis;
  } 
  else if (isTouched[1] && !wasTouched[1] && lrStage == 1 && currentMillis - firstTouchTime[0] < SWIPE_TIMEOUT) {
    lrStage = 2; 
    firstTouchTime[1] = currentMillis;
  }
  else if (isTouched[2] && !wasTouched[2] && lrStage == 2 && currentMillis - firstTouchTime[1] < SWIPE_TIMEOUT) {
    lrStage = 3; 
    firstTouchTime[2] = currentMillis;
  }
  else if (isTouched[3] && !wasTouched[3] && lrStage == 3 && currentMillis - firstTouchTime[2] < SWIPE_TIMEOUT) {
    controlAllLeds(HIGH); 
  }
  
  // ----------------------------------------------------
  // XỬ LÝ VUỐT PHẢI -> TRÁI
  // ----------------------------------------------------
  if (isTouched[3] && !wasTouched[3] && rlStage == 0 && currentMillis - lastSwipeTime > DEBOUNCE_TIME) {
    rlStage = 1; 
    firstTouchTime[3] = currentMillis;
  } 
  else if (isTouched[2] && !wasTouched[2] && rlStage == 1 && currentMillis - firstTouchTime[3] < SWIPE_TIMEOUT) {
    rlStage = 2; 
    firstTouchTime[2] = currentMillis;
  }
  else if (isTouched[1] && !wasTouched[1] && rlStage == 2 && currentMillis - firstTouchTime[2] < SWIPE_TIMEOUT) {
    rlStage = 3; 
    firstTouchTime[1] = currentMillis;
  }
  else if (isTouched[0] && !wasTouched[0] && rlStage == 3 && currentMillis - firstTouchTime[1] < SWIPE_TIMEOUT) {
    controlAllLeds(LOW); 
  }
  
  // ----------------------------------------------------
  // XỬ LÝ CHẠM ĐƠN (CHỈ KHI KHÔNG ĐANG VUỐT)
  // ----------------------------------------------------
  if (currentMillis - lastSwipeTime > DEBOUNCE_TIME) {
    for(int i = 0; i < n; i++){
      // Phát hiện cạnh lên (vừa mới chạm)
      if (isTouched[i] && !wasTouched[i]) {
        firstTouchTime[i] = currentMillis;
      }
      
      // Phát hiện cạnh xuống (vừa mới nhả)
      if (!isTouched[i] && wasTouched[i]) {
        unsigned long touchDuration = currentMillis - firstTouchTime[i];
        
        // Kiểm tra xem có đang trong quá trình vuốt không
        bool isSwipeActive = false;
        
        // Với đèn ngoài cùng: chỉ coi là chạm đơn nếu KHÔNG có cảm biến kế bên được chạm
        if (i == 0) {
          isSwipeActive = (lrStage > 1) || (touchDuration < EDGE_HOLD_TIME && (isTouched[1] || wasTouched[1]));
        } 
        else if (i == 3) {
          isSwipeActive = (rlStage > 1) || (touchDuration < EDGE_HOLD_TIME && (isTouched[2] || wasTouched[2]));
        }
        // Với đèn giữa: coi là vuốt nếu có Stage active
        else {
          isSwipeActive = (lrStage > 0 || rlStage > 0);
        }
        
        // Nếu không phải vuốt → Toggle đèn
        if (!isSwipeActive && currentMillis - lastToggleTime[i] > DEBOUNCE_TIME) {
          ledState[i] = !ledState[i];
          digitalWrite(ledPins[i], ledState[i]);
          lastToggleTime[i] = currentMillis;
          
          // Reset stage để tránh xung đột
          if(i == 0) lrStage = 0;
          if(i == 3) rlStage = 0;
        }
      }
    }
  }
  
  // Cập nhật trạng thái cho vòng lặp tiếp theo
  for(int i = 0; i < n; i++){
    wasTouched[i] = isTouched[i];
  }
  
  delay(10); // Giảm nhiễu
}