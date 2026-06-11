Digital-Communication-Arduino-FSO-Project

Time-synced, LSB-reconstructed, and hardware-encrypted. We didn't just transfer data; we mastered the optical spectrum.

-- THE ARCHITECTURE --

• Cryptographic Handshake & Temporal Header Sync
Before the payload is sent, the transmitter pushes a 'Sync-Header' containing the Temporal Encryption Key and calculated Time-to-Live (TTL). The receiver calibrates its decryption buffer to these exact parameters. No handshake? No data.

• LSB-First Bitwise Reconstruction Algorithm
We aren't just reading light; we are building data byte-by-byte. Using the custom logic data |= (1less than i);, the receiver captures the photon stream and reconstructs 8-bit packets from the Least Significant Bit upwards during precise 120ms sampling windows.

• High-Intensity Green Spectrum (532nm) Isolation
Sunlight interference? Eliminated. By fine-tuning the dynamic TTL-Level emulation (Threshold calibration), the system ignores ambient solar noise and responds only to the high-power 532nm coherent green photons.

• Pre-emptive Signal Interruption Detection
If the optical link is physically severed before the handshake time-window expires, the Premature Termination Protocol triggers instantly. It prevents corrupted characters and flashes a "SIGNAL LOST" status, ensuring absolute data integrity.

• Universal ASCII Mapping
Fully supports Extended Binary Character Mapping. From Alpha-Numerics (Aa-Zz, 0-9) to complex operational symbols (@#$%^&*?!), every photon pulse is accurately decoded.

• Autonomous Power-Save & Memory Verification
Features an OLED/LCD Backlight Hibernation protocol and a Pseudo-Random Glitch Animation (glitchTitle()) to verify memory integrity during the boot sequence.




🛠️ HARDWARE SETUP:

Microcontrollers: 1x Arduino UNO & 1x ESP32

Optical Transmitter: 532nm High-Power Green Laser Diode Module

Optical Receiver: Precision LDR (Light Dependent Resistor)

Display UI: 16x2 I2C LCD Displays

Protocol: Custom Asynchronous Serial over Light (REGITE Protocol)
