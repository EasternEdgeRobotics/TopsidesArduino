//array number           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
int inputPin[] =        {2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };   // an array of pin numbers to be used as digital inputs (buttons), starting at 2 because 0,1 are used for hardware serial
//button numbers         1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24      // correspond to the physical top side buttons
int outputPin[] =      {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 };   // an array of pin numbers to be used as digital outputs (lights)
int analogPin[] =      {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};                              // an array of pin numbers to be used as analog inputs (pots)


char button_status[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                            // a char array of button status', true when a button is pressed
char button_led[]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                            // a char array of button toggled status, updated from the top side, when true the button led is on
char analog_status[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                                       // an int array of the analog voltage for each analog input


int baud = 19200;																					                // rate of serial communication to topsides (9600 - 115200)

void setup() {
  Serial.begin(baud);																				              // begin communication over the serial-USB line at selected baud
  for (int thisPin = 2; thisPin < 26; thisPin++)  {												// initialize each physical inputPin as a digital input
    pinMode(inputPin[thisPin], INPUT);
  }
  
  for (int thisPin = 26; thisPin < 50; thisPin++)  {											// initialize each physical outputPin as a digital output
    pinMode(outputPin[thisPin], OUTPUT); 
  }
}


void loop() {
  check_buttons();                                                        // call to check for button input
  //print_button_status();                                                  // print array to console for testing
  Serial.write (button_status,4);                                         // write the button_status array to the serial line in 4 bytes
  Serial.readBytes (button_led,4);                                        // read 4 bytes and enter that into button_led
  light_buttons();
  check_analog();                                                         // call to check for analog input values
  Serial.write (analog_status,4);
  delay (1000);
  Serial.flush();                                                         // flush the serial line between pins (each pin is sent as one full byte)
}

void check_buttons() {                                                    // checks buttons to see if they are being pressed and stores that in array button_status
  for (int i=0; i < 24; i++) {
    if (digitalRead (inputPin[i])) {                                      // if there is digital signal from a pin set status to true
      button_status[i] = true;
    } else {                                                              // else set status to false
      button_status[i] = false;
    }
  }
}

void print_button_status() {                                              // prints the array button_status as a single line in the console for testing
  for (int i=0; i < 24; i++) {
    Serial.print (button_status[i]);
  }
  Serial.println (" ");
}

void light_buttons() {                                                    // turns the led on and off
  for (int i=0; i < 24; i++) {
    if (button_led[i]) {                                                  // if true set the out pin to high
      digitalWrite (outputPin[i], HIGH);
    } else {                                                              // else set to low
      digitalWrite (outputPin[i], LOW);
    }
  }
}

void check_analog() {                                                     // update the array analog_status with the votage of the analog pins
  for (int i=0; i < 16; i++) {
    analog_status[i] = analogRead (analogPin[i]);                         // read the analog pin and save in array
  }
}



/*
void loop() {
  if (!Serial.available()) {																		// if serial is not available, continue looping here
    return;
  }
  char buffer[4];																					// define the length of serial message as an array of 4 characters (bytes)
  Serial.readBytes(buffer, 4);																		// store the data from the serial line in the buffer
  if (buffer[0] == WRITE) {
    for (int j = 1; j < 4; j++)
    for (int i = 0; i < 8; i++) {
     digitalWrite(outputPin[i], (0x70&(buffer[j]<<i))); 											// extract state for each pin (by shifting & masking) from the serial data
    }
  }
   if (buffer[0] == READ_DIGITAL) {
    for (int i = 0; i < 24; i++) {
      Serial.write (digitalRead(inputPin[i]));														// read each input pin and write state to the serial line
      Serial.flush();																				// flush the serial line between pins (each pin is sent as one full byte)
    }
  }
  
  if (buffer[0] == READ_ANALOG) {
    for (int i = 0; i < 16; i++) {
      Serial.write (analogRead(analogPin[i]));														// write each analog value (10 bits) to the serial line
      Serial.flush();																				// flush the line between analog pins
    }
  }
}
*/
