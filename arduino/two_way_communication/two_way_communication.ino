void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
}

char inByte;
void loop() {
  int din[2];
  // put your main code here, to run repeatedly:
  if(Serial.available() >= 3) {
    for (int i = 0; i < 3; i++) {
        din[i] = Serial.read();
    }
    Serial.print(din[0]);
    Serial.print(" ");
    Serial.print(din[1]);
    Serial.print(" ");
    Serial.println(din[2]);
    delay(5);
  }
}

