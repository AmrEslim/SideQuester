# LoRa E22-900M30S Wiring Guide

## Overview

This guide provides detailed wiring instructions for connecting the E22-900M30S LoRa module to an ESP32 development board for the SideQuester project. The E22-900M30S is a high-performance LoRa module based on the Semtech SX1262 chipset with integrated TCXO for improved frequency stability.

## Module Specifications

### E22-900M30S Key Features
- **Chipset**: Semtech SX1262 with TCXO
- **Frequency Range**: 850-930MHz (use 915MHz for US, 868MHz for EU)
- **Interface**: UART (TTL level)
- **Transmission Power**: Up to 30dBm (1W)
- **Sensitivity**: -148dBm @ SF12, BW125kHz
- **Range**: Up to 3km (line of sight)
- **Operating Voltage**: 2.3V - 5.2V
- **Current Consumption**: 
  - Transmit: ~900mA @ 30dBm
  - Receive: ~15mA
  - Sleep: ~2µA

### Pin Configuration

| Pin | Function | Description |
|-----|----------|-------------|
| VCC | Power Supply | 3.3V or 5V (recommend 3.3V for ESP32) |
| GND | Ground | Common ground |
| RXD | UART Receive | Data input to module (from ESP32 TX) |
| TXD | UART Transmit | Data output from module (to ESP32 RX) |
| AUX | Auxiliary | Status indicator (HIGH when ready) |
| M0 | Mode Control 0 | Operating mode selection |
| M1 | Mode Control 1 | Operating mode selection |

## ESP32 Connection Diagram

```
ESP32 Development Board                    E22-900M30S LoRa Module
┌─────────────────────────┐               ┌─────────────────────────┐
│                         │               │                         │
│  3.3V ●─────────────────┼───────────────┼─● VCC                   │
│                         │               │                         │
│  GND  ●─────────────────┼───────────────┼─● GND                   │
│                         │               │                         │
│ GPIO16●─────────────────┼───────────────┼─● RXD (Module RX)       │
│                         │               │                         │
│ GPIO17●─────────────────┼───────────────┼─● TXD (Module TX)       │
│                         │               │                         │
│ GPIO18●─────────────────┼───────────────┼─● AUX (Status)          │
│                         │               │                         │
│ GPIO4 ●─────────────────┼───────────────┼─● M0 (Mode Control)     │
│                         │               │                         │
│ GPIO5 ●─────────────────┼───────────────┼─● M1 (Mode Control)     │
│                         │               │                         │
└─────────────────────────┘               └─────────────────────────┘
```

## Detailed Wiring Instructions

### Step 1: Power Connections

⚠️ **Important**: Always connect power last and disconnect first to prevent damage.

1. **VCC (Power Supply)**
   - Connect E22-900M30S VCC pin to ESP32 3.3V pin
   - **Note**: While the module can accept 5V, using 3.3V is recommended for ESP32 compatibility
   - Ensure your power supply can provide sufficient current (up to 1A during transmission)

2. **GND (Ground)**
   - Connect E22-900M30S GND pin to ESP32 GND pin
   - Ensure solid ground connection for reliable operation

### Step 2: UART Communication

3. **RXD (Module Receive)**
   - Connect E22-900M30S RXD pin to ESP32 GPIO16
   - This pin receives data FROM the ESP32 TO the LoRa module
   - GPIO16 will be configured as TX in SoftwareSerial

4. **TXD (Module Transmit)**
   - Connect E22-900M30S TXD pin to ESP32 GPIO17
   - This pin sends data FROM the LoRa module TO the ESP32
   - GPIO17 will be configured as RX in SoftwareSerial

### Step 3: Control Pins

5. **AUX (Auxiliary Status)**
   - Connect E22-900M30S AUX pin to ESP32 GPIO18
   - This pin indicates module status (HIGH = ready, LOW = busy)
   - Use this pin to ensure module is ready before sending commands

6. **M0 (Mode Control 0)**
   - Connect E22-900M30S M0 pin to ESP32 GPIO4
   - Used to control operating mode (see Operating Modes section)

7. **M1 (Mode Control 1)**
   - Connect E22-900M30S M1 pin to ESP32 GPIO5
   - Used to control operating mode (see Operating Modes section)

## Operating Modes

The E22-900M30S has three primary operating modes controlled by M0 and M1 pins:

| M1 | M0 | Mode | Description |
|----|----|----- |-------------|
| 0  | 0  | **Transparent Transmission** | Normal operation mode for data transmission |
| 0  | 1  | WOR (Wake on Radio) | Low power mode with wake-up capability |
| 1  | 1  | **Configuration Mode** | For setting module parameters |

### Mode Details

#### Transparent Transmission Mode (M0=0, M1=0)
- **Use**: Normal data transmission and reception
- **UART**: Full duplex communication
- **Power**: Normal consumption
- **Configuration**: This is the mode used for regular LoRa communication

#### WOR Mode (M0=1, M1=0)
- **Use**: Power-saving mode with periodic wake-up
- **UART**: Limited functionality
- **Power**: Reduced consumption
- **Configuration**: Used for battery-powered applications

