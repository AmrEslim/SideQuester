/*
 * SideQuester LoRa Sender Test
 * 
 * Test code to verify LoRa communication using SX1262 module
 * ESP32-WROOM-32 with SX1262 via RadioLib
 * 
 * Hardware Configuration:
 * - ESP32: ESP32-WROOM-32
 * - LoRa: SX1262 Module
 * 
 * Pin Configuration:
 * SX1262 -> ESP32
 * NSS    -> GPIO15 (Chip Select)
 * DIO1   -> GPIO4  (Interrupt)
 * NRST   -> GPIO19 (Reset)
 * BUSY   -> GPIO18 (Busy status)
 * SCK    -> GPIO14 (SPI Clock)
 * MISO   -> GPIO12 (SPI MISO)
 * MOSI   -> GPIO13 (SPI MOSI)
 */

#include <RadioLib.h>
#include <SPI.h>

// SX1262 pin configuration for ESP32-WROOM-32
#define NSS_PIN     15  // Chip Select
#define DIO1_PIN    4   // Interrupt
#define NRST_PIN    19  // Reset
#define BUSY_PIN    18  // Busy status
#define SCK_PIN     14  // SPI Clock  
#define MISO_PIN    12  // SPI MISO
#define MOSI_PIN    13  // SPI MOSI

// LoRa configuration for SideQuester
#define FREQUENCY   915.0   // MHz (915 for US, 868 for EU)
#define BANDWIDTH   125.0   // kHz
#define SPREADING_FACTOR 7  // 7-12
#define CODING_RATE 5       // 5-8
#define OUTPUT_POWER 20     // dBm (max 22 for SX1262)

// Message configuration
#define MESSAGE_INTERVAL 2500  // milliseconds (2.5 seconds)

// Create SX1262 instance
SX1262 radio = new Module(NSS_PIN, DIO1_PIN, NRST_PIN, BUSY_PIN);

// Message counter and timing
unsigned long messageCounter = 0;
unsigned long lastTransmission = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println(F("=== SideQuester LoRa Sender Test ==="));
  Serial.println(F("Initializing SX1262 module..."));
  
  // Configure SPI pins
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, NSS_PIN);
  
  // Initialize SX1262
  Serial.print(F("Initializing SX1262 ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("SUCCESS!"));
  } else {
    Serial.print(F("FAILED, code: "));
    Serial.println(state);
    while (true);
  }
  
  // Configure LoRa parameters
  Serial.println(F("Configuring LoRa parameters..."));
  
  // Set frequency
  if (radio.setFrequency(FREQUENCY) == RADIOLIB_ERR_NONE) {
    Serial.print(F("Frequency set to: "));
    Serial.print(FREQUENCY);
    Serial.println(F(" MHz"));
  } else {
    Serial.println(F("Failed to set frequency!"));
    while (true);
  }
  
  // Set bandwidth
  if (radio.setBandwidth(BANDWIDTH) == RADIOLIB_ERR_NONE) {
    Serial.print(F("Bandwidth set to: "));
    Serial.print(BANDWIDTH);
    Serial.println(F(" kHz"));
  } else {
    Serial.println(F("Failed to set bandwidth!"));
    while (true);
  }
  
  // Set spreading factor
  if (radio.setSpreadingFactor(SPREADING_FACTOR) == RADIOLIB_ERR_NONE) {
    Serial.print(F("Spreading factor set to: "));
    Serial.println(SPREADING_FACTOR);
  } else {
    Serial.println(F("Failed to set spreading factor!"));
    while (true);
  }
  
  // Set coding rate
  if (radio.setCodingRate(CODING_RATE) == RADIOLIB_ERR_NONE) {
    Serial.print(F("Coding rate set to: "));
    Serial.println(CODING_RATE);
  } else {
    Serial.println(F("Failed to set coding rate!"));
    while (true);
  }
  
  // Set output power
  if (radio.setOutputPower(OUTPUT_POWER) == RADIOLIB_ERR_NONE) {
    Serial.print(F("Output power set to: "));
    Serial.print(OUTPUT_POWER);
    Serial.println(F(" dBm"));
  } else {
    Serial.println(F("Failed to set output power!"));
    while (true);
  }
  
  Serial.println(F(""));
  Serial.println(F("LoRa configuration complete!"));
  Serial.println(F("Starting transmission test..."));
  Serial.println(F("Message format: SideQuester_Test_#[counter]_[timestamp]"));
  Serial.println(F("====================================="));
  Serial.println(F(""));
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check if it's time to send a message
  if (currentTime - lastTransmission >= MESSAGE_INTERVAL) {
    messageCounter++;
    
    // Create test message
    String message = "SideQuester_Test_#";
    message += String(messageCounter, DEC);
    message += "_";
    message += String(currentTime);
    
    Serial.print(F("Sending message #"));
    Serial.print(messageCounter);
    Serial.print(F(": "));
    Serial.println(message);
    
    // Send the message
    int state = radio.transmit(message);
    
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("✓ Transmission successful!"));
      
      // Print transmission statistics
      Serial.print(F("  Data rate: "));
      Serial.print(radio.getDataRate());
      Serial.println(F(" bps"));
      
      Serial.print(F("  Output power: "));
      Serial.print(OUTPUT_POWER);
      Serial.println(F(" dBm"));
      
    } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
      Serial.println(F("✗ Transmission failed: packet too long!"));
    } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
      Serial.println(F("✗ Transmission failed: timeout!"));
    } else {
      Serial.print(F("✗ Transmission failed, code: "));
      Serial.println(state);
    }
    
    Serial.println(F(""));
    lastTransmission = currentTime;
  }
  
  // Small delay to prevent overwhelming the serial output
  delay(10);
}