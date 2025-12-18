# ESP32 Spectrum Sniffer: Real-Time WiFi Traffic & Signal Analysis

A high-performance WiFi diagnostic tool designed specifically for the **ESP32 1.9" LCD (ST7789)**. This project transforms a standard development board into a portable, real-time packet sniffer and signal heat-map display.

---

## Executive Summary
In an increasingly saturated wireless landscape, identifying network congestion, interference, and unauthorised traffic requires specialised hardware. The **ESP32 Spectrum Sniffer** provides a low-cost, open-source alternative to expensive RF analysers.

By utilising the ESP32’s **promiscuous mode**, this device intercepts 802.11 management, data, and control frames without needing to authenticate or connect to an access point. It serves as an essential tool for:

* **Wireless Site Surveys:** Visualising signal strength (RSSI) across different channels to identify "dead zones" or optimal router placement.
* **Security Auditing:** Detecting the presence of hidden devices, rogue access points, or unusual traffic patterns in real-time.
* **Educational Research:** Providing a window into the invisible world of WiFi, demonstrating how channel hopping and packet types (Beacons, Data, and Control frames) function in a live environment.

---

## Technical Features
* **Multi-Coloured Waterfall Display:** A real-time "heat-map" where packet colours transition based on signal strength (RSSI), allowing for instant visual identification of nearby vs. distant devices.
* **Automatic Channel Hopping:** Cycles through the 2.4GHz spectrum (Channels 1–13) to capture a comprehensive snapshot of local wireless activity.
* **Glitch-Free UI:** Implements a software semaphore (drawing flag logic) to prevent screen tearing and SPI bus collisions, ensuring a smooth visual experience even during high traffic.
* **Packet Classification:** Visually distinguishes between Management (Beacons/Probes) and Data packets via colour-coded text.
* **Integrated Signal Bar:** A dynamic signal strength indicator accompanying every captured packet for precise proximity tracking.

---

## Hardware Configuration
This project is optimised for the **DIYMORE / Lilygo 1.9" LCD ESP32** module.

### Arduino IDE Settings:
To compile this project correctly, ensure your environment is configured as follows:
* **Board:** `ESP32 Dev Module`
* **Flash Mode:** `QIO`
* **Flash Frequency:** `80MHz`
* **Core Debug Level:** `None`



---

## Installation & Setup

### 1. Library Dependencies
Ensure you have the following library installed via the Arduino Library Manager:
* **TFT_eSPI** by Bodmer

### 2. Pin Mapping
This board uses non-standard SPI pins. Your `User_Setup.h` within the TFT_eSPI library folder must match these hardware definitions:
* **TFT_MOSI:** 13
* **TFT_SCLK:** 14
* **TFT_CS:** 15
* **TFT_DC:** 2
* **TFT_RST:** 12
* **TFT_BL:** 21 (Required for physical backlight power)

### 3. Usage
Once the sketch is uploaded, the device will immediately begin sniffing.
* **Header Bar:** Displays the current WiFi channel and the total count of packets captured since the device was powered on.
* **Waterfall:** The most recent packets appear at the bottom and scroll upwards.
* **RSSI Colour Scale:**
    * **Magenta/Red:** Devices in very close proximity (Strongest signal).
    * **Yellow/Green:** Standard range signals.
    * **Blue:** Distant or faint signals.

---

## Disclaimer
This tool is intended for educational and diagnostic purposes only. Always respect local laws regarding wireless privacy. Do not use this device to intercept private data or interfere with wireless networks.
