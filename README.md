# SideQuester 🧭

**Your Ultimate Friend Finder for Festivals & Off-Grid Adventures**

SideQuester is a dual ESP32-based compass system that helps you stay connected with friends in crowded festivals, remote camping sites, or any location where cellular networks fail. Using GPS coordinates transmitted via LoRa radio, each device acts as a compass that always points toward your companion, ensuring you never lose each other again.

## 🎯 Perfect For

- **Music Festivals** - Navigate massive crowds without losing your group
- **Camping Trips** - Stay connected in remote wilderness areas  
- **Hiking Adventures** - Track your hiking partner's location on trails
- **Emergency Situations** - Maintain contact when cell towers are down
- **Outdoor Events** - Find friends at large gatherings or markets
- **Adventure Racing** - Keep teams coordinated across challenging terrain

## ✨ Key Features

- 🧭 **Real-time Direction Pointing** - Physical compass needle always points to your friend
- 📡 **Long-Range Communication** - Up to 3km range with LoRa technology
- 🛰️ **GPS Tracking** - Precise location sharing using satellite positioning
- 📱 **OLED Display** - Shows distance, direction, GPS status, and connection info
- 🔋 **Battery Powered** - Portable design for all-day adventures
- 🌧️ **Weather Resistant** - Built for outdoor conditions
- 📶 **No Network Required** - Works completely offline

## 🎪 Festival Use Case

Imagine you're at a massive music festival with 100,000+ attendees:

1. **Split Up Safely** - Give one SideQuester to a friend, keep the other
2. **Explore Independently** - Check out different stages or vendors
3. **Always Find Each Other** - Your compass points directly to your friend's location
4. **See Real Distance** - Know exactly how far away they are (e.g., "0.8km NE")
5. **Meet Up Easily** - Navigate through crowds following your personal compass

No more endless texts that won't send, confusing "meet at the main stage" plans, or losing friends in the chaos!

## 🛠️ Hardware Components

### Core Electronics (Per Device)
- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
- **E22-900M30S LoRa Module** (868MHz/915MHz depending on region)
- **GPS Module** (NEO-6M, NEO-8M, or NEO-9M)
- **Digital Compass/Magnetometer** (HMC5883L or MPU9250)
- **OLED Display** (128x64 SSD1306 or similar)
- **Servo Motor** (SG90 or similar) for physical compass needle
- **Battery Pack** (Li-ion 18650 with charging circuit)
- **Breadboard/PCB** for connections
- **Jumper wires and resistors**

### Estimated Cost
- **Per Device**: ~$45-60 USD
- **Complete Pair**: ~$90-120 USD

## 🚀 Quick Start Guide

### 1. Hardware Assembly
Wire the components according to this connection guide:

#### ESP32 to E22-900M30S LoRa Module
```
E22-900M30S → ESP32
VCC → 3.3V
GND → GND
M0 → GPIO4 (mode control)
M1 → GPIO5 (mode control)
RXD → GPIO16 (software serial TX)
TXD → GPIO17 (software serial RX)
AUX → GPIO18 (auxiliary control)
```

#### GPS Module to ESP32
```
GPS Module → ESP32
VCC → 3.3V or 5V (check module specs)
GND → GND
TX → GPIO2 (software serial RX)
RX → GPIO0 (software serial TX)
```

#### Magnetometer/Compass Module
```
HMC5883L → ESP32
VCC → 3.3V
GND → GND
SDA → GPIO21 (I2C)
SCL → GPIO22 (I2C)
```

#### OLED Display
```
SSD1306 → ESP32
VCC → 3.3V
GND → GND
SDA → GPIO21 (I2C, shared with compass)
SCL → GPIO22 (I2C, shared with compass)
```

#### Servo Motor
```
SG90 → ESP32
VCC → 5V (external power recommended)
GND → GND
Signal → GPIO13 (PWM capable)
```

### 2. Software Installation

