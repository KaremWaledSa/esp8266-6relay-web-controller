# ESP8266 6-Relay Web Controller

> A self-hosted relay controller that runs entirely on the ESP8266 — no cloud, no app, no internet required. Connect your phone to the device's Wi-Fi and control six relays from any browser.

![Platform](https://img.shields.io/badge/Platform-ESP8266%20%2F%20NodeMCU-blue)
![Language](https://img.shields.io/badge/Language-Arduino%20C%2B%2B-lightgrey)
![Connectivity](https://img.shields.io/badge/Connectivity-Wi--Fi%20AP%20mode-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## Overview

The ESP8266 creates its own Wi-Fi access point and serves a mobile-friendly control page on port 80. No router or internet connection is needed. Any device that connects to the AP can open the control page and toggle or momentarily trigger relays.

Two relay types are supported in the same firmware:

| Relay | Type | Behavior |
|---|---|---|
| Relay 1 | Momentary | Active only while button is held (press / release) |
| Relay 2 | Momentary | Active only while button is held (press / release) |
| Relay 3 | Toggle | Latches ON or OFF on each tap |
| Relay 4 | Toggle | Latches ON or OFF on each tap |
| Relay 5 | Toggle | Latches ON or OFF on each tap |
| Relay 6 | Toggle | Latches ON or OFF on each tap |

---

## Hardware

| Component | Notes |
|---|---|
| ESP8266 / NodeMCU | Any ESP8266 board works |
| 6-channel relay module | Use an optocoupler-isolated board for safety |
| 5V power supply | Power relay coils separately from the ESP8266 3.3V rail |
| Level shifter (optional) | Required if relay module uses 5V logic signals |

> ⚠️ Do not power relay coils directly from the ESP8266 VCC pin — inductive kickback and current draw will reset or damage the module.

**Pin mapping (default firmware):**

| Relay | ESP8266 Pin | NodeMCU Label |
|---|---|---|
| Relay 1 | GPIO5 | D1 |
| Relay 2 | GPIO4 | D2 |
| Relay 3 | GPIO0 | D3 |
| Relay 4 | GPIO2 | D4 |
| Relay 5 | GPIO14 | D5 |
| Relay 6 | GPIO12 | D6 |

---

## How It Works

```
Phone / Laptop
     │
     │  Connect to Wi-Fi AP: "YOUR_AP_NAME"
     ▼
ESP8266 (192.168.4.1)
     │
     ├── GET /           → serves the relay control page
     ├── GET /press?relay=N    → sets relay N HIGH (momentary ON)
     ├── GET /release?relay=N  → sets relay N LOW  (momentary OFF)
     └── GET /toggle?relay=N   → flips relay N state (toggle)
```

The control page is generated directly from firmware — no external files, no SD card needed.

---

## Repository Structure

```
esp8266-6relay-web-controller/
├── firmware/
│   └── esp8266_6relay_web_controller/
│       ├── esp8266_6relay_web_controller.ino   # Main firmware
│       └── secrets.example.h                  # AP credentials template
├── web-ui/
│   └── reference_dashboard_mockup.html        # Standalone UI reference
├── docs/
│   ├── HARDWARE.md
│   └── SETUP.md
├── .gitignore
├── LICENSE
└── README.md
```

---

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x
- ESP8266 board package: add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to board manager URLs
- No additional libraries required

### Configure & Flash

```
1. Open: firmware/esp8266_6relay_web_controller/esp8266_6relay_web_controller.ino
2. Edit lines 3–4:
      const char* apSsid     = "YOUR_AP_NAME";
      const char* apPassword = "YOUR_AP_PASSWORD";
3. Select board: NodeMCU 1.0 (ESP-12E Module) or your variant
4. Upload via USB
```

### Connect & Control

```
1. Power the ESP8266
2. On your phone/laptop: connect to Wi-Fi network "YOUR_AP_NAME"
3. Open browser → http://192.168.4.1
4. The relay control panel appears — no login required
```

---

## Customization

To change which relays are momentary vs. toggle, edit the check in `handlePress` / `handleRelease` / `handleToggle`:

```cpp
// Currently: relays 0–1 are momentary, 2–5 are toggle
if (relayIndex >= 0 && relayIndex < 2)  // momentary range
if (relayIndex >= 2 && relayIndex < numRelays)  // toggle range
```

To rename relay labels, edit the `label` string inside `handleRoot()`.

---

## Security

This project uses an open Wi-Fi AP or a password-protected AP — there is no per-user authentication on the control page. Anyone connected to the AP can control all relays. **Do not use this on a shared or public network.**

Never commit real AP credentials. Copy `secrets.example.h` → `secrets.h` and add `secrets.h` to `.gitignore`.

---

## License

MIT — see [LICENSE](LICENSE).
