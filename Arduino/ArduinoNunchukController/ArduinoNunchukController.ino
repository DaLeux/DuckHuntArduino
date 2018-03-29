/*
 * ArduinoNunchukController.ino
 *
 * Authors : Asif ADAMSHA, Fabien GRISELLES, Thomas LEUX
 *
 * Project page: https://github.com/DaLeux/DuckHuntArduino
 *
 */

#include <Wire.h>
#include <ArduinoNunchuk.h>
#include <math.h> 

#define BAUDRATE 9600

const int RESET_BUTTON = 0;
const int UP = 1;
const int UP_RIGHT = 2;
const int RIGHT = 3;
const int DOWN_RIGHT = 4;
const int DOWN = 5;
const int DOWN_LEFT = 6;
const int LEFT = 7;
const int UP_LEFT = 8;
const int SHOT_BUTTON = 9;

const int NEUTRAL_NUNCHUK_X = 122;
const int NEUTRAL_NUNCHUK_Y = 127;

const int MIN_DETECTION_RANGE = 10;

const int UP_ANGLE = 90;
const int RIGHT_ANGLE = 0;
const int DOWN_ANGLE = -90;
const int LEFT_ANGLE_POSTIVE = 180;
const int LEFT_ANGLE_NEGATIVE = -180;

const int ANGLE_PERIMETER = 20;

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();
}

void loop()
{
  nunchuk.update();

  sendNunchukDirection();

  sendNunchukButton();

  readIncommingData();
  
  //logData();
}

void readIncommingData(){
  if(Serial.available()){

    int receivedValue = Serial.read();
    
  }
}

void sendNunchukButton() {
  if(nunchuk.zButton == 1) {
    Serial.print(SHOT_BUTTON);
  }

  if(nunchuk.cButton == 1) {
    Serial.print(RESET_BUTTON);
  }
}

void sendNunchukDirection() { 

  int analogX = nunchuk.analogX;
  int analogY = nunchuk.analogY;

  int deltaX = analogX - NEUTRAL_NUNCHUK_X;
  int deltaY = analogY - NEUTRAL_NUNCHUK_Y;
  
  int distance = getDistance(deltaX, deltaY);

  if(distance > MIN_DETECTION_RANGE){

    double angle = getAngle(deltaX, deltaY);

    // UP
    if(isUpPosition(angle)){
      Serial.print(UP);
    }

    // UP RIGHT
    else if(isUpRightPosition(angle)){
      Serial.print(UP_RIGHT);
    }

    // RIGHT
    else if(isRightPosition(angle)){
      Serial.print(RIGHT);
    }

    // DOWN RIGHT
    else if(isDownRightPosition(angle)){
      Serial.print(DOWN_RIGHT);
    }

    // DOWN
    else if(isDownPosition(angle)){
      Serial.print(DOWN);
    }

    // DOWN LEFT
    else if(isDownLeftPosition(angle)){
      Serial.print(DOWN_LEFT);
    }

    // UP LEFT
    else if(isUpLeftPosition(angle)){
      Serial.print(UP_LEFT);
    }
    
    // LEFT
    else if(isLeftPosition(angle)){
      Serial.print(LEFT);
    }

  }
  
}

double getDistance(int deltaX, int deltaY){
  return pow(square(deltaX) + square(deltaY), 0.5);
}

double getAngle(int deltaX, int deltaY){
  return atan2(deltaY,deltaX) * 180 / PI;
}

boolean isUpPosition(double angle){
  return (angle <= UP_ANGLE + ANGLE_PERIMETER) && (angle >= UP_ANGLE - ANGLE_PERIMETER);
}

boolean isUpRightPosition(double angle){
  return (angle <= UP_ANGLE - ANGLE_PERIMETER) && (angle >= RIGHT_ANGLE + ANGLE_PERIMETER);
}

boolean isRightPosition(double angle){
  return (angle <= RIGHT_ANGLE + ANGLE_PERIMETER) && (angle >= RIGHT_ANGLE - ANGLE_PERIMETER);
}

boolean isDownRightPosition(double angle){
  return (angle <= RIGHT_ANGLE - ANGLE_PERIMETER) && (angle >= DOWN_ANGLE + ANGLE_PERIMETER);
}

boolean isDownPosition(double angle){
  return (angle <= DOWN_ANGLE + ANGLE_PERIMETER) && (angle >= DOWN_ANGLE - ANGLE_PERIMETER);
}

boolean isDownLeftPosition(double angle){
  return (angle <= DOWN_ANGLE - ANGLE_PERIMETER) && (angle >= LEFT_ANGLE_NEGATIVE + ANGLE_PERIMETER);
}

boolean isLeftPosition(double angle){
  return (angle <= LEFT_ANGLE_POSTIVE + ANGLE_PERIMETER) && (angle >= LEFT_ANGLE_NEGATIVE - ANGLE_PERIMETER);
}

boolean isUpLeftPosition(double angle){
  return (angle <= LEFT_ANGLE_POSTIVE - ANGLE_PERIMETER) && (angle >= UP_ANGLE + ANGLE_PERIMETER);
}

void logData() {
  Serial.print("X : ");
  Serial.print(nunchuk.analogX, DEC);
  Serial.print(", Y : ");
  Serial.print(nunchuk.analogY, DEC);
  Serial.println();
  /*Serial.print(nunchuk.accelX, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelY, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.accelZ, DEC);
  Serial.print(' ');
  Serial.print(nunchuk.zButton, DEC);
  Serial.print(' ');
  Serial.println(nunchuk.cButton, DEC);*/
  
}


