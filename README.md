# Carrera-OS

## Idea

In this project we want to develop custom controls for a Carrera slotcar. We accomplish this by mounting an ESP32 board onto the car to control the motor instead of the controllers that are connected to the track.

Further, we want to add a camera to the vehicle to see a live videofeed from the cockpit to enable first person driving.

## Web-Interface

After copying the HTML code from the `index.html` file into the `constants.h` file you need to replace all `%` characters with `%%` in order to escape them. Otherwise the web interface will not work.

You should also adapt the version metatag in the header to verify that the chip boots with the correct version (see OTA Issues).

## Protocol

### HTTP Endpoints

`192.168.4.1/` will open the frontend for controlling the vehicle.

`192.168.4.1/OTA` will enable OTA flashing of the board.

`192.168.4.1/set_cred?ssid=<NewSSID>&pwd=<NewPassword>` is used for changing the ESPs WiFi-Name and Password. **IMPORTANT:** The password needs to be at least 8 characters long.

`192.168.4.1/cred` will return the current WiFi SSID and password.

There are further endpoints for debugging such as `/runtime` or `/nvs_reset`.

### WS Messages

`<number>` will set the speed to the provided number. This will be clamped by `config->min` and `config->max`.

`L` will enable the LED on the car. The duration is set in the config of the vehicle, however if you want to change this remember to change the `animation-duration` in the frontend as well.

`B` will boost the vehicle for a short amount of time.

`S` acts as an emergency stop and will set the speed to `0`.

## IR LED

The car uses the Ghostcar frequency which allows it to change lanes by simply turning on the IR-Led with the corresponding frequency.

| Vehicle |Frequency |
|---------|----------|
|    1    |  15600   |
|    2    |   7810   |
|    3    |   5210   |
|    4    |   3910   |
|    5    |   3120   |
|    6    |   2600   |
|Ghostcar |   2230   |
| Pacecar |   1950   |

[Source](https://redlichelectronics.de/ir-diode.html)

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

## TODO

1. Left Joycon fix
2. Guide für Erstnutzung