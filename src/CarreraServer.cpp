#include "CarreraServer.h"

#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include <functional>

#include "Config.h"
#include "constants.h"
#include "driving.h"

using namespace std::placeholders;

CarreraServer::CarreraServer() : m_server{80}, socket("/ws"), ssid{"Carrera-Car2"}, password{"CarreraMachtSpass"}, ota_mode{false}, irl_enabled{false} {}

void CarreraServer::notifyClients(int newSpeed) {
    socket.textAll(String(newSpeed));
}

void CarreraServer::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        String command = String((char *)data);
        handleCommand(command);
    }
}

void CarreraServer::handleCommand(String command) {
    // IR-LED Command
    if (command.startsWith("L") && command.length() == 1) {
        enableIRLed();
        return;
    }
    // Speed Command
    int speed = command.toInt();
    driving::setSpeed(speed);
    notifyClients(speed);
}

void CarreraServer::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                            void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            break;
        case WS_EVT_DISCONNECT:
            emergencyStop();
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            emergencyStop();
            break;
    }
}

void CarreraServer::emergencyStop() {
    driving::setSpeed(0);
    notifyClients(0);
}

void CarreraServer::initWebSocket() {
    socket.onEvent(std::bind(&CarreraServer::onEvent, this, _1, _2, _3, _4, _5, _6));
    m_server.addHandler(&socket);
}

void CarreraServer::setup() {
    WiFi.softAP(ssid, password);

    initWebSocket();

    m_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });
    m_server.on("/OTA", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Ready for OTA!");
        ota_mode = true;
    });
    m_server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(millis()));
    });
    m_server.on("/speed", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncWebParameter* p = request->getParam(0);
        handleCommand(p->value());
        request->send(200, "application/json", "{ \"" + p->name() + "\" : " + p->value() + " }");
    });

    m_server.begin();
}

void CarreraServer::emergencyOTA() {
    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    delay(100);

    ArduinoOTA.setHostname("Carrera-Vehicle");

    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else  // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([]() { Serial.println("\nEnd"); })
        .onProgress([](unsigned int progress, unsigned int total) { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
        .onError([](ota_error_t error) {
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

    while (true) {
        ArduinoOTA.handle();
        ledcWrite(SLED_PWM_CHANNEL, 0);
        delay(100);
        ArduinoOTA.handle();
        ledcWrite(SLED_PWM_CHANNEL, 125);
        delay(100);
    }
}

void CarreraServer::enableIRLed() {
    irl_enabled = true;
    irl_toggle_time = millis();
    digitalWrite(IRLED_PIN, HIGH);
}

void CarreraServer::updateIRLed() {
    if (millis() - irl_toggle_time >= config->irl_time) {
        digitalWrite(IRLED_PIN, LOW);
        irl_enabled = false;
    }
}

void CarreraServer::loop() {
    if (ota_mode) emergencyOTA();
    if (irl_enabled) updateIRLed();
}