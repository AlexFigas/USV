# USV – Unmanned Surface Vehicle

This repository contains the code and documentation of the **USV (Unmanned Surface Vehicle)** prototype developed as part of the Master’s Thesis in Computer and Multimedia Engineering (ISEL).  
The goal of the project was to implement a **modular, scalable, and low-cost cyber-physical system**, capable of navigating autonomously in maritime environments.

---

## 🚤 Overview

The system was designed to support up to **four independent thrusters**, integrating multiple sensors and communication interfaces. The modular architecture ensures that new sensors, actuators, or algorithms can be added without compromising the existing structure.

Main features:
- **Autonomous navigation based on GPS waypoints**.  
- **Manual mode** with remote control via LoRa.  
- **Sensor integration** (IMU and GPS) with data fusion for drift correction.  
- **Long-range communication** using structured messages with [Protobuf](https://protobuf.dev/).  
- **Modular software architecture**, inspired by the [Didactic-Robot](https://github.com/AlexFigas/Didactic-Robot-Project) project.  

---

## 🛠️ Hardware

- **Microcontroller:** TTGO LoRa32 (ESP32 + LoRa SX1276 transceiver + OLED).  
- **Propulsion:** 2x brushless U01 motors with dedicated ESCs.  
- **Sensors:**  
  - IMU ICM-20948 (9 DOF).  
  - GPS BN-880.  
- **Custom PCB** integrating controllers, sensors, and power management.  
- **Communication:** LoRa (868 MHz, SF12).  
- **Batteries:** 12V dedicated for the thrusters.  

---

## 💻 Software

- **Development environment:** [PlatformIO](https://platformio.org/) (VS Code).  
- **Framework:** Arduino.  
- **Main libraries:**  
  - [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) (GPS).  
  - [LoRa](https://github.com/sandeepmistry/arduino-LoRa) (communication).  
  - [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) (OLED display).  

---

## 📊 Structure

- **Incremental implementation**: individual testing of each module (thrusters, GPS, IMU, LoRa, OLED).  
- **Final validation**: carried out during the international exercise [REPMUS 25](https://www.iddportugal.pt/agenda/exercicio-repmus25/), in cooperation with the Portuguese Navy, Naval Academy, ISEL, and ENIDH.  

---

## 📹 Demonstration

A demonstration video of the USV in operation (automatic and manual modes) is available on YouTube:  
👉 [Watch here](https://www.youtube.com/watch?v=LxEEflRIIYM)

---

## 📄 License

This project is distributed under the **GNU General Public License v3.0 (GPL-3.0)**.  
See the [LICENSE](./LICENSE) file for more details.  

---

## 🙌 Acknowledgments

- **ISEL** – for academic support throughout the entire program.  
- **ENIDH** – for providing facilities, the vessel, and collaboration in testing.  
- **Portuguese Navy and Naval Academy** – for the opportunity to validate the system during REPMUS 25 / REX 25.  
- **Supervisors (Prof. Carlos Gonçalves and Prof. Pedro Teodoro)** – for their technical and scientific guidance.  
- **Critical Techworks** – for the support as employer during the Master’s program.  
- **Family, friends, and colleagues** – for their understanding and motivation along the journey.  

---
