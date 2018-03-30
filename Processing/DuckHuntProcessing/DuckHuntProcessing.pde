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
int maxScore = 0;

//le canard
Duck duck1;
Duck2 duck2;

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
  duck2 = new Duck2(widthGame, heightGame);
  
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

  
  if ((playIntro) && (serialPort.available() > 0))  {
    println("intro");
    serialPort.write(1);
    playIntro = false;
    duck1.initDuckPosition();
    duck2.initDuckPosition();
    
    showIntro();
    
  } 
  
  else if (playIntro) {
    introTimerEnd = millis()+2000;
    showIntro();
  } 
  
  else if (millis() >= introTimerEnd) {
    //affichage du fond d'écran
     background(bg);
  
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

    if ((duck1.isOut()) || (duck2.isOut())){
      //playIntro = true;
      
      if (score > maxScore) {
        maxScore = score;
      }

      duck1.initDuckPosition();
      duck2.initDuckPosition();
      score = 0;
      
      serialPort.write(1);
      
    }
      
    
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
    fill(0, 102, 153);
    textAlign(LEFT);
    text("Max score : "+ maxScore , 150, 50);
    textAlign(RIGHT);
    text("Votre score : "+score, 850, 50);

  }
}

void showIntro () {
    background(0);
    textAlign(CENTER);
    text("DUCK HUNT", 500, 300);
    image(loadImage("src/img/duck_1.png"), 450, 200);
}

void initializeCursor() {
  //initialisation du curseur
  curseurX = widthGame/2;
  curseurY = heightGame/2;
}
