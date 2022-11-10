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

  void emergencyOTA();

};
