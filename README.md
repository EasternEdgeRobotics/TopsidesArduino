#TopsidesArduino

The topside arduino software creates a serial interface for controlling and listening to pin changes on an arduino.

Each message begins with a start byte `0xAA` followed by a command byte. Depending on the command byte, N bytes will follow with the message payload.
