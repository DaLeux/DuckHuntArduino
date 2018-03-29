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

int buzzer = 3;

//notes
int e0  = 330;
int e1  = 349;
int e2  = 370;
int e3  = 392;
int e4  = 415;
int e5  = 440;
int e6  = 466;
int e7  = 494;
int e8  = 523;
int e9  = 554;
int e10 = 587;

int b0  = 247;
int b1  = 262;
int b2  = 277;
int b3  = 294;
int b4  = 311;
int b5  = 330;
int b6  = 349;
int b7  = 370;
int b8  = 392;
int b9  = 415;
int b10 = 440;

int g0  = 196;
int g1  = 208;
int g2  = 220;
int g3  = 233;
int g4  = 247;
int g5  = 262;
int g6  = 277;
int g7  = 294;
int g8  = 311;
int g9  = 330;
int g10 = 349;

int d0  = 147;
int d1  = 156;
int d2  = 165;
int d3  = 175;
int d4  = 185;
int d5  = 196;
int d6  = 208;
int d7  = 220;
int d8  = 233;
int d9  = 247;
int d10 = 262;

int a0  = 110;
int a1  = 117;
int a2  = 123;
int a3  = 131;
int a4  = 139;
int a5  = 147;
int a6  = 156;
int a7  = 165;
int a8  = 175;
int a9  = 185;
int a10 = 196;

int E0  = 82;
int E1  = 87;
int E2  = 92;
int E3  = 98;
int E4  = 104;
int E5  = 110;
int E6  = 117;
int E7  = 123;
int E8  = 131;
int E9  = 139;
int E10 = 147;

int eighth  = 50;
int quarter = 100;
int half    = 200;
int whole   = 400;

int introSong[]     = {e4,      e4,       e3,       e2,     e1,     b5,     b6  };
int introDuration[] = {quarter, quarter,  quarter,  half,   quarter, whole, half };

int fallSong[]     = {e10,      e9,       e8,       e7,     e6,     e5, e1};

int shootSong[]     = {E0, a0, d0, g0, b0, e0};

boolean playIntro = false;
boolean playFall = false;
boolean playShoot = false;


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

int buttonPin = 2;
int lastButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool panic = false;
int buttonState;

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{  
  //mise à jour des données en provenance du nunchuck
  nunchuk.update();

  int reading = digitalRead(buttonPin)?false:true;

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {

        panic = !panic;
      }
    }
  }
  lastButtonState = reading;

  sendNunchukDirection(panic);
  sendNunchukButton();

  readIncommingData();
  sound();
}



void sound() {
  if (playIntro) {
    for (int i = 0; i < sizeof(introSong); i++) {
      tone(buzzer, introSong[i], introDuration[i]);
      delay(introDuration[i]*1.1);
    }
    tone(buzzer, 0, 10);
    playIntro = false;
  }
  
  if (playFall) {
    for (int i = 0; i < sizeof(fallSong); i++) {
      tone(buzzer, fallSong[i], 30);
      delay(30);
    }
    tone(buzzer, 0, 10);
    playFall = false;
  }
  
  if (playShoot) {
    for (int i = 0; i < sizeof(shootSong); i++) {
      tone(buzzer, shootSong[i], 10);
      delay(10);
    }
    playShoot = false;
  }

  if ((!playFall) && (!playIntro) && (!playShoot)) {
    tone(buzzer, 0, 1);
    delay(1);
  }
}

void readIncommingData()
{
  if(Serial.available()){
    int receivedValue = Serial.read();
    if (receivedValue == 1) {
      playIntro = true;
    } else if (receivedValue == 2)  {
      playFall = true;
    }
    
  }
}

void sendNunchukButton() {
  if(nunchuk.zButton == 1) {
    Serial.print(SHOT_BUTTON);
    playShoot = true;
  }

  if(nunchuk.cButton == 1) {
    Serial.print(RESET_BUTTON);
  }
}

void sendNunchukDirection(boolean panic) { 

  int analogX = nunchuk.analogX;
  int analogY = nunchuk.analogY;

  int deltaX = analogX - NEUTRAL_NUNCHUK_X;
  int deltaY = analogY - NEUTRAL_NUNCHUK_Y;
  
  int distance = getDistance(deltaX, deltaY);

  if(distance > MIN_DETECTION_RANGE){

    double angle = getAngle(deltaX, deltaY);

    // UP
    if(isUpPosition(angle)){
      Serial.print((!panic)?UP:DOWN);
    }

    // UP RIGHT
    else if(isUpRightPosition(angle)){
      Serial.print((!panic)?UP_RIGHT:DOWN_LEFT);
    }

    // RIGHT
    else if(isRightPosition(angle)){
      Serial.print((!panic)?RIGHT:LEFT);
    }

    // DOWN RIGHT
    else if(isDownRightPosition(angle)){
      Serial.print((!panic)?DOWN_RIGHT:UP_LEFT);
    }

    // DOWN
    else if(isDownPosition(angle)){
      Serial.print((!panic)?DOWN:UP);
    }

    // DOWN LEFT
    else if(isDownLeftPosition(angle)){
      Serial.print((!panic)?DOWN_LEFT:UP_RIGHT);
    }

    // UP LEFT
    else if(isUpLeftPosition(angle)){
      Serial.print((!panic)?UP_LEFT:DOWN_RIGHT);
    }
    
    // LEFT
    else if(isLeftPosition(angle)){
      Serial.print((!panic)?LEFT:RIGHT);
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


