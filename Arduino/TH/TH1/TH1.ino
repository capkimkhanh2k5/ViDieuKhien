// C++ code

int Start_pin = 6;
int End_pin = 13;
int Btn1_pin = 3;
int Btn2_pin = 2;
int Mode = 0;

void setup()
{
  for(int i = Start_pin; i <= End_pin; i ++){
  	pinMode(i, OUTPUT);
  }
  
  pinMode(Btn1_pin, INPUT_PULLUP);
  pinMode(Btn2_pin, INPUT_PULLUP);
}

int getCirclePin(int current){
	if(current > End_pin)
      return Start_pin + (current - End_pin - 1);
  	else
      return current;
}

void allLow() {
  for (int i = Start_pin; i <= End_pin; i++) 
    digitalWrite(i, LOW);
}

void Led3Loop(int startPin, int endPin){
  for(int i = startPin; i <= endPin; i += 3){
    allLow();
    
    digitalWrite(i, HIGH);
    digitalWrite(getCirclePin(i + 1), HIGH);
    digitalWrite(getCirclePin(i + 2), HIGH);
    
    delay(300);
  }
}

void AllCircleLoop(int startPin, int endPin){
  allLow();
  
  for(int i = startPin; i <= endPin; i ++){
  	digitalWrite(i, HIGH);
    delay(300);
  }
  
  for(int i = endPin; i >= startPin; i --){
    digitalWrite(i, LOW);
    delay(300);
  }
}

void loop()
{
  //1 -> mode1
  //1 -> mode2
  if(digitalRead(Btn1_pin) == LOW) Mode = 1;
  if(digitalRead(Btn2_pin) == LOW) Mode = 2;
    
  switch(Mode){
    case 1:
    	Led3Loop(Start_pin, End_pin);
    	break;
    case 2:
 		AllCircleLoop(Start_pin, End_pin);
    	break;
    default:
    	allLow();
  }
}