#### Required Libraries (install via Arduino IDE Library Manager):
```cpp
#include <SoftwareSerial.h>    // For LoRa and GPS communication
#include <TinyGPS++.h>         // GPS parsing
#include <Wire.h>              // I2C communication
#include <Adafruit_SSD1306.h>  // OLED display
#include <Adafruit_HMC5883_U.h> // Magnetometer
#include <Servo.h>             // Servo control
```

### 3. Configuration Parameters

```cpp
// LoRa Configuration
#define LORA_FREQUENCY 915  // MHz (adjust for region: 868 EU, 915 US)
#define LORA_POWER 30       // dBm
#define LORA_CHANNEL 23

// Device Configuration
#define DEVICE_ID 1         // Set to 1 for first device, 2 for second

// GPS Configuration
#define GPS_BAUD 9600
#define GPS_UPDATE_RATE 1000  // milliseconds

// Compass Configuration
#define COMPASS_DECLINATION 4.5  // Adjust for your location
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
```

### 4. Upload and Test
1. Clone this repository or download the code
2. Configure DEVICE_ID (1 for first device, 2 for second)
3. Upload code to both ESP32 devices
4. Power on both devices outdoors (for GPS signal)
5. Wait for GPS fix (indicated on OLED display)
6. Watch the compass point to your friend!

## 📋 System Architecture

```
Device A (Friend 1)              Device B (Friend 2)
      ↓                                ↓
  GPS Module                      GPS Module
      ↓                                ↓
 Get Location                   Get Location
      ↓                                ↓
  LoRa Module  ←→ Radio Link ←→   LoRa Module
      ↓                                ↓
Calculate Bearing              Calculate Bearing
      ↓                                ↓
 Servo Compass                  Servo Compass
      ↓                                ↓
 OLED Display                   OLED Display
```

## 🎮 How It Works

1. **GPS Acquisition**: Each device continuously reads its GPS coordinates
2. **Data Transmission**: Coordinates are sent via LoRa radio every 5 seconds
3. **Bearing Calculation**: Using the Haversine formula to calculate direction and distance
4. **Compass Control**: Servo motor positions needle to point toward friend
5. **Display Update**: OLED shows distance, direction, and system status

### Data Packet Structure
```
[Device_ID][Latitude][Longitude][Timestamp][Checksum]
```

### Mathematical Calculations
- **Haversine Formula** for distance calculation
- **Bearing Calculation** for direction
- **Servo Angle Mapping** (0-180° rotation)

## 📊 Display Information

```
┌─────────────────────┐
│ GPS: FIXED          │
│ Lat: 40.7128°N      │
│ Lon: 74.0060°W      │
│ Friend: 0.8km NE    │
│ LoRa: CONNECTED     │
│ Battery: 87%        │
└─────────────────────┘
```

## 🔧 Project Status

