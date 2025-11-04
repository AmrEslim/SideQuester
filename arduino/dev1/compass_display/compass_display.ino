/*
 * Compass Display with TFT Round Display
 * Combines QMC5883L compass sensor with TFT_eSPI display
 * Shows real-time compass heading with visual interface
 */

#include <Wire.h>
#include <TFT_eSPI.h>

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
#define ODR_200HZ       0x0C
#define RNG_8G          0x10
#define OSR_512         0x00

// Display settings for round display
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240
#define CENTER_X      120
#define CENTER_Y      120
#define COMPASS_RADIUS 100
#define NEEDLE_LENGTH  80

// Colors
#define COLOR_BACKGROUND  0x0000  // Black
#define COLOR_COMPASS     0x2945  // Dark blue-grey
#define COLOR_CARDINAL    0xF800  // Red for N/S/E/W
#define COLOR_TEXT        0xFFFF  // White
#define COLOR_NEEDLE      0xF800  // Red
#define COLOR_NEEDLE_TIP  0xFFE0  // Yellow
#define COLOR_CIRCLE      0x4208  // Grey

// Compass data structure
struct CompassData {
  int16_t x;
  int16_t y;
  int16_t z;
  float heading;
  float smoothedHeading;
};

CompassData compass;
TFT_eSPI tft = TFT_eSPI();

// Smoothing variables
float lastHeading = 0;
const float SMOOTHING_FACTOR = 0.15; // Lower = smoother but slower response

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("Compass Display Initializing...");
  
  // Initialize display
  tft.init();
  tft.setRotation(0); // Adjust based on your display orientation
  tft.fillScreen(COLOR_BACKGROUND);
  
  // Show initialization message
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.setTextDatum(MC_DATUM); // Middle center
  tft.setTextSize(2);
  tft.drawString("COMPASS", CENTER_X, CENTER_Y - 20);
  tft.setTextSize(1);
  tft.drawString("Initializing...", CENTER_X, CENTER_Y + 10);
  
  // Initialize I2C
  Wire.begin();
  delay(100);
  
  // Check if sensor is connected
  if (!checkSensor()) {
    Serial.println("ERROR: Compass sensor not found!");
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.drawString("SENSOR ERROR", CENTER_X, CENTER_Y - 10);
    tft.drawString("Check wiring", CENTER_X, CENTER_Y + 10);
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("Compass sensor detected!");
  
  // Initialize sensor
  if (!initSensor()) {
    Serial.println("ERROR: Failed to initialize sensor!");
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.drawString("INIT ERROR", CENTER_X, CENTER_Y);
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("Sensor initialized successfully");
  delay(500);
  
  // Draw static compass elements
  drawCompassBase();
  
  Serial.println("Starting compass display...");
}

void loop() {
  // Read compass data
  if (readCompass()) {
    // Calculate heading
    calculateHeading();
    
    // Smooth the heading for better visual stability
    smoothHeading();
    
    // Update display
    updateCompassDisplay();
    
    // Print to serial for debugging
    printCompassData();
  } else {
    Serial.println("ERROR: Failed to read compass data");
  }
  
  delay(100); // Update every 100ms
}

// Check if sensor is present on I2C bus
bool checkSensor() {
  Wire.beginTransmission(COMPASS_ADDR);
  byte error = Wire.endTransmission();
  return (error == 0);
}

// Initialize the compass sensor
bool initSensor() {
  writeRegister(REG_SET_RESET, 0x01);
  delay(10);
  
  uint8_t ctrl1 = MODE_CONTINUOUS | ODR_200HZ | RNG_8G | OSR_512;
  writeRegister(REG_CONTROL1, ctrl1);
  delay(10);
  
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
  
  Wire.requestFrom(COMPASS_ADDR, 6);
  if (Wire.available() < 6) {
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
void calculateHeading() {
  float heading = atan2((float)compass.y, (float)compass.x);
  heading = heading * 180.0 / PI;
  
  if (heading < 0) {
    heading += 360;
  }
  
  compass.heading = heading;
}

// Smooth heading for stable display
void smoothHeading() {
  // Handle 360/0 degree boundary
  float diff = compass.heading - lastHeading;
  if (diff > 180) {
    diff -= 360;
  } else if (diff < -180) {
    diff += 360;
  }
  
  compass.smoothedHeading = lastHeading + diff * SMOOTHING_FACTOR;
  
  // Normalize
  if (compass.smoothedHeading < 0) {
    compass.smoothedHeading += 360;
  } else if (compass.smoothedHeading >= 360) {
    compass.smoothedHeading -= 360;
  }
  
  lastHeading = compass.smoothedHeading;
}

// Draw the static compass base (only called once)
void drawCompassBase() {
  tft.fillScreen(COLOR_BACKGROUND);
  
  // Draw outer circle
  tft.drawCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS, COLOR_CIRCLE);
  tft.drawCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS - 1, COLOR_CIRCLE);
  
  // Draw inner circle
  tft.drawCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS - 10, COLOR_CIRCLE);
  
  // Draw cardinal direction markers and labels
  drawCardinalMarkers();
  
  // Draw center dot
  tft.fillCircle(CENTER_X, CENTER_Y, 5, COLOR_CIRCLE);
}

