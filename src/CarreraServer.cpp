#include "CarreraServer.h"

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <functional>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Config.h"
#include "constants.h"
#include "driving.h"

using namespace std::placeholders;

CarreraServer::CarreraServer(): m_server{80}, socket("/ws"), ssid{"CarreraHotspot"}, password{"CarreraMachtSpass"}, otaMode{false}, irl_enabled{false} {}

void CarreraServer::notifyClients(int newSpeed) {
  socket.textAll(String(newSpeed));
}

void CarreraServer::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String payload = String((char*)data);
    if (len == 1 && payload.equals("L")) { // enable IRL
      irl_toggle_time = millis();
      digitalWrite(IRLED_PIN, HIGH);
    } else {
      int speed = String((char*)data).toInt();
      driving::setSpeed(speed);
      notifyClients(speed);
    }
  }
}
void CarreraServer::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void CarreraServer::initWebSocket() {
  socket.onEvent(std::bind(&CarreraServer::onEvent, this, _1, _2, _3, _4, _5, _6));
  m_server.addHandler(&socket);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    return String(driving::getSpeed());
  }
  return String();
}

void CarreraServer::connectToWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    ledcWrite(SLED_PWM_CHANNEL, 0);
    delay(500);
    ledcWrite(SLED_PWM_CHANNEL, 255);
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
}

void CarreraServer::setup() {
  connectToWifi();

  initWebSocket();

  // Route for root / web page
  m_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  m_server.on("/OTA", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Ready for OTA!");
    this->otaMode = true;
  });

  // Start server
  m_server.begin();
}

void CarreraServer::emergencyOTA()
{
  Serial.begin(115200);

  connectToWifi();

  delay(100);

  ArduinoOTA.setHostname("Carrera-Vehicle");

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
    ledcWrite(SLED_PWM_CHANNEL, 0);
    delay(100);
    ArduinoOTA.handle();
    ledcWrite(SLED_PWM_CHANNEL, 125);
    delay(100);
  }
}

bool CarreraServer::getOtaMode() {
  return otaMode;
}

void CarreraServer::loop() {
  if (irl_enabled && irl_toggle_time-millis() >= config->irl_time)
    digitalWrite(IRLED_PIN, LOW);
}