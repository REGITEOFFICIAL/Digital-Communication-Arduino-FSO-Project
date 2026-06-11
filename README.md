# Digital-Communication-Arduino-FSO-Project by Dennis Binoy

Time-synced, LSB-reconstructed, and hardware-encrypted. We didn't just transfer data; we mastered the optical spectrum.

### ─── THE ARCHITECTURE ───

* **Cryptographic Handshake & Temporal Header Sync:** Before the payload is sent, the transmitter pushes a 'Sync-Header' containing the Temporal Encryption Key and calculated Time-to-Live (TTL). The receiver calibrates its decryption buffer to these exact parameters. No handshake? No data.

* **LSB-First Bitwise Reconstruction Algorithm:** We aren't just reading light; we are building data byte-by-byte. Using the custom logic `data |= (1 << i);`, the receiver captures the photon stream and reconstructs 8-bit packets from the Least Significant Bit upwards during precise 120ms sampling windows.

* **High-Intensity Green Spectrum (532nm) Isolation:** Sunlight interference? Eliminated. By fine-tuning the dynamic TTL-Level emulation (Threshold calibration), the system ignores ambient solar noise and responds only to the high-power 532nm coherent green photons.

* **Pre-emptive Signal Interruption Detection:** If the optical link is physically severed before the handshake time-window expires, the Premature Termination Protocol triggers instantly. It prevents corrupted characters and flashes a "SIGNAL LOST" status, ensuring absolute data integrity.

* **Universal ASCII Mapping:** Fully supports Extended Binary Character Mapping. From Alpha-Numerics (Aa-Zz, 0-9) to complex operational symbols (@#$%^&*?!), every photon pulse is accurately decoded.

* **Autonomous Power-Save & Memory Verification:** Features an OLED/LCD Backlight Hibernation protocol and a Pseudo-Random Glitch Animation (`glitchTitle()`) to verify memory integrity during the boot sequence.

---

### 🛠️ HARDWARE SETUP

* **Microcontrollers:** 1x Arduino UNO & 1x ESP32
* **Optical Transmitter:** 532nm High-Power Green Laser Diode Module
* **Optical Receiver:** Precision LDR (Light Dependent Resistor)
* **Display UI:** 16x2 I2C LCD Displays
* **Protocol:** Custom Asynchronous Serial over Light (REGITE Protocol)


# 📡 Laser FSO (Free Space Optics) Communication System

An advanced Laser-based Free Space Optics (FSO) communication network using **Arduino UNO R3** as the Transmitter (TX) and **ESP32** as the Receiver (RX). The system translates serial data into Morse pulses over a 532nm laser link, decodes the optical photon stream in real-time, and provides interactive visual and audio feedback.

---

## 🚀 Key Features

* **Dual-Controller Architecture:** Powered by Arduino UNO (Transmitter) and ESP32 (Receiver).
* **Asynchronous Light Serial:** Custom byte-reconstruction logic sampling at precise `120ms` intervals.
* **Smart Noise Isolation:** Dynamic LDR thresholding to filter out environmental ambient sunlight.
* **Interactive UI/UX:** Real-time character decoding status on 16x2 I2C LCD screens with a custom boot-up glitch animation.
* **Audio Telemetry:** Integrated piezo buzzer/speaker on the TX side for acoustic feedback of transmitted signals.

---

## 🔌 Complete Hardware Connection Diagram

### 1. Transmitter (TX) — Arduino UNO R3 Setup
The Transmitter reads incoming text via Serial, converts characters into timed laser/audio pulses, and updates the display.

| Component | Component Pin | Arduino UNO Pin | Notes |
| :--- | :--- | :--- | :--- |
| **I2C LCD Display** | GND | GND | Power Ground |
| | VCC | 5V | Power Supply |
| | SDA | A4 | I2C Data Line |
| | SCL | A5 | I2C Clock Line |
| **Laser Diode Module** | Positive (+) | Digital Pin 8 (SPEAKER) | Controlled simultaneously with audio |
| **Piezo Speaker/Buzzer**| Positive (+) | Digital Pin 8 (SPEAKER) | 700Hz Audio frequency indicator |
| | Negative (-) | GND | Ground |

> 💡 **Note:** In the provided code, both the Speaker and the Laser module share **Digital Pin 8** to sync sound and light pulses perfectly.

---

### 2. Receiver (RX) — ESP32 Setup
The Receiver captures light fluctuations through a Photoresistor (LDR), processes the incoming bitstream, and decodes it back to ASCII string data.

| Component | Component Pin | ESP32 Pin | Notes |
| :--- | :--- | :--- | :--- |
| **I2C LCD Display** | GND | GND | Power Ground |
| | VCC | Vin (5V) / 3.3V | Check your LCD module voltage |
| | SDA | GPIO 21 | ESP32 Default I2C SDA |
| | SCL | GPIO 22 | ESP32 Default I2C SCL |
| **LDR (Light Dependent Resistor)** | Analog Out (AO) | GPIO 36 (A0) | Connected via ADC1_CH0 |
| | VCC | 3.3V / 5V | Power input |
| | GND | GND | Ground |

---

## ⚙️ Core System Architecture & Logic

### 1. Transmitter Operations (Arduino UNO)
* **Serial Input:** Listens to input strings through the hardware Serial buffer at `9600` baud.
* **Morse Table Conversion:** Characters are mapped to relative dots (`.`) and dashes (`-`) using a precise internal time unit (`120ms`).
* **Multi-Mode Routing:** 
  * If a single character is passed, it assumes **Receiver Debug Mode** and mirrors it directly to the local LCD display with a short success beep.
  * If a full word/sentence is passed, it flashes **"TRANSMITTING..."** and begins modulating Pin 8.

### 2. Receiver Operations (ESP32)
* **Idle State:** Constantly samples Analog Pin `A0`. If the intensity is below the `700` threshold, it stays in power-saving/idle loop.
* **Header Synchronization:** Once the light intensity crosses the `700` barrier for a stable duration (`>240ms`), the system triggers the decoding routine.
* **Bitwise Reconstruction:** Reads 8-bit frames sequentially using:
  ```cpp
  data |= (1 << i);
  ```
  It captures data from the **Least Significant Bit (LSB)** upwards during tight `120ms` sampling windows.
* **Timeout Protection:** If no light pulse transitions occur within `400ms` (`IDLE_TIMEOUT`), the system flags packet termination, closes the string, and displays the full text.

---

## 🛠️ Software Deployment Guide

1. Clone or download this repository to your system.
2. Ensure you have the `LiquidCrystal_I2C` and `Wire` libraries installed in your Arduino IDE.
3. **Upload TX Code:** Select **Arduino UNO** board and upload the Transmitter sketch.
4. **Upload RX Code:** Select **ESP32 Dev Module** board, confirm the pin mappings, and upload the Receiver sketch.
5. Open your Serial Monitor on the Transmitter port (`9600` Baud), type a message, and watch it transmit over light!

---

### 📡 Protocol Timings Summary
* **Base Bit Unit:** `120ms`
* **Dot Duration:** `120ms`
* **Dash Duration:** `360ms` (`unit * 3`)
* **LDR Sampling Threshold:** `>= 700` (Adjustable depending on room lighting)


---

### On YouTube :- https://youtu.be/xCA_Jc2crYo?si=7SA04mJBuKOCwYf5

---
