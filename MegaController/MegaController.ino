/* Message parsing states */
const byte READ_NEW_COMMAND = 0x00;
const byte WRITE_DIGITAL_VAL = 0x01;
const byte WRITE_DIGITAL_MODE = 0x02;
const byte HEARTBEAT_REQUEST = 0x03;

/* Message lengths */
const int WRITE_DIGITAL_VAL_LEN = 2;
const int WRITE_DIGITAL_MODE_LEN = 2;

/* Digital pin modes */
const byte INPUT_MODE = 0x01;
const byte OUTPUT_MODE = 0x02;
const byte INPUT_PULLUP_MODE = 0x03;

/* Message numerical meanings */
const byte START_BYTE = 0xAA;
const byte DIGITAL_CHANGE = 0x00;
const byte ANALOG_CHANGE = 0x01;
const byte HEARTBEAT = 0x02;

/* Device details */
const int DIGITAL_PIN_COUNT = 54;
const int ANALOG_PIN_COUNT = 16;
const int ANALOG_STEP = 20;

/* Maintains the current device state */
byte operationState = READ_NEW_COMMAND;

/* Digital pin information */
byte digitalPinChange[DIGITAL_PIN_COUNT];
byte digitalPins[DIGITAL_PIN_COUNT];

/* Analog pin information */
byte analogPinChange[ANALOG_PIN_COUNT];
short analogPins[ANALOG_PIN_COUNT];

void setup() {
    // Set all pin values to transmit on startup
    for(int i = 0; i < DIGITAL_PIN_COUNT; i++) {
        digitalPins[i] = -1;
    }
    for(int i = 0; i < ANALOG_PIN_COUNT; i++) {
        analogPins[i] = -ANALOG_STEP - 1;
    }
    Serial.begin(115200);
    Serial.write(START_BYTE);
}

void loop() {
    processUserRequests();
    
    readDigital();
    readAnalog();
    
    reportDigital();
    reportAnalog();
    Serial.flush();
}

void processUserRequests() {
    while (Serial.available() > 0) {
        if (operationState == READ_NEW_COMMAND) {
            if (Serial.available() >= 2) {
                if (Serial.read() == START_BYTE) {
                    operationState = Serial.read();
                }
            }
        }
        if (operationState == WRITE_DIGITAL_VAL) {
            if (Serial.available() >= WRITE_DIGITAL_VAL_LEN) {
                byte address = Serial.read();
                byte value = Serial.read();
                writeDigitalPinValue(address, value);
                operationState = READ_NEW_COMMAND;
            }
        }
        else if (operationState == WRITE_DIGITAL_MODE) {
            if (Serial.available() >= WRITE_DIGITAL_MODE_LEN) {
                byte address = Serial.read();
                byte value = Serial.read();
                writeDigitalPinMode(address, value);
                operationState = READ_NEW_COMMAND;
            }
        }
        else if (operationState == HEARTBEAT_REQUEST) {
            Serial.write(START_BYTE);
            Serial.write(HEARTBEAT);
            Serial.write(1);
            operationState = READ_NEW_COMMAND;
        }
        else {
            operationState = READ_NEW_COMMAND;
            break;
        }
    }
}

void writeDigitalPinValue(byte address, byte value) {
    digitalWrite(address, value > 0 ? HIGH : LOW);
}

void writeDigitalPinMode(byte address, byte value) {
    switch(value) {
        case INPUT_MODE:
            pinMode(address, INPUT);
            break;
        case OUTPUT_MODE:
            pinMode(address, OUTPUT);
            break;
        case INPUT_PULLUP_MODE:
            pinMode(address, INPUT_PULLUP);
            break;
        default: 
            break;
    }
}

void readDigital() {
    for (int pin = 0; pin < DIGITAL_PIN_COUNT; pin++) {
        byte value = digitalRead(pin);
        digitalPinChange[pin] = value == digitalPins[pin] ? 0 : 1;
        digitalPins[pin] = value;
    }
}

void reportDigital() {
    for (int pin = 0; pin < DIGITAL_PIN_COUNT; pin++) {
        if(digitalPinChange[pin] != 0) {
            Serial.write(START_BYTE);
            Serial.write(DIGITAL_CHANGE);
            Serial.write(pin);
            Serial.write(digitalPins[pin]);
        }
    }
}

void readAnalog() {
    for (int pin = 0; pin < ANALOG_PIN_COUNT; pin++) {
        short value = analogRead(pin);
        short diff = value - analogPins[pin];
        if (diff < 0) {
           diff = -diff;
        }
        analogPinChange[pin] = diff < ANALOG_STEP ? 0 : 1;
        analogPins[pin] = value;
    }
}

void reportAnalog() {
    for (int pin = 0; pin < ANALOG_PIN_COUNT; pin++) {
        if(analogPinChange[pin] != 0) {
            Serial.write(START_BYTE);
            Serial.write(ANALOG_CHANGE);
            Serial.write(pin);
            Serial.write(highByte(analogPins[pin]));
            Serial.write(lowByte(analogPins[pin]));
        }
    }
}
