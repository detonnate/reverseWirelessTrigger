#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

const uint32_t DEVICE_ID = 0x72A91F31;
const uint8_t ESPNOW_CHANNEL = 1;
const unsigned long SEND_INTERVAL_MS = 75;

uint8_t broadcastAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

struct ReversePacket {
  uint32_t deviceID;
  uint32_t counter;
  uint8_t reverseActive;
};

ReversePacket packet;
unsigned long lastSendTime = 0;
uint32_t packetCounter = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n==============================");
  Serial.println("WIRELESS REVERSE TRIGGER");
  Serial.println("REAR TRANSMITTER");
  Serial.println("==============================");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  Serial.print("TX MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ERROR: ESP-NOW failed to initialise.");
    while (true) delay(1000);
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = ESPNOW_CHANNEL;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("ERROR: Failed to add broadcast peer.");
    while (true) delay(1000);
  }

  Serial.println("TRANSMITTER READY\n");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSendTime >= SEND_INTERVAL_MS) {
    lastSendTime = now;
    packet.deviceID = DEVICE_ID;
    packet.counter = packetCounter++;
    packet.reverseActive = 1;

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&packet, sizeof(packet));
    if (result == ESP_OK) {
      Serial.print("Reverse heartbeat sent: ");
      Serial.println(packet.counter);
    } else {
      Serial.print("Send error: ");
      Serial.println(result);
    }
  }
}
