class Duck2 {
  //les images du canard
  PImage duck1;
  PImage duck2;
  PImage duck3;
  PImage duck4;
  PImage duck5;
  
  //taille de la surface 
  int widthGame   = 1000;
  int heightGame  = 713;
    
  //information sur son état
  boolean dead;
  boolean out;
  
  //position du canard
  int duckInitialeY = 0;
  int duckFinaleY   = 0;
  int duckX = 0;
  int duckY = 0;
  
  //image du canard à afficher
  int duckSpriteToUse;
  
  //image du canard à afficher
  int duckSpriteCounter = 0;
  
  //
  float positionDuck = 0.0;
  
  public Duck2(int widthGame, int heightGame) 
  {
    //chargement des images du canard
    this.duck1   = loadImage("src/img/duck_1_2.png");
    this.duck2   = loadImage("src/img/duck_2_2.png");
    this.duck3   = loadImage("src/img/duck_3_2.png");
    this.duck4   = loadImage("src/img/duck_4_2.png");
    this.duck5   = loadImage("src/img/duck_5_2.png");
    
    //initilisation du statut du canard
    this.dead = false;
    this.out = false;
    
    //initialisation de la position du canard
    initDuckPosition();
    
    //mémorisation de la taille de la zone d'affichage dans laquelle le canard va se déplacer
    this.widthGame   = widthGame;
    this.heightGame  = heightGame;
  }
  
  public void move() 
  {
      if (!dead) {
        //incrémentation de la position du canard
        positionDuck=positionDuck+0.002;
          
        //calcul de la position du canard
        if (positionDuck < 1.0) {
          duckX = (int)(positionDuck * ((float) widthGame));
          duckY = (int) (duckInitialeY + (pow(positionDuck, 4) * (duckFinaleY-duckInitialeY)));
        } else {
          out = true;
          
          duckSpriteCounter = 0;
        }
        //incrépentation du compteur de sprite
        duckSpriteCounter++;
        
        //Tout les trois raffraichissement, on change de sprite à utiliser
        if (duckSpriteCounter%7 == 0) {
          duckSpriteToUse = (duckSpriteToUse == 3)?1:(duckSpriteToUse+1);
        }
      } else if (duckSpriteToUse == 4) {
        //incrépentation du compteur de sprite
        duckSpriteCounter++;
        
        //Tout les trois raffraichissement, on change de sprite à utiliser
        if (duckSpriteCounter%20 == 0) {
          duckSpriteToUse = 5;
          duckSpriteCounter=0;
        }
      } else {
        duckY+=5;
        
        duckSpriteCounter++;
        tint(255, (255-duckSpriteCounter * 10));
        
        if ((duckY >= heightGame) || (duckSpriteCounter> 24)) {
          dead = false;
          duckSpriteToUse = 1;
          duckSpriteCounter = 0;
          
          initDuckPosition();
        }
      }
    }
    
    //Méthode permettant l'affichage du canard 
    public void display() 
    {
      //vérification de l'affichge à
      switch (duckSpriteToUse) {
        case 1 : 
          tint(255, 255);
          image(duck1, (duckX-duck1.width/2), (duckY-duck1.height/2));
          break;
        case 2 : 
          tint(255, 255);
          image(duck2, (duckX-duck2.width/2), (duckY-duck2.height/2));
          break;
        case 3 : 
          tint(255, 255);
          image(duck3, (duckX-duck3.width/2), (duckY-duck3.height/2));
          break;
        case 4 : 
          tint(255, 255);
          image(duck4, (duckX-duck4.width/2), (duckY-duck4.height/2));
          //text(100, duckX + 40, duckY);
          break;
        case 5 : 
          image(duck5, (duckX-duck5.width/2), (duckY-duck5.height/2));
          break;
      }
    }
    
    void initDuckPosition() {
      //récupération de la position du canard
      duckInitialeY  = getRandomPosition()+25;
      duckFinaleY    = getRandomPosition()+25; 
      positionDuck   = 0;
      out = false;
    }
    
    int getRandomPosition() {
      return (int) random(heightGame-50);
    }

    public boolean shoot(int curseurX, int curseurY) {
      
      if ((!dead) && (curseurX > (duckX- duck1.width/2)) &&  (curseurX <(duckX + duck1.width/2)) && (curseurY > (duckY - duck1.height/2)) && (curseurY < (duckY + duck1.height/2))) {
        initDuckPosition();
        dead = true;
        duckSpriteToUse = 4;
        return true;
      }
      return false;
    }
    
    public boolean isOut() {
      return out;
    }
}
