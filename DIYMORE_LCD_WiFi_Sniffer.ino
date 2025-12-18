#include <TFT_eSPI.h>
#include <WiFi.h>
#include "esp_wifi.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

unsigned long lastChannelSwitch = 0;
int currentCh = 1;
long packetCount = 0;
bool isDrawing = false; // The "Flag" to prevent glitches

uint16_t getRSSIColor(int rssi) {
  if (rssi > -40) return TFT_MAGENTA;
  if (rssi > -50) return TFT_RED;
  if (rssi > -60) return TFT_ORANGE;
  if (rssi > -70) return TFT_YELLOW;
  if (rssi > -80) return TFT_GREEN;
  return TFT_BLUE;
}

void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (isDrawing) return; // Skip if we are currently busy to prevent flickering
  isDrawing = true;

  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  int rssi = pkt->rx_ctrl.rssi;
  packetCount++;

  // Waterfall Scroll
  img.scroll(0, -18); 
  img.setCursor(0, 112);
  
  uint16_t color = getRSSIColor(rssi);
  img.setTextColor(color, TFT_BLACK);
  
  const char* type_str = (type == WIFI_PKT_MGMT) ? "MGMT" : (type == WIFI_PKT_DATA) ? "DATA" : "CTRL";
  img.printf("[%s] RSSI:%d", type_str, rssi);
  
  // Draw a small signal strength bar on the right side of the line
  int barWidth = map(rssi, -100, -30, 0, 60);
  img.fillRect(250, 115, barWidth, 10, color);
  
  img.pushSprite(0, 40);

  // Update Header only every 10 packets to save CPU/Reduce glitching
  if (packetCount % 10 == 0) {
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setCursor(10, 10);
    tft.printf("CH: %d   PKTS: %ld   ", currentCh, packetCount);
  }
  
  isDrawing = false;
}

void setup() {
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 320, 38, TFT_BLUE);
  tft.setTextSize(2);

  img.createSprite(320, 130);
  img.fillSprite(TFT_BLACK);
  img.setTextSize(2);
  
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
  esp_wifi_set_channel(currentCh, WIFI_SECOND_CHAN_NONE);
}

void loop() {
  if (millis() - lastChannelSwitch > 3000) {
    if (!isDrawing) { // Only switch if screen is not currently updating
      currentCh++;
      if (currentCh > 13) currentCh = 1;
      esp_wifi_set_channel(currentCh, WIFI_SECOND_CHAN_NONE);
      lastChannelSwitch = millis();
    }
  }
}