#include "DebugServer.h"

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <functional>

#include "Config.h"
#include "constants.h"
#include "driving.h"

DebugServer::DebugServer(): m_server{80}, ssid{"CarreraHotspot"}, password{"CarreraMachtSpass"} {}

void DebugServer::setCrossOrigin(){
    m_server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    m_server.sendHeader(F("Access-Control-Max-Age"), F("600"));
    m_server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
    m_server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
};

void DebugServer::sendCrossOriginHeader(){
    m_server.sendHeader(F("access-control-allow-credentials"), F("false"));
    setCrossOrigin();
    m_server.send(204);
}

void DebugServer::setup() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED)
  {
    ledcWrite(SLED_PWM_CHANNEL, 0);
    delay(500);
    ledcWrite(SLED_PWM_CHANNEL, 255);
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  setupRouting();
}

void DebugServer::getHome() {
  setCrossOrigin();
  String response = "<h1> Carrera Fahrzeug </h1>";
  if (m_server.arg("Speed") != ""){
    String speed = m_server.arg("Speed");
    response += "<p> Speed = " + speed + "</p>";
    try {
      int speedValue = std::atoi(speed.c_str());
      driving::setSpeed(speedValue);
    }
    catch(...) {
      response += "<strong> THIS IS NOT A NUMBER, IDIOT </strong>";
    }
  }
  m_server.send(200, "text/html", response);
}

void DebugServer::setSpeed() {
  setCrossOrigin();
  String response = "";
  if (m_server.arg("Speed") != ""){
    String speed = m_server.arg("Speed");
    try {
      int speedValue = std::atoi(speed.c_str());
      driving::setSpeed(speedValue);
      response = "{\"speed\":" + speed + "}";
    }
    catch(...) {
      response = "{\"error\": \"Speed is not a number\"}";
    }
  }
  m_server.send(200, "application/json", response);
}

void DebugServer::activateOTA() {
  setCrossOrigin();
  String response = "<h1> OTA Updates enabled! </h1> <p> The OTA process will start right now. </p> <p> The vehicle won't respond to any further commands unless you restart it! </p>";
  m_server.send(200, "text/html", response);
  emergencyOTA();
}

void DebugServer::setupRouting() {
  m_server.on("/", HTTP_GET, std::bind(&DebugServer::getHome, this));
  m_server.on("/", HTTP_POST, std::bind(&DebugServer::setSpeed, this));
  m_server.on("/", HTTP_OPTIONS, std::bind(&DebugServer::sendCrossOriginHeader, this));
  m_server.on("/OTA", HTTP_GET, std::bind(&DebugServer::activateOTA, this));
  m_server.begin();
}

void DebugServer::emergencyOTA()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  delay(100);

  ArduinoOTA
      .onStart([]()
               {
                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";

                 // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                 Serial.println("Start updating " + type);
               })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
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

  while (true)  
  {
    ArduinoOTA.handle();
    delay(10);
  }
}

void DebugServer::loop() {
  m_server.handleClient();
}