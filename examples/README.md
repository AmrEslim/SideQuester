# SideQuester LoRa Test Examples

This directory contains test code to verify LoRa communication between ESP32-WROOM-32 devices using SX1262 modules before integrating GPS functionality.

## Hardware Configuration

### ESP32-WROOM-32 to SX1262 Wiring

```
SX1262 Module → ESP32-WROOM-32
NSS    → GPIO15 (Chip Select)
DIO1   → GPIO4  (Interrupt)
NRST   → GPIO19 (Reset)
BUSY   → GPIO18 (Busy status)
SCK    → GPIO14 (SPI Clock)
MISO   → GPIO12 (SPI MISO)
MOSI   → GPIO13 (SPI MOSI)
VCC    → 3.3V
GND    → GND
```

## Required Libraries

Install these libraries through Arduino IDE Library Manager:

- **RadioLib** by Jan Gromeš (for SX1262 communication)
- **SPI** (usually included with Arduino core)

## LoRa Configuration

Both test sketches use these parameters:

```cpp
Frequency: 915.0 MHz (US) / 868.0 MHz (EU)
Bandwidth: 125.0 kHz
Spreading Factor: 7
Coding Rate: 5
Output Power: 20 dBm
```

## Test Files

### 1. `lora_sender_test.ino`

**Purpose**: Transmits test messages every 2.5 seconds  
**Message Format**: `SideQuester_Test_#[counter]_[timestamp]`  
**Example**: `SideQuester_Test_#001_12345678`

**Features**:
- Automatic SX1262 initialization
- LoRa parameter configuration
- Message counter and timestamp
- Transmission status feedback
- Error handling for failed transmissions
- Serial output with transmission statistics

### 2. `lora_receiver_test.ino`

**Purpose**: Continuously listens for incoming LoRa messages  

**Features**:
- Interrupt-driven packet reception
- RSSI and SNR measurement
- Message validation (SideQuester format check)
- Reception statistics every 30 seconds
- Error handling for CRC errors
- Frequency error measurement

## Testing Protocol

### Setup

1. **Hardware Assembly**: Wire two ESP32 devices with SX1262 modules according to the pin configuration above
2. **Library Installation**: Install RadioLib library in Arduino IDE
3. **Regional Configuration**: 
   - US: Use 915.0 MHz (default)
   - EU: Change `FREQUENCY` to 868.0 MHz in both sketches

### Testing Steps

1. **Upload Sender Code**: Flash `lora_sender_test.ino` to first ESP32
2. **Upload Receiver Code**: Flash `lora_receiver_test.ino` to second ESP32
3. **Power Both Devices**: Connect via USB or battery power
4. **Monitor Serial Output**: 
   - Sender: 115200 baud, shows transmission attempts and status
   - Receiver: 115200 baud, shows received messages with signal quality
5. **Verify Communication**: Look for successful message transmission/reception
6. **Range Testing**: Move devices apart and monitor RSSI values

### Expected Output

**Sender Serial Output**:
```
=== SideQuester LoRa Sender Test ===
Initializing SX1262 module...
Initializing SX1262 ... SUCCESS!
Configuring LoRa parameters...
...
Sending message #1: SideQuester_Test_#1_15423
✓ Transmission successful!
  Data rate: 5468.75 bps
  Output power: 20 dBm
```

**Receiver Serial Output**:
```
=== SideQuester LoRa Receiver Test ===
...
===== MESSAGE RECEIVED =====
Packet #1
Message: SideQuester_Test_#1_15423
RSSI: -45.5 dBm
SNR: 9.25 dB
✓ Valid SideQuester test message format
```

## Success Criteria

- [ ] Sender successfully initializes SX1262 module
- [ ] Receiver successfully initializes SX1262 module  
- [ ] Messages transmitted every 2-3 seconds
- [ ] Messages received and displayed with RSSI/SNR
- [ ] Error handling for failed transmissions/receptions
- [ ] Serial output shows clear status information
- [ ] Range testing capability (RSSI monitoring)

## RSSI Range Guidelines

- **-30 to -50 dBm**: Excellent signal (close range)
- **-50 to -70 dBm**: Good signal (medium range)
- **-70 to -90 dBm**: Fair signal (long range)
- **-90 to -100 dBm**: Poor signal (maximum range)
- **Below -100 dBm**: Very poor/no signal

## Troubleshooting

### Common Issues

1. **Module Not Initializing**:
   - Check wiring connections
   - Verify power supply (3.3V)
   - Ensure SPI pins are correct

2. **No Messages Received**:
   - Verify both devices use same frequency
   - Check antenna connections
   - Ensure line of sight for initial testing

3. **Poor Signal Quality**:
   - Check antenna placement
   - Reduce distance between devices
   - Verify ground plane and connections

4. **Compilation Errors**:
   - Install RadioLib library
   - Select correct ESP32 board in Arduino IDE
   - Verify Arduino ESP32 core is installed

## Next Steps

Once LoRa communication is verified working:
1. Test at various distances to establish range
2. Document RSSI values for different distances
3. Proceed with GPS module integration
4. Implement the full SideQuester application