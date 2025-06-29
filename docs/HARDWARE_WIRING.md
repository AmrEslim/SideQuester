# Hardware Wiring Guide

Complete wiring documentation for the SideQuester system. This guide provides detailed connection diagrams, pin tables, and troubleshooting information for all components.

## 🔌 Complete System Wiring Overview

```
                     ESP32 Development Board
                    ┌─────────────────────────┐
                    │                         │
                    │  ┌─────────────────┐   │
                    │  │     ESP32       │   │
                    │  │   WROOM-32      │   │
                    │  └─────────────────┘   │
                    │                         │
                    └─────────────────────────┘
                           │ │ │ │ │ │ │ │
                           │ │ │ │ │ │ │ │
        ┌──────────────────┘ │ │ │ │ │ │ └──────────────────┐
        │                    │ │ │ │ │ │                    │
        │             ┌──────┘ │ │ │ │ └──────┐             │
        │             │        │ │ │ │        │             │
        │             │   ┌────┘ │ │ └────┐   │             │
        │             │   │      │ │      │   │             │
        ▼             ▼   ▼      ▼ ▼      ▼   ▼             ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│             │ │             │ │             │ │             │
│ E22-900M30S │ │ GPS Module  │ │ OLED Display│ │ Magnetometer│
│ LoRa Module │ │ (NEO-6M/8M) │ │ (SSD1306)   │ │ (HMC5883L)  │
│             │ │             │ │             │ │             │
└─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘
```

## 🎯 E22-900M30S LoRa Module Wiring (SPI Communication)

### Pin Connection Table
| E22-900M30S Pin | ESP32 Pin | Function | Notes |
|-----------------|-----------|----------|-------|
| VCC | 3.3V | Power Supply | **Critical**: Use 3.3V only |
| GND | GND | Ground | Connect to ESP32 GND |
| NSS | GPIO15 | Chip Select | SPI Slave Select |
| SCK | GPIO14 | SPI Clock | SPI Clock Line |
| MISO | GPIO12 | Master In Slave Out | Data from LoRa module |
| MOSI | GPIO13 | Master Out Slave In | Data to LoRa module |
| DIO1 | GPIO4 | Digital I/O 1 | Interrupt/Status pin |
| BUSY | GPIO18 | Module Busy | Module ready status |
| NRST | GPIO19 | Reset | Reset pin (optional) |
| ANT | - | Antenna | External antenna connection |

### Wiring Diagram
```
E22-900M30S Module          ESP32
┌─────────────────┐      ┌─────────────────┐
│                 │      │                 │
│  VCC  ●─────────┼──────┼─● 3.3V          │
│  GND  ●─────────┼──────┼─● GND           │
│  NSS  ●─────────┼──────┼─● GPIO15        │
│  SCK  ●─────────┼──────┼─● GPIO14        │
│  MISO ●─────────┼──────┼─● GPIO12        │
│  MOSI ●─────────┼──────┼─● GPIO13        │
│  DIO1 ●─────────┼──────┼─● GPIO4         │
│  BUSY ●─────────┼──────┼─● GPIO18        │
│  NRST ●─────────┼──────┼─● GPIO19        │
│  ANT  ●─────────┼──────┼─● (Antenna)     │
│                 │      │                 │
└─────────────────┘      └─────────────────┘
```

### ⚠️ Important Notes
- **Power**: E22-900M30S requires exactly 3.3V. Do NOT connect to 5V.
- **Antenna**: Always connect an appropriate antenna before powering on.
- **Reset Pin**: NRST can be tied to 3.3V if not used for software reset.

## 📡 GPS Module Wiring (UART Communication)

### Pin Connection Table
| GPS Pin | ESP32 Pin | Function | Notes |
|---------|-----------|----------|-------|
| VCC | 3.3V or 5V | Power Supply | Check module specifications |
| GND | GND | Ground | Connect to ESP32 GND |
| TX | GPIO2 | UART Transmit | GPS data to ESP32 |
| RX | GPIO0 | UART Receive | Commands to GPS (optional) |

### Wiring Diagram
```
GPS Module (NEO-6M/8M)      ESP32
┌─────────────────┐      ┌─────────────────┐
│                 │      │                 │
│  VCC  ●─────────┼──────┼─● 3.3V/5V       │
│  GND  ●─────────┼──────┼─● GND           │
│  TX   ●─────────┼──────┼─● GPIO2         │
│  RX   ●─────────┼──────┼─● GPIO0         │
│                 │      │                 │
└─────────────────┘      └─────────────────┘
```

