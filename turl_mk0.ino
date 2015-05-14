#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *engineM1 = AFMS.getMotor(1);
Adafruit_DCMotor *engineM2 = AFMS.getMotor(2);
Adafruit_DCMotor *engineM3 = AFMS.getMotor(3);
Adafruit_DCMotor *engineM4 = AFMS.getMotor(4);
String inData;

int state = 0;
int speed_val = 0;
int turning_speed_val = 0;
int ledPin = 2;
int flag = 0;
boolean isTurningLeft = false;
boolean isTurningRight = false;

#define trigPin 13
#define echoPin 12

void setup() {  
  Serial.begin(9600);       

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  

  AFMS.begin();
  delay(500);
}

void loop(){
  
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
    Serial.println("too close revert");
    all_reverse();
  }
  
  if (distance < 10) {
    all_stop();
  }
  
  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  
  if(Serial.available() > 0) {     
    state = Serial.read();  
   
    Serial.print("I received: ");
    Serial.println(state);
    delay(10);
    
    if (state == 49) { //ONWARDS
      speed_val = speed_val + 25;
      apply_speed();
      runEngines();
      } 
    else if (state == 48) { //STOP
      all_stop();
    }
    else if (state == 50) { //LEFT
      
      if (isTurningRight == true && isTurningLeft == false) {
        resetTurningRight();
        isTurningRight = false;
      }
      
      if (isTurningLeft == true) {
        // increase left speed to balance
        resetTurningLeft();
      }
      else {
        // decrease left speed to turn
        turnLeft();
        isTurningLeft = true;
      }
    }
    else if (state == 51) { //SLOW
      speed_val = speed_val - 25;
      apply_speed();
      runEngines();
    } 
    else if (state == 52) { //RIGHT
      
      if (isTurningLeft == true && isTurningRight == false) {
          resetTurningLeft();
          isTurningLeft = false;
      }
      
      if (isTurningRight == true) {
          // increase right speed to balance
          resetTurningRight();
      }
      else {
          // decrease right speed to turn
          turnRight();
          isTurningRight = true;
      }
    }
    else {
      all_stop();
    }
  }
  
  delay(500);
}

void apply_speed() {
  if (speed_val > 250) {
    speed_val = 255;
    apply_speed();
  }
  if (speed_val <= 0) {
    speed_val = 0;
    apply_speed();    
  }
  
  Serial.println(speed_val);
}

void all_stop() {
    speed_val = 0;
    apply_speed();
} 

void all_reverse() {
  engineM1->run(BACKWARD);
  engineM2->run(BACKWARD);
  engineM3->run(BACKWARD);
  engineM4->run(BACKWARD);
}

void turnLeft() {
  turning_speed_val = 0;
  engineM1->setSpeed(turning_speed_val);
  engineM4->setSpeed(turning_speed_val);
  engineM1->run(FORWARD);
  engineM4->run(FORWARD);
}

void resetTurningLeft() {
  turning_speed_val = speed_val;
  engineM1->setSpeed(turning_speed_val);
  engineM4->setSpeed(turning_speed_val);
  engineM1->run(FORWARD);
  engineM4->run(FORWARD);
  
  isTurningLeft = false;
}

void turnRight() {
  turning_speed_val = 0;
  engineM2->setSpeed(turning_speed_val);
  engineM3->setSpeed(turning_speed_val);
  engineM2->run(FORWARD);
  engineM3->run(FORWARD);
}

void resetTurningRight() {
  turning_speed_val = speed_val;
  engineM2->setSpeed(turning_speed_val);
  engineM3->setSpeed(turning_speed_val);
  engineM2->run(FORWARD);
  engineM3->run(FORWARD);
  
  isTurningRight = false;
}

void runEngines() {
  engineM1->setSpeed(speed_val);
  engineM2->setSpeed(speed_val);
  engineM3->setSpeed(speed_val);
  engineM4->setSpeed(speed_val);
  engineM1->run(FORWARD);
  engineM2->run(FORWARD);
  engineM3->run(FORWARD);
  engineM4->run(FORWARD);
}
