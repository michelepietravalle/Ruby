# ESP32-WROOM-Ethernet Wiring Diagram

## ESP32 Pin Connections

```
ESP32-WROOM-32           LAN8720 Ethernet PHY
GPIO 23        ------    MDC
GPIO 18        ------    MDIO  
GPIO 17        ------    CLK
GPIO 19        ------    TXD0
GPIO 21        ------    TX_EN
GPIO 22        ------    TXD1
GPIO 25        ------    RXD0
GPIO 26        ------    RXD1
GPIO 27        ------    CRS_DV
GND            ------    GND
3V3            ------    VCC
```

## I2S Audio Output

```
ESP32 Pin              Audio Device
GPIO 27 (SCK)    ---   Bit Clock (BCLK)
GPIO 25 (WS)     ---   Word Select (LRCLK) 
GPIO 26 (SD)     ---   Serial Data (DIN)
GND              ---   Ground
3V3              ---   VCC (if needed)
```

## Power Supply
- ESP32: 3.3V (via USB or external 5V with onboard regulator)
- LAN8720: 3.3V (shared with ESP32)
- Audio amplifier: Typically 5V or 12V (separate supply)

## Network Connection
Connect Ethernet cable to LAN8720 RJ45 jack.
Device will automatically configure to IP 10.42.42.31

## Audio Output Connection
Connect I2S output to:
- I2S DAC (PCM5102A, MAX98357A, etc.)
- Digital amplifier with I2S input
- Audio interface with I2S input

For Rubens tube applications, connect to a powerful audio amplifier
capable of driving the tube's speaker system with sufficient power
for flame modulation effects.