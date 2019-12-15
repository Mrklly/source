2018-12-27 19:57:00
The second generation RF id reader with features:
1. read the 6 bytes RF id.
2. print to console with rs485 @19200 baudrate.
3. the default RF channel is 0, when the device signed to central controller,     it changes to the same channel and saves the signed channel to eeprom.
4. short pressing 4th key can change between 0 and after signed channel.
5. long pressing 4th key over 2 seconds can clear the signed channel from eeprom and switch to 0 channel.
6. LED 2 on indicates current chanel is signed channel and off indicates 0.

