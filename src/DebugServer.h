#pragma once

#include <string>
#include <WebServer.h>
#include "Config.h"

class DebugServer
{
private:
  WebServer m_server;
  const char* ssid;
  const char* password;
public:
  DebugServer();
  void setup();
  void setupRouting();

  void emergencyOTA();
  void loop();
  void getHome();
  void setSpeed();
  void activateOTA();

  void setCrossOrigin();
  void sendCrossOriginHeader();
};
