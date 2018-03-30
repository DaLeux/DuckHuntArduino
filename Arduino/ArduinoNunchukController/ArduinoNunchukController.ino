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

const int BAUDRATE = 9600;
const int BUZZER = 3;
const int BUTTON_PIN = 2;

//Définitions des différentes notes (on se base sur les fréquences d'une guitares)
int const NOTES_EE0  = 330;
int const NOTES_EE1  = 349;
int const NOTES_EE2  = 370;
int const NOTES_EE3  = 392;
int const NOTES_EE4  = 415;
int const NOTES_EE5  = 440;
int const NOTES_EE6  = 466;
int const NOTES_EE7  = 494;
int const NOTES_EE8  = 523;
int const NOTES_EE9  = 554;
int const NOTES_EE10 = 587;

int const NOTES_B0  = 247;
int const NOTES_B1  = 262;
int const NOTES_B2  = 277;
int const NOTES_B3  = 294;
int const NOTES_B4  = 311;
int const NOTES_B5  = 330;
int const NOTES_B6  = 349;
int const NOTES_B7  = 370;
int const NOTES_B8  = 392;
int const NOTES_B9  = 415;
int const NOTES_B10 = 440;

int const NOTES_G0  = 196;
int const NOTES_G1  = 208;
int const NOTES_G2  = 220;
int const NOTES_G3  = 233;
int const NOTES_G4  = 247;
int const NOTES_G5  = 262;
int const NOTES_G6  = 277;
int const NOTES_G7  = 294;
int const NOTES_G8  = 311;
int const NOTES_G9  = 330;
int const NOTES_G10 = 349;

int const NOTES_D0  = 147;
int const NOTES_D1  = 156;
int const NOTES_D2  = 165;
int const NOTES_D3  = 175;
int const NOTES_D4  = 185;
int const NOTES_D5  = 196;
int const NOTES_D6  = 208;
int const NOTES_D7  = 220;
int const NOTES_D8  = 233;
int const NOTES_D9  = 247;
int const NOTES_D10 = 262;

int const NOTES_A0  = 110;
int const NOTES_A1  = 117;
int const NOTES_A2  = 123;
int const NOTES_A3  = 131;
int const NOTES_A4  = 139;
int const NOTES_A5  = 147;
int const NOTES_A6  = 156;
int const NOTES_A7  = 165;
int const NOTES_A8  = 175;
int const NOTES_A9  = 185;
int const NOTES_A10 = 196;

int const NOTES_E0  = 82;
int const NOTES_E1  = 87;
int const NOTES_E2  = 92;
int const NOTES_E3  = 98;
int const NOTES_E4  = 104;
int const NOTES_E5  = 110;
int const NOTES_E6  = 117;
int const NOTES_E7  = 123;
int const NOTES_E8  = 131;
int const NOTES_E9  = 139;
int const NOTES_E10 = 147;

//Durée des notes
int const DURATION_EIGHTH   = 50;
int const DURATION_QUARTER  = 100;
int const DURATION_HALF     = 200;
int const DURATION_WHOLE    = 400;

//définitions des musiques
int introSong[]     = { NOTES_EE4, NOTES_EE4, NOTES_EE3, NOTES_EE2, NOTES_EE1, NOTES_B5, NOTES_B6 };
int introDuration[] = { DURATION_QUARTER, DURATION_QUARTER,  DURATION_QUARTER,  DURATION_HALF,   DURATION_QUARTER, DURATION_WHOLE, DURATION_HALF };
int fallSong[]      = { NOTES_EE10, NOTES_EE9, NOTES_EE8, NOTES_EE7, NOTES_EE6, NOTES_EE5, NOTES_EE1 };
int shootSong[]     = { NOTES_EE0, NOTES_A0, NOTES_D0, NOTES_G0, NOTES_B0, NOTES_E0 };

//boolean indiquant quelle musique jouer
boolean playIntro = false;
boolean playFall = false;
boolean playShoot = false;

//Les différentes signaux que peux envoyer le nunchuck
const int RESET_BUTTON  = 0;
const int UP            = 1;
const int UP_RIGHT      = 2;
const int RIGHT         = 3;
const int DOWN_RIGHT    = 4;
const int DOWN          = 5;
const int DOWN_LEFT     = 6;
const int LEFT = 7;
const int UP_LEFT       = 8;
const int SHOT_BUTTON   = 9;

//la valeur au repos du nunchuck
const int NEUTRAL_NUNCHUK_X   = 122;
const int NEUTRAL_NUNCHUK_Y   = 127;

//le déplacement nécessaire validant le déplacement au nunchuck
const int MIN_DETECTION_RANGE = 10;

//définition des angles permettant de savoir la direction du nunchuck
const int UP_ANGLE            = 90;
const int RIGHT_ANGLE         = 0;
const int DOWN_ANGLE          = -90;
const int LEFT_ANGLE_POSTIVE  = 180;
const int LEFT_ANGLE_NEGATIVE = -180;

//définition de l'angle permettant de savoir si une zone UP, DOWN, LEFT ou RIGHT ou les variantes entre deux positions
const int ANGLE_PERIMETER = 20;

//Variables utilisées pour la gestion du mode panic
int lastButtonState             = 0;
unsigned long lastDebounceTime  = 0;
unsigned long debounceDelay     = 50;
bool panic                      = false;
int buttonState;

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{  
  //mise à jour des données en provenance du nunchuck
  nunchuk.update();

  //lecture du bouton de panic
  readPanic();

  //envoie du message de la position du nunchuck (si besoin). Si panic est à true, on envoi un signal inversé
  sendNunchukDirection(panic);

  //envoie du message du bouton pressé (si besoin)
  sendNunchukButton();

  //lecture du port serie et lecture du son s'il y a une demande
  readIncommingData();
  playSound();
}

