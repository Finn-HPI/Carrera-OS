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
	<title>Carrera 2.0</title>
</head>
<body>
	<div id="speedInput">
		<div id="speedValueDisplay">
			00
		</div>
	</div>
	<div id="ledPanel">
		<div id="ledDurationDisplay"></div>
		<div id="ledButton"></div>
	</div>
</body>

<script>
// This handles the input Slider.
setupMouseEvent();

function setupMouseEvent() {
	document.getElementById("speedInput").addEventListener("mousedown", handleMouseClick);
	document.getElementById("speedInput").addEventListener("mousemove", handleMouseClick);
	document.getElementById("speedInput").addEventListener("touchmove", handleTouchEvent);
	document.getElementById("ledButton").addEventListener("mousedown", clickLedButton);
}

function handleMouseClick(event) {
	if (!event.buttons || event.buttons != 1) return;
	const yPos = event.offsetY;
	const height = document.getElementById("speedInput").offsetHeight;
	const value = clampPctValue(1-(yPos/height));
	handleUserInput(value);
}

function handleTouchEvent(touchEvent) {
	const touch = touchEvent.touches[0];
	const inputFieldBounds = document.getElementById("speedInput").getBoundingClientRect();
	const yPos = touch.clientY - inputFieldBounds.top;
	const height = inputFieldBounds.height;
	const value = clampPctValue(1-(yPos/height));
	handleUserInput(value);
}

function handleUserInput(pctValue) {
	displaySpeedValue(pctValue);
	updateSpeed(convertPctToAbsoluteSpeed(pctValue));
}

function clickLedButton() {
	console.log("TODO: Activate LED!");
	const display = document.getElementById("ledDurationDisplay");
	display.classList.remove("animate");
	setTimeout(() => {display.classList.add("animate")}, 1);
}

function displaySpeedValue(pctValue) {
	const input = document.getElementById("speedInput");
	const value = convertPctToAbsoluteSpeed(pctValue);
	// document.getElementById("speedValueDisplay").innerHTML = value;
	const v = Math.round(pctValue * 100);
	input.style.background = `linear-gradient(0deg, var(--darkColor) 0%, var(--darkColor) ${v}%, var(--lightColor) ${v}%, var(--lightColor) 100%)`;
}

function clampPctValue(pctValue) {
	return Math.min(Math.max(0, pctValue), 1);
}

function convertPctToAbsoluteSpeed(pctValue) {
	const min = 0;
	const max = 130;
	const value = min + Math.round(pctValue * (max - min));
	return value;
}

//Websockets:
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
	const value = event.data;
	document.getElementById("speedValueDisplay").innerHTML = value;
}
function onLoad(event) {
	initWebSocket();
}
function updateSpeed(value) {
	websocket.send(value);
}
</script>

<style>
:root {
	--buttonColor: #3D7EAA;
	--shadow: 0 0 10px 1px rgba(23, 23, 23, 0.2);
}
body {
	background: linear-gradient(135deg, #283048, #859398);
}

#speedInput {
	opacity: 0.8;
	--lightColor: lightGray;
	--darkColor: gray;
	position: fixed;
	left: 50%;
	top: 50%;
	transform: translate(-50%, -50%);
	width: 80vw;
	height: 90vh;
	border-radius: 10px;
	background: var(--lightColor);
	box-shadow: var(--shadow);
}

#speedValueDisplay {
	position: absolute;
	left: 50%;
	top: 50%;
	font-size: 10rem;
	transform: translate(-50%, -50%);
	-webkit-user-select: none; /* Safari */
	-ms-user-select: none; /* IE 10 and IE 11 */
	user-select: none; /* Standard syntax */
}

#ledPanel {
	position: fixed;
	right: 0px;
	top: 0px;
	height: 100vh;
	width: 10vw;
}

#ledButton {
	position: absolute;
	bottom: 1em;
	left: 50%;
	border-radius: 50%;
	box-shadow: 0 0 10px 1px rgba(23, 23, 23, 0.2);
	transform: translate(-50%);
	width: 80%;
	aspect-ratio: 1;
	background-color: var(--buttonColor);
	box-shadow: var(--shadow);
}

#ledDurationDisplay {
	--activeColor: var(--buttonColor);
	--inactiveColor: rgba(211, 211, 211, 0);
	position: absolute;
	background: linear-gradient(to top, var(--activeColor) 0%, var(--activeColor) 50%, var(--inactiveColor) 50%, var(--inactiveColor) 100%);
	background-size: 100% 200%; /* For animating the background */
	border-radius: 5px;
	left: 50%;
	top: 50%;
	transform: translate(-50%, -50%);
	width: 10px;
	height: 80%;
}

#ledDurationDisplay.animate {
	animation-name: durationDisplay;
	animation-timing-function: linear;
	animation-duration: 1s;
}
/* This moves the display for the duration of the led */
@keyframes durationDisplay {
	0% {
		background-position: 0% 100%; 
	}
	100% {
		background-position: 0% 0%;
	}
}
</style>
</html>
)rawliteral";
