# DuckHuntArduino :

This project allows a DuckHunt game to run on processing (https://processing.org/), controlled with a Wii Nunchuk via an Arduino Uno.

![Duck hunt](https://github.com/DaLeux/DuckHuntArduino/blob/master/Images/DuckHuntScreen.png)

## Getting Started :

- 1 Arduino Card (Tested on a DFRobot Romeo v1 and an Arduino Leonardo)
- 1 DFRobot nunchuk adapter
- 1 Wii Nunchuk
- 1 Speaker (8 Ohm)
- 1 Push Button

### Installing :

Download Processing : https://processing.org/download/

Install ArduinoNunchuk library :  https://github.com/GabrielBianconi/arduino-nunchuk

Nunchuk setup : https://create.arduino.cc/projecthub/infusion/using-a-wii-nunchuk-with-arduino-597254

Hardware Connections :

- Speaker : pin 3
- Push Button : pin 2
- Nunchuk's adapter d : SDA
- Nunchuk's adapter c : SCL

To update the speaker and the button's pin, simply go to the ArduinoNunchukController.ion, and update the BUZZER and BUTTON_PIN const accordingly.

![Arduino setup](https://github.com/DaLeux/DuckHuntArduino/blob/master/Images/DuckHuntFritzing.png)

Upload the code on your Arduino and start Processing.

### Game rules :

Use the nunchuk's joystick to move your cursor, shoot with the Z button. The C button will allow you to recenter your cursor. Press the push button to enter panic mode, all controls are opposed !
