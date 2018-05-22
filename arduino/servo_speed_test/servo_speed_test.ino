// Servo speed test.
#include <Servo.h>

byte PWM_PIN1 = 15;
byte SERVO_PIN1 = 10;
Servo myservo;
int pwm1;
int t1;
int t2;

void setup() {
  myservo.attach(SERVO_PIN1);  // attaches the servo on pin 9 to the servo object
  Serial.begin(2000000);
  pinMode(PWM_PIN1, INPUT);
  t1 = 0;
  t2 = 0;
}

void loop() {
    myservo.write(1);              // tell servo to go to position in variable 'pos'
    pwm1 = pulseIn(PWM_PIN1, HIGH);
//    Serial.println(pwm1);
    if(pwm1 < 40) 
    {
      t1 = millis();
    }
    if(t1 - t2 > 400) 
    {
         Serial.println(t1 - t2);
        t2 = t1;
    }
}

