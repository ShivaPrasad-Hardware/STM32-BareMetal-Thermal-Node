# 🌡️ Industrial Thermal Node (Hybrid HAL & Bare-Metal)

A temperature monitoring and control system built on the ARM Cortex-M4 architecture. This project demonstrates pragmatic hardware-software co-design through a **hybrid architecture**: utilizing the STM32 HAL for standard communications, while engineering a strict, bare-metal driver from scratch for microsecond-accurate 1-Wire sensor telemetry.

## 🎯 Project Overview
This system continuously monitors environmental temperature. If the temperature exceeds a dynamically configurable threshold, it triggers an industrial 5V relay for cooling control. System telemetry is bridged to an ESP32 for IoT cloud dashboarding.

## 🧰 Hardware Architecture
*   **MCU:** STM32 Nucleo-F401RE (Main Controller), ESP32 Development Board (Network Co-processor)
*   **Sensor:** DS18B20 1-Wire Temperature Sensor
*   **Display:** 0.96" $I^2C$ OLED
*   **Control:** 5V Relay Module (High-current isolation)
*   **Diagnostics:** 24MHz 8-Channel USB Logic Analyzer

## 📂 Repository Structure
*   `/src` — Hybrid C source code (HAL standard initialization + Bare-metal 1-Wire driver).
*   `/inc` — Header files, macros, and function prototypes.
*   `/esp32_bridge` — C++ payload and UART bridging code for the ESP32.
*   `/docs` — Logic analyzer waveform captures, component datasheets, and wiring diagrams.

## ⚙️ Engineering Phases
This project is developed in isolated, modular phases before final integration:
1.  **Input Isolation:** Configuring GPIO via HAL for tactile inputs with software debouncing.
2.  **Visual Feedback:** Utilizing the STM32 HAL $I^2C$ peripheral to interface with the OLED display for real-time local telemetry.
3.  **Bare-Metal Sensor Driver:** Bypassing standard libraries to write a from-scratch 1-Wire protocol driver for the DS18B20, utilizing direct hardware timer manipulation for strict microsecond delays. Timing verified via logic analyzer.
4.  **Industrial Control:** HAL GPIO output logic mapped to threshold states to trigger the 5V relay.
5.  **Telemetry Bridge:** UART communication mapping between the STM32 and ESP32 to transmit payload data to a cloud dashboard.

## 🔬 Timing Verification
*(Upload a screenshot of your 1-Wire logic analyzer waveforms to the /docs folder and link it here later)*
> "Custom bare-metal 1-Wire driver timing verified down to the microsecond using an 8-channel logic analyzer to ensure strict data sheet compliance."

## 🚀 Getting Started
1.  Clone this repository to your local machine.
2.  Open the STM32 project files in **STM32CubeIDE**.
3.  Compile and flash the firmware to the Nucleo-F401RE via the onboard ST-LINK.
4.  Open the `/esp32_bridge` sketch in **Arduino IDE** and flash it to the ESP32 board.
5.  Wire peripherals according to the pinout definitions in `main.h`.

## 🛠️ Software & Tools
*   **IDE:** STM32CubeIDE, Arduino IDE
*   **Configuration:** STM32CubeMX
*   **Languages:** Embedded C (Hybrid HAL/Bare-Metal), C++
*   **Dashboards:** ThingSpeak (IoT Time-Series Telemetry)

## 📄 License
This project is licensed under the MIT License.
