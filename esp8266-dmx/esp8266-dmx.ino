// - - - - -
// ESPDMX - A Arduino library for sending and receiving DMX using the builtin
// serial hardware port.
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com>
// This work is licensed under a GNU style license.
//
// Last change: Musti <https://github.com/IRNAS> (edited by Musti)
//
// Documentation and samples are available at https://github.com/Rickgg/ESP-Dmx
// Connect GPIO02 - TDX1 to MAX3485 or other driver chip to interface devices
// Pin is defined in library
// - - - - -

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

#include "ESPDMX.h"
#include "secrets.h"

DMXESPSerial dmx;
unsigned long delay_time;

#define dmx_channels 32

WiFiServer wifiServer(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Serial started.");
  WiFi.hostname("esp-dmx-" + WiFi.macAddress());

  // try to connect to existing network
  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
  Serial.print("\n\nTry to connect to existing network ");
  Serial.print(SECRET_SSID);

  // Wait for connection
  do {
    delay(500);
    Serial.print(".");
  } while (WiFi.status() != WL_CONNECTED);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() { Serial.println("Start OTA update"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd OTA update"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  wifiServer.begin();

  dmx.init(dmx_channels); // initialization for first 32 addresses by default
  Serial.println("Ready");
}

void loop() {
  ArduinoOTA.handle();

  WiFiClient client = wifiServer.available();

  if (client) {
    Serial.println("Client Connected");
    int current_channel = 0;

    while (client.connected()) {

      while (client.available() > 0) {
        current_channel += 1;
        int channel_value = int(client.read());
        Serial.println(channel_value);

        dmx.write(current_channel, channel_value);
      }

      dmx_delay(50);
    }
    current_channel = 0;

    client.stop();
    Serial.println("\nClient disconnected");
  }
  dmx_delay(50);
}

// it's unclear why this is nessesary
// a regular delay of > 500ms (aprox) leads to weird issues
void dmx_delay(int ms) {
  unsigned long delay_until = millis() + ms;
  do {
    if (millis() > delay_until) {
      break;
    }
    dmx.update();
  } while (true);
}
