#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <utility>
#include <vector>
#include <queue>
#include <thread>
using namespace std;
typedef pair<String, byte> psb;

const int id = 2;
int hp = 100;
int boost = 10;
int bullet = 30;
int armor = 2;

String makeUrl(const String& path, const vector<psb>& vars) {
  String Url = path;
  for(int i = 0; i < vars.size(); i++) {
    Url += (i == 0 ? "?" : "&");
    Url += vars[i].first;
    Url += "=";
    Url += vars[i].second;
  }
  return Url;
}

void printWiFis() {
  //n : 검색된 와이파이 수 
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete!");
  if (n == 0) {
    Serial.println("No networks available.");
  } else {
    Serial.print(n);
    Serial.println(" networks discovered.");
    for (int i = 0; i < n; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      //비번 필요하면 뒤에 * 출력
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      Serial.println(WiFi.BSSIDstr(i));
      delay(10);
    }
  }
}

void connectWiFi(const String& WiFiname, const String& WiFipw) {
  WiFi.begin(WiFiname, WiFipw); //ssid, pw 입력
  //연결되는동안 *출력
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
  Serial.print(WiFi.localIP());
}

void query(const String& Url) {
  //여기부턴 인터넷서 긁어온거
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(Url);  //Specify request destination
    int httpCode = http.GET(); //Send the request 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString(); //Get the request response payload
      Serial.println(payload); //Print the response payload
    }
    http.end();   //Close connection
  }
}

void pushData(queue<vector<psb>>& Q) {
  vector<psb> datas;
  datas.push_back(psb(String("id"), id));
  datas.push_back(psb(String("hp"), hp));
  datas.push_back(psb(String("boost"), boost));
  datas.push_back(psb(String("bullet"), bullet));
  datas.push_back(psb(String("armor"), armor));
  Q.push(datas);
}

void setup() {
  const String WiFiname = "Bill's Laptop";
  const String WiFipw = "vL2650%2";
  Serial.begin(115200);
  printWiFis();
  connectWiFi(WiFiname, WiFipw);
}

void loop() {
  queue<vector<psb>> Q;
  pushData(Q);
  if(WiFi.status() != WL_CONNECTED) {
    const String WiFiname = "Bill's Laptop";
    const String WiFipw = "vL2650%2";
    connectWiFi(WiFiname, WiFipw);
  }
  while(WiFi.status() == WL_CONNECTED && Q.size()) {
    const String Url = makeUrl("http://192.168.137.1:4000/gun", Q.front());
    Q.pop();
    Serial.println(Url);
    query(Url);
  }
  delay(2000);
}
