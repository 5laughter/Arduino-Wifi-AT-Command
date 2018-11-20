#include "ESP8266.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 10); /* RX:D11, TX:D10 */
ESP8266 wifi(mySerial);

#define HOST_NAME "hostname"
#define HOST_PORT (8080) // port name

const char* ssid = "your ssid";
const char* password = "your password";

int isConnected = 0;

void setup () {
  dht.begin();
  Serial.begin(9600);
  Serial.print("setup begin\r\n");
  
  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());
  
  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }
  
  if (wifi.joinAP(ssid, password)) {
    Serial.print("Join WiFi success\r\n");
    Serial.print("IP:");
    Serial.println( wifi.getLocalIP().c_str()); 
  } else {
    Serial.print("Join WiFi failure\r\n");
  }
  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }
  
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
  
    Serial.print("create tcp ok\r\n");
    isConnected = 1;
    delay(1000);
  } else {
    Serial.print("create tcp err\r\n");
    return;
  }
}

void loop() {
  uint8_t buffer[1024] = {0};
  if(isConnected == 0){}
  else if(isConnected == 1){
    Serial.println("start");
    delay(1000);
    String http = String();
    http += "GET yoururl";
    http += " HTTP/1.1\r\n";
    http += "Host: ";
    http += HOST_NAME;
    http += "\r\n";
    http += "Connection: close\r\n\r\n";
    delay(1000);
    Serial.println(http);
    wifi.send((const uint8_t*)http.c_str(), http.length());
    delay(1000);
    Serial.println("end");
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
      Serial.print("Received:[");
      for(uint32_t i = 0; i < len; i++) {
        Serial.print((char)buffer[i]);
      }
      Serial.print("]\r\n");
    }
  
    if (wifi.releaseTCP()) {
      Serial.print("release tcp ok\r\n");
    } else {
      Serial.print("release tcp err\r\n");
    }
  }
  delay(4000);
}