// Draw cardinal direction markers (N, S, E, W)
void drawCardinalMarkers() {
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  
  // North (0°)
  tft.setTextColor(COLOR_CARDINAL, COLOR_BACKGROUND);
  tft.drawString("N", CENTER_X, CENTER_Y - COMPASS_RADIUS + 15);
  drawTick(0, 15, COLOR_CARDINAL);
  
  // East (90°)
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.drawString("E", CENTER_X + COMPASS_RADIUS - 15, CENTER_Y);
  drawTick(90, 15, COLOR_TEXT);
  
  // South (180°)
  tft.drawString("S", CENTER_X, CENTER_Y + COMPASS_RADIUS - 15);
  drawTick(180, 15, COLOR_TEXT);
  
  // West (270°)
  tft.drawString("W", CENTER_X - COMPASS_RADIUS + 15, CENTER_Y);
  drawTick(270, 15, COLOR_TEXT);
  
  // Draw intermediate ticks
  tft.setTextSize(1);
  for (int angle = 0; angle < 360; angle += 30) {
    if (angle % 90 != 0) { // Skip cardinal directions
      drawTick(angle, 8, COLOR_CIRCLE);
    }
  }
}

// Draw a tick mark at specified angle
void drawTick(float angle, int length, uint16_t color) {
  float rad = (angle - 90) * PI / 180.0;
  int x1 = CENTER_X + (COMPASS_RADIUS - length) * cos(rad);
  int y1 = CENTER_Y + (COMPASS_RADIUS - length) * sin(rad);
  int x2 = CENTER_X + COMPASS_RADIUS * cos(rad);
  int y2 = CENTER_Y + COMPASS_RADIUS * sin(rad);
  tft.drawLine(x1, y1, x2, y2, color);
}

// Update compass display with new heading
void updateCompassDisplay() {
  static float lastDisplayHeading = -1;
  
  // Only update if heading changed significantly (reduces flicker)
  if (abs(compass.smoothedHeading - lastDisplayHeading) < 0.5) {
    return;
  }
  
  // Redraw the rotating compass rose
  drawRotatingCompass(compass.smoothedHeading);
  
  // Draw static needle pointing up
  drawStaticNeedle();
  
  // Update heading text
  displayHeadingText();
  
  lastDisplayHeading = compass.smoothedHeading;
}

// Draw static needle pointing up (showing our heading direction)
void drawStaticNeedle() {
  // Needle always points straight up (0 degrees / North position)
  int needleX = CENTER_X;
  int needleY = CENTER_Y - NEEDLE_LENGTH;
  
  // Draw main needle line (thicker) - pointing up
  tft.drawLine(CENTER_X, CENTER_Y, needleX, needleY, COLOR_NEEDLE);
  tft.drawLine(CENTER_X + 1, CENTER_Y, needleX + 1, needleY, COLOR_NEEDLE);
  tft.drawLine(CENTER_X, CENTER_Y + 1, needleX, needleY + 1, COLOR_NEEDLE);
  
  // Draw needle tip (make it stand out)
  tft.fillCircle(needleX, needleY, 4, COLOR_NEEDLE_TIP);
  tft.drawCircle(needleX, needleY, 4, COLOR_NEEDLE);
  
  // Draw opposite end of needle (shorter) - pointing down
  int backX = CENTER_X;
  int backY = CENTER_Y + (NEEDLE_LENGTH / 3);
  tft.drawLine(CENTER_X, CENTER_Y, backX, backY, COLOR_NEEDLE);
}

// Draw rotating compass rose based on heading
void drawRotatingCompass(float heading) {
  // Clear the compass area (but not the text area at bottom)
  tft.fillCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS + 2, COLOR_BACKGROUND);
  
  // Draw outer circle
  tft.drawCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS, COLOR_CIRCLE);
  tft.drawCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS - 1, COLOR_CIRCLE);
  
  // Draw inner circle
  tft.drawCircle(CENTER_X, CENTER_Y, COMPASS_RADIUS - 10, COLOR_CIRCLE);
  
  // Draw rotated cardinal direction markers and labels
  drawRotatedCardinalMarkers(heading);
  
  // Draw center dot
  tft.fillCircle(CENTER_X, CENTER_Y, 5, COLOR_CIRCLE);
}

