//Calibrate the servo to 180 deg position
#include <Servo.h>

byte PWM_PIN1 = 14;
byte PWM_PIN2 = 15;
byte SERVO1 = 9;
byte SERVO2 = 10;
int vel;
int pos1 = -1;
int pos1_last = -1;
int POS10 = 314;
int POS20 = 180;
int POS30 = 170;
int t;
int t_last;

int Q_MAX = 45;
int my_vel = 254;

// servo
Servo serv1;
Servo serv2;


void setup() {
  serv1.attach(SERVO1);
  serv2.attach(SERVO2);
  
  pinMode(PWM_PIN1, INPUT);
  pinMode(PWM_PIN2, INPUT);
  Serial.begin(2000000);
  for(int i = 0; i < 500; i++) {    
    calibrate(POS20, serv1, PWM_PIN1);
    calibrate(POS30, serv2, PWM_PIN2);
  }
  serv1.write(92);
  serv2.write(92);
  t = millis();
  t_last = millis();
//  Serial.println("Please input a number for the given servo.(0 to 1023)");
//  my_vel = readint();
//  Serial.println(my_vel);
}


void loop() {
}



int readint(){
  //  read an integer from serial port
  String inString = "";
  while(!Serial.available()) {}
  while(Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;  
    }
    if (inChar == '\n') {
      return inString.toInt();
    }  
  }  
}

void calibrate(int pos_target, Servo myservo, int PWM_PIN) {
    //  vel and pos are reversed
    int pos = map(pulseIn(PWM_PIN, HIGH), 0, 1062, 0, 360);
    if(pos_target < pos) {
      myservo.write(max(98,  pos - pos_target + 95));
    }
    else{
      myservo.write(min(87, pos - pos_target + 90));
    }
}



