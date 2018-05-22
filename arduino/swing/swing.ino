//vel_target - (0, 255) =
//vel and pos are revsersed

#include <Servo.h>
//pin config
byte PWM_PIN1 = 14;
byte PWM_PIN2 = 15;
byte POTEN_PIN = A8;
byte SERVO1 = 9;
byte SERVO2 = 10;

// servo
Servo serv1;
Servo serv2;

//intial state and constraints
int t1_millis = 30000;
int Q_MAX = 75;
int POS10 = 360;
int POS20 = 180;
int POS30 = 165;
int a1 = 127; // action 1
int a2 = 127; // action 2
int TIMEOUT_MAX = 1500;
int timeout = 0;

//variables
unsigned long t;
unsigned long t_last;
int pos1;
int pos2;
int pos3;

int read_pos1() {
  return map(analogRead(POTEN_PIN), 0, 1023, -180, 180) + 2;
}

int read_pos2() {
  return map(pulseIn(PWM_PIN1, HIGH), 0, 1062, -180, 180) + (180 - POS20);
}

int read_pos3() {
  return map(pulseIn(PWM_PIN2, HIGH), 0, 1062, -180, 180) + (180 - POS30);
}

void setup() {
  Serial.begin(500000);
  serv1.attach(SERVO1);
  serv2.attach(SERVO2);
  pinMode(PWM_PIN1, INPUT);
  pinMode(PWM_PIN2, INPUT);
  pos1 = read_pos1();
  pos2 = read_pos2();
  t = millis();
  t_last = t;
  for(int i = 0; i < 1000; i++) {    
    calibrate(POS20, serv1, PWM_PIN1);
    calibrate(POS30, serv2, PWM_PIN2);
  }
  set_vel(127, serv1, PWM_PIN1, POS20);
  set_vel(127, serv2, PWM_PIN2, POS30);
}

void loop() {
  while(1) {
    t = millis();
    if (t - t_last >= 1) {
      readData();
      t_last = t;
      getAction();
    }
  }
}

void getAction(){
  if(Serial.available()) {
    a1 = Serial.read();
    a2 = Serial.read();
    set_vel(a1, serv1, PWM_PIN1, POS20);
    set_vel(a2, serv2, PWM_PIN2, POS30);
    timeout = 0;
  }
//  else{
//    timeout ++;
//    if(timeout > TIMEOUT_MAX) {
//      for(int i = 0; i < 1000; i++) {    
//        calibrate(POS20, serv1, PWM_PIN1);
//        calibrate(POS30, serv2, PWM_PIN2);
//      }
//      set_vel(127, serv1, PWM_PIN1, POS20);
//      set_vel(127, serv2, PWM_PIN2, POS30);
//      while(1) {
//        readData();  
//      }
//    }
//  }
}


void readData() {
  t = millis();
  if(t == t_last) {
    return;  
  }
  pos1 = read_pos1();
  pos2 = read_pos2();
  pos3 = read_pos3();
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(pos1);
  Serial.print(" ");
  Serial.print(pos2);
  Serial.print(" ");
  Serial.println(pos3);
  t_last = t; 
}


void set_vel(int vel, Servo servo, byte pwm, int pos0){
  int pos = map(pulseIn(pwm, HIGH), 0, 1062, 0, 360);
  if(vel < 127) {
     vel = map(vel, 0, 127, 115, 95);
//    vel = 120;
  }
  if(vel > 127){
     vel = map(vel, 128, 255, 90, 55);  
//    vel = 70;
  }
  if (vel == 127) {
    vel = 92;  
  }
//  // constraints
//  if (Q_MAX - abs(pos - POS20) < 10) {
//    vel = (vel - 92) / 4 + 92;  
//  }
  // reverse the speed if out of bound, reverse the speed
  if (abs(pos - pos0) > Q_MAX && (pos - pos0) * (vel - 92) < 0) {
      vel = 180 - vel;
  }
  servo.write(vel);
}


void calibrate(int pos_target, Servo myservo, int PWM_PIN) {
    //  vel and pos are reversed
    int pos = map(pulseIn(PWM_PIN, HIGH), 0, 1062, 0, 360);
    if(pos_target < pos) {
      myservo.write(max(95,  pos - pos_target + 95));
    }
    else{
      myservo.write(min(90, pos - pos_target + 90));
    }
}

