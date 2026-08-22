# Dual-MCU IoT Smart Greenhouse System

A modular, robust, dual-microcontroller embedded solution for automated environmental monitoring and micro-climate control in smart agriculture applications. The system offloads real-time sensor processing and localized safety feedback loops to a primary **ATmega328P** controller while delegating cloud connectivity, telemetry streaming, and remote overrides to an **ESP8266** Wi-Fi node using MQTT.

---

## 📸 System Architecture

The architecture separates execution responsibilities into a **local control tier** and a **telemetry/cloud tier** to ensure continuous greenhouse operation even during network dropouts or cloud downtime.

```
                  +-----------------------------------+
                  |        Cloud Services             |
                  |     (HiveMQ Cloud MQTT)           |
                  +-----------------------------------+
                                    ^
                                    | TLS / MQTT
                                    v
+-----------------------------------------------------------------------+
|                            ESP8266 Node                               |
|  - WiFi Management & Auto-Reconnect                                  |
|  - MQTT Telemetry Publisher (JSON Packets)                            |
|  - MQTT Command Callback Listener                                     |
+-----------------------------------------------------------------------+
                                    ^
                                    | UART / Serial Bridge
                                    v
+-----------------------------------------------------------------------+
|                         ATmega328P Controller                         |
|  - Real-Time Sensor Data Acquisition (DHT11, Analog Soil Moisture)    |
|  - 4-Digit 7-Segment Display Driver (TM1637)                           |
|  - Automated Fan Relay Control                                        |
|  - Timed Irrigation Actuation (1s Pulse / 10min Cooldown Safety Guard)|
+-----------------------------------------------------------------------+
        |                  |                 |                  |
        v                  v                 v                  v
    [ DHT11 ]     [ Soil Moisture ]   [ TM1637 ]      [ Actuators ]
  Temp & Humidity    Capacitive/Resist    Display     (Fan Relay & Pump)
```

---

## 🛠 Features

* **Dual-MCU Separation of Concerns:**
  * **ATmega328P:** Handles low-latency sensor sampling, local display refresh rates, and hard safety timers for irrigation and ventilation.
  * **ESP8266:** Handles network connectivity, automatic Wi-Fi/MQTT reconnection loops, and bi-directional payload transfer.
* **Automated Environmental Control:**
  * Dynamic actuation of 5V ventilation fan using MOSFET in switch mode based on ambient temperature and relative humidity thresholds.
  * Timed irrigation 12V pump actuation (1-second watering pulse with a mandatory 10-minute non-blocking software cooldown guard to prevent soil over-saturation and motor burn-out).
* **Local Visual Status:** Real-time metrics output on a TM1637 4-digit 7-segment display.
* **Cloud Telemetry & Remote Override:** Transmits telemetry payloads to **HiveMQ Cloud** and Listens for remote manual control commands via subscribed MQTT topics.

---

## 🔌 Hardware Pinout & Wiring

| Component | Microcontroller | Pin / Interface | Description |
| :--- | :--- | :--- | :--- |
| **DHT11 Sensor** | ATmega328P | Digital Pin `2` | Temperature & Relative Humidity Data |
| **Soil Moisture Sensor** | ATmega328P | Analog Pin `A0` | Soil Moisture Analog Level |
| **TM1637 Display (CLK)** | ATmega328P | Digital Pin `6` | Clock line for 4-digit display |
| **TM1637 Display (DIO)** | ATmega328P | Digital Pin `5` | Data input/output line for display |
| **Ventilation Fan Relay**| ATmega328P | Digital Pin `8` | MOSFET / Relay control for exhaust fan |
| **Water Pump Driver** | ATmega328P | Digital Pin `9` | MOSFET driver for 12V DC water pump |
| **UART Bridge (TX $
ightarrow$ RX)**| ATmega328P $
ightarrow$ ESP8266 | `TX (D1)` $
ightarrow$ `RX (GPIO3)` | Telemetry string transmission |
| **UART Bridge (RX $\leftarrow$ TX)**| ATmega328P $\leftarrow$ ESP8266 | `RX (D0)` $\leftarrow$ `TX (GPIO1)` | Remote control command reception |

