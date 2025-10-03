<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <img src="https://raw.githubusercontent.com/AlexFigas/USV-thesis/master/figuras/usv-2d.png" alt="Logo" height="120">

  <h3 align="center">USV – Unmanned Surface Vehicle</h3>

  <p align="center">
    Modular and scalable cyber-physical system for autonomous maritime navigation.  
    Developed as part of the Master’s Thesis in Computer and Multimedia Engineering (ISEL).  
    <br />
    <br />
    <a href="https://github.com/AlexFigas/USV/issues">Report Issue</a>
  </p>
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#hardware">Hardware</a></li>
    <li><a href="#software">Software</a></li>
    <li><a href="#structure">Structure</a></li>
    <li><a href="#demonstration">Demonstration</a></li>
    <li><a href="#report">Report</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ABOUT THE PROJECT -->
## About The Project

The **USV (Unmanned Surface Vehicle)** is a modular, scalable, and low-cost cyber-physical system designed to navigate autonomously in maritime environments.  
It supports up to **four independent thrusters**, integrates multiple sensors, and provides long-range communication.

Main features:
- **Autonomous navigation based on GPS waypoints**.  
- **Manual mode** with remote control via LoRa.  
- **Sensor integration** (IMU and GPS) with data fusion for drift correction.  
- **Long-range communication** with [Protobuf](https://protobuf.dev/).  
- **Modular software architecture**, inspired by the [Didactic-Robot](https://github.com/AlexFigas/Didactic-Robot-Project).  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

To get started with this project, you need an **Arduino development environment** compatible with [PlatformIO](https://platformio.org/) on VS Code, configured for ESP32-based boards.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Installation

1. Install VS Code and PlatformIO:  
   * https://code.visualstudio.com/  
   * https://platformio.org/  

2. Install the required libraries:  
   * [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)  
   * [LoRa](https://github.com/sandeepmistry/arduino-LoRa)  
   * [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)  

3. Clone the repository:  
   ```sh
   git clone https://github.com/your-username/usv-thesis.git
   ```

4. Open the project in PlatformIO and build/upload the firmware to the ESP32.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- HARDWARE -->
## Hardware

- **Microcontroller:** TTGO LoRa32 (ESP32 + LoRa SX1276 + OLED).  
- **Propulsion:** 2x brushless U01 motors with ESCs.  
- **Sensors:** ICM-20948 (IMU 9 DOF), BN-880 (GPS).  
- **Custom PCB** for integration and power management.  
- **Communication:** LoRa (868 MHz, SF12).  
- **Batteries:** 12V for propulsion.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- SOFTWARE -->
## Software

- **Environment:** PlatformIO (VS Code).  
- **Framework:** Arduino.  
- **Main libraries:** TinyGPSPlus, LoRa, Adafruit SSD1306.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- STRUCTURE -->
## Structure

- **Incremental implementation**: each module tested separately (thrusters, GPS, IMU, LoRa, OLED).  
- **Final validation**: performed during [REPMUS 25](https://www.iddportugal.pt/agenda/exercicio-repmus25/) with the Portuguese Navy, Naval Academy, ISEL, and ENIDH.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- DEMONSTRATION -->
## Demonstration

Video demonstration of the USV in both **automatic** and **manual** modes:  
👉 [Watch here](https://www.youtube.com/watch?v=LxEEflRIIYM)  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- REPORT -->
## Report

The full Master’s Thesis report is available in PDF.  

* [Download the report here](https://github.com/AlexFigas/USV-thesis/blob/master/TFM.pdf)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the **GNU General Public License v3.0 (GPL-3.0)**.  
See `LICENSE` for details.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Alexandre Figueiredo - [Linkedin](https://www.linkedin.com/in/alexfigas/) - [Email](mailto:alexfigas11@gmail.com)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

We would like to express our gratitude to all institutions and people who contributed to the success of this project:

- **ISEL** – academic support throughout the Master’s.  
- **ENIDH** – facilities, vessel, and collaboration during testing.  
- **Portuguese Navy and Naval Academy** – validation during REPMUS 25 / REX 25.  
- **Supervisors** – Prof. Carlos Gonçalves and Prof. Pedro Teodoro for technical and scientific guidance.  
- **Critical Techworks** – employer support during the Master’s program.  
- **Family, friends, and colleagues** – for continuous encouragement and motivation.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>
