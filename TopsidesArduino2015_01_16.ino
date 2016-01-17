#define READ_DIGITAL 1
#define READ_ANALOG 2
#define WRITE 3

int inputPin[] = { 
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };             // an array of pin numbers to be used as digital inputs (buttons)

int outputPin[] = {
  24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };   // an array of pin numbers to be used as digital outputs (lights)
  
int analogPin[] = {
  A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };                             // an array of pin numbers to be used as analog inputs (sliders/pots)

int baud = 19200;

void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  Serial.begin(baud);
  for (int thisPin = 0; thisPin < 24; thisPin++)  {
    pinMode(inputPin[thisPin], INPUT);
  }
  
  for (int thisPin = 0; thisPin < 23; thisPin++)  {
    pinMode(inputPin[thisPin], OUTPUT); 
  }
}

void loop() {
  if (!Serial.available()) {
    return;
  }
  char buffer[4];
  Serial.readBytes(buffer, 4);
  if (buffer[0] == WRITE) {
    for (int j = 1; j < 4; j++)
    for (int i = 0; i < 8; i++) {
     digitalWrite(outputPin[i], (0x70&(buffer[j]<<i)));
    }
  }
   if (buffer[0] == READ_DIGITAL) {
    for (int i = 0; i < 24; i++) {
      Serial.write (digitalRead(inputPin[i]));
      Serial.flush();
    }
  }
  
  if (buffer[0] == READ_ANALOG) {
    for (int i = 0; i < 16; i++) {
      Serial.write (analogRead(analogPin[i]));
      Serial.flush();
    }
  }
}
