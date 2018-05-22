//Calibrate the servo to 180 deg position
#include <Servo.h>

byte PWM_PIN1 = 14;
byte SERVO1 = 9;
int pos_target;
Servo myservo;

void setup() {
  myservo.attach(SERVO1);
  pinMode(PWM_PIN1, INPUT);
  Serial.begin(2000000);
  Serial.println("Please input initial angle.");
  while(Serial.available() <= 0) {
    delay(100);  
  }
  pos_target = Serial.parseInt();
  Serial.println("Training begins: ");
}

void loop() {
  calibrate(pos_target, myservo, PWM_PIN1, 0);
}

void calibrate(int pos_target, Servo myservo, int PWM_PIN, int POS0) {
    int pos = map(pulseIn(PWM_PIN1, HIGH), 0, 1062, -180, 180) + (180 - POS0);
    if(pos_target < pos) {
      myservo.write(max(95,  pos - pos_target + 95));
    }
    else{
      myservo.write(min(90, pos - pos_target + 90));
    }
}
