#include <SoftwareSerial.h>
SoftwareSerial BTSerial(8, 9);

int speedRight = 6;

int dirLeft = 3;
int speedLeft = 11;

int dirRight = 7;
int angleDirection = 4;
int angleSpeed = 5;

int pinAngleStop = 12;

String val;

int speedTurn = 180;

int pinRed = A0;
int pinWhite = A1;
int pinBlack = A2;

long lastTakeInformation;
boolean readAngle = false;
boolean readSpeed = false;

void setup() {
  // put your setup code here, to run once:

   
  pinMode(dirLeft, OUTPUT);
  pinMode(speedLeft, OUTPUT);
  
  pinMode(dirRight, OUTPUT);
  pinMode(speedRight, OUTPUT);
  
  pinMode(pinRed, INPUT);
  pinMode(pinBlack, INPUT);
  pinMode(pinWhite, INPUT);

  pinMode(pinAngleStop, OUTPUT);

  pinMode(angleDirection, OUTPUT);
  pinMode(angleSpeed, OUTPUT);

  BTSerial.begin(38400);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(BTSerial.available() > 0) {
     char a = BTSerial.read();
     
    if (a == '@') {
      val = "";
      readSpeed = true;
    } else if (readSpeed) {
      if(a == '#') {
        Serial.println(val);
        readSpeed = false;
        go(val.toInt());
        val = "";
        return;
      }
      val+=a;
    } else if (a == '*') {
      readAngle = true; 
    } else if (readAngle) {
      if(a == '#') {
       Serial.println(val);
       Serial.println("-----");
        readAngle = false;
        turn(val.toInt());
        val= "";
        return;
      }
      val+=a;
    }
    
    lastTakeInformation = millis();
  } else {
     if(millis() - lastTakeInformation > 150) {
     lastTakeInformation = 0;
     analogWrite(angleSpeed, 0);
     analogWrite(speedRight, 0);
     analogWrite(speedLeft, 0);
     }
     
  }

  
}



void go(int mySpeed) {
  if(mySpeed > 0) {
  digitalWrite(dirRight, HIGH);
  analogWrite(speedRight, mySpeed);
  digitalWrite(dirLeft, HIGH);
  analogWrite(speedLeft, mySpeed);
  } else {
    digitalWrite(dirRight, LOW);
    analogWrite(speedRight, abs(mySpeed) + 30);
    digitalWrite(dirLeft, LOW);
     analogWrite(speedLeft, abs(mySpeed) + 30);
  }
  delay(10);
 
}

void turn(int angle) {
  digitalWrite(pinAngleStop, HIGH);
  delay(5);
  
  if(angle > 149) {
        if( digitalRead(pinWhite) == HIGH && digitalRead(pinBlack) == LOW && digitalRead(pinBlack) == LOW) {
          return;
        }
        digitalWrite(angleDirection, HIGH);
        analogWrite(angleSpeed, speedTurn);
  } else if (angle < 31) { 
        if(digitalRead(pinRed) == HIGH && digitalRead(pinBlack) == HIGH && digitalRead(pinWhite) == HIGH) {
          return;
        }
        digitalWrite(angleDirection, LOW);
        analogWrite(angleSpeed, speedTurn);
  }
  digitalWrite(pinAngleStop, LOW);
  delay(5);
}
