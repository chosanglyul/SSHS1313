#include <ESP8266WiFi.h>
#include <thread>
#include "WiFiextension.h"
#include "Query.h"

const char* const ssid = "SSHS-WiFi";
const char* const username = "comnc";
const char* const password = "sshs1sshs!";
const String host = "10.56.219.241";
const int port = 4000;
const String path = "/gun";
userinfo user = {2, 100, 0, 0, 0};
Query queryManager(path, host, port);

void setup() {
  Serial.begin(115200);
  //printWiFis();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED)
    connectWpa2(ssid, username, password);
  queryManager.pushData(user);
  queryManager.sendQuery(username, password);
  delay(2000);
}