// Draw cardinal direction markers rotated by the heading
void drawRotatedCardinalMarkers(float heading) {
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  
  // The compass rose rotates opposite to our heading
  // If we're facing 90° (East), North should be rotated 90° counter-clockwise (to the left)
  float rotation = -heading;
  
  // North (0°) - Red
  float northAngle = rotation;
  drawRotatedLabel("N", northAngle, COLOR_CARDINAL);
  drawRotatedTick(northAngle, 15, COLOR_CARDINAL);
  
  // East (90°)
  float eastAngle = rotation + 90;
  drawRotatedLabel("E", eastAngle, COLOR_TEXT);
  drawRotatedTick(eastAngle, 15, COLOR_TEXT);
  
  // South (180°)
  float southAngle = rotation + 180;
  drawRotatedLabel("S", southAngle, COLOR_TEXT);
  drawRotatedTick(southAngle, 15, COLOR_TEXT);
  
  // West (270°)
  float westAngle = rotation + 270;
  drawRotatedLabel("W", westAngle, COLOR_TEXT);
  drawRotatedTick(westAngle, 15, COLOR_TEXT);
  
  // Draw intermediate ticks
  tft.setTextSize(1);
  for (int angle = 0; angle < 360; angle += 30) {
    if (angle % 90 != 0) { // Skip cardinal directions
      drawRotatedTick(rotation + angle, 8, COLOR_CIRCLE);
    }
  }
}

// Draw a label at a rotated position
void drawRotatedLabel(const char* label, float angle, uint16_t color) {
  float rad = (angle - 90) * PI / 180.0;
  int labelX = CENTER_X + (COMPASS_RADIUS - 15) * cos(rad);
  int labelY = CENTER_Y + (COMPASS_RADIUS - 15) * sin(rad);
  
  tft.setTextColor(color, COLOR_BACKGROUND);
  tft.drawString(label, labelX, labelY);
}

// Draw a tick mark at rotated angle
void drawRotatedTick(float angle, int length, uint16_t color) {
  float rad = (angle - 90) * PI / 180.0;
  int x1 = CENTER_X + (COMPASS_RADIUS - length) * cos(rad);
  int y1 = CENTER_Y + (COMPASS_RADIUS - length) * sin(rad);
  int x2 = CENTER_X + COMPASS_RADIUS * cos(rad);
  int y2 = CENTER_Y + COMPASS_RADIUS * sin(rad);
  tft.drawLine(x1, y1, x2, y2, color);
}

// Display heading text at bottom
void displayHeadingText() {
  // Clear text area
  tft.fillRect(CENTER_X - 60, CENTER_Y + COMPASS_RADIUS + 10, 120, 30, COLOR_BACKGROUND);
  
  // Display numeric heading
  tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  
  char headingStr[10];
  sprintf(headingStr, "%3.0f°", compass.smoothedHeading);
  tft.drawString(headingStr, CENTER_X, CENTER_Y + COMPASS_RADIUS + 20);
  
  // Display direction
  tft.setTextSize(1);
  tft.drawString(getDirection(), CENTER_X, CENTER_Y + COMPASS_RADIUS + 35);
}

// Get compass direction (N, NE, E, SE, S, SW, W, NW)
String getDirection() {
  float heading = compass.smoothedHeading;
  
  if (heading >= 337.5 || heading < 22.5) return "NORTH";
  else if (heading >= 22.5 && heading < 67.5) return "NORTH-EAST";
  else if (heading >= 67.5 && heading < 112.5) return "EAST";
  else if (heading >= 112.5 && heading < 157.5) return "SOUTH-EAST";
  else if (heading >= 157.5 && heading < 202.5) return "SOUTH";
  else if (heading >= 202.5 && heading < 247.5) return "SOUTH-WEST";
  else if (heading >= 247.5 && heading < 292.5) return "WEST";
  else if (heading >= 292.5 && heading < 337.5) return "NORTH-WEST";
  
  return "UNKNOWN";
}

// Print compass data to serial (for debugging)
void printCompassData() {
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint < 500) return; // Print every 500ms
  lastPrint = millis();
  
  Serial.print("X: ");
  Serial.print(compass.x);
  Serial.print("\tY: ");
  Serial.print(compass.y);
  Serial.print("\tZ: ");
  Serial.print(compass.z);
  Serial.print("\t| Heading: ");
  Serial.print(compass.smoothedHeading, 1);
  Serial.print("° (");
  Serial.print(getDirection());
  Serial.println(")");
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
