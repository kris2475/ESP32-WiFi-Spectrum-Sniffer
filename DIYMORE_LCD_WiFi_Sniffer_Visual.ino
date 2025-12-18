#include <TFT_eSPI.h>
#include <WiFi.h>
#include "esp_wifi.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite waterfall = TFT_eSprite(&tft);

int currentCh = 1;
unsigned long lastHop = 0;
bool drawing = false;

// Maps RSSI to a 16-bit color "Heat" scale
uint16_t heatMap(int rssi) {
  int val = map(rssi, -100, -30, 0, 255);
  if (val < 85) return tft.color565(0, 0, val * 3);          // Deep Blue
  if (val < 170) return tft.color565(0, (val - 85) * 3, 255 - (val - 85) * 3); // Cyan/Green
  return tft.color565((val - 170) * 3, 255 - (val - 170) * 3, 0); // Yellow/Red
}

void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (drawing) return;
  drawing = true;

  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  int rssi = pkt->rx_ctrl.rssi;
  
  // Calculate horizontal position based on Channel (mapped across 320 pixels)
  int xPos = map(currentCh, 1, 13, 20, 300);
  uint16_t pColor = heatMap(rssi);

  // Scroll the waterfall down
  waterfall.scroll(0, 1); 
  
  // Draw the packet "blip"
  waterfall.fillSmoothRoundRect(xPos - 10, 0, 20, 12, 4, pColor);
  
  // Add a letter indicator for type
  waterfall.setTextColor(TFT_BLACK);
  waterfall.setTextSize(1);
  if (type == WIFI_PKT_MGMT) waterfall.drawString("M", xPos - 3, 2);
  else if (type == WIFI_PKT_DATA) waterfall.drawString("D", xPos - 3, 2);

  waterfall.pushSprite(0, 40);
  drawing = false;
}

void setup() {
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH); // Backlight

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Static UI: Channel Markers
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  for (int i = 1; i <= 13; i++) {
    int x = map(i, 1, 13, 20, 300);
    tft.drawFastVLine(x, 35, 5, TFT_DARKGREY);
    tft.drawNumber(i, x - 3, 25);
  }

  waterfall.createSprite(320, 130);
  waterfall.fillSprite(TFT_BLACK);
  
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
  esp_wifi_set_channel(currentCh, WIFI_SECOND_CHAN_NONE);
}

void loop() {
  if (millis() - lastHop > 200) { // Fast hopping for a "live" feel
    currentCh++;
    if (currentCh > 13) currentCh = 1;
    esp_wifi_set_channel(currentCh, WIFI_SECOND_CHAN_NONE);
    lastHop = millis();
    
    // Highlight current scanning channel
    tft.fillRect(0, 0, 320, 20, TFT_BLACK);
    tft.setCursor(10, 5);
    tft.printf("SCANNING CHANNEL: %d", currentCh);
  }
}