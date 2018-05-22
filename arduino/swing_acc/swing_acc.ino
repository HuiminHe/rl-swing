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
int POS10 = 0;
int POS20 = 180;
int POS30 = 165;
int a1; // action 1
int a2; // action 2
int TIMEOUT_MAX = 1500;
int timeout = 0;

//variables
unsigned long t;
unsigned long t_last;
int pos1;
int pos2;
int pos3;

int vel1 = 92;
int vel2 = 92;

// serial
const byte numBytes = 5;
byte receivedBytes[numBytes];
byte numReceived = 0;
boolean newData = false;

void setup() {
  Serial.begin(2000000);
  serv1.attach(SERVO1);
  serv2.attach(SERVO2);
  pinMode(PWM_PIN1, INPUT);
  pinMode(PWM_PIN2, INPUT);
  pos1 = read_pos1();
  pos2 = read_pos2();
  t = millis();
  t_last = t;
  // calibrate
  for(int i = 0; i < 1000; i++) {    
    calibrate(POS20, serv1, PWM_PIN1);
    calibrate(POS30, serv2, PWM_PIN2);
  }
  // hold still
  serv1.write(92);
  serv2.write(92);
}
  
void loop() {
  t = millis();
  sendData();
  recvBytesWithStartEndMarkers();
  takeAction();
  t_last = t;
}

int read_pos1() {
  return map(analogRead(POTEN_PIN), 0, 1062, -90, 90) + POS10;
}

int read_pos2() {
  return map(pulseIn(PWM_PIN1, HIGH), 0, 1062, -180, 180) + (180 - POS20);
}

int read_pos3() {
  return map(pulseIn(PWM_PIN2, HIGH), 0, 1062, -180, 180) + (180 - POS30);
}

void sendData(){
  pos1 = read_pos1();
  pos2 = read_pos2();
  pos3 = read_pos3();
  Serial.print(t);
  Serial.print(" ");
  Serial.print(pos1);
  Serial.print(" ");
  Serial.print(pos2);
  Serial.print(" ");
  Serial.print(pos3);
  Serial.print(" ");
  Serial.print(vel1);
  Serial.print(" ");
  Serial.println(vel2);
}

void recvBytesWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3C; // '<'
    byte endMarker = 0x3E;   // '>'
    byte rb;
   

    while (Serial.available() > 0 && newData == false) {
        rb = Serial.read();

        if (recvInProgress == true) {
            if (rb != endMarker) {
                receivedBytes[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                }
            }
            else {
                receivedBytes[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }

        else if (rb == startMarker) {
            recvInProgress = true;
        }
    }
}

void takeAction(){
  if (newData == true) {
    a1 = receivedBytes[0];
    vel1 += a1;
    vel2 += a2;
    serv1.write(vel1);
    serv2.write(vel2);
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



