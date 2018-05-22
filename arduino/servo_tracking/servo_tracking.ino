#include <Servo.h>

byte PWM_PIN1 = 14;
byte SERVO1 = 9;
Servo serv1;

const float pi = 3.1415926;
int pos2;
int POS20 = 180;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  serv1.attach(SERVO1);
}


int read_pos2() {
  return map(pulseIn(PWM_PIN1, HIGH), 0, 1062, -180, 180) + (180 - POS20);
}

const int period = 1700;
void loop() {
  // put your main code here, to run repeatedly:
  int t = millis();
 // square wave
  int y = 50;
  if(t % period < period/2) {
    y = 205;  
  }
  // sine wave
//  float x = t / 1000.0 * 2.0 * pi / period;
//  int y = sin(x) * 127 + 127;
  set_vel(y, serv1, PWM_PIN1, 180);
  if (t % 20 == 0) {
    Serial.print(read_pos2());
    Serial.print(" ");
    Serial.println(y);  
  }
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
  servo.write(vel);
}


