//Images à utiliserr
PImage bg;
PImage boum;
PImage curseur;

//police à utiliser
PFont f;

//taille de l'image
final int widthGame = 1000;
final int heightGame = 713;

//valeur de l'incrémentation
final int incrementX = 10;
final int incrementY = 7;

//position du curseur
int curseurX;
int curseurY;

//le score;
int score = 0;

//le canard
Duck duck1;
Duck duck2;

//le nunchuck
Nunchuck nunchuck;

//communication serie
Serial serialPort;
String portName;

//information sur la lecture de l'intro
boolean playIntro = true;
int introTimerEnd;

void setup() {
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
  
  //récupération du port serial à utiliser
  this.portName = Serial.list()[9];
  println(Serial.list());
  
  println("Port entrée utilisé (tty) : ", portName);
  
  serialPort = new Serial(this, this.portName, 9600);

  //initialisation du curseur et de sa position
  nunchuck = new Nunchuck(serialPort);
  initializeCursor();

  introTimerEnd = millis()+2000;
}


void draw() {


  //affichage du fond d'écran
  background(bg);
  
  if ((millis() < introTimerEnd) && (playIntro))  {
    println("intro");


  } else if (millis() >= introTimerEnd) {
    //lecture du port serie
    nunchuck.read();
    
    if (nunchuck.isReset()) {
      initializeCursor();
    }
      
    curseurX += nunchuck.getX() * incrementX;
    curseurY += -nunchuck.getY() * incrementY;
    
    //curseur
    image(nunchuck.isShooted()?boum:curseur, (curseurX-curseur.width/2), (curseurY-curseur.height/2));
    
    //déplacement du canard
    duck1.move();
    duck2.move();
    
    //tentative de tir sur le canard
    if (nunchuck.isShooted()) {
      if (duck1.shoot(curseurX, curseurY)) {
        score += 100;
        serialPort.write(2);
      }
      if (duck2.shoot(curseurX, curseurY)) {
        score += 50;
        serialPort.write(2);
      }
    }
    
    //affichage du canard
    duck1.display();
    duck2.display();
    
    //affichage du score
    text("Score : ", widthGame - 250, heightGame - 10);
    text(score, widthGame - 100, heightGame - 10);
  }
}

void initializeCursor() {
  //initialisation du curseur
  curseurX = widthGame/2;
  curseurY = heightGame/2;
}
