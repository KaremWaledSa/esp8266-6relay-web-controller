# Hardware Notes

## Components

| Part | Specification |
|---|---|
| Microcontroller | ESP8266 / NodeMCU v2 (ESP-12E) |
| Relay module | 6-channel, optocoupler-isolated, active-LOW trigger |
| Power supply | 5V 2A minimum (relays draw ~70 mA each under load) |
| Level shifter | Optional — required if relay board uses 5V logic signals |

## Power Architecture

```
5V adapter
    │
    ├──► Relay module VCC (coil supply)
    │
    └──► NodeMCU VIN (onboard 3.3V regulator)
              │
              └──► ESP8266 3.3V logic (D1–D6 GPIO outputs)
```

Do not connect relay coils to the ESP8266 3.3V pin. Relay coils draw more current than the ESP8266 regulator can supply and generate inductive spikes on the supply rail.

## Active-LOW vs. Active-HIGH Relay Modules

Most optocoupler relay boards are **active-LOW**: the relay activates when the input pin is pulled LOW (GPIO LOW = relay ON). If your module is active-LOW, invert the `digitalWrite` logic in the firmware:

```cpp
// Active-LOW example (relay ON when pin LOW)
digitalWrite(relayPins[i], LOW);   // ON
digitalWrite(relayPins[i], HIGH);  // OFF
```

The default firmware uses active-HIGH logic. Adjust to match your hardware.
