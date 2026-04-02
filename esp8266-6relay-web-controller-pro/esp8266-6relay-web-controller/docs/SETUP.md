# Setup Guide

## Requirements

| Tool | Notes |
|---|---|
| Arduino IDE 2.x | https://www.arduino.cc/en/software |
| ESP8266 board package | Add URL in Preferences → Additional Boards Manager URLs |

Board manager URL:
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

No external libraries needed — only built-in ESP8266 packages.

## Board Settings (Arduino IDE)

| Setting | Value |
|---|---|
| Board | NodeMCU 1.0 (ESP-12E Module) |
| Upload Speed | 115200 |
| Flash Size | 4MB (FS:2MB OTA:~1019KB) |
| CPU Frequency | 80 MHz |

## Flash Steps

```
1. Open firmware/esp8266_6relay_web_controller/esp8266_6relay_web_controller.ino
2. Set your AP name and password in lines 3–4
3. Select your board and COM port
4. Sketch → Upload
```

## First Connection

```
1. Power on the board
2. Connect your device to Wi-Fi: "YOUR_AP_NAME"
3. Open: http://192.168.4.1
```

Default AP IP is `192.168.4.1`. This is hardcoded via `WiFi.softAPConfig()` in `setup()`.

## Relay Wiring

Connect each relay module IN pin to the corresponding D-pin on the NodeMCU:

```
Relay 1 IN → D1 (GPIO5)
Relay 2 IN → D2 (GPIO4)
Relay 3 IN → D3 (GPIO0)
Relay 4 IN → D4 (GPIO2)
Relay 5 IN → D5 (GPIO14)
Relay 6 IN → D6 (GPIO12)
GND        → GND (shared)
VCC        → External 5V (not ESP8266 3.3V)
```
