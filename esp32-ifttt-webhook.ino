#include <M5Atom.h>
#include <time.h>
#include <HTTPClient.h>
#include "config.h"
#include "certificate.h"
#include <WiFiClientSecure.h>

WiFiClientSecure wifiClient;

void connect_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      // wait 1 second for re-trying
      delay(1000);
    }
    Serial.print("Connected to ");
    Serial.print(ssid);
    Serial.print(" / ");
    Serial.println(WiFi.localIP());
    wifiClient.setCACert(ca_cert);
  }
}

void setup() {
  //Initialize serial and wait for port to open:
  M5.begin(true, false, true);
  delay(50);
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  
  configTime( gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop(){
  connect_wifi();
  M5.update();
  if (M5.Btn.isPressed()){
    httpRequest("atom_lite_btn_pressed");
  }
  delay(100);
}

void httpRequest(String event) {
  HTTPClient http;
  String url = "https://maker.ifttt.com/trigger/" + event + "/with/key/" + maker_key;
  http.begin(url);
  http.GET();
  Serial.println(http.getString());
  http.end();
}