---

## 📁 Repository Structure

```text
IoT-Smart-Greenhouse/
├── ATMega628P/                  # ATmega328P Firmware (Arduino Modular Code)
│   ├── ATMega628P.ino           # Main setup & execution loop
│   ├── DHT.ino                  # Temperature & Humidity sensor reading logic
│   ├── Display.ino              # TM1637 display driver & update routines
│   ├── FanWork.ino              # Relay actuation & threshold logic for fan
│   ├── SoilMoisture.ino         # Analog soil moisture reading & calibration
│   └── Watering.ino             # Non-blocking pump pulse & cooldown control
└── ESP8266/                     # ESP8266 Wi-Fi & MQTT Firmware
    ├── ESP8266.ino              # Main setup & execution loop
    ├── WIFI_CONNECT.ino         # Wi-Fi station initialization & reconnection logic
    ├── TELEMETRY.ino            # Serial parsing & MQTT telemetry publishing
    ├── CALLBACK.ino             # Subscribed MQTT topic payload handling
    └── MQTT_RECONNECT.ino       # HiveMQ broker authentication & client keep-alive
```

---

## ⚙️ Setup & Deployment Instructions

### 1. Prerequisites & Dependencies

Ensure you have installed the **Arduino IDE** (or VS Code with PlatformIO/Arduino Extensions) along with the following libraries:

* `DHT sensor library` by Adafruit
* `Adafruit Unified Sensor`
* `TM1637Display` by Avishay Orpaz
* `PubSubClient` by Nick O'Leary (for ESP8266 MQTT)
* `ESP8266WiFi` (Included in ESP8266 core)

### 2. Sanitizing Credentials

1. Open `ESP8266/WIFI_CONNECT.ino` and replace placeholders with your local Wi-Fi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
2. Open `ESP8266/MQTT_RECONNECT.ino` and configure your HiveMQ Cloud endpoint and credentials:
   ```cpp
   const char* mqtt_server = "YOUR_HIVEMQ_ENDPOINT.hivemq.cloud";
   const int mqtt_port = 8883; // Secure TLS
   const char* mqtt_user = "YOUR_MQTT_USERNAME";
   const char* mqtt_pass = "YOUR_MQTT_PASSWORD";
   ```

### 3. Flashing Firmware

1. **ATmega328P:**
Connect your board via USB, select **Arduino Uno** (or ATmega328P board definition)
Set the dip switches 3 and 4 to on and all the others off
open `ATMega628P/ATMega628P.ino`, and click **Upload**.
2. **ESP8266:** 
Select **NodeMCU 1.0** or **Generic ESP8266 Module**, select the corresponding COM port
Set the dip switches 5 6 7 on and the others off and reset the board before uploading
Open `ESP8266/ESP8266.ino`, and click **Upload**.
Put the switch 7 to off 
3. **ESP8266:** 
set the switches 1 2 on and the others off 
reset the board


---

## 📡 MQTT Topic Specification

| Topic | Direction | Payload Format | Purpose |
| :--- | :--- | :--- | :--- |
| `greenhouse/telemetry` | Publish (ESP $
ightarrow$ Cloud) | `{"temp":24.5,"hum":60,"soil":45}` | Real-time sensor metrics |
| `greenhouse/control/fan` | Subscribe (Cloud $
ightarrow$ ESP) | `ON` / `OFF` | Manual override for fan relay |
| `greenhouse/control/pump`| Subscribe (Cloud $
ightarrow$ ESP) | `PULSE` | Trigger manual watering pulse |

---

## 📜 License

This project is open-source and released under the [MIT License](LICENSE).
