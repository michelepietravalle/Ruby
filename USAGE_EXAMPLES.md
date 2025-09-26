# Usage Examples

## Testing the Ruby ESP32 RTP-MIDI System

### Basic Connection Test

1. **Upload the sketch** to your ESP32-WROOM-Ethernet
2. **Open Serial Monitor** (115200 baud) to see initialization
3. **Check network connection**:
   ```
   ETH Started
   ETH Connected
   ETH MAC: XX:XX:XX:XX:XX:XX, IPv4: 10.42.42.31, 100Mbps
   RTP-MIDI initialized
   Listening for RTP-MIDI connections on port 5004
   ```

### RTP-MIDI Connection from DAW

#### Logic Pro X (macOS)
1. Open Logic Pro X
2. Go to **Project Settings > MIDI > Input**
3. Enable **Network** in the MIDI Input list
4. The device should appear as "Ruby-ESP32"
5. Create a software instrument track
6. Set output to **Ruby-ESP32**

#### Reaper (Windows/macOS/Linux)
1. Install **rtpMIDI** driver (Windows) or use built-in network MIDI (macOS/Linux)
2. Configure network MIDI to connect to 10.42.42.31:5004
3. In Reaper, add the network MIDI device as input/output
4. Create a track with MIDI output to Ruby-ESP32

#### Using rtpMIDI Software (Windows)
1. Download and install **rtpMIDI** from Tobias Erichsen
2. Start rtpMIDI
3. Add session: Name="Ruby", Port=5004
4. Connect to 10.42.42.31
5. Use any MIDI software with "rtpMIDI Ruby" as output device

### Testing with MIDI Controllers

#### Hardware MIDI Controller
- Connect MIDI controller to computer
- Use MIDI routing software to send to Ruby-ESP32:
  - **Windows**: rtpMIDI + MIDI-OX
  - **macOS**: Built-in Audio MIDI Setup
  - **Linux**: QjackCtl + a2jmidid

#### Virtual MIDI Keyboard
- Use software like **VMPK** (Virtual MIDI Piano Keyboard)
- Set output to Ruby-ESP32 network device
- Play notes to test polyphonic capabilities

### Audio Output Testing

#### Expected Behavior
- **Single notes**: Clean sine wave with harmonics
- **Chords**: Mixed polyphonic output (up to 8 voices)
- **Velocity**: Affects volume (0-127 MIDI velocity range)
- **Note Off**: Immediately stops the corresponding voice

#### Connection to Amplifier
1. Connect I2S output (GPIO 26) to audio DAC input
2. Connect DAC output to amplifier input
3. For Rubens tube: Use high-power audio amplifier
4. Monitor audio levels to prevent speaker damage

### Example MIDI Sequences

#### Simple C Major Chord
```
Note On:  C4 (60), velocity 100
Note On:  E4 (64), velocity 100  
Note On:  G4 (67), velocity 100
[Hold for 2 seconds]
Note Off: C4 (60)
Note Off: E4 (64)
Note Off: G4 (67)
```

#### Polyphony Test (8-voice)
Play these notes simultaneously:
- C4, D4, E4, F4, G4, A4, B4, C5
- Should play all 8 voices
- Adding 9th note should steal oldest voice

### Troubleshooting Test Results

#### No Audio
- Check Serial Monitor for "Note ON" messages
- Verify I2S connections (GPIO 25, 26, 27)
- Test with headphones/different amplifier

#### MIDI Not Received
- Check "RTP-MIDI initialized" in Serial Monitor
- Verify network connectivity: `ping 10.42.42.31`
- Check MIDI routing in host software

#### Distorted Audio
- Reduce simultaneous note count
- Check power supply stability
- Lower output volume in host software

## Advanced Testing

### Network Latency Test
- Use DAW with MIDI timing display
- Compare Ruby latency vs hardware MIDI devices
- Typical network MIDI latency: 1-5ms

### Polyphony Stress Test
- Play rapid arpeggios or dense chords
- Monitor voice allocation in Serial output
- Test voice stealing behavior with >8 simultaneous notes

### Long-term Stability Test
- Run continuous MIDI playback for several hours
- Monitor for memory leaks or audio dropouts
- Check for network disconnection recovery