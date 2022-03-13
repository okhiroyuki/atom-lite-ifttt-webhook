#include <M5Atom.h>
#include <time.h>
#include <HTTPClient.h>
#include "config.h"
#include "certificate.h"
#include <WiFiClientSecure.h>

volatile byte stop_state = LOW;
volatile byte start_state = LOW;

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
    wifiClient.setCACert(test_ca_cert);
  }
}

void setup() {
  //Initialize serial and wait for port to open:
  M5.begin(true, false, true);
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  
  configTime( gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(26, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);
  attachInterrupt(26, interrupt_red_btn, RISING);
  attachInterrupt(32, interrupt_blue_btn, RISING);
}

void loop(){
  connect_wifi();
  if (start_state == HIGH){
    httpRequest("spotify_start");
    start_state = LOW;
  }
  if (stop_state == HIGH){
    httpRequest("spotify_stop");
    stop_state = LOW;
  }
  delay(100);
}

void interrupt_red_btn(){
  if (stop_state == LOW){
    stop_state = HIGH;  
  }
}

void interrupt_blue_btn(){
  if (start_state == LOW){
    start_state = HIGH; 
  }
}

void httpRequest(String event) {
  HTTPClient http;
  String url = "https://maker.ifttt.com/trigger/" + event + "/with/key/" + maker_key;
  http.begin(url);
  http.GET();
  Serial.println(http.getString());
  http.end();
}
