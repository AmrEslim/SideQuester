# LoRa Communication Test Guide

This directory contains complete test sketches for the E22-900M30S LoRa module communication setup used in the SideQuester project. These tests demonstrate basic LoRa functionality and serve as a foundation for the full friend-finder system.

## 📁 Directory Contents

```
examples/lora_test/
├── transmitter/
│   └── lora_transmitter_test.ino    # Transmitter test sketch
├── receiver/
│   └── lora_receiver_test.ino       # Receiver test sketch
└── README.md                        # This file
```

## 🎯 Test Objectives

These test sketches verify:
- ✅ Proper E22-900M30S module wiring and initialization
- ✅ UART communication between ESP32 and LoRa module
- ✅ Transparent transmission mode operation
- ✅ Basic message transmission and reception
- ✅ Signal strength (RSSI) monitoring
- ✅ Error handling and status reporting

## 🔧 Hardware Requirements

### For Each Test Device:
- **ESP32 Development Board** (ESP32-WROOM-32 or compatible)
- **E22-900M30S LoRa Module** (SX1262 chipset)
- **Breadboard and jumper wires**
- **USB cable for programming and power**
- **Antenna** (optional for initial testing, recommended for range tests)

### Minimum Setup:
You need **two complete setups** (2x ESP32 + 2x E22-900M30S) to test communication.

## 🔌 Wiring Configuration

Refer to the [detailed wiring guide](../../docs/LORA_WIRING_GUIDE.md) for complete instructions.

**Quick Reference:**
```
E22-900M30S → ESP32
VCC → 3.3V
GND → GND  
RXD → GPIO16 (ESP32 TX)
TXD → GPIO17 (ESP32 RX)
AUX → GPIO18
M0 → GPIO4
M1 → GPIO5
```

## 🚀 Quick Start Guide

### Step 1: Hardware Setup
1. Wire both ESP32 boards to their E22-900M30S modules following the wiring guide
2. Double-check all connections, especially power and ground
3. Connect antennas if available (not required for close-range testing)

### Step 2: Software Setup
1. **Install Arduino IDE** (version 1.8.x or 2.x)
2. **Add ESP32 board support** to Arduino IDE
3. **No additional libraries required** - uses built-in SoftwareSerial

### Step 3: Upload Code
1. **Device A (Transmitter)**:
   - Open `transmitter/lora_transmitter_test.ino`
   - Select correct ESP32 board and COM port
   - Upload sketch

2. **Device B (Receiver)**:
   - Open `receiver/lora_receiver_test.ino`
   - Select correct ESP32 board and COM port  
   - Upload sketch

### Step 4: Testing
1. **Power on both devices**
2. **Open Serial Monitor** for both devices (115200 baud)
3. **Observe communication** - receiver should display transmitted messages

## 📊 Expected Output

### Transmitter Output:
```
===========================================
     E22-900M30S LoRa Transmitter Test    
           SideQuester Project            
===========================================

[00:00:01] Initializing LoRa transmitter...
→ Set to configuration mode (M0=1, M1=1)
→ Configuring LoRa module parameters...
→ Using default configuration for frequency 915MHz
→ Power set to 22dBm, Channel 23
→ Set to transparent transmission mode (M0=0, M1=0)
[00:00:02] ✓ LoRa module initialized successfully!
✓ Ready to transmit test messages every 2 seconds
✓ Configuration: 915MHz, 22dBm, Channel 23
✓ Transparent transmission mode active

--- Transmission Log ---
[00:00:03] TX [1]: Hello from Device A - Count: 1
[00:00:05] TX [2]: Hello from Device A - Count: 2
[00:00:07] TX [3]: Hello from Device A - Count: 3
...
```

### Receiver Output:
```
===========================================
      E22-900M30S LoRa Receiver Test      
           SideQuester Project            
===========================================

[00:00:01] Initializing LoRa receiver...
→ Set to configuration mode (M0=1, M1=1)
→ Configuring LoRa module parameters...
→ Using default configuration for frequency 915MHz
→ Power set to 22dBm, Channel 23
→ Set to transparent transmission mode (M0=0, M1=0)
[00:00:02] ✓ LoRa module initialized successfully!
✓ Ready to receive messages
✓ Configuration: 915MHz, 22dBm, Channel 23
✓ Transparent transmission mode active

--- Reception Log ---
Waiting for incoming messages...
[00:00:03] RX [1] Hello from Device A - Count: 1 | RSSI: -82 dBm
[00:00:05] RX [2] Hello from Device A - Count: 2 | RSSI: -85 dBm
[00:00:07] RX [3] Hello from Device A - Count: 3 | RSSI: -83 dBm

[00:00:13] --- Status Update ---
Messages received: 3
Last RSSI: -83 dBm
Uptime: 00:00:13
Listening for messages...
```

## ⚙️ Configuration Options

### Module Parameters
The sketches are pre-configured with the following settings:

```cpp
// LoRa Configuration
#define LORA_FREQUENCY 915    // MHz (US band)
#define LORA_POWER 22         // dBm (configurable: 10-30)
#define LORA_CHANNEL 23       // Channel number
```

### Regional Settings
- **United States**: Use 915MHz (default)
- **Europe**: Change to 868MHz
- **Other regions**: Check local LoRa frequency regulations

