# Software Setup Guide

This guide covers software installation, configuration, and programming for SideQuester.

## 🛠️ Development Environment Setup

### 1. Arduino IDE Installation
1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install ESP32 board support:
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://espressif.github.io/arduino-esp32/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search for "ESP32" and install "esp32 by Espressif Systems"

### 2. Required Libraries

Install these libraries via Arduino IDE Library Manager (Tools → Manage Libraries):

```cpp
#include <SoftwareSerial.h>    // For LoRa and GPS communication
#include <TinyGPS++.h>         // GPS parsing
#include <Wire.h>              // I2C communication  
#include <Adafruit_SSD1306.h>  // OLED display
#include <Adafruit_HMC5883_U.h> // Magnetometer
```

**Library Installation:**
- `TinyGPS++` by Mikal Hart
- `Adafruit SSD1306` by Adafruit
- `Adafruit Unified Sensor` by Adafruit  
- `Adafruit HMC5883 Unified` by Adafruit

## ⚙️ Configuration Parameters

### Device Configuration
```cpp
// Device Configuration
#define DEVICE_ID 1         // Set to 1 for first device, 2 for second
#define DEVICE_NAME "SideQuester_01"

// LoRa Configuration
#define LORA_FREQUENCY 868  // MHz (adjust for region: 868 EU, 915 US)
#define LORA_POWER 30       // dBm transmission power
#define LORA_CHANNEL 23     // Communication channel
#define LORA_ADDRESS 0x1234 // Device address

// GPS Configuration
#define GPS_BAUD 9600
#define GPS_UPDATE_RATE 1000  // milliseconds between updates
#define GPS_TIMEOUT 30000     // GPS acquisition timeout

// Compass Configuration  
#define COMPASS_DECLINATION 4.5  // Magnetic declination for your location
#define COMPASS_CALIBRATION_SAMPLES 50

// Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1       // Reset pin (-1 if not used)
#define SCREEN_ADDRESS 0x3C // I2C address
```

### Regional Frequency Settings
```cpp
// United States / Canada
#define LORA_FREQUENCY 915

// Europe / Asia  
#define LORA_FREQUENCY 868

// Check your local regulations!
```

## 📡 Communication Protocol

### Data Packet Structure
```cpp
struct LocationPacket {
  uint8_t deviceId;      // Device identifier
  float latitude;        // GPS latitude
  float longitude;       // GPS longitude  
  uint32_t timestamp;    // Unix timestamp
  uint8_t checksum;      // Data integrity check
};
```

### Message Types
- `0x01`: Location update
- `0x02`: Status request
- `0x03`: Acknowledgment
- `0xFF`: Emergency beacon

## 🧭 Mathematical Calculations

### Haversine Formula (Distance)
```cpp
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  const float R = 6371000; // Earth radius in meters
  float phi1 = radians(lat1);
  float phi2 = radians(lat2);
  float deltaPhi = radians(lat2 - lat1);
  float deltaLambda = radians(lon2 - lon1);
  
  float a = sin(deltaPhi/2) * sin(deltaPhi/2) +
            cos(phi1) * cos(phi2) *
            sin(deltaLambda/2) * sin(deltaLambda/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  
  return R * c; // Distance in meters
}
```

### Bearing Calculation (Direction)
```cpp
float calculateBearing(float lat1, float lon1, float lat2, float lon2) {
  float phi1 = radians(lat1);
  float phi2 = radians(lat2);
  float deltaLambda = radians(lon2 - lon1);
  
  float y = sin(deltaLambda) * cos(phi2);
  float x = cos(phi1) * sin(phi2) - 
            sin(phi1) * cos(phi2) * cos(deltaLambda);
  
  float bearing = degrees(atan2(y, x));
  return fmod((bearing + 360), 360); // Normalize to 0-360°
}
```

## 📱 Display Layout

### Main Screen Format
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

### Navigation Screen Layout
```
┌─────────────────────┐
│     NAVIGATION      │
│                     │
│     ↗ Device_02     │
│     1.2km           │
│                     │
│    YOU    ↙ Device_03│
│     ●      0.5km    │
│                     │
│ ← Device_04  Device_05│
│   2.1km      ↑ 0.8km │
└─────────────────────┘
```

