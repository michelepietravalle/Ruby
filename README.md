# Ruby
ESP32 RTP-MIDI Polyphonic Audio System for Rubens Tube

## Overview
Ruby is an Arduino-based project that transforms an ESP32-WROOM-Ethernet board into a polyphonic MIDI audio synthesizer specifically designed for Rubens tube applications. The system receives MIDI data via RTP-MIDI over Ethernet and generates real-time polyphonic audio output.

## Features
- **RTP-MIDI Connectivity**: Receives MIDI data over Ethernet network
- **Static IP**: Configured for 10.42.42.31
- **Polyphonic Audio**: Up to 8 simultaneous voices
- **Real-time Synthesis**: Wave table-based audio generation at 44.1kHz
- **I2S Output**: High-quality 16-bit stereo audio output
- **Hardware**: ESP32-WROOM-Ethernet with LAN8720 PHY

## Quick Start
1. Flash `Ruby.ino` to your ESP32-WROOM-Ethernet board
2. Connect to network (device will use IP 10.42.42.31)
3. Connect I2S audio output to your amplifier
4. Connect via RTP-MIDI from your DAW or MIDI controller
5. Play MIDI notes to generate polyphonic audio

## Hardware Configuration
See `HARDWARE_CONFIG.md` for detailed pin assignments and setup instructions.

## Target Application
Designed specifically for Rubens tube flame visualization systems where synchronized audio and visual effects are critical.