This project is currently in development. See our [detailed roadmap](https://github.com/AmrEslim/SideQuester/issues/1) for complete implementation plan.

### Development Phases

#### Phase 1: Hardware Setup & Basic Connectivity ⏳
- [ ] Complete hardware assembly
- [ ] Basic GPS functionality
- [ ] LoRa communication testing
- [ ] Compass calibration
- [ ] OLED display integration

#### Phase 2: Software Development - Core Modules
- [ ] GPS module integration
- [ ] LoRa communication protocol
- [ ] Compass/magnetometer module
- [ ] Mathematical calculations (Haversine formula)

#### Phase 3: Communication Protocol Design
- [ ] Data packet structure implementation
- [ ] Device discovery and pairing
- [ ] Error handling and retries
- [ ] Acknowledgment system

#### Phase 4: User Interface & Display
- [ ] OLED display layout design
- [ ] Servo compass control
- [ ] Status indicators
- [ ] User feedback systems

#### Phase 5: Integration & Testing
- [ ] End-to-end system integration
- [ ] Field testing at various ranges
- [ ] Power consumption optimization
- [ ] Error handling and recovery

#### Phase 6: Enclosure & Deployment
- [ ] Weatherproof enclosure design
- [ ] 3D printable case files
- [ ] Assembly documentation
- [ ] User manual creation

### Estimated Timeline
- **Phase 1-2**: 1-2 weeks (Hardware setup + basic software)
- **Phase 3-4**: 1 week (Communication protocol + UI)
- **Phase 5**: 1 week (Integration)
- **Phase 6**: 1-2 weeks (Testing + deployment)

**Total Estimated Time**: 4-6 weeks

## 🛡️ Safety & Legal Considerations

### Radio Regulations
- **US**: Use 915MHz frequency band
- **EU**: Use 868MHz frequency band
- **Other Regions**: Check local LoRa frequency regulations
- Ensure compliance with local radio transmission laws

### Battery Safety
- Use proper charging circuits for Li-ion batteries
- Implement over-discharge protection
- Never leave charging unattended
- Use quality battery management systems

### Outdoor Use
- Device requires weatherproof enclosure for rain protection
- GPS needs clear sky view for best performance
- Consider temperature effects on battery performance
- Magnetic interference can affect compass accuracy

### Privacy & Ethics
- GPS coordinates are transmitted locally via LoRa only
- No data is stored on external servers
- Respect privacy laws in your jurisdiction
- Inform friends about location tracking

## 🏗️ Expected Challenges & Solutions

1. **GPS Accuracy**
   - Solution: Use high-quality GPS modules, allow adequate sky view
   - Cold start times can be 30+ seconds

2. **LoRa Range Limitations**
   - Solution: Consider terrain, obstacles, and antenna placement
   - Range varies greatly with environment (urban vs rural)

3. **Power Management**
   - Solution: Implement sleep modes and efficient coding
   - GPS and LoRa modules can consume significant power

4. **Magnetic Interference**
   - Solution: Proper compass calibration and placement away from metal
   - Requires periodic recalibration

5. **Weather Protection**
   - Solution: Adequate enclosure design with proper sealing
   - Consider condensation and temperature cycling

## 🎯 Success Criteria

- [ ] Both devices successfully acquire GPS fix within 2 minutes
- [ ] LoRa communication works reliably at 1km+ range
- [ ] Compass points accurately to partner device (±10° accuracy)
- [ ] Display shows clear, readable information in daylight
- [ ] System operates reliably for 8+ hours on battery
- [ ] Weatherproof operation in light rain
- [ ] Easy setup and pairing process

## 🔮 Future Enhancements

### Version 2.0 Features
- [ ] **Multi-Device Support** - Track more than 2 friends
- [ ] **Data Logging** - SD card storage of location history
- [ ] **Web Interface** - Configuration via WiFi hotspot
- [ ] **Altitude Tracking** - 3D positioning for mountaineering
- [ ] **Speed Calculation** - Real-time movement tracking
- [ ] **Geofencing** - Alerts when friends are too far away
- [ ] **Emergency Mode** - SOS broadcasting feature
- [ ] **Solar Charging** - Extended battery life for long expeditions

### Hardware Upgrades
- Custom PCB design for smaller form factor
- Improved antenna design for better range
- E-ink display for better battery life
- Haptic feedback for direction indication

## 🤝 Contributing

We welcome contributions from the community! Here's how you can help:

### Ways to Contribute
- **Build Your Own** - Follow the guide and share your experience
- **Code Improvements** - Submit pull requests for bug fixes or features
- **Documentation** - Help improve setup guides and troubleshooting
- **Testing** - Try the system in different environments and report results
- **Hardware Mods** - Share alternative component suggestions or improvements
- **3D Models** - Design and share enclosure files

### Getting Started
1. Fork this repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style
- Use clear, descriptive variable names
- Comment complex algorithms and hardware interfaces
- Follow Arduino IDE formatting conventions
- Test thoroughly before submitting

## 📁 Repository Structure

```
SideQuester/
├── src/
│   ├── SideQuester.ino       # Main Arduino sketch
│   ├── config.h              # Configuration parameters
│   ├── gps_module.h/.cpp     # GPS handling functions
│   ├── lora_module.h/.cpp    # LoRa communication
│   ├── compass_module.h/.cpp # Magnetometer and servo control
│   ├── display_module.h/.cpp # OLED display functions
│   └── math_utils.h/.cpp     # Bearing calculations
├── docs/
│   ├── HARDWARE_GUIDE.md     # Detailed wiring instructions
│   ├── SOFTWARE_GUIDE.md     # Code documentation
│   ├── TROUBLESHOOTING.md    # Common issues and solutions
│   └── API_REFERENCE.md      # Function documentation
├── examples/
│   ├── gps_test/            # Test GPS module individually
│   ├── lora_test/           # Test LoRa communication
│   ├── compass_test/        # Test compass and servo
│   └── display_test/        # Test OLED display
├── hardware/
│   ├── schematics/          # Circuit diagrams
│   ├── pcb/                 # PCB design files
│   └── bom.md               # Bill of materials
├── enclosure/
│   ├── 3d_models/           # 3D printable enclosure files
│   └── assembly_guide.md    # Enclosure assembly instructions
├── tests/
│   └── field_tests/         # Real-world testing results
├── LICENSE                  # MIT License
└── README.md               # This file
```

## 🐛 Troubleshooting

### Common Issues

#### GPS Not Getting Fix
- Ensure clear sky view (outdoors)
- Check wiring connections
- Wait 2-5 minutes for cold start
- Verify GPS module power (3.3V or 5V depending on module)

#### LoRa Communication Not Working
- Check antenna connections
- Verify frequency settings match local regulations
- Ensure both devices use same channel and parameters
- Check for physical obstructions

#### Compass Not Pointing Correctly
- Calibrate magnetometer away from metal objects
- Check servo wiring and power supply
- Verify magnetic declination setting for your location
- Ensure compass module is level and properly oriented

#### Display Issues
- Check I2C connections (SDA/SCL)
- Verify display address (usually 0x3C for SSD1306)
- Ensure adequate power supply
- Check for conflicting I2C devices

## 📞 Support & Community

### Getting Help
- **GitHub Issues** - Report bugs or request features
- **GitHub Discussions** - Ask questions and share experiences  
- **Documentation** - Check our comprehensive guides
- **Community** - Connect with other SideQuester builders

### Contact
- **Project Maintainer**: [@AmrEslim](https://github.com/AmrEslim)
- **Repository**: [SideQuester](https://github.com/AmrEslim/SideQuester)

### Community Guidelines
- Be respectful and helpful
- Share your builds and improvements
- Help newcomers get started
- Report issues constructively
- Contribute back to the project

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### What this means:
- ✅ Commercial use allowed
- ✅ Modification allowed
- ✅ Distribution allowed
- ✅ Private use allowed
- ❌ No liability or warranty provided

## 🌟 Acknowledgments

- **Festival Communities** - Who inspired the original idea
- **Arduino & ESP32 Communities** - For excellent libraries and support
- **LoRa Alliance** - For developing long-range wireless technology
- **Open Source Contributors** - Who make projects like this possible
- **Beta Testers** - Early adopters who help improve the system

## 📈 Project Metrics

- **Development Started**: June 2025
- **Current Version**: 0.1.0 (Pre-release)
- **Target Release**: Q3 2025
- **Contributors**: 1 (growing!)
- **Estimated Range**: 1-3km depending on terrain
- **Battery Life**: 8-12 hours (estimated)

---

**Built with ❤️ for adventurers who refuse to lose their friends**

*SideQuester - Because getting lost together is better than getting lost alone*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue.svg)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![LoRa](https://img.shields.io/badge/LoRa-E22--900M30S-red.svg)](https://www.ebyte.com/en/product-view-news.html?id=437)

**Ready to build your SideQuester? Star ⭐ this repo and let's get started!**