
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
#define UP 'z'
#define DOWN 's'
ArduinoNunchuk nunchuk = ArduinoNunchuk();
void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();
}
void loop()
{
  nunchuk.update();
  //nunchukDirection();
  data();
  delay(100);
}
char nunchukDirection() {
  // Zero value - X : 121, Y : 127 
  int analogX = nunchuk.analogX;
  int analogY = nunchuk.analogY;
  // UP
  if(analogX >= 110 &&  analogY >= 215){
    Serial.println("UP");
  }
  // UP RIGHT
  if(analogX >= 180 &&  analogY >= 190){
    Serial.println("UP RIGHT");
  }
  // UP LEFT
  if(analogX <= 60 &&  analogY >= 190){
    Serial.println("UP LEFT");
  }
  // RIGHT
  if(analogX <= 60 &&  analogY >= 190){
    Serial.println("UP LEFT");
  }
  // DOWN
  /*else if(analogY <= 40){
    Serial.println("DOWN");
  }
  // RIGHT
  else if(analogX >= 215){
    Serial.println("RIGHT");
  }
  // LEFT
  else if(analogX <= 25){
    Serial.println("LEFT");
  }*/
  return 'A';
}
void data() {
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

