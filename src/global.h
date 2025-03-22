#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include <BluetoothSerial.h>
#include <ArduinoJson.h>

extern BluetoothSerial SerialBT;
extern String teamID; // Received teamID (acts as a key in communications between the pia app and esp32)
extern StaticJsonDocument<15000> JSONDocument;

#endif
