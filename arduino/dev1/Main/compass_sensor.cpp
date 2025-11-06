#include <Wire.h>
#include "compass_sensor.h"

// Sensor I2C Address
#define COMPASS_ADDR 0x0D

// QMC5883L Register Addresses
#define REG_DATA_X_LSB 0x00
#define REG_DATA_X_MSB 0x01
#define REG_DATA_Y_LSB 0x02
#define REG_DATA_Y_MSB 0x03
#define REG_DATA_Z_LSB 0x04
#define REG_DATA_Z_MSB 0x05
#define REG_STATUS 0x06
#define REG_TEMP_LSB 0x07
#define REG_TEMP_MSB 0x08
#define REG_CONTROL1 0x09
#define REG_CONTROL2 0x0A
#define REG_SET_RESET 0x0B

// Control Register 1 Settings
#define MODE_CONTINUOUS 0x01
#define ODR_200HZ 0x0C
#define RNG_8G 0x10
#define OSR_512 0x00

// Smoothing variables
float lastHeading = 0;
const float SMOOTHING_FACTOR = 0.15; // Lower = smoother but slower response

// Forward declarations
void writeRegister(uint8_t reg, uint8_t value);
uint8_t readRegister(uint8_t reg);

//objects
CompassData compass;

// Check if sensor is present on I2C bus
// Checks if the compass sensor is present on the I2C bus.
// Returns true if the sensor acknowledges, false otherwise.
bool checkSensor()
{
    Wire.beginTransmission(COMPASS_ADDR);
    uint8_t error = Wire.endTransmission();
    return (error == 0);
}

// Initialize the compass sensor
bool initSensor()
{
    writeRegister(REG_SET_RESET, 0x01);
    delay(10);

    uint8_t ctrl1 = MODE_CONTINUOUS | ODR_200HZ | RNG_8G | OSR_512;
    writeRegister(REG_CONTROL1, ctrl1);
    delay(10);

    uint8_t readBack = readRegister(REG_CONTROL1);
    return (readBack == ctrl1);
}

// Read compass data from sensor
bool readCompass()
{
    Wire.beginTransmission(COMPASS_ADDR);
    Wire.write(REG_DATA_X_LSB);
    if (Wire.endTransmission() != 0)
    {
        return false;
    }

    Wire.requestFrom(COMPASS_ADDR, 6);
    if (Wire.available() < 6)
    {
        return false;
    }

    uint8_t xLSB = Wire.read();
    uint8_t xMSB = Wire.read();
    compass.x = (int16_t)((xMSB << 8) | xLSB);

    uint8_t yLSB = Wire.read();
    uint8_t yMSB = Wire.read();
    compass.y = (int16_t)((yMSB << 8) | yLSB);

    uint8_t zLSB = Wire.read();
    uint8_t zMSB = Wire.read();
    compass.z = (int16_t)((zMSB << 8) | zLSB);

    return true;
}

// Calculate heading in degrees (0-360)
void calculateHeading()
{
    float heading = atan2((float)compass.y, (float)compass.x);
    heading = heading * 180.0 /  M_PI; // Convert to degrees

    if (heading < 0)
    {
        heading += 360;
    }

    compass.heading = heading;
}

// Smooth heading for stable display
void smoothHeading()
{
    // Handle 360/0 degree boundary
    float diff = compass.heading - lastHeading;
    if (diff > 180)
    {
        diff -= 360;
    }
    else if (diff < -180)
    {
        diff += 360;
    }

    compass.smoothedHeading = lastHeading + diff * SMOOTHING_FACTOR;

    // Normalize
    if (compass.smoothedHeading < 0)
    {
        compass.smoothedHeading += 360;
    }
    else if (compass.smoothedHeading >= 360)
    {
        compass.smoothedHeading -= 360;
    }

    lastHeading = compass.smoothedHeading;
}

// Get compass direction (N, NE, E, SE, S, SW, W, NW)
String getDirection()
{
    float heading = compass.smoothedHeading;

    if (heading >= 337.5 || heading < 22.5)
        return "NORTH";
    else if (heading >= 22.5 && heading < 67.5)
        return "NORTH-EAST";
    else if (heading >= 67.5 && heading < 112.5)
        return "EAST";
    else if (heading >= 112.5 && heading < 157.5)
        return "SOUTH-EAST";
    else if (heading >= 157.5 && heading < 202.5)
        return "SOUTH";
    else if (heading >= 202.5 && heading < 247.5)
        return "SOUTH-WEST";
    else if (heading >= 247.5 && heading < 292.5)
        return "WEST";
    else if (heading >= 292.5 && heading < 337.5)
        return "NORTH-WEST";

    return "UNKNOWN";
}

// Write a byte to a register
void writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(COMPASS_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Read a byte from a register
uint8_t readRegister(uint8_t reg)
{
    Wire.beginTransmission(COMPASS_ADDR);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(COMPASS_ADDR, 1);
    if (Wire.available())
    {
        return Wire.read();
    }
    return 0;
}
