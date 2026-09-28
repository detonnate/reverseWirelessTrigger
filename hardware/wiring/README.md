# Wiring Documentation

Store wiring diagrams, schematics and PCB information here.

Current prototype:
- Rear: reverse-light 12 V -> fused USB-C converter -> ESP32-C3.
- Front: ACC 12 V -> fused USB-C converter -> ESP32-C3.
- ESP32 front: 3V3 -> relay VCC, GND -> relay GND, GPIO4 -> relay IN.
- Relay: COM -> +12 V ACC, NO -> head-unit reverse input, NC unused.

The relay module is the 3.3 V HIGH-level variant.
