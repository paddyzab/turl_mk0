#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *engineM1 = AFMS.getMotor(1);
Adafruit_DCMotor *engineM2 = AFMS.getMotor(2);
Adafruit_DCMotor *engineM3 = AFMS.getMotor(3);
Adafruit_DCMotor *engineM4 = AFMS.getMotor(4);

#define trigPin 13
#define echoPin 12

char dataIn='S';
char determinant;
char det;

int speed_vel = 0;


void setup() {  
  Serial.begin(9600);       

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  AFMS.begin();
  delay(500);
}

void loop(){
  det = check();
  
  long duration; 
  long distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  if (distance < 25) {
    //all_reverse();
  }

  if (distance < 10) {

  }


  while(det == 'W') {
    setSpeeds(speed_vel);
    runEngines();
    det = check();
  }

  while (det == 'S') { 
    all_reverse();
    det = check();
  }

  while (det == 'A') {
    turnLeft(speed_vel);
    runEngines();
    det = check();
  }

  while (det  == 'D') {
    turnRight(speed_vel);
    runEngines();
    det = check();
  }

  while (det == 'X') {
    setSpeeds(0);
    runEngines();
    det = check();
  }

  delay(500);
}

int check() {
  if (Serial.available() > 0) {
    dataIn = Serial.read();


    if (dataIn == 'W') {  // speed up
      determinant  = 'W';
    } 
    else if (dataIn == 'X') { // full stop
      determinant = 'X';
    } 
    else if (dataIn == 'A') { // left
      determinant = 'A';
    } 
    else if (dataIn == 'S') { // reverse
      determinant = 'S';
    } 
    else if (dataIn == 'D') { // right
      determinant = 'D';
    } 
    else if (dataIn == '1') {
      speed_vel = 20;
    }
    else if (dataIn == '2') {
      speed_vel = 40;
    }
    else if (dataIn == '3') {
      speed_vel = 60;
    }
    else if (dataIn == '4') {
      speed_vel = 80;
    }
    else if (dataIn == '5') {
      speed_vel = 100;
    }
    else if (dataIn == '6') {
      speed_vel = 120;
    }
    else if (dataIn == '7') {
      speed_vel = 140;
    }
    else if (dataIn == '8') {
      speed_vel = 160;
    }
    else if (dataIn == '9') {
      speed_vel = 180;
    }
    else if (dataIn == '10') {
      speed_vel = 200;
    }
  }

  return determinant;
}

void all_reverse() {
  engineM1->run(BACKWARD);
  engineM2->run(BACKWARD);
  engineM3->run(BACKWARD);
  engineM4->run(BACKWARD);
}

void setSpeeds(int velocity) {
  engineM1->setSpeed(velocity);
  engineM2->setSpeed(velocity);
  engineM3->setSpeed(velocity);
  engineM4->setSpeed(velocity);
}

void turnLeft(int velocity) {
  engineM1->setSpeed(velocity);
  engineM2->setSpeed(velocity/4);
  engineM3->setSpeed(velocity/4);
  engineM4->setSpeed(velocity);
}

void turnRight(int velocity) {
  engineM1->setSpeed(velocity/4);
  engineM2->setSpeed(velocity);
  engineM3->setSpeed(velocity);
  engineM4->setSpeed(velocity/4);
}

void runEngines() {
  engineM1->run(FORWARD);
  engineM2->run(FORWARD);
  engineM3->run(FORWARD);
  engineM4->run(FORWARD);
}




