#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

const uint32_t DEVICE_ID = 0x72A91F31;
const uint8_t ESPNOW_CHANNEL = 1;
const int RELAY_PIN = 4;
const unsigned long SIGNAL_TIMEOUT_MS = 400;

unsigned long lastValidPacket = 0;
bool reverseActive = false;

struct ReversePacket {
  uint32_t deviceID;
  uint32_t counter;
  uint8_t reverseActive;
};

void reverseOn() {
  if (!reverseActive) {
    reverseActive = true;
    digitalWrite(RELAY_PIN, HIGH); // 3.3 V HIGH-level relay module
    Serial.println("\n*** REVERSE ON ***\n");
  }
}

void reverseOff() {
  if (reverseActive) {
    reverseActive = false;
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("\n*** REVERSE OFF ***\n");
  }
}

void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len != sizeof(ReversePacket)) return;

  ReversePacket receivedPacket;
  memcpy(&receivedPacket, incomingData, sizeof(receivedPacket));

  if (receivedPacket.deviceID != DEVICE_ID) return;

  if (receivedPacket.reverseActive == 1) {
    lastValidPacket = millis();
    reverseOn();
    Serial.print("Heartbeat received: ");
    Serial.println(receivedPacket.counter);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  delay(500);

  Serial.println("\n==============================");
  Serial.println("WIRELESS REVERSE TRIGGER");
  Serial.println("FRONT RECEIVER");
  Serial.println("==============================");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  Serial.print("RX MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ERROR: ESP-NOW failed to initialise.");
    while (true) {
      digitalWrite(RELAY_PIN, LOW);
      delay(1000);
    }
  }

  esp_now_register_recv_cb(onDataReceived);
  Serial.println("RECEIVER READY");
  Serial.println("Waiting for rear transmitter...");
}

void loop() {
  if (reverseActive && (millis() - lastValidPacket > SIGNAL_TIMEOUT_MS)) {
    reverseOff();
  }
  delay(5);
}
