#pragma once

#include "CarreraServer.h"

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

extern CarreraServer server;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Carrera 2.0</title>
	<meta name="version" content="06.12_16:46">
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
	<div id="stopGoButton" class="stop" hidden></div>
	<span id="versionDisplay"></span>
</body>

<script>
function displayVersion() {
	const version = document.querySelector("meta[name='version']").content;
	document.getElementById("versionDisplay").innerHTML = version;
}
displayVersion();
// Variables and constants
const min = 0;
const max = 130;
var currentSpeed = 0;
var speedInput = 0;

// This handles the input Slider.
setupClickEvents();

function setupClickEvents() {
	document.getElementById("speedInput").addEventListener("mousedown", handleMouseClick);
	document.getElementById("speedInput").addEventListener("mousemove", handleMouseClick);
	document.getElementById("speedInput").addEventListener("touchmove", handleTouchEvent);
	document.getElementById("ledButton").addEventListener("click", clickLedButton);
	document.getElementById("ledButton").addEventListener("touchstart", clickLedButton);
	document.getElementById("stopGoButton").addEventListener("click", clickStopGoButton);
	document.getElementById("stopGoButton").addEventListener("touchstart", clickStopGoButton);
}

function handleMouseClick(event) {
	if (!event.buttons || event.buttons != 1) return;
	const value = calculateClickValue(event.y);
	handleUserInput(value);
}

function calculateClickValue(yCoordinate) {
	const inputFieldBounds = document.getElementById("speedInput").getBoundingClientRect();
	const yPos = yCoordinate - inputFieldBounds.top;
	const height = inputFieldBounds.height;
	return clampPctValue(1-(yPos/height));
}

function handleTouchEvent(touchEvent) {
	const touch = touchEvent.touches[0];
	const value = calculateClickValue(touch.clientY);
	handleUserInput(value);
}

function handleUserInput(pctValue) {
	const absolute = convertPctToAbsoluteSpeed(pctValue);
	if (speedInput == absolute) return; // No change happened, we can ignore this
	speedInput = absolute;
	updateSpeedInputDisplay();
	updateSpeed(absolute);
}

function clickLedButton(event) {
	if (event.pointerId && event.pointerId != 1) // Prevent action to execute twice on touch click
		return;
	activateLed();
	const display = document.getElementById("ledDurationDisplay");
	display.classList.remove("animate");
	setTimeout(() => {display.classList.add("animate")}, 1);
}

function clickStopGoButton(event) {
	if (event.pointerId && event.pointerId != 1) // Prevent action to execute twice on touch click
		return;
	console.log("Do something");
}

function updateSpeedInputDisplay() {
	const input = document.getElementById("speedInput");
	const inputValue = 100 * absoluteToPctSpeed(speedInput);
	const actualValue = 100 * absoluteToPctSpeed(currentSpeed);
	document.getElementById("speedValueDisplay").innerHTML = currentSpeed;
	if (actualValue < inputValue) {
		input.style.background = `linear-gradient(0deg, var(--darkColor) 0%%, var(--darkColor) ${actualValue}%%, var(--changeColor) ${actualValue}%%, var(--changeColor) ${inputValue}%%, var(--lightColor) ${inputValue}%%, var(--lightColor) 100%%)`;
	} else {
		input.style.background = `linear-gradient(0deg, var(--darkColor) 0%%, var(--darkColor) ${inputValue}%%, var(--changeColor) ${inputValue}%%, var(--changeColor) ${actualValue}%%, var(--lightColor) ${actualValue}%%, var(--lightColor) 100%%)`;
	}
	
}

function clampPctValue(pctValue) {
	return Math.min(Math.max(0, pctValue), 1);
}

function convertPctToAbsoluteSpeed(pctValue) {
	const value = min + Math.round(pctValue * (max - min));
	return value;
}

function absoluteToPctSpeed(absoluteValue) {
	return (absoluteValue - min) / max;
}

//Websockets:
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var connected = false;
window.addEventListener("load", onLoad);
function initWebSocket() {
	console.log("Trying to open a WebSocket connection...");
	websocket = new WebSocket(gateway);
	websocket.onopen = onOpen;
	websocket.onclose = onClose;
	websocket.onmessage = onMessage;
}
function onOpen(event) {
	connected = true;
	document.getElementById("speedInput").classList.add("connected");
	console.log("Connection opened");
}
function onClose(event) {
	connected = false;
	document.getElementById("speedInput").classList.remove("connected");
	console.log("Connection closed");
	setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
	console.log("Recieving: ", event);
	currentSpeed = event.data;
	updateSpeedInputDisplay();
}
function onLoad(event) {
	initWebSocket();
}
function updateSpeed(value) {
	sendViaWebsocket(value.toString());
}
function activateLed() {
	if (!connected) return;
	websocket.send("L");
}
function sendViaWebsocket(message) {
	if (!connected) return;
	console.log("Sending:", message);
	websocket.send(message);
}
</script>

<style>
:root {
	--buttonColor: #3D7EAA;
	--stopButtonColor: rgb(187, 57, 57);
	--goButtonColor: rgb(32, 107, 32);
	--shadow: 0 0 10px 1px rgba(23, 23, 23, 0.2);
}
body {
	background: linear-gradient(135deg, #283048, #859398);
}

#speedInput {
	opacity: 0.8;
	--lightColor: lightGray;
	--darkColor: gray;
	--changeColor: rgb(225, 199, 93);
	position: fixed;
	left: 10%%;
	top: 50%%;
	transform: translateY(-50%%);
	width: 50%%;
	height: 90%%;
	border-radius: 10px;
	background: var(--lightColor);
	box-shadow: 0 0 10px 1px;
	color: red;
}

#speedInput.connected {
	color: green;
}

#speedValueDisplay {
	position: absolute;
	left: 50%%;
	top: 50%%;
	font-size: 10rem;
	transform: translate(-50%%, -50%%);
	color: black;
	-webkit-user-select: none; /* Safari */
	-ms-user-select: none; /* IE 10 and IE 11 */
	user-select: none; /* Standard syntax */
}

#ledPanel {
	position: fixed;
	right: 0px;
	top: 0px;
	height: 100vh;
	width: 30vw;
}

#ledButton {
	position: absolute;
	bottom: 5%%;
	left: 50%%;
	border-radius: 50%%;
	box-shadow: 0 0 10px 1px rgba(23, 23, 23, 0.2);
	transform: translate(-50%%);
	width: 80%%;
	max-width: 10rem;
	aspect-ratio: 1;
	background-color: var(--buttonColor);
	box-shadow: var(--shadow);
}

#ledDurationDisplay {
	--activeColor: var(--buttonColor);
	--inactiveColor: rgba(211, 211, 211, 0);
	position: absolute;
	background: linear-gradient(to top, var(--activeColor) 0%%, var(--activeColor) 50%%, var(--inactiveColor) 50%%, var(--inactiveColor) 100%%);
	background-size: 100%% 200%%; /* For animating the background */
	border-radius: 5px;
	left: 50%%;
	top: 50%%;
	transform: translate(-50%%, -50%%);
	width: 3rem;
	height: 80%%;
}

#ledDurationDisplay.animate {
	animation-name: durationDisplay;
	animation-timing-function: linear;
	animation-duration: 2s;
}
/* This moves the display for the duration of the led */
@keyframes durationDisplay {
	0%% {
		background-position: 0%% 100%%; 
	}
	100%% {
		background-position: 0%% 0%%;
	}
}

#stopGoButton {
	position: absolute;
	top: 0px;
	right: 0px;
	margin: 1rem;
	border-radius: 40%%;
	box-shadow: 0 0 10px 1px rgba(23, 23, 23, 0.2);
	width: 10%%;
	aspect-ratio: 1;
	background-color: var(--goButtonColor);
	box-shadow: var(--shadow);
}

.stop {
	background-color: var(--stopButtonColor) !important;
}

#versionDisplay{
	position: fixed;
	bottom: 0px;
	left: 0px;
	margin: 2px;
	color: lightgray;
	opacity: 0.25;
}
</style>
</html>
)rawliteral";
