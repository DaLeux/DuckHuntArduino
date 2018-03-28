//configuration de la wiimote
import processing.serial.*;

final int RESET_BUTTON = 0;
final int UP = 1;
final int UP_RIGHT = 2;
final int RIGHT = 3;
final int DOWN_RIGHT = 4;
final int DOWN = 5;
final int DOWN_LEFT = 6;
final int LEFT = 7;
final int UP_LEFT = 8;
final int SHOT_BUTTON = 9;
final int CHAR_ZERO = 48;

final int incrementX = 12;
final int incrementY = 7;

Serial serialPort;  // Create object from Serial class
int receivedValue;     // Data received from the serial port
int buttonPressed;

//Images à utiliserr
PImage bg;
PImage boum;
PImage curseur;

//police à utiliser
PFont f;

//taille de l'image
int widthGame = 1000;
int heightGame = 713;

//position du curseur
int curseurX;
int curseurY;

//le score;
int score = 0;

//le canard
Duck duck1;
Duck duck2;

void setup() {
  //communication sur le port serie
  print(Serial.list());
  String portName = Serial.list()[9];
  serialPort = new Serial(this, portName, 9600);
  
  //configuration de la zone d'affichage
  size(1000, 713);

  //récupération des images
  boum    = loadImage("src/img/shot.png");
  curseur = loadImage("src/img/crosshair.png");
  bg      = loadImage("src/img/background.jpg");

  //Configuration de l'affichage
  f = createFont("Opificio", 24);
  textFont(f);
  
  //initialisation du canard 
  duck1 = new Duck(widthGame, heightGame);
  duck2 = new Duck(widthGame, heightGame);
  
  //initialisation du curseur
  initializeCursor();
}


void draw() {
  //affichage du fond d'écran
  background(bg);
  buttonPressed = handleSerial();

  //curseur
  image((buttonPressed==2)?boum:curseur, (curseurX-curseur.width/2), (curseurY-curseur.height/2));
  
  //déplacement du canard
  duck1.move();
  //duck2.move();
  
  //tentative de tir sur le canard
  if ((buttonPressed==2)) {
    if (duck1.shoot(curseurX, curseurY)) {
      score += 100;
    }
    /*
    if (duck2.shoot(curseurX, curseurY)) {
      score += 100;
      
    }*/
  }
  
  //affichage du canard
  duck1.display();
  //duck2.display();
  
  //affichage du score
  text("Score : ", widthGame - 250, heightGame - 10);
  text(score, widthGame - 100, heightGame - 10);
}

public int handleSerial() {
  if ( serialPort.available() <= 0)
    return 0;
    
  receivedValue = serialPort.read() - CHAR_ZERO; 
  
  switch(receivedValue){
      case RESET_BUTTON:
        initializeCursor();
        return 1;

      case SHOT_BUTTON:
        return 2;
        
      case UP:
        curseurY -= incrementY;
        break;
      case DOWN:
        curseurY += incrementY;
        break;
      case RIGHT:
        curseurX += incrementX;
        break;
      case LEFT:
        curseurX -= incrementY;
        break;
        
      case UP_RIGHT:
        curseurX += incrementX;
        curseurY -= incrementY;
        break;
      case DOWN_RIGHT:
        curseurX += incrementX;
        curseurY += incrementY;
        break;
      case DOWN_LEFT:
        curseurX -= incrementX;
        curseurY += incrementY;
        break;
      case UP_LEFT:
        curseurX -= incrementX;
        curseurY -= incrementY;
        break;
    }
    return 0;
  }
  
  void initializeCursor() {
    //initialisation du curseur
    curseurX = widthGame/2;
    curseurY = heightGame/2;
  }
