#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Query.h"

Query::Query(const String& host, const int port, const String& path) : mHost(host), mPort(port), mPath(path) {}

String Query::makeUrl() {
  String Url = mPath;
  for(int i = 0; i < Q.front().size(); i++) {
    Url += (i == 0 ? "?" : "&");
    Url += Q.front()[i].first;
    Url += "=";
    Url += Q.front()[i].second;
  }
  return Url;
}

void Query::sendQuery(const char* const username, const char* const password) {
  if (Q.size() && WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient 
    http.setAuthorization(username, password);
    http.begin(mHost, mPort, makeUrl());  //Specify request destination
    Q.pop();
    int httpCode = http.GET(); //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString(); //Get the request response payload
      Serial.println(payload); //Print the response payload
    }
    http.end();   //Close connection
  }
}

void Query::pushData(const userinfo user) {
  vector<psb> datas;
  datas.push_back(psb("id", user.id));
  datas.push_back(psb("hp", user.hp));
  datas.push_back(psb("boost", user.boost));
  datas.push_back(psb("bullet", user.bullet));
  datas.push_back(psb("armor", user.armor));
  Q.push(datas);
}