//fonction permettant de déterminer si on a appuyé sur le joystick ou pas
void readPanic() {
  //Lecture du bouton de panic
  int reading = digitalRead(BUTTON_PIN)?false:true;

  //si on a eu une modification de l'état du bouton
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
  //enregistrement du dernier etat du bouton
  lastButtonState = reading;
}

//Fonction permettant de jouer de la musique
void playSound() {

  //musique d'introduction
  if (playIntro) {
    for (int i = 0; i < sizeof(introSong); i++) {
      tone(BUZZER, introSong[i], introDuration[i]);
      delay(introDuration[i]*1.1);
    }
    tone(BUZZER, 0, 10);
    playIntro = false;
  }

  //musique de la chute du canard
  if (playFall) {
    for (int i = 0; i < sizeof(fallSong); i++) {
      tone(BUZZER, fallSong[i], 30);
      delay(30);
    }
    tone(BUZZER, 0, 10);
    playFall = false;
  }

  //musique du tir
  if (playShoot) {
    for (int i = 0; i < sizeof(shootSong); i++) {
      tone(BUZZER, shootSong[i], 10);
      delay(10);
    }
    playShoot = false;
  }

  //s'il n'y a aucun son, on envoit rien (on évite les bruits parasites)
  if ((!playFall) && (!playIntro) && (!playShoot)) {
    tone(BUZZER, 0, 1);
    delay(1);
  }
}

//fonction permettant de lire des informations reçues sur le port serie
void readIncommingData()
{
  //lecture uniquement quand le port serie est disponible
  if(Serial.available()){
    int receivedValue = Serial.read();

    //réception d'une demande de lecture de l'introduction
    if (receivedValue == 1) {
      playIntro = true;
    } 
    //réception d'une demande de lecture de chute de canard
    else if (receivedValue == 2)  {
      playFall = true;
    }
    
  }
}

//fonction permettant d'envoyer des commandes sur le port serie
void sendNunchukButton() {

  //Boutton de tir (Z)
  if(nunchuk.zButton == 1) {
    Serial.print(SHOT_BUTTON);
    playShoot = true;
  }

  //Boutton réinitialisation du curseur (C)
  if(nunchuk.cButton == 1) {
    Serial.print(RESET_BUTTON);
  }
}

//fonction permettant de lire les commandesdu nunchuck
void sendNunchukDirection(boolean panic) { 
  
  //récupération de position du joystick
  int analogX = nunchuk.analogX;
  int analogY = nunchuk.analogY;

  //suppression de la valeur neutre du joystick
  int deltaX = analogX - NEUTRAL_NUNCHUK_X;
  int deltaY = analogY - NEUTRAL_NUNCHUK_Y;

  //calcul de la distance de déplacement du joystick
  int distance = getDistance(deltaX, deltaY);

  //si la distance effectuée est faible (parasite ou pas assez représentative) on gère le déplacement
  if(distance > MIN_DETECTION_RANGE){

    //calcul de l'angle du joystick
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
//fonction permettant de calculer la distance du joystick
double getDistance(int deltaX, int deltaY){
  return pow(square(deltaX) + square(deltaY), 0.5);
}

//fonction permettant de récupérer l'angle du joystick
double getAngle(int deltaX, int deltaY){
  return atan2(deltaY,deltaX) * 180 / PI;
}

//fonction permettant de savoir si le joystick est dirigé vers UP
boolean isUpPosition(double angle){
  return (angle <= UP_ANGLE + ANGLE_PERIMETER) && (angle >= UP_ANGLE - ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers UP_RIGHT
boolean isUpRightPosition(double angle){
  return (angle <= UP_ANGLE - ANGLE_PERIMETER) && (angle >= RIGHT_ANGLE + ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers RIGHT
boolean isRightPosition(double angle){
  return (angle <= RIGHT_ANGLE + ANGLE_PERIMETER) && (angle >= RIGHT_ANGLE - ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers DOWN_RIGHT
boolean isDownRightPosition(double angle){
  return (angle <= RIGHT_ANGLE - ANGLE_PERIMETER) && (angle >= DOWN_ANGLE + ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers DOWN
boolean isDownPosition(double angle){
  return (angle <= DOWN_ANGLE + ANGLE_PERIMETER) && (angle >= DOWN_ANGLE - ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers DOWN_LEFT
boolean isDownLeftPosition(double angle){
  return (angle <= DOWN_ANGLE - ANGLE_PERIMETER) && (angle >= LEFT_ANGLE_NEGATIVE + ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers LEFT
boolean isLeftPosition(double angle){
  return (angle <= LEFT_ANGLE_POSTIVE + ANGLE_PERIMETER) && (angle >= LEFT_ANGLE_NEGATIVE - ANGLE_PERIMETER);
}

//fonction permettant de savoir si le joystick est dirigé vers UP_LEFT
boolean isUpLeftPosition(double angle){
  return (angle <= LEFT_ANGLE_POSTIVE - ANGLE_PERIMETER) && (angle >= UP_ANGLE + ANGLE_PERIMETER);
}

//fonction de débugage
void logData() {
  Serial.print("X : ");
  Serial.print(nunchuk.analogX, DEC);
  Serial.print(", Y : ");
  Serial.print(nunchuk.analogY, DEC);
  Serial.println();  
}


