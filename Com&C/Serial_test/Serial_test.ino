#include <SoftwareSerial.h>
SoftwareSerial data(A2, A3);

void setup() {
  Serial.begin(9600);
  data.begin(9600);
}

void loop() {
  while(data.available()) {
    Serial.print(data.read());
  }
  while(Serial.available()) {
    data.write(Serial.parseInt());
  }
}
