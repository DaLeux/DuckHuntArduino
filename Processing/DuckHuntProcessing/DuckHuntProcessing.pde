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
}


void draw() {
  //affichage du fond d'écran
  background(bg);

  //curseur
  image(mousePressed?boum:curseur, (mouseX-curseur.width/2), (mouseY-curseur.height/2));
  
  //déplacement du canard
  duck1.move();
  duck2.move();
  
  //tentative de tir sur le canard
  if (mousePressed) {
    if (duck1.shoot(mouseX, mouseY)) {
      score += 100;
    }
    if (duck2.shoot(mouseX, mouseY)) {
      score += 100;
    }
  }
  
  //affichage du canard
  duck1.display();
  duck2.display();
  
  //affichage du score
  text("Score : ", widthGame - 250, heightGame - 10);
  text(score, widthGame - 100, heightGame - 10);
}

int getRandomPosition() {
  return (int) random(heightGame-50);
}
