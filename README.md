# SideQuester 🧭

**Your Ultimate Friend Finder for Festivals & Off-Grid Adventures**

SideQuester is a dual ESP32-based compass system that helps you stay connected with friends in crowded festivals, remote camping sites, or any location where cellular networks fail. Using GPS coordinates transmitted via LoRa radio, each device acts as a compass that always points toward your companion.

## ✨ Key Features

- 🧭 **Real-time Direction Pointing** - Always points to your friend's location
- 📡 **Long-Range Communication** - Up to 3km range with LoRa technology  
- 🛰️ **GPS Tracking** - Precise location sharing using satellite positioning
- 📱 **OLED Display** - Shows distance, direction, and status
- 📶 **No Network Required** - Works completely offline
- 🔋 **All-Day Battery** - Portable design for extended adventures

## 🚀 Quick Start

**Hardware Required (per device):**
- ESP32 development board
- E22-900M30S / E220-900T30D LoRa module
- GPS module (NEO-6M/8M/9M)
- Digital compass (HMC5883L)
- OLED display (128x64)
- Battery pack with charging circuit

**Software:**
1. Install Arduino IDE with ESP32 support
2. Install required libraries (see [docs/SOFTWARE_GUIDE.md](docs/SOFTWARE_GUIDE.md))
3. Upload code to both devices
4. Power on outdoors and wait for GPS fix

📖 **Full Instructions:** See [docs/HARDWARE_GUIDE.md](docs/HARDWARE_GUIDE.md) for detailed wiring and assembly.

## 🎯 Project Status

**Current Phase:** Hardware setup and basic software development  
**Estimated Completion:** Q1 2026

- [x] Project planning and design
- [ ] Hardware assembly and testing
- [ ] Software development
- [ ] Field testing
- [ ] Documentation and deployment

📊 **Detailed Roadmap:** See our [development tracker](https://github.com/AmrEslim/SideQuester/issues/1)

## 📁 Documentation

- [Hardware Assembly Guide](docs/HARDWARE_GUIDE.md)
- [Software Setup Guide](docs/SOFTWARE_GUIDE.md) 
- [Troubleshooting](docs/TROUBLESHOOTING.md)
- [Contributing Guidelines](CONTRIBUTING.md)
- [License](LICENSE)

## 🤝 Contributing

We welcome contributions! Please read our [Contributing Guidelines](CONTRIBUTING.md) and feel free to:
- Build your own and share feedback
- Submit bug reports or feature requests
- Contribute code improvements
- Help with documentation

## 📞 Support

- **Issues:** [GitHub Issues](https://github.com/AmrEslim/SideQuester/issues)
- **Discussions:** [GitHub Discussions](https://github.com/AmrEslim/SideQuester/discussions)
- **Maintainer:** [@AmrEslim](https://github.com/AmrEslim)

## 📜 License

This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. 

- ✅ **Personal use, education, research**: Allowed
- ✅ **Modifications and derivatives**: Allowed  
- ✅ **Sharing**: Allowed with attribution
- ❌ **Commercial use**: Prohibited without permission

For commercial licensing inquiries, please contact [@AmrEslim](https://github.com/AmrEslim).


**Built with ❤️ for adventurers who refuse to lose their friends**

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue.svg)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-green.svg)](https://www.espressif.com/en/products/socs/esp32)
