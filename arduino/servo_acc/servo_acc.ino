#include <Servo.h>

byte PWM_PIN1 = 14;
byte SERVO1 = 9;
Servo serv1;

const float pi = 3.14;

int t1;
int t0;
void setup() {
  Serial.begin(2000000);
  serv1.attach(SERVO1);
  t0 = millis();
  t1 = t0;
}

void loop() {
  t1 = millis();
  
  if(t1 % 20 < 1)
  Serial.println(t1);
    t0 = t1;
}
