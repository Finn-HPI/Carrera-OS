#pragma once

#include <string>
#include <WebServer.h>
#include "Config.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"

class DebugServer
{
private:
  AsyncWebServer m_server;
  AsyncWebSocket socket;
  const char* ssid;
  const char* password;

public:
  DebugServer();
  void setup();
  void initWebSocket();
  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void notifyClients();
  void emergencyOTA();
  void loop();
};
