

/*
 * ArduinoNunchukDemo.ino
 *
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 */

#include <Wire.h>
#include <ArduinoNunchuk.h>

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
  //logData();

  //delay(0);
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

  // Zero value - X : 121, Y : 127 

  int analogX = nunchuk.analogX;
  int analogY = nunchuk.analogY;

  // UP
  if(analogX >= 110 &&  analogY >= 215){
    Serial.print(UP);
  }

  // UP RIGHT
  else if(analogX >= 180 &&  analogY >= 190){
    Serial.print(UP_RIGHT);
  }

  // RIGHT
  else if(analogX >= 200 &&  analogY >= 127){
    Serial.print(RIGHT);
  }

  // DOWN RIGHT
  else if(analogX >= 180 &&  analogY <= 80){
    Serial.print(DOWN_RIGHT);
  }

  // DOWN
  else if(analogX <= 122 &&  analogY <= 40){
    Serial.print(DOWN);
  }

  // DOWN LEFT
  else if(analogX <= 70 &&  analogY <= 70){
    Serial.print(DOWN_LEFT);
  }

  // LEFT
  else if(analogX <= 50 &&  analogY <= 127){
    Serial.print(LEFT);
  }
  
  // UP LEFT
  else if(analogX <= 60 &&  analogY >= 190){
    Serial.print(UP_LEFT);
  }
  
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