### Transmission Timing
```cpp
#define TRANSMISSION_INTERVAL 2000  // 2 seconds between messages
```

### Status Updates
```cpp
#define STATUS_UPDATE_INTERVAL 10000  // 10 seconds between status reports
```

## 🔧 Customization

### Changing Device Names
In the transmitter sketch:
```cpp
#define DEVICE_ID "Device A"    // Change to any name
```

### Adjusting Transmission Power
```cpp
#define LORA_POWER 22    // Range: 10-30 dBm
```
- **Lower power**: Shorter range, less power consumption
- **Higher power**: Longer range, more power consumption

### Modifying Message Content
Edit the `sendTestMessage()` function in the transmitter:
```cpp
String message = "Hello from " + String(DEVICE_ID) + " - Count: " + String(messageCounter);
// Change to any custom message format
```

### Frequency Configuration
For different regions, modify:
```cpp
#define LORA_FREQUENCY 915    // US: 915, EU: 868
```

## 🧪 Testing Scenarios

### 1. Close Range Test (0-5 meters)
- **Setup**: Both devices on same desk/table
- **Expected**: Perfect communication, strong RSSI (-40 to -60 dBm)
- **Purpose**: Verify basic functionality

### 2. Indoor Range Test (5-50 meters)
- **Setup**: Devices in different rooms
- **Expected**: Good communication, moderate RSSI (-60 to -90 dBm)
- **Purpose**: Test through walls and obstacles

### 3. Outdoor Range Test (50-1000+ meters)
- **Setup**: Line-of-sight outdoor environment
- **Expected**: Excellent range, variable RSSI (-70 to -120 dBm)
- **Purpose**: Maximum range evaluation

### 4. Urban Environment Test
- **Setup**: City environment with buildings
- **Expected**: Reduced range due to obstacles
- **Purpose**: Real-world performance assessment

## 📈 Performance Metrics

### Signal Strength (RSSI) Guidelines
| RSSI Range | Quality | Expected Performance |
|------------|---------|---------------------|
| -30 to -60 dBm | Excellent | Perfect communication |
| -60 to -80 dBm | Good | Reliable communication |
| -80 to -100 dBm | Fair | Occasional packet loss |
| -100 to -120 dBm | Poor | Frequent packet loss |
| Below -120 dBm | Very Poor | Unreliable communication |

### Expected Range
| Environment | Estimated Range |
|-------------|----------------|
| Indoor (WiFi interference) | 50-200m |
| Urban (buildings, obstacles) | 500m-1km |
| Suburban (some obstacles) | 1-2km |
| Rural (line of sight) | 2-5km |
| Open field (ideal conditions) | 3-10km |

## 🐛 Troubleshooting

### No Communication Between Devices

**Check List:**
- [ ] Both devices powered on and initialized successfully
- [ ] Wiring connections secure (especially UART cross-connection)
- [ ] Same frequency configured on both devices (915MHz)
- [ ] Both devices in transparent mode (M0=0, M1=0)
- [ ] Antennas connected (if using external antennas)

**Solutions:**
1. **Restart both devices** and check initialization messages
2. **Verify wiring** using the wiring guide
3. **Check power supply** - ensure adequate current for transmission
4. **Test with devices closer together** (< 1 meter)

### Intermittent Communication

**Possible Causes:**
- Weak signal strength (check RSSI values)
- Electromagnetic interference
- Power supply instability
- Loose connections

**Solutions:**
1. **Move devices closer** to improve signal strength
2. **Check environment** for interference sources (WiFi, Bluetooth, microwaves)
3. **Verify power connections** and consider adding decoupling capacitors
4. **Re-seat all connections** on breadboard

### Module Initialization Failure

**Error Message**: "LoRa initialization failed!"

**Solutions:**
1. **Check power connections** (VCC, GND)
2. **Verify AUX pin connection** (GPIO18)
3. **Check mode pins** (M0=GPIO4, M1=GPIO5)
4. **Try different ESP32 board** to rule out hardware issues

### Poor Signal Strength

**If RSSI is consistently poor (-100 dBm or worse):**
1. **Check antenna connections**
2. **Reduce distance between devices**
3. **Ensure line-of-sight communication**
4. **Verify transmission power setting**

## 🔄 Next Steps

After successful testing:

1. **Range Testing**: Gradually increase distance to find maximum reliable range
2. **Integration**: Use these working modules with the main SideQuester code
3. **Optimization**: Experiment with different power levels and channels
4. **Enclosure**: Design weatherproof enclosures for outdoor use

## 📚 Additional Resources

- [Complete Wiring Guide](../../docs/LORA_WIRING_GUIDE.md)
- [Main SideQuester Project](../../README.md)
- [E22-900M30S Datasheet](https://www.ebyte.com/en/pdf-down.html?id=437)
- [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)

## 🤝 Contributing

Found issues or improvements? Please:
1. Check existing [GitHub issues](https://github.com/AmrEslim/SideQuester/issues)
2. Create new issue with detailed description
3. Submit pull request with fixes or enhancements

---

**Happy Testing!** 🎉

These test sketches provide the foundation for reliable LoRa communication in the SideQuester project. Once you have confirmed successful bidirectional communication, you're ready to integrate with GPS and compass modules for the complete friend-finder system.