### Compass Navigation Display
```
┌─────────────────────┐
│        N            │
│       ↑             │
│   W ← ● → E         │
│       ↓             │
│        S            │
│                     │
│ Target: Device_02   │
│ Bearing: 045° (NE)  │
│ Distance: 1.2km     │
└─────────────────────┘
```

### Arrow Direction Indicators
- `↑` North (0°)
- `↗` Northeast (45°)
- `→` East (90°)
- `↘` Southeast (135°)
- `↓` South (180°)
- `↙` Southwest (225°)
- `←` West (270°)
- `↖` Northwest (315°)
```

### Status Indicators
- **GPS Status**: SEARCHING, FIXED, LOST
- **LoRa Status**: CONNECTING, CONNECTED, LOST
- **Battery**: Percentage and voltage
- **Friend Location**: Distance and cardinal direction

## 🔄 System States

### Device States
1. **INIT**: Initializing hardware modules
2. **GPS_SEARCH**: Acquiring GPS fix
3. **PAIRING**: Establishing LoRa connection  
4. **ACTIVE**: Normal operation mode
5. **LOW_POWER**: Battery saving mode
6. **ERROR**: Hardware failure state

### Error Handling
```cpp
enum ErrorCode {
  ERR_NONE = 0,
  ERR_GPS_TIMEOUT = 1,
  ERR_LORA_INIT = 2,
  ERR_COMPASS_INIT = 3,
  ERR_DISPLAY_INIT = 4,
  ERR_LOW_BATTERY = 5
};
```

## 📤 Upload Process

### 1. Board Selection
- Board: "ESP32 Dev Module"
- CPU Frequency: "240MHz (WiFi/BT)"
- Flash Frequency: "80MHz"
- Flash Size: "4MB"
- Partition Scheme: "Default"

### 2. Device Programming
1. Connect ESP32 via USB
2. Select correct COM port
3. Set DEVICE_ID (1 for first device, 2 for second)
4. Upload sketch to both devices
5. Open Serial Monitor for debugging

### 3. Initial Testing
1. Power both devices outdoors
2. Wait for GPS fix (LED indicator or serial output)
3. Verify LoRa communication
4. Check compass calibration
5. Test display functionality

## 🔍 Debugging

### Serial Monitor Output
```
SideQuester v1.0 Starting...
GPS: Searching for satellites...
LoRa: Initializing on 915MHz...  
Compass: Calibrating magnetometer...
Display: OLED ready
GPS: Fix acquired (5 satellites)
LoRa: Connected to peer device
System: Ready for operation
```

### Common Debug Commands
- `GPS_STATUS`: Check GPS fix and satellite count
- `LORA_TEST`: Send test packet
- `COMPASS_CAL`: Start compass calibration
- `BATTERY_CHECK`: Display voltage and percentage

## 🔋 Power Management

### Sleep Modes
```cpp
// Light sleep (GPS active, LoRa periodic)
esp_sleep_enable_timer_wakeup(1000000); // 1 second
esp_light_sleep_start();

// Deep sleep (emergency power saving)
esp_sleep_enable_timer_wakeup(30000000); // 30 seconds  
esp_deep_sleep_start();
```

### Battery Monitoring
```cpp
float readBatteryVoltage() {
  int rawValue = analogRead(A0);
  float voltage = (rawValue / 4095.0) * 3.3 * 2; // Voltage divider
  return voltage;
}
```

## 🎯 Performance Tuning

### GPS Optimization
- Use GPS module's power saving modes
- Adjust update rate based on movement
- Implement predictive location caching

### LoRa Optimization  
- Adjust transmission power for range/battery balance
- Implement automatic retry with exponential backoff
- Use acknowledgment system for critical messages

### Display Optimization
- Reduce refresh rate when stationary
- Implement screen timeout
- Use partial screen updates

## 📚 Code Examples

See the `examples/` directory for:
- `gps_test/`: Basic GPS functionality
- `lora_test/`: LoRa communication testing
- `compass_test/`: Magnetometer calibration
- `display_test/`: OLED display examples
- `integration_test/`: Full system testing

## 🐛 Troubleshooting

For software-related issues, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md).