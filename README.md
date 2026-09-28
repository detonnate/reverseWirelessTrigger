# ESP32 Wireless Reverse Trigger

A tested wireless reverse-trigger system for vehicles using two ESP32-C3 SuperMini boards and ESP-NOW. It was developed to add an automatic reverse-camera trigger to an aftermarket head unit **without running a trigger wire from the rear of the vehicle to the dashboard**.

> **Status:** Prototype tested and working.

## How it works

The rear transmitter is powered from the vehicle's reverse-light circuit. When reverse is selected it powers up and broadcasts a heartbeat over ESP-NOW every 75 ms. The front receiver is powered from ignition/ACC. While valid heartbeats are received it drives GPIO 4 HIGH, activating a 3.3 V HIGH-level relay module. The relay then supplies +12 V to the head unit's reverse-trigger input.

If the radio heartbeat disappears for more than 400 ms, the receiver switches the relay off. The existing wireless video link/camera is independent of this project.

## Hardware used

- 2 × ESP32-C3 SuperMini USB-C boards
- 2 × automotive 12 V → 5 V USB-C converters
- 2 × 1 A inline fuses
- 1 × 3.3 V HIGH-level single-channel relay module
- Automotive wire/connectors/heat-shrink
- Rear: off-the-shelf waterproof enclosure
- Front: custom enclosure (STL folder provided for the design)

## Wiring

### Rear / transmitter

```text
Reverse-light +12 V -> 1 A fuse -> 12 V-to-5 V USB-C converter + input
Vehicle GND ---------------------> converter - input
USB-C converter -----------------> ESP32-C3 USB-C
```

No GPIO wiring is required at the rear. The transmitter firmware assumes that if the ESP32 has power, reverse is active.

### Front / receiver

```text
ACC +12 V -> 1 A fuse -> 12 V-to-5 V USB-C converter + input
Vehicle GND ------------> converter - input
USB-C converter --------> ESP32-C3 USB-C

ESP32 3V3  -------------> Relay VCC / +
ESP32 GND  -------------> Relay GND / -
ESP32 GPIO 4 -----------> Relay IN / S

ACC +12 V --------------> Relay COM
Relay NO ----------------> Head-unit reverse-trigger input
Relay NC ----------------> Not used
```

The relay module is the **3.3 V HIGH-level** variant. The contacts switch the vehicle's 12 V trigger signal; 3.3 V is only used to power/control the relay module.

**Verify that your particular head unit expects +12 V on its reverse-trigger input before connecting it.**

## Firmware

- `firmware/transmitter/ReverseTrigger_TX.ino`
- `firmware/receiver/ReverseTrigger_RX.ino`

Both sketches must use the same `DEVICE_ID` and `ESPNOW_CHANNEL`.

Current defaults:

```cpp
DEVICE_ID = 0x72A91F31
ESPNOW_CHANNEL = 1
SEND_INTERVAL_MS = 75
SIGNAL_TIMEOUT_MS = 400
RELAY_PIN = GPIO 4
```

The firmware uses the newer ESP-NOW receive callback API used by current ESP32 Arduino/ESP-IDF packages. The transmitter deliberately does not register a send callback, avoiding callback-signature differences seen in newer ESP32-C3 packages.

## Arduino setup

1. Install Arduino IDE and Espressif's ESP32 board support.
2. Connect the rear ESP32-C3 over USB-C.
3. Open `ReverseTrigger_TX.ino`, select the appropriate ESP32-C3 board/port and upload.
4. Connect the front ESP32-C3.
5. Open `ReverseTrigger_RX.ino` and upload.
6. Bench-test before installing in the vehicle.

## Enclosure

The rear electronics use an off-the-shelf waterproof enclosure, so no custom rear STL is required.

The front/head-unit enclosure is intended to hold the receiver-side ESP32-C3, relay module and associated wiring. Put printable models in:

`enclosure/head-unit/STL/`

Put editable CAD/source files in:

`enclosure/head-unit/source/`

A placeholder is included until the final enclosure dimensions and STL are completed.

## Repository structure

```text
Wireless-Reverse-Trigger/
├── README.md
├── LICENSE
├── .gitignore
├── firmware/
│   ├── transmitter/
│   │   └── ReverseTrigger_TX.ino
│   └── receiver/
│       └── ReverseTrigger_RX.ino
├── hardware/
│   └── wiring/
│       └── README.md
├── enclosure/
│   ├── head-unit/
│   │   ├── STL/
│   │   │   └── README.md
│   │   └── source/
│   │       └── README.md
│   └── rear-unit/
│       └── README.md
├── docs/
│   └── BUILD.md
└── images/
    └── README.md
```

## Safety / production notes

This repository documents a working prototype, not an automotive-qualified production product. Vehicle electrical systems can experience transients and load-dump events. A commercial version should use automotive-rated power protection, EMC/RED testing as applicable, protected outputs, robust pairing/authentication and appropriate regulatory/compliance work.

The prototype uses a shared `DEVICE_ID` and unencrypted ESP-NOW broadcast. Do not treat that as secure authentication for a commercial product.

## License

MIT — see `LICENSE`.
