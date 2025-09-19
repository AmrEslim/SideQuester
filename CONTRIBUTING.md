# Contributing to SideQuester

We welcome contributions from the maker community! Whether you're a hardware hacker, software developer, or documentation writer, there are many ways to help improve SideQuester.

## 🎯 Ways to Contribute

### 🔨 Build and Test
- **Build your own SideQuester** and share your experience
- **Test in different environments** (urban, rural, festivals, hiking)
- **Document range testing** results with photos and GPS logs
- **Try alternative components** and report compatibility

### 💻 Code Contributions  
- **Bug fixes** and software improvements
- **New features** like multi-device support or power optimization
- **Library updates** and compatibility improvements
- **Example code** for specific use cases

### 📚 Documentation
- **Improve setup guides** with clearer instructions
- **Add troubleshooting solutions** for issues you've solved
- **Create video tutorials** for assembly and setup
- **Translate documentation** to other languages

### 🎨 Design and Hardware
- **PCB layouts** for smaller, more reliable builds
- **3D printable enclosures** with STL files
- **Circuit improvements** and alternative component suggestions
- **Antenna design** optimizations

## 🚀 Getting Started

### 1. Fork and Clone
```bash
# Fork the repository on GitHub
git clone https://github.com/YOUR_USERNAME/SideQuester.git
cd SideQuester
```

### 2. Create a Branch
```bash
git checkout -b feature/amazing-new-feature
# or
git checkout -b fix/bug-description
# or  
git checkout -b docs/improve-setup-guide
```

### 3. Make Your Changes
- Follow the coding style guidelines below
- Test thoroughly before submitting
- Update documentation if needed
- Add examples for new features

### 4. Submit Pull Request
```bash
git add .
git commit -m "Add amazing new feature that does X"
git push origin feature/amazing-new-feature
```

Then create a pull request on GitHub with a clear description of your changes.

## 📝 Coding Standards

### Arduino C++ Style
```cpp
// Use descriptive variable names
float batteryVoltage = readBatteryVoltage();
int satelliteCount = gps.satellites.value();

// Comment complex algorithms
// Calculate bearing using Haversine formula
float bearing = calculateBearing(lat1, lon1, lat2, lon2);

// Use constants for magic numbers
#define GPS_TIMEOUT 30000  // 30 seconds
#define LORA_RETRY_COUNT 3
```

### File Organization
```
src/
├── SideQuester.ino          # Main sketch
├── config.h                 # Configuration constants
├── gps_module.h/.cpp        # GPS handling
├── lora_module.h/.cpp       # LoRa communication  
├── compass_module.h/.cpp    # Magnetometer
├── display_module.h/.cpp    # OLED display
└── utils.h/.cpp             # Helper functions
```

### Code Comments
- **Explain WHY, not WHAT**: `// Reduce power consumption during GPS search`
- **Document pin assignments**: `#define GPS_TX_PIN 2  // ESP32 GPIO2`
- **Include units**: `float distance_m = 1500;  // Distance in meters`
- **Reference sources**: `// Formula from: https://www.movable-type.co.uk/scripts/latlong.html`

## 🧪 Testing Guidelines

### Before Submitting Code
1. **Compile successfully** on Arduino IDE
2. **Test on actual hardware** (not just simulation)
3. **Verify GPS functionality** outdoors
4. **Test LoRa communication** between two devices
5. **Check power consumption** impact
6. **Document any new library requirements**

### Test Cases to Verify
```cpp
// GPS module tests
✓ GPS fix acquisition under clear sky
✓ GPS timeout handling indoors
✓ GPS coordinate parsing accuracy
✓ GPS module power cycling

// LoRa communication tests  
✓ Device discovery and pairing
✓ Message transmission/reception
✓ Range testing in different environments
✓ Error handling for lost connections

// Compass tests
✓ Compass calibration procedure
✓ Bearing calculation accuracy
✓ Magnetic interference rejection
✓ Temperature stability

// Display tests
✓ Text rendering and readability
✓ Graphics and icons
✓ Screen refresh performance
✓ Power consumption
```

### Hardware Testing
- Test with different ESP32 board variants
- Verify compatibility with alternative GPS modules
- Check I2C bus stability with multiple devices
- Measure actual battery life in typical usage

## 📋 Pull Request Guidelines

### PR Description Template
```markdown
## Description
Brief description of what this PR does.

## Type of Change
- [ ] Bug fix
- [ ] New feature  
- [ ] Documentation update
- [ ] Hardware improvement
- [ ] Performance optimization

## Testing
- [ ] Tested on hardware
- [ ] GPS functionality verified
- [ ] LoRa communication tested
- [ ] Battery life impact assessed
- [ ] Documentation updated

## Screenshots/Videos
Include photos of hardware changes or screenshots of new features.

## Hardware Tested
- ESP32 board model:
- GPS module:  
- LoRa module:
- Other components:
```

### Code Review Process
1. **Automated checks** run on all PRs
2. **Maintainer review** for code quality and compatibility
3. **Community feedback** welcome on all contributions
4. **Hardware testing** by maintainers when possible
5. **Merge** after approval and successful testing

## 🐛 Reporting Issues

### Bug Reports
Use this template for bug reports:

```markdown
**Describe the Bug**
A clear description of what the bug is.

**Hardware Configuration**
- ESP32 board: [e.g. ESP32-WROOM-32]
- GPS module: [e.g. NEO-8M]
- LoRa module: [e.g. E22-900M30S, E220-900T30D]  
- Power supply: [e.g. 18650 Li-ion]
- Other components:

**Steps to Reproduce**
1. Power on device
2. Wait for GPS fix
3. Try to pair with second device
4. Error occurs

**Expected Behavior**
What you expected to happen.

**Actual Behavior**  
What actually happened.

**Serial Output**
```
Copy and paste relevant serial monitor output here
```

**Environment**
- Location: [Urban/Rural/Indoor/Outdoor]
- Weather conditions:
- Temperature:
- Other nearby electronics:
```

### Feature Requests
```markdown
**Feature Description**
Clear description of the requested feature.

**Use Case**
Explain how this would improve SideQuester.

**Implementation Ideas**
Any thoughts on how this could be implemented.

**Alternative Solutions**
Other ways to achieve the same goal.
```

**Ready to contribute? We can't wait to see what you build! 🚀**

Start by building your own SideQuester, then come back and share what you learned. Every contribution, no matter how small, helps make the project better for everyone.

**Questions?** Don't hesitate to ask in GitHub Discussions. The community is here to help!

---

*Thank you for helping make SideQuester awesome! 🧭❤️*