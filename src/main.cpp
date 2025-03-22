#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "get_networks.h"
#include "global.h"
#include "connect_wifi.h"
#include "get_data.h"
#include "get_details.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void set_teamID()
{
  teamID = JSONDocument["teamId"].as<String>();
}

String data = ""; // Received data

// The receivedData function is called when data is available on Bluetooth (see loop function)
void receivedData()
{
  // Read the data using the appropriate SerialBT functions
  // according to the app specifications
  // The data is terminated by the new line character (\n)
  while (SerialBT.available())
  {
    data = SerialBT.readStringUntil('\n');
  }

  Serial.println(data); // <-- This is the message sent from the app, according to the specs

  delay(250);

  // deserialize data into jsondocument; deserialize implicitly clears JSONDocument :)
  DeserializationError error = deserializeJson(JSONDocument, data.c_str());

  if (error)
  {
    Serial.println(error.c_str());
  }
  else
  {
    if (JSONDocument["action"] == "getNetworks")
    {
      Serial.print("\naction received: ");
      Serial.println(JSONDocument["action"].as<String>());

      set_teamID();

      // verify teamid is set up correctly in main program
      Serial.print("Team ID received: ");
      Serial.println(teamID);

      // Scan WiFi networks & send the list to PIA App
      scan_wifi();
    }
    else if (JSONDocument["action"] == "connect")
    {
      Serial.print("\naction received: ");
      Serial.println(JSONDocument["action"].as<String>());

      // Connect to the chosen WiFi by the PIA App
      connect_wifi();
    }
    else if (JSONDocument["action"] == "getData")
    {
      Serial.print("\naction received: ");
      Serial.println(JSONDocument["action"].as<String>());

      // Send hero list to PIA App
      get_data();
    }
    else if (JSONDocument["action"] == "getDetails")
    {
      Serial.print("\naction received: ");
      Serial.println(JSONDocument["action"].as<String>());

      // Send selected hero details to PIA App
      get_details();
    }
  }

  // Reset the received data string after processing
  data = "";
}

void setup()
{
  Serial.begin(115200);

  // Initialize BTC
  SerialBT.begin("ESP32 Marvel Characters"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!\n");
}

void loop()
{
  // Check available Bluetooth data and perform read from the app
  if (SerialBT.available())
  {
    receivedData();
  }
}