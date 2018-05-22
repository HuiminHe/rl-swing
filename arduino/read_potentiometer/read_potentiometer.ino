// potentiometer reading is faster than 1ms

int pos1;
int pos1_last;
int vel1;

void setup() {
  // initialize the serial communication:
  Serial.begin(2000000);
  pos1 = analogRead(A8);
  pos1_last = pos1;
}

void loop() {
  // send the value of analog input 0:
  pos1 = map(analogRead(A8), 0 , 1023, -180, 180);
  vel1 = pos1 - pos1_last;
  pos1_last = pos1;
  Serial.println(pos1);
//  Serial.print(" ");
//  Serial.print(vel1);
//  Serial.print(" ");
//  Serial.print(0);
//  Serial.print(" ");
//  Serial.println(1000);  
  
  delay(10);
}