## 🧭 Magnetometer/Compass Module Wiring (I2C Communication)

### Pin Connection Table
| HMC5883L Pin | ESP32 Pin | Function | Notes |
|--------------|-----------|----------|-------|
| VCC | 3.3V | Power Supply | 3.3V only |
| GND | GND | Ground | Connect to ESP32 GND |
| SDA | GPIO21 | I2C Data | Shared with OLED |
| SCL | GPIO22 | I2C Clock | Shared with OLED |

### Wiring Diagram
```
HMC5883L Module             ESP32
┌─────────────────┐      ┌─────────────────┐
│                 │      │                 │
│  VCC  ●─────────┼──────┼─● 3.3V          │
│  GND  ●─────────┼──────┼─● GND           │
│  SDA  ●─────────┼──────┼─● GPIO21        │
│  SCL  ●─────────┼──────┼─● GPIO22        │
│                 │      │                 │
└─────────────────┘      └─────────────────┘
```

## 📺 OLED Display Wiring (I2C Communication)

### Pin Connection Table
| SSD1306 Pin | ESP32 Pin | Function | Notes |
|-------------|-----------|----------|-------|
| VCC | 3.3V | Power Supply | 3.3V only |
| GND | GND | Ground | Connect to ESP32 GND |
| SDA | GPIO21 | I2C Data | Shared with compass |
| SCL | GPIO22 | I2C Clock | Shared with compass |

### Wiring Diagram
```
SSD1306 OLED Display        ESP32
┌─────────────────┐      ┌─────────────────┐
│                 │      │                 │
│  VCC  ●─────────┼──────┼─● 3.3V          │
│  GND  ●─────────┼──────┼─● GND           │
│  SDA  ●─────────┼──────┼─● GPIO21        │
│  SCL  ●─────────┼──────┼─● GPIO22        │
│                 │      │                 │
└─────────────────┘      └─────────────────┘
```

## 🔧 Complete System Integration

### Master Pin Assignment Table
| ESP32 Pin | Component | Function | Protocol |
|-----------|-----------|----------|----------|
| 3.3V | All modules | Power supply | - |
| GND | All modules | Ground | - |
| GPIO0 | GPS RX | GPS receive | UART |
| GPIO2 | GPS TX | GPS transmit | UART |
| GPIO4 | LoRa DIO1 | LoRa interrupt | SPI |
| GPIO12 | LoRa MISO | SPI data in | SPI |
| GPIO13 | LoRa MOSI | SPI data out | SPI |
| GPIO14 | LoRa SCK | SPI clock | SPI |
| GPIO15 | LoRa NSS | SPI chip select | SPI |
| GPIO18 | LoRa BUSY | Module status | Digital |
| GPIO19 | LoRa NRST | Reset (optional) | Digital |
| GPIO21 | OLED/Compass SDA | I2C data | I2C |
| GPIO22 | OLED/Compass SCL | I2C clock | I2C |

### Complete Wiring Schematic
```
                                    ESP32 WROOM-32
                              ┌─────────────────────────┐
                              │                         │
                              │  EN              D23    │
                              │  VP              D22    │●──── SCL (OLED + Compass)
                              │  VN              TX0    │
                              │  D34             RX0    │●──── GPS RX
                              │  D35             D21    │●──── SDA (OLED + Compass)
                              │  D32             D19    │●──── LoRa NRST
                              │  D33             D18    │●──── LoRa BUSY
                              │  D25             D5     │
                              │  D26             TX2    │
                              │  D27             RX2    │
                              │  D14             D4     │●──── LoRa DIO1
                              │  D12             D2     │●──── GPS TX
                              │  D13             D15    │●──── LoRa NSS
                              │  GND             GND    │●──── All GND
                              │  VIN             3V3    │●──── All VCC
                              │                         │
                              └─────────────────────────┘
                                    │ │ │ │ │ │ │
                                    │ │ │ │ │ │ │
                       ┌────────────┘ │ │ │ │ │ └────────────┐
                       │              │ │ │ │ │              │
                       │         ┌────┘ │ │ │ └────┐         │
                       │         │      │ │ │      │         │
                       │         │   ┌──┘ │ └──┐   │         │
                       │         │   │    │    │   │         │
                       ▼         ▼   ▼    ▼    ▼   ▼         ▼
                 ┌─────────┐ ┌─────────┐ GPIO12 GPIO13 ┌─────────┐
                 │         │ │         │ (MISO) (MOSI) │         │
                 │ OLED    │ │ Compass │      │ │      │ GPS     │
                 │ Display │ │ Module  │      │ │      │ Module  │
                 │         │ │         │      │ │      │         │
                 └─────────┘ └─────────┘      │ │      └─────────┘
                                              │ │
                                              ▼ ▼
                                        ┌─────────┐
                                        │         │
                                        │ E22-900M30S
                                        │ LoRa    │
                                        │ Module  │
                                        │         │
                                        └─────────┘
```