#### Configuration Mode (M0=1, M1=1)
- **Use**: Setting module parameters (frequency, power, etc.)
- **UART**: Command interface only
- **Power**: Normal consumption
- **Configuration**: Required for initial setup

## Physical Assembly Tips

### 1. Breadboard Layout
```
    ESP32 Dev Board
    ┌─────────────────┐
    │ ●●●●●●●●●●●●●●● │
    │                 │
    │ ●●●●●●●●●●●●●●● │
    └─────────────────┘
           │
    ┌─────────────────┐
    │   Breadboard    │
    │ ●●●●●●●●●●●●●●● │
    │ ●●●●●●●●●●●●●●● │
    │ ●●●●●●●●●●●●●●● │
    │ ●●●●●●●●●●●●●●● │
    └─────────────────┘
           │
    ┌─────────────────┐
    │  E22-900M30S    │
    │ ●●●●●●●●●●●●●●● │
    └─────────────────┘
```

### 2. Wire Management
- Use different colored wires for easy identification:
  - **Red**: VCC (3.3V)
  - **Black**: GND
  - **Yellow**: Data lines (RXD, TXD)
  - **Blue**: Control lines (AUX, M0, M1)
- Keep wires as short as possible to minimize interference
- Avoid running power and signal wires parallel to each other

### 3. Antenna Connection
- Ensure proper antenna connection to the E22-900M30S
- Use appropriate antenna for 915MHz (US) or 868MHz (EU)
- Keep antenna away from other electronics to minimize interference
- For testing, a simple wire antenna can be used (1/4 wavelength ≈ 8.2cm for 915MHz)

## Power Supply Considerations

### Current Requirements
- **Idle/Receive**: ~15mA
- **Transmit (22dBm)**: ~120mA
- **Transmit (30dBm)**: ~900mA

### Power Supply Recommendations
1. **USB Power**: Adequate for testing and development
2. **Battery Operation**: Use Li-ion battery with adequate capacity
3. **Power Management**: Consider adding capacitors for peak current handling

### Voltage Levels
- **ESP32 Logic**: 3.3V TTL
- **E22-900M30S**: 3.3V compatible
- **No level shifters required** when using 3.3V supply

## Troubleshooting Common Wiring Issues

### Problem: Module not responding
**Solutions:**
1. Check power connections (VCC and GND)
2. Verify UART cross-connection (ESP32 TX → Module RXD, ESP32 RX → Module TXD)
3. Ensure AUX pin is connected and monitored
4. Check mode pins (M0, M1) are properly connected

### Problem: No data transmission
**Solutions:**
1. Verify both devices are in transparent mode (M0=0, M1=0)
2. Check antenna connections
3. Ensure both modules use same frequency and channel
4. Verify UART baud rate matches (default 9600)

### Problem: Intermittent communication
**Solutions:**
1. Check for loose connections
2. Verify adequate power supply
3. Add decoupling capacitors near power pins
4. Check for electromagnetic interference

### Problem: Short range
**Solutions:**
1. Verify antenna connection and orientation
2. Check transmission power settings
3. Ensure line-of-sight communication
4. Check for obstacles or interference sources

## Safety Considerations

### Electrical Safety
- Always connect power last and disconnect first
- Use appropriate current ratings for all connections
- Avoid short circuits between VCC and GND

### RF Safety
- The E22-900M30S can transmit up to 1W (30dBm)
- Maintain safe distance during operation
- Use proper antenna to prevent near-field exposure
- Follow local regulations for transmission power limits

### Regulatory Compliance
- **US**: Use 915MHz frequency band only
- **EU**: Use 868MHz frequency band only
- **Other regions**: Check local LoRa frequency regulations
- Ensure transmission power complies with local limits

## Testing Your Connection

After completing the wiring, use the provided test sketches to verify your connection:

1. **Upload transmitter sketch** to one ESP32
2. **Upload receiver sketch** to another ESP32
3. **Power on both devices**
4. **Monitor Serial output** for successful communication
5. **Verify LED indicators** on both devices

Expected output on receiver:
```
[00:00:15] RX [1] Hello from Device A - Count: 1 | RSSI: -85 dBm
[00:00:17] RX [2] Hello from Device A - Count: 2 | RSSI: -83 dBm
```

## Next Steps

Once your wiring is complete and tested:
1. Review the [LoRa Test README](../README.md) for configuration options
2. Experiment with different transmission powers and channels
3. Test communication range in your environment
4. Integrate with the main SideQuester project code

## Additional Resources

- [E22-900M30S Datasheet](https://www.ebyte.com/en/pdf-down.html?id=437)
- [SX1262 Technical Reference](https://www.semtech.com/products/wireless-rf/lora-core/sx1262)
- [SideQuester Project Documentation](../../README.md)

---

**Need help?** Check the [troubleshooting section](../../README.md#troubleshooting) or open an issue on the [GitHub repository](https://github.com/AmrEslim/SideQuester/issues).