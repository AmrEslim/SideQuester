/*
 * Compass Sensor Test (I2C Address: 0x0D)
 * Reads magnetometer data from compass sensor
 * Likely QMC5883L or compatible sensor
 */

#include <Wire.h>

// Sensor I2C Address
#define COMPASS_ADDR 0x0D

// QMC5883L Register Addresses
#define REG_DATA_X_LSB  0x00
#define REG_DATA_X_MSB  0x01
#define REG_DATA_Y_LSB  0x02
#define REG_DATA_Y_MSB  0x03
#define REG_DATA_Z_LSB  0x04
#define REG_DATA_Z_MSB  0x05
#define REG_STATUS      0x06
#define REG_TEMP_LSB    0x07
#define REG_TEMP_MSB    0x08
#define REG_CONTROL1    0x09
#define REG_CONTROL2    0x0A
#define REG_SET_RESET   0x0B

// Control Register 1 Settings
#define MODE_CONTINUOUS 0x01
#define ODR_200HZ       0x0C  // Output Data Rate
#define RNG_8G          0x10  // Range ±8 Gauss
#define OSR_512         0x00  // Over Sample Ratio

// Compass data structure
struct CompassData {
  int16_t x;
  int16_t y;
  int16_t z;
  float heading;
};

CompassData compass;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("Compass Sensor Test - I2C Address 0x0D");
  Serial.println("========================================");
  
  // Initialize I2C
  Wire.begin();
  
  // Wait for sensor to stabilize
  delay(100);
  
  // Check if sensor is connected
  if (!checkSensor()) {
    Serial.println("ERROR: Compass sensor not found at address 0x0D!");
    Serial.println("Check your wiring:");
    Serial.println("  - SDA pin connected");
    Serial.println("  - SCL pin connected");
    Serial.println("  - VCC connected (3.3V or 5V depending on module)");
    Serial.println("  - GND connected");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("Compass sensor detected!");
  
  // Initialize sensor
  if (initSensor()) {
    Serial.println("Sensor initialized successfully");
  } else {
    Serial.println("ERROR: Failed to initialize sensor!");
    while (1) {
      delay(1000);
    }
  }
  
  delay(100);
  Serial.println("\nStarting compass readings...\n");
}

void loop() {
  // Read compass data
  if (readCompass()) {
    // Calculate heading
    calculateHeading();
    
    // Print data
    printCompassData();
  } else {
    Serial.println("ERROR: Failed to read compass data");
  }
  
  delay(500); // Read every 500ms
}

// Check if sensor is present on I2C bus
bool checkSensor() {
  Wire.beginTransmission(COMPASS_ADDR);
  byte error = Wire.endTransmission();
  return (error == 0);
}

// Initialize the compass sensor
bool initSensor() {
  // Set/Reset Period
  writeRegister(REG_SET_RESET, 0x01);
  
  delay(10);
  
  // Configure Control Register 1
  // Mode: Continuous | ODR: 200Hz | Range: 8G | OSR: 512
  uint8_t ctrl1 = MODE_CONTINUOUS | ODR_200HZ | RNG_8G | OSR_512;
  writeRegister(REG_CONTROL1, ctrl1);
  
  delay(10);
  
  // Verify configuration
  uint8_t readBack = readRegister(REG_CONTROL1);
  return (readBack == ctrl1);
}

// Read compass data from sensor
bool readCompass() {
  Wire.beginTransmission(COMPASS_ADDR);
  Wire.write(REG_DATA_X_LSB);
  if (Wire.endTransmission() != 0) {
    return false;
  }
  
  // Request 6 bytes (X, Y, Z - LSB and MSB for each)
  Wire.requestFrom(COMPASS_ADDR, 6);
  
  if (Wire.available() < 6) {
    return false;
  }
  
  // Read X axis
  uint8_t xLSB = Wire.read();
  uint8_t xMSB = Wire.read();
  compass.x = (int16_t)((xMSB << 8) | xLSB);
  
  // Read Y axis
  uint8_t yLSB = Wire.read();
  uint8_t yMSB = Wire.read();
  compass.y = (int16_t)((yMSB << 8) | yLSB);
  
  // Read Z axis
  uint8_t zLSB = Wire.read();
  uint8_t zMSB = Wire.read();
  compass.z = (int16_t)((zMSB << 8) | zLSB);
  
  return true;
}

// Calculate heading in degrees (0-360)
void calculateHeading() {
  // Calculate heading using atan2
  float heading = atan2((float)compass.y, (float)compass.x);
  
  // Convert radians to degrees
  heading = heading * 180.0 / PI;
  
  // Normalize to 0-360
  if (heading < 0) {
    heading += 360;
  }
  
  compass.heading = heading;
}

// Print compass data to serial
void printCompassData() {
  Serial.print("X: ");
  Serial.print(compass.x);
  Serial.print("\tY: ");
  Serial.print(compass.y);
  Serial.print("\tZ: ");
  Serial.print(compass.z);
  Serial.print("\t| Heading: ");
  Serial.print(compass.heading, 1);
  Serial.print("° (");
  Serial.print(getDirection());
  Serial.println(")");
}

// Get compass direction (N, NE, E, SE, S, SW, W, NW)
String getDirection() {
  float heading = compass.heading;
  
  if (heading >= 337.5 || heading < 22.5) return "N";
  else if (heading >= 22.5 && heading < 67.5) return "NE";
  else if (heading >= 67.5 && heading < 112.5) return "E";
  else if (heading >= 112.5 && heading < 157.5) return "SE";
  else if (heading >= 157.5 && heading < 202.5) return "S";
  else if (heading >= 202.5 && heading < 247.5) return "SW";
  else if (heading >= 247.5 && heading < 292.5) return "W";
  else if (heading >= 292.5 && heading < 337.5) return "NW";
  
  return "?";
}

// Write a byte to a register
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(COMPASS_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Read a byte from a register
uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(COMPASS_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(COMPASS_ADDR, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}
