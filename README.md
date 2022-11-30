# Carrera-OS

## Idea

In this project we want to develop custom controls for a Carrera slotcar. We accomplish this by mounting an ESP32 board onto the car to control the motor instead of the controllers that are connected to the track.

Further, we want to add a camera to the vehicle to see a live videofeed from the cockpit to enable first person driving.

## Hardware

ESP Lipo Dev-Kit: https://www.olimex.com/Products/IoT/ESP32/ESP32-DevKit-LiPo/open-source-hardware (9.95€)

## Web-Interface

After copying the HTML code from the .html file into the ESP file you need to replace all `%` characters with `%%` in order to escape them.
