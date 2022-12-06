# Carrera-OS

## Idea

In this project we want to develop custom controls for a Carrera slotcar. We accomplish this by mounting an ESP32 board onto the car to control the motor instead of the controllers that are connected to the track.

Further, we want to add a camera to the vehicle to see a live videofeed from the cockpit to enable first person driving.

## Hardware

ESP Lipo Dev-Kit: https://www.olimex.com/Products/IoT/ESP32/ESP32-DevKit-LiPo/open-source-hardware (9.95€)

## Web-Interface

After copying the HTML code from the `index.html` file into the `constants.h` file you need to replace all `%` characters with `%%` in order to escape them. Otherwise the web interface will not work.

You should also adapt the version metatag in the header to verify that the chip boots with the correct version (see OTA Issues).

## Known Issues

### OTA Issues

1. The Slotcar sometimes boots an old version from before the OTA Update.
2. The Car reboots during the OTA Update process (motor boosts forward) which then fails.

Problemsolving:

If OTA repeatably fails, use UART bridge for flashing. After that the OTA should hopefully work again. You should also keep a version in your webpage (see Web-Interface) to check that the correct code is running on the chip.

### Slotcar Issues

1. While placing the car on the track the motor gets powered for a second, because its pin is HIGH by default. Make sure to hold the back wheels up while placing the car on the track.
2. The IR-Led does not work (even though the code is already implemented).

## Temperature Measurements

### Gelbes Auto

ESP ~30 °C
Capacitor (Top) 45.5 - 50 °C
HBridge 46.6 - 63 °C

### Weißes Auto

ESP ~30 °C
Capacitor (Top) 45-46 °C
HBridge 48.5 - 64 °C