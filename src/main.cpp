#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
 
const char *SSID = "EagleEyePixel";
const char *PWD = "JuliansHotspot42";

#define LED 2
 
// Web server running on port 80
WebServer server(80);
 
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void getHome() {
  server.send(200, "application/json", "{'hello': 1}");
}

void setup_routing() {	 	 
  server.on("/", getHome);
  // start server	 	 
  server.begin();	 	 
}
 

void setup() {	 	 
  Serial.begin(115200);	 	 
  pinMode(LED, OUTPUT);
 	 	 
  connectToWiFi();
  setup_routing();
  digitalWrite(LED, HIGH);
}	 	 
  	 	 
void loop() {	 	 
  server.handleClient();	 	 
}