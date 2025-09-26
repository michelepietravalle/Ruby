# Arduino Library Dependencies

## Required Libraries for Ruby ESP32 RTP-MIDI System

### Core ESP32 Libraries (Built-in)
- WiFi.h - For network connectivity
- ETH.h - For Ethernet support
- WiFiUdp.h - For UDP communication
- driver/i2s.h - For I2S audio output

### External Libraries (Install via Arduino Library Manager)

#### AppleMIDI Library
- **Name**: AppleMIDI
- **Version**: >= 3.2.0
- **Author**: lathoub
- **URL**: https://github.com/lathoub/Arduino-AppleMIDI-Library
- **Purpose**: RTP-MIDI protocol implementation
- **Installation**: 
  ```
  Arduino IDE > Tools > Manage Libraries > Search "AppleMIDI" > Install
  ```

### Alternative Installation Methods

#### PlatformIO
Add to `platformio.ini`:
```ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    lathoub/AppleMIDI@^3.2.0
```

#### Manual Installation
1. Download AppleMIDI library from GitHub
2. Extract to Arduino/libraries/ folder
3. Restart Arduino IDE

## Board Configuration
- **Board**: ESP32 Dev Module (or ESP32-WROOM-32)
- **Upload Speed**: 921600
- **CPU Frequency**: 240MHz (WiFi/BT)
- **Flash Frequency**: 80MHz  
- **Flash Mode**: QIO
- **Flash Size**: 4MB (32Mb)
- **Partition Scheme**: Default 4MB with spiffs
- **Core Debug Level**: None
- **PSRAM**: Disabled

## Compilation Notes
- Ensure ESP32 board package is installed (>= 2.0.0)
- AppleMIDI library must be installed for RTP-MIDI functionality
- Code uses standard ESP32 I2S drivers (no additional audio libraries required)