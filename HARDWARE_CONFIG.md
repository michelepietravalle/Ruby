# Ruby ESP32 Hardware Configuration

## Board: ESP32-WROOM-Ethernet
**Target IP Address:** 10.42.42.31

## Pin Configuration

### Ethernet (LAN8720)
- ETH_MDC_PIN: GPIO 23
- ETH_MDIO_PIN: GPIO 18
- ETH_CLK_MODE: GPIO 17 (output)
- ETH_POWER_PIN: Not connected (-1)

### I2S Audio Output
- I2S_SCK (Bit Clock): GPIO 27
- I2S_WS (Word Select): GPIO 25
- I2S_SD (Serial Data): GPIO 26

## Network Configuration
- Static IP: 10.42.42.31
- Gateway: 10.42.42.1
- Subnet: 255.255.255.0
- RTP-MIDI Port: 5004

## Audio Specifications
- Sample Rate: 44.1 kHz
- Bit Depth: 16-bit
- Channels: Stereo
- Max Polyphony: 8 voices
- Wave Table Size: 1024 samples

## Dependencies
- AppleMIDI library for RTP-MIDI support
- ESP32 I2S driver for audio output
- ESP32 Ethernet support

## Usage
1. Connect ESP32-WROOM-Ethernet to your network
2. Power on the device
3. Connect to RTP-MIDI on IP 10.42.42.31:5004
4. Send MIDI notes for polyphonic audio output
5. Connect I2S audio output to your amplifier/speaker system

## MIDI Features
- Polyphonic note playback (up to 8 simultaneous voices)
- Note On/Off velocity sensitivity
- Real-time audio synthesis
- Wave table-based synthesis with harmonics