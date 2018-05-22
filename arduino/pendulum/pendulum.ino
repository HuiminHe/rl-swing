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
int poten0 = 53;
int t1_millis = 20000;
int Q_MAX = 75;
int POS10 = 314;
int POS20 = 180;
int POS30 = 180;

//variables
unsigned long t;
unsigned long t_last;
int pos1;
int pos2;
int pos3;

int a1 = 127;

int read_pos1() {
  return map(analogRead(POTEN_PIN), 0, 1023, 0, 360) - POS10;
}

int read_pos2() {
  return map(pulseIn(PWM_PIN1, HIGH), 0, 1062, 0, 360) - POS20;
}

int read_pos3() {
  return map(pulseIn(PWM_PIN2, HIGH), 0, 1062, 0, 360) - POS20;
}

void setup() {
  Serial.begin(9600);
  serv1.attach(SERVO1);
  pinMode(PWM_PIN1, INPUT);
  pos1 = read_pos1();
  pos2 = read_pos2();
  t = millis();
  t_last = t;
  Serial.println("Please input initial angle.");
  while(Serial.available() <= 0) {
    delay(100);  
  }
  int pos_target = Serial.parseInt();
  calibrate(pos_target, serv1, PWM_PIN1);
  Serial.println("Training begins: ");
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
    int a1 = Serial.read();
//    if(a1 > 255 || a1 < 0) {
//      Serial.println("Waiting for reeset");
//      calibrate(180, serv1, PWM_PIN1);
//      while(1) {}
//    }
    set_vel(serv1, a1);
    
  }
  else {
    set_vel(serv1, 127);  
  }
}


void readData() {
  t = millis();
  if(t == t_last) {
    return;  
  }
  pos1 = read_pos1();
  pos2 = read_pos2();
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(pos1);
  Serial.print(" ");
  Serial.print(pos2);
  Serial.print(" ");
  Serial.println(a1);

  t_last = t;
  
}


void set_vel(Servo servo, int vel){
  int pos = map(pulseIn(PWM_PIN1, HIGH), 0, 1062, 0, 360);
  if(vel < 127) {
    // vel = map(vel, 0, 127, 115, 95);
    vel = 120;
  }
  if(vel > 127){
    // vel = map(vel, 128, 255, 90, 55);  
    vel = 70;
  }
  if (vel == 127) {
    vel = 92;  
  }
  // constraints
  if (Q_MAX - abs(pos - POS20) < 10) {
    vel = (vel - 92) / 4 + 92;  
  }
  if (abs(pos - POS20) > Q_MAX && (pos - POS20) * (vel - 92) < 0) {
      vel = 92;
  }
  servo.write(vel);
}


void calibrate(int pos_target, Servo myservo, int PWM_PIN) {
    //  vel and pos are reversed
    for(int i = 0; i < 1000; i++) {    
      int pos = map(pulseIn(PWM_PIN, HIGH), 0, 1062, 0, 360);
      if(pos_target < pos) {
        myservo.write(max(95,  pos - pos_target + 95));
      }
      else{
        myservo.write(min(90, pos - pos_target + 90));
      }
      // Serial.println(pos_target - pos);
    }
}

