# 🌧️ ESP SmartHub

This project showcases an IoT solution using the **ESP RainMaker** platform on an **ESP32** board. It integrates a **DHT11 temperature & humidity sensor** and a **controllable LED** via a virtual switch. The system supports **real-time sensor monitoring**, **remote LED control**, and **OTA updates** via the RainMaker ecosystem.

---

## 📦 Features

- 🌡️ Read temperature and humidity from a DHT11 sensor.
- 💡 Control an light switch remotely via the RainMaker app.
- ⚠️ Raise an alert if temperature exceeds 30°C.
- 🔁 Periodically update and report sensor data every 5 seconds.
- 🔄 Supports OTA firmware updates.
- 🔁 Wi-Fi and factory reset with a physical button.
- 📊 Insights enabled for device diagnostics.

---

## 🔧 Hardware Requirements

- ESP32 Dev Board
- DHT11 Sensor
- LED + Resistor
- Breadboard & Jumper Wires

---

## 📐 GPIO Configuration

| Purpose            | GPIO Pin   |
|--------------------|------------|
| LED/ Light Switch  | GPIO 10    |
| DHT11 Data         | GPIO 2     |

> You can modify these pin numbers in `app_driver.c` as needed.

---

## 🧠 Architecture Overview

ESP32 Device
│
├── DHT11 Sensor (GPIO 2)
│   ├── Temperature reading
│   └── Humidity reading
│
├── LED (GPIO 10)
│   └── Controlled by virtual switch via RainMaker
│
├── ESP RainMaker Node
│   ├── Temperature Sensor Device
│   ├── Humidity Sensor Device
│   └── LED Switch Device
