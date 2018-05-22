// Display pwm signal with time in millisecond.
// Each loop tooks 2ms

byte PWM_PIN1 = 14;
byte PWM_PIN2 = 15;
byte POTEN_PIN = A8;

unsigned long t;
int pwm1;
int pwm2;
int poten;
int poten_last;
//int poten0 = 980;
//int initialized = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  pinMode(PWM_PIN1, INPUT);
  poten= analogRead(POTEN_PIN);
  poten_last = poten;
}

void loop() {
  read();
}
void read() {
  // put your main code here, to run repeatedly:
  pwm1 = pulseIn(PWM_PIN1, HIGH);
  pwm2 = pulseIn(PWM_PIN2, HIGH);
  poten= analogRead(POTEN_PIN);
  t = millis();
  Serial.print("Time: ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(pwm1);
  Serial.print(" ");
  Serial.print(pwm2);
  Serial.print(" ");
  Serial.println(poten);
  delay(1);
}
