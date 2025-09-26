# Troubleshooting Guide

## Common Issues and Solutions

### Network Issues

#### Device not getting IP 10.42.42.31
- Check Ethernet cable connection
- Verify network allows static IP 10.42.42.31
- Check if IP is already in use by another device
- Monitor Serial output for Ethernet connection status

#### Cannot connect via RTP-MIDI
- Ensure device shows "ETH Got IP" in Serial Monitor
- Verify RTP-MIDI client is connecting to 10.42.42.31:5004
- Check firewall settings on host computer
- Try pinging the device: `ping 10.42.42.31`

### Audio Issues

#### No audio output
- Verify I2S wiring (GPIO 25, 26, 27)
- Check audio amplifier/DAC connections
- Ensure MIDI notes are being received (check Serial Monitor)
- Test with simple MIDI file or keyboard

#### Audio quality issues
- Check sample rate compatibility (44.1kHz)
- Verify power supply stability (3.3V for ESP32)
- Reduce number of simultaneous voices if distortion occurs
- Check for electromagnetic interference

### MIDI Issues

#### MIDI notes not triggering
- Verify RTP-MIDI connection in Serial Monitor
- Check MIDI channel settings (device responds to all channels)
- Ensure MIDI velocity > 0
- Test with different MIDI controllers/software

#### Polyphony issues
- Maximum 8 simultaneous voices
- Oldest voices are automatically replaced when limit exceeded
- Check voice allocation in Serial Monitor debug output

### Hardware Issues

#### ESP32 not booting
- Check power supply (stable 3.3V or 5V input)
- Verify proper grounding
- Check for short circuits in wiring
- Try different ESP32 board if available

#### Ethernet not working
- Verify LAN8720 wiring (MDC, MDIO, CLK pins)
- Check 3.3V power to LAN8720
- Ensure proper grounding between ESP32 and LAN8720
- Test with known-good Ethernet cable

## Debug Information

### Serial Monitor Output
Monitor at 115200 baud for:
- Ethernet connection status
- IP address assignment
- RTP-MIDI connection status
- MIDI message reception
- Audio engine status

### LED Indicators
- ESP32 built-in LED: Should blink during operation
- LAN8720 LEDs: Link/Activity indicators

### Test Commands
```bash
# Test network connectivity
ping 10.42.42.31

# Test RTP-MIDI port
telnet 10.42.42.31 5004
```

## Getting Help
- Check Serial Monitor output for detailed error messages
- Verify all wiring connections match WIRING.md
- Ensure all required libraries are installed (see LIBRARIES.md)
- Test individual components (Ethernet, I2S, MIDI) separately