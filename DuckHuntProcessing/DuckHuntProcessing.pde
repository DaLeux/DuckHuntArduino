//Images à utiliserr
PImage bg;
PImage boum;
PImage duck1;
PImage duck2;
PImage duck3;
PImage duck4;
PImage duck5;
PImage curseur;

//police à utiliser
PFont f;

//taille de l'image
int widthGame = 1000;
int heightGame = 713;

//position du curseur
int curseurX;
int curseurY;

//image du canard à afficher
int duckSpriteToUse;

//image du canard à afficher
int duckSpriteCounter = 0;

//position du canard
int duckInitialeY = 0;
int duckFinaleY   = 0;
int duckX = 0;
int duckY = 0;

float positionDuck = 0.0;

//le score;
int score = 0;

boolean deadDuck = false;

void setup() {
  //configuration de la zone d'affichage
  size(1000, 713);

  //récupération des images
  boum    = loadImage("src/img/shot.png");
  curseur = loadImage("src/img/crosshair.png");
  bg      = loadImage("src/img/background.jpg");
  duck1   = loadImage("src/img/duck_1.png");
  duck2   = loadImage("src/img/duck_2.png");
  duck3   = loadImage("src/img/duck_3.png");
  duck4   = loadImage("src/img/duck_4.png");
  duck5   = loadImage("src/img/duck_5.png");
    
  //initialisation de la position du canard
  initDuckPosition();
  
  //Configuration de l'affichage
  f = createFont("Opificio", 24);
  textFont(f);
}


void draw() {
  //affichage du fond d'écran
  background(bg);
  
  if (!deadDuck) {
    //incrémentation de la position du canard
    positionDuck=positionDuck+0.005;
      
    //calcul de la position du canard
    if (positionDuck < 1.0) {
      duckX = (int)(positionDuck * ((float) widthGame));
      duckY = (int) (duckInitialeY + (pow(positionDuck, 4) * (duckFinaleY-duckInitialeY)));
    } else {
      initDuckPosition();
      duckSpriteCounter = 0;
    }
    //incrépentation du compteur de sprite
    duckSpriteCounter++;
    
    //Tout les trois raffraichissement, on change de sprite à utiliser
    if (duckSpriteCounter%3 == 0) {
      duckSpriteToUse = (duckSpriteToUse == 3)?1:(duckSpriteToUse+1);
    }
  } else if (duckSpriteToUse == 4) {
    //incrépentation du compteur de sprite
    duckSpriteCounter++;
    
    //Tout les trois raffraichissement, on change de sprite à utiliser
    if (duckSpriteCounter%11 == 0) {
      duckSpriteToUse = 5;
      duckSpriteCounter=0;
    }
  } else {
    duckY+=5;
    
    duckSpriteCounter++;
    tint(255, (255-duckSpriteCounter * 10));
    
    if ((duckY >= heightGame) || (duckSpriteCounter> 22)) {
      deadDuck = false;
      duckSpriteToUse = 1;
      duckSpriteCounter = 0;
      tint(255, 255);
      initDuckPosition();
      return;
    }
  }
    
  
  //vérification de l'affichge à
  switch (duckSpriteToUse) {
    case 1 : 
      image(duck1, (duckX-duck1.width/2), (duckY-duck1.height/2));
      break;
    case 2 : 
      image(duck2, (duckX-duck2.width/2), (duckY-duck2.height/2));
      break;
    case 3 : 
      image(duck3, (duckX-duck3.width/2), (duckY-duck3.height/2));
      break;
    case 4 : 
      image(duck4, (duckX-duck4.width/2), (duckY-duck4.height/2));
      text(100, duckX + 40, duckY);
      break;
    case 5 : 
      image(duck5, (duckX-duck5.width/2), (duckY-duck5.height/2));
      break;
  }

  //curseur
  image(mousePressed?boum:curseur, (mouseX-curseur.width/2), (mouseY-curseur.height/2));

  if ( mousePressed &&
      (!deadDuck) &&
      (mouseX > (duckX- duck1.width/2)) && 
      (mouseX < (duckX + duck1.width/2)) && 
      (mouseY > (duckY - duck1.height/2)) && 
      (mouseY < (duckY + duck1.height/2))) {
        
    deadDuck();

  }
  
  //affichage du score
  text("Score : ", widthGame - 250, heightGame - 10);
  text(score, widthGame - 100, heightGame - 10);
}

int getRandomPosition() {
  return (int) random(heightGame-50);
}


void initDuckPosition() {
  //récupération de la position du canard
  duckInitialeY  = getRandomPosition()+25;
  duckFinaleY    = getRandomPosition()+25; 
  
  positionDuck   = 0;
}

void deadDuck() {
    initDuckPosition();
    score+=100;
    deadDuck = true;
    duckSpriteToUse = 4;
}
