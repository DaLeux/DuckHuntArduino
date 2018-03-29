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

class Nunchuck 
{
  Serial serialPort;    // Create object from Serial class

  int positionX = 0;
  int positionY = 0;
  boolean shoot = false;
  boolean reset = false;
  
  int receivedValue = 0;

  
  public Nunchuck(Serial serialPort) 
  {
    this.serialPort = serialPort;
  }
  
  public void read() {
    this.shoot = false;
    this.reset = false;
    this.positionX = 0;
    this.positionY = 0;
    
    if ( serialPort.available() <= 0)
      return;

    switch(serialPort.read() - CHAR_ZERO){
        case RESET_BUTTON:
          reset = true;
          break;
  
        case SHOT_BUTTON:
          shoot = true;
          break;
          
        case UP:
          positionY = 1;
          break;
        case DOWN:
          positionY = -1;
          break;
        case RIGHT:
          positionX = 1;
          break;
        case LEFT:
          positionX = -1;
          break;
          
        case UP_RIGHT:
          positionX = 1;
          positionY = 1;
          break;
        case DOWN_RIGHT:
          positionX = 1;
          positionY = -1;
          break;
        case DOWN_LEFT:
          positionX = -1;
          positionY = -1;
          break;
        case UP_LEFT:
          positionX = -1;
          positionY = 1;
          break;
      }
  }
  
  public int getX() {
    return positionX;
  }
  public int getY() {
    return positionY;
  }
  public boolean isShooted() {
    return shoot;
  }
  
  public boolean isReset() {
    return reset;
  }
  
}
