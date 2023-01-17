#pragma once

#include <WebServer.h>

#include <string>

#include "AsyncTCP.h"
#include "Config.h"
#include "ESPAsyncWebServer.h"

class CarreraServer {
   private:
    AsyncWebServer m_server;
    AsyncWebSocket socket;
    const char *ssid;
    const char *password;
    bool ota_mode;
    bool irl_enabled;
    unsigned long irl_toggle_time;
    int clients;

   public:
    CarreraServer();
    void setup();
    void initWebSocket();
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    void handleCommand(String payload);
    void notifyClients(int newSpeed);
    void emergencyStop();
    void emergencyOTA();
    void enableIRLed();
    void updateIRLed();
    void loop();
};
