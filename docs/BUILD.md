# Build Guide

## 1. Program both ESP32-C3 boards
Upload the transmitter sketch to the rear board and the receiver sketch to the front board. Keep `DEVICE_ID` and `ESPNOW_CHANNEL` identical.

## 2. Bench-test the wireless link
Power the receiver first. Power the transmitter. GPIO 4 should go HIGH and the 3.3 V HIGH-level relay should energise. Remove transmitter power; the relay should release after approximately 400 ms.

## 3. Rear installation
Take fused +12 V from the reverse-light circuit and ground from a suitable vehicle ground. Feed these into the 12 V-to-5 V USB-C converter and plug the converter into the transmitter ESP32. Mount the electronics in the off-the-shelf waterproof enclosure.

## 4. Front installation
Take fused ignition/ACC +12 V and ground to the front USB-C converter. Plug it into the receiver ESP32. Connect ESP32 3V3, GND and GPIO 4 to the corresponding relay module power, ground and trigger input.

Connect fused/appropriate ACC +12 V to relay COM. Connect relay NO to the head unit's reverse-trigger input. Leave NC unused.

## 5. Final test
With ignition on and the vehicle safely stationary, verify the receiver is powered but the relay is off. Select reverse: the rear unit powers, the receiver detects it, and the relay supplies +12 V to the head-unit reverse input. Exit reverse: the relay should release after the timeout.

## Important
- Confirm the head unit expects a +12 V reverse trigger.
- Never feed vehicle 12 V directly into an ESP32 GPIO/3V3/5V pin.
- Verify converter output before first connection.
- Fuse both vehicle-side supplies.
- Secure and insulate all wiring before road use.
