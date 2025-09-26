/*
 * Ruby - ESP32 RTP-MIDI Polyphonic Audio System
 * For Rubens Tube MIDI Speaker
 * 
 * Hardware: ESP32-WROOM-Ethernet
 * IP Address: 10.42.42.31
 * Features: RTP-MIDI connectivity, Polyphonic audio synthesis
 */

#include <WiFi.h>
#include <ETH.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>
#include <AppleMIDI.h>
#include <math.h>

// Network Configuration
#define ETH_ADDR 1
#define ETH_POWER_PIN -1
#define ETH_MDC_PIN 23
#define ETH_MDIO_PIN 18
#define ETH_TYPE ETH_PHY_LAN8720
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT

// Static IP Configuration
IPAddress local_IP(10, 42, 42, 31);
IPAddress gateway(10, 42, 42, 1);
IPAddress subnet(255, 255, 255, 0);

// I2S Audio Configuration
#define I2S_WS 25
#define I2S_SD 26  
#define I2S_SCK 27
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024

// Polyphonic Audio Engine
#define MAX_VOICES 8
#define WAVE_TABLE_SIZE 1024

struct Voice {
  bool active;
  uint8_t note;
  uint8_t velocity;
  float frequency;
  float phase;
  float amplitude;
  uint32_t startTime;
};

Voice voices[MAX_VOICES];
int16_t waveTable[WAVE_TABLE_SIZE];
bool ethernetConnected = false;

// RTP-MIDI
APPLEMIDI_CREATE_INSTANCE(WiFiUDP, MIDI, "Ruby-ESP32", 5004);

void setup() {
  Serial.begin(115200);
  Serial.println("Ruby - ESP32 RTP-MIDI Polyphonic Audio System");
  Serial.println("Initializing...");
  
  // Initialize voices
  for (int i = 0; i < MAX_VOICES; i++) {
    voices[i].active = false;
    voices[i].phase = 0.0f;
  }
  
  // Generate sine wave table
  generateWaveTable();
  
  // Initialize Ethernet
  initEthernet();
  
  // Initialize I2S Audio
  initI2S();
  
  // Initialize RTP-MIDI
  initRTPMIDI();
  
  Serial.println("System initialized successfully!");
  Serial.print("IP Address: ");
  Serial.println(ETH.localIP());
}

void loop() {
  // Process RTP-MIDI
  MIDI.read();
  
  // Generate and output audio
  generateAudio();
  
  // Small delay to prevent watchdog issues
  delay(1);
}

void initEthernet() {
  Serial.println("Initializing Ethernet...");
  
  WiFi.onEvent(onEthernetEvent);
  
  // Configure static IP
  if (!ETH.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure Ethernet");
  }
  
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
}

void onEthernetEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      ETH.setHostname("ruby-esp32");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      ethernetConnected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      ethernetConnected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      ethernetConnected = false;
      break;
    default:
      break;
  }
}

void initI2S() {
  Serial.println("Initializing I2S...");
  
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = BUFFER_SIZE,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };
  
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_SD,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_start(I2S_NUM_0);
}

void initRTPMIDI() {
  Serial.println("Initializing RTP-MIDI...");
  
  // Wait for Ethernet connection
  while (!ethernetConnected) {
    delay(100);
  }
  
  MIDI.begin();
  
  // Set up MIDI callbacks
  MIDI.setHandleNoteOn(onNoteOn);
  MIDI.setHandleNoteOff(onNoteOff);
  MIDI.setHandleControlChange(onControlChange);
  
  Serial.println("RTP-MIDI initialized");
  Serial.println("Listening for RTP-MIDI connections on port 5004");
}

void generateWaveTable() {
  Serial.println("Generating wave table...");
  
  for (int i = 0; i < WAVE_TABLE_SIZE; i++) {
    float phase = (2.0f * PI * i) / WAVE_TABLE_SIZE;
    // Generate a sine wave with some harmonics for richer sound
    float sample = sin(phase) + 0.3f * sin(2.0f * phase) + 0.1f * sin(3.0f * phase);
    waveTable[i] = (int16_t)(sample * 16000.0f);
  }
}

void onNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("Note ON: Ch=%d, Note=%d, Vel=%d\n", channel, note, velocity);
  
  // Find available voice
  int voiceIndex = findAvailableVoice();
  if (voiceIndex >= 0) {
    voices[voiceIndex].active = true;
    voices[voiceIndex].note = note;
    voices[voiceIndex].velocity = velocity;
    voices[voiceIndex].frequency = noteToFrequency(note);
    voices[voiceIndex].phase = 0.0f;
    voices[voiceIndex].amplitude = velocity / 127.0f;
    voices[voiceIndex].startTime = millis();
  }
}

void onNoteOff(byte channel, byte note, byte velocity) {
  Serial.printf("Note OFF: Ch=%d, Note=%d, Vel=%d\n", channel, note, velocity);
  
  // Find and deactivate voice
  for (int i = 0; i < MAX_VOICES; i++) {
    if (voices[i].active && voices[i].note == note) {
      voices[i].active = false;
      break;
    }
  }
}

void onControlChange(byte channel, byte control, byte value) {
  Serial.printf("Control Change: Ch=%d, CC=%d, Val=%d\n", channel, control, value);
  // Handle control changes (volume, modulation, etc.)
}

int findAvailableVoice() {
  // First try to find an inactive voice
  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active) {
      return i;
    }
  }
  
  // If all voices are active, steal the oldest one
  int oldestVoice = 0;
  uint32_t oldestTime = voices[0].startTime;
  
  for (int i = 1; i < MAX_VOICES; i++) {
    if (voices[i].startTime < oldestTime) {
      oldestTime = voices[i].startTime;
      oldestVoice = i;
    }
  }
  
  return oldestVoice;
}

float noteToFrequency(byte note) {
  return 440.0f * pow(2.0f, (note - 69) / 12.0f);
}

void generateAudio() {
  static int16_t audioBuffer[BUFFER_SIZE * 2]; // Stereo
  
  for (int sample = 0; sample < BUFFER_SIZE; sample++) {
    int32_t mixedSample = 0;
    
    // Mix all active voices
    for (int voice = 0; voice < MAX_VOICES; voice++) {
      if (voices[voice].active) {
        // Calculate wave table index
        int tableIndex = (int)(voices[voice].phase * WAVE_TABLE_SIZE / (2.0f * PI)) % WAVE_TABLE_SIZE;
        
        // Get sample from wave table
        int16_t voiceSample = waveTable[tableIndex];
        
        // Apply amplitude
        voiceSample = (int16_t)(voiceSample * voices[voice].amplitude);
        
        // Add to mix
        mixedSample += voiceSample;
        
        // Update phase
        voices[voice].phase += (2.0f * PI * voices[voice].frequency) / SAMPLE_RATE;
        if (voices[voice].phase >= 2.0f * PI) {
          voices[voice].phase -= 2.0f * PI;
        }
      }
    }
    
    // Prevent clipping and scale down for polyphony
    mixedSample = mixedSample / (MAX_VOICES / 2);
    if (mixedSample > 32767) mixedSample = 32767;
    if (mixedSample < -32768) mixedSample = -32768;
    
    // Stereo output (same signal on both channels)
    audioBuffer[sample * 2] = (int16_t)mixedSample;      // Left
    audioBuffer[sample * 2 + 1] = (int16_t)mixedSample;  // Right
  }
  
  // Output to I2S
  size_t bytes_written;
  i2s_write(I2S_NUM_0, audioBuffer, sizeof(audioBuffer), &bytes_written, portMAX_DELAY);
}