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

Serial serialPort;  // Create object from Serial class
int receivedValue;     // Data received from the serial port

void setup()
{
  String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port
  serialPort = new Serial(this, portName, 9600);
}

void draw()
{
  if ( serialPort.available() > 0) 
  {  // If data is available,
    receivedValue = serialPort.read() - CHAR_ZERO;         // read it and store it in val
    //println(receivedValue); //print it out in the console
    
    switch(receivedValue){
    
      case RESET_BUTTON:
        println("Reset game");
        break;
        
      case SHOT_BUTTON:
        println("Shot !!!");
        break;
    
      
    }
    
  }
}