## 🛠️ Assembly Instructions

### Step 1: Prepare Components
1. Verify all components are present
2. Check voltage requirements (3.3V vs 5V)
3. Prepare jumper wires and breadboard

### Step 2: Power Connections
1. Connect all VCC pins to ESP32 3.3V
2. Connect all GND pins to ESP32 GND
3. **Double-check power connections before proceeding**

### Step 3: LoRa Module (SPI)
1. Connect NSS → GPIO15 (Chip Select)
2. Connect SCK → GPIO14 (Clock)
3. Connect MISO → GPIO12 (Data from module)
4. Connect MOSI → GPIO13 (Data to module)
5. Connect DIO1 → GPIO4 (Interrupt)
6. Connect BUSY → GPIO18 (Status)
7. Connect NRST → GPIO19 (Reset, optional)
8. Connect antenna to ANT pin

### Step 4: GPS Module (UART)
1. Connect GPS TX → ESP32 GPIO2
2. Connect GPS RX → ESP32 GPIO0

### Step 5: I2C Devices (OLED + Compass)
1. Connect SDA → GPIO21 (both devices)
2. Connect SCL → GPIO22 (both devices)

### Step 6: Final Verification
1. Check all connections against pin tables
2. Verify no short circuits
3. Ensure proper power supply voltages

## 🚨 Troubleshooting Guide

### LoRa Communication Issues

#### Problem: LoRa module not responding
**Symptoms:**
- No radio communication
- Module initialization fails
- SPI communication errors

**Solutions:**
1. **Check Power Supply**
   - Verify 3.3V power (NOT 5V)
   - Check current capacity (module needs ~100mA)
   - Measure voltage at module VCC pin

2. **Verify SPI Connections**
   - NSS (GPIO15) - must be low during communication
   - SCK (GPIO14) - check for clock signal
   - MISO/MOSI (GPIO12/13) - verify data lines
   - Use oscilloscope or logic analyzer if available

3. **Check Control Pins**
   - DIO1 (GPIO4) - interrupt pin
   - BUSY (GPIO18) - should go low when module ready
   - NRST (GPIO19) - reset pin (high = normal operation)

4. **Antenna Issues**
   - Always connect antenna before powering on
   - Check antenna connection and impedance
   - Verify antenna frequency matches module (868/915MHz)

#### Problem: Poor range or signal quality
**Solutions:**
1. Check antenna placement and orientation
2. Verify frequency settings match local regulations
3. Check for interference sources
4. Ensure line-of-sight when possible

### GPS Module Issues

#### Problem: GPS not getting fix
**Symptoms:**
- No satellite lock
- Invalid coordinates
- Long acquisition times

**Solutions:**
1. **Location Requirements**
   - Move outdoors with clear sky view
   - Avoid tall buildings or tree cover
   - Wait 2-5 minutes for cold start

2. **Power and Connections**
   - Check voltage (3.3V or 5V depending on module)
   - Verify TX/RX connections (GPS TX → ESP32 GPIO2)
   - Check baud rate (usually 9600)

3. **Module-Specific Issues**
   - Some modules require 5V power
   - Check datasheet for power requirements
   - Verify module LED indicators

### I2C Communication Issues

#### Problem: OLED display blank or compass not working
**Symptoms:**
- Display shows nothing
- Compass readings incorrect
- I2C device not found errors

**Solutions:**
1. **Check I2C Connections**
   - SDA → GPIO21 (both devices)
   - SCL → GPIO22 (both devices)
   - Verify pull-up resistors (often built-in)

2. **Address Conflicts**
   - Scan I2C bus for device addresses
   - Common addresses: OLED=0x3C, HMC5883L=0x1E
   - Check for duplicate addresses

3. **Power Issues**
   - Verify 3.3V power supply
   - Check ground connections
   - Measure voltage at device pins

### General Troubleshooting

