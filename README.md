# TopsidesArduino
The topside Arduino Mega will be connected to LED's, push buttons and potentiometers using the board's digital input/output and analog input. 

[Firmata firmware](https://github.com/firmata/arduino) is loaded onto the Arduino which allows a user to communicate via serial by using the [firmata protocol](https://github.com/firmata/arduino). The Arduino Mega requires some slight modification to the StandardFirmata.ino file.
 - Analog inputs are bit shifted twice to reduce the resolution from 10 to 8 bits to work with the firmata protocol.
 - PWM option is removed as it will not be used.
   
#Installation
 - Download the latest [Ardiuno IDE](https://www.arduino.cc/en/Main/Software) 
 - Open the IDE, go to `Sketch -> Include Library -> Manage Libraries...`
 - filter by `firmata` select the latest version from the dropdown and press install
 - Open `TopsidesArduino/StandardFirmata/StandardFirmata.ino`
 - Select the appropriate Board, Processor, Port and Programmer from the Tools menu
 - Verify and upload the `ino` file to the Mega
