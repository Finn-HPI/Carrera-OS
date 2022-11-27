#pragma once

#include "DebugServer.h"

using namespace std;

#define BUTTON_PIN 23
#define SLED_PIN 26
#define IRLED_PIN 18


#define ADC_PIN 34
#define TRK_PIN 32

#define INTERRUPT_PIN 13

//I2C interface
#define SDA 21
#define SCL 22

#define MOTOR_PIN 14
#define MOTOR_BRK 12

#define MOTOR_PWM_CHANNEL 0
#define MOTOR_BRK_PWM_CHANNEL 1
#define SLED_PWM_CHANNEL 2

extern DebugServer server;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Carrera RC </title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />
    <style>
      html {
        font-family: Arial, Helvetica, sans-serif;
        text-align: center;
      }
      h1 {
        font-size: 1.8rem;
        color: white;
      }
      h2 {
        font-size: 1.5rem;
        font-weight: bold;
        color: #143642;
      }
      body {
        margin: 0;
      }
      .state {
        font-size: 1.5rem;
        color: #8c8c8c;
        font-weight: bold;
      }
      .slider {
        -webkit-appearance: none;
        height: 50px;
        width: 300px;
        transform: rotate(-90deg) translate(0, -100%);
        transform-origin: 100% 0;
        border-radius: 25px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: 0.2s;
        transition: opacity 0.2s;
      }

      .slider:hover {
        opacity: 1;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        border-radius: 25px;
        width: 50px;
        height: 50px;
        background: #04aa6d;
        cursor: pointer;
      }

      .slider::-moz-range-thumb {
        width: 50px;
        height: 50px;
        border-radius: 25px;
        background: #04aa6d;
        cursor: pointer;
      }
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />
  </head>
  <body>
    <p class="state:">Throttle: <span id="state">0%</span></p>
    <input id="throttle-slider" class="slider" type="range" min="0" max="100" value="0" step="1" />
    <script>
      var gateway = `ws://${window.location.hostname}/ws`;
      var websocket;
      window.addEventListener("load", onLoad);
      function initWebSocket() {
        console.log("Trying to open a WebSocket connection...");
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onmessage = onMessage;
      }
      function onOpen(event) {
        console.log("Connection opened");
      }
      function onClose(event) {
        console.log("Connection closed");
        setTimeout(initWebSocket, 2000);
      }
      function onMessage(event) {
        var state;
        state = event.data;
        document.getElementById("state").innerHTML = state;
      }
      function onLoad(event) {
        initWebSocket();
        initSlider();
      }
      function initSlider() {
        document.getElementById("throttle-slider").addEventListener("input", update);
      }
      function update() {
        state = document.getElementById("throttle-slider").value;
        websocket.send(state);
      }
    </script>
  </body>
  </html>

)rawliteral";
