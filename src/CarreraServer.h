#pragma once

#include <WebServer.h>

#include <string>

#include "AsyncTCP.h"
#include "Config.h"
#include "ESPAsyncWebServer.h"
#include "Preferences.h"
#include "nvs_flash.h"

class CarreraServer {
   private:
    AsyncWebServer m_server;
    AsyncWebSocket socket;
    String s;
    String p;
    String ssid;
    String password;
    bool ota_mode;
    bool irl_enabled;
    unsigned long irl_toggle_time;
    Preferences preferences;

   public:
    CarreraServer();
    void setup();
    void initWebSocket();
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    void handleCommand(String payload);
    void notifyClients(int newSpeed);
    void setCredentials(String ssid, String password);
    void emergencyStop();
    void emergencyOTA();
    void enableIRLed();
    void updateIRLed();
    void loop();
};