#### Problem: Intermittent failures or resets
**Solutions:**
1. **Power Supply Issues**
   - Check power supply capacity
   - Add decoupling capacitors
   - Verify stable 3.3V supply

2. **Wiring Problems**
   - Check for loose connections
   - Verify no short circuits
   - Use multimeter for continuity tests

3. **Software Issues**
   - Check library versions
   - Verify pin definitions in code
   - Enable debug output

#### Problem: High power consumption
**Solutions:**
1. Implement sleep modes
2. Reduce transmission frequency
3. Check for stuck modules
4. Optimize code for power efficiency

### Diagnostic Tools

#### I2C Scanner Code
```cpp
#include <Wire.h>

void scanI2C() {
  byte error, address;
  int nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) Serial.print("0");
      Serial.println(address,HEX);
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  }
}
```

#### SPI Test Code
```cpp
#include <SPI.h>

void testSPI() {
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  
  digitalWrite(15, LOW);  // NSS low
  byte response = SPI.transfer(0x42);  // Test command
  digitalWrite(15, HIGH); // NSS high
  
  SPI.endTransaction();
  Serial.print("SPI response: 0x");
  Serial.println(response, HEX);
}
```

## 📋 Component Specifications

### E22-900M30S LoRa Module
- **Frequency**: 868MHz/915MHz
- **Power**: 3.3V, ~100mA
- **Communication**: SPI
- **Range**: Up to 3km line-of-sight
- **Antenna**: 50Ω impedance

### ESP32 Development Board
- **Power**: 3.3V logic, 5V USB power
- **I2C**: GPIO21 (SDA), GPIO22 (SCL)
- **SPI**: GPIO12 (MISO), GPIO13 (MOSI), GPIO14 (SCK)
- **UART**: Multiple UART ports available

### GPS Module (NEO-6M/8M)
- **Power**: 3.3V or 5V (check specifications)
- **Communication**: UART, 9600 baud
- **Accuracy**: 2.5m CEP
- **Cold start**: 26-30 seconds

### OLED Display (SSD1306)
- **Power**: 3.3V
- **Communication**: I2C
- **Address**: 0x3C (typical)
- **Resolution**: 128x64 pixels

### Magnetometer (HMC5883L)
- **Power**: 3.3V
- **Communication**: I2C
- **Address**: 0x1E
- **Resolution**: 1-2 degrees

## 📊 Testing Checklist

### Pre-Power-On Checks
- [ ] All power connections verified (3.3V/5V as appropriate)
- [ ] All ground connections verified
- [ ] No short circuits detected
- [ ] Antenna connected to LoRa module
- [ ] Visual inspection of all connections

### Power-On Tests
- [ ] ESP32 boots successfully
- [ ] Power LED indicators active
- [ ] No unusual heating of components
- [ ] Serial monitor shows startup messages

### Individual Component Tests
- [ ] LoRa module initialization successful
- [ ] GPS module receiving data
- [ ] I2C devices detected and responding
- [ ] OLED display shows content
- [ ] Compass readings reasonable

### System Integration Tests
- [ ] All modules working simultaneously
- [ ] LoRa communication between devices
- [ ] GPS coordinates being transmitted
- [ ] Compass pointing calculation working
- [ ] Display showing correct information

## 🔧 Maintenance and Calibration

### Compass Calibration
1. Power on device away from metal objects
2. Hold device flat and rotate 360° slowly
3. Repeat rotation in different orientations
4. Check for consistent readings
5. Adjust magnetic declination for your location

### GPS Optimization
1. Update to latest firmware if possible
2. Configure update rate (1Hz typical)
3. Enable only required GNSS systems
4. Check antenna placement and connections

### LoRa Range Testing
1. Start with devices close together
2. Gradually increase distance
3. Test in different environments
4. Document range limitations
5. Optimize antenna placement

## 📚 Additional Resources

### Datasheets
- [E22-900M30S LoRa Module Datasheet](https://www.ebyte.com)
- [SX1262 Chip Datasheet](https://www.semtech.com)
- [ESP32 Reference Manual](https://espressif.com)

### Libraries
- [RadioLib](https://github.com/jgromes/RadioLib) - LoRa communication
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus) - GPS parsing
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) - OLED display

### Community Support
- [SideQuester GitHub Issues](https://github.com/AmrEslim/SideQuester/issues)
- [Arduino Forums](https://forum.arduino.cc)
- [ESP32 Community](https://www.esp32.com)

---

**Remember**: Always double-check your wiring before powering on. When in doubt, measure voltages and check continuity with a multimeter.