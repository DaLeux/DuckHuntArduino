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

//le nunchuck
Nunchuck nunchuck;

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
  
  //initialisation du curseur et de sa position
  nunchuck = new Nunchuck(this, 9);
  initializeCursor();
}


void draw() {
  //affichage du fond d'écran
  background(bg);
  
  //lecture du port serie
  nunchuck.read();
  
  curseurX += nunchuck.getX() * incrementX;
  curseurY += -nunchuck.getY() * incrementY;
  
  //curseur
  image(nunchuck.isShooted()?boum:curseur, (curseurX-curseur.width/2), (curseurY-curseur.height/2));
  
  //déplacement du canard
  duck1.move();
  
  //tentative de tir sur le canard
  if (nunchuck.isShooted()) {
    if (duck1.shoot(curseurX, curseurY)) {
      score += 100;
    }
  }
  
  //affichage du canard
  duck1.display();
  //duck2.display();
  
  //affichage du score
  text("Score : ", widthGame - 250, heightGame - 10);
  text(score, widthGame - 100, heightGame - 10);
}

void initializeCursor() {
  //initialisation du curseur
  curseurX = widthGame/2;
  curseurY = heightGame/2;
}
