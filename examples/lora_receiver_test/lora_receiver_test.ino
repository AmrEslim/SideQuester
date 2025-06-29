/*
 * SideQuester LoRa Receiver Test
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

// Create SX1262 instance
SX1262 radio = new Module(NSS_PIN, DIO1_PIN, NRST_PIN, BUSY_PIN);

// Reception statistics
unsigned long totalPacketsReceived = 0;
unsigned long lastPacketTime = 0;
float lastRSSI = 0;
float lastSNR = 0;

// Flag to indicate if packet was received (used in interrupt)
volatile bool receivedFlag = false;

// Function called when packet is received (interrupt)
void setFlag() {
  receivedFlag = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println(F("=== SideQuester LoRa Receiver Test ==="));
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
  
  // Set output power (needed for proper initialization)
  if (radio.setOutputPower(OUTPUT_POWER) == RADIOLIB_ERR_NONE) {
    Serial.print(F("Output power set to: "));
    Serial.print(OUTPUT_POWER);
    Serial.println(F(" dBm"));
  } else {
    Serial.println(F("Failed to set output power!"));
    while (true);
  }
  
  // Set the function that will be called when packet is received
  radio.setDio1Action(setFlag);
  
  Serial.println(F(""));
  Serial.println(F("LoRa configuration complete!"));
  Serial.println(F("Starting receiver mode..."));
  Serial.println(F("Listening for SideQuester test messages..."));
  Serial.println(F("=========================================="));
  Serial.println(F(""));
  
  // Start listening for packets
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("✓ Receiver mode started successfully!"));
    Serial.println(F("Waiting for incoming messages..."));
    Serial.println(F(""));
  } else {
    Serial.print(F("✗ Failed to start receiver mode, code: "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  // Check if a packet was received
  if (receivedFlag) {
    // Clear the flag
    receivedFlag = false;
    
    // Read the received data
    String receivedMessage;
    int state = radio.readData(receivedMessage);
    
    if (state == RADIOLIB_ERR_NONE) {
      totalPacketsReceived++;
      lastPacketTime = millis();
      
      // Get signal quality metrics
      lastRSSI = radio.getRSSI();
      lastSNR = radio.getSNR();
      
      Serial.println(F("===== MESSAGE RECEIVED ====="));
      Serial.print(F("Packet #"));
      Serial.println(totalPacketsReceived);
      Serial.print(F("Message: "));
      Serial.println(receivedMessage);
      Serial.print(F("RSSI: "));
      Serial.print(lastRSSI);
      Serial.println(F(" dBm"));
      Serial.print(F("SNR: "));
      Serial.print(lastSNR);
      Serial.println(F(" dB"));
      Serial.print(F("Frequency error: "));
      Serial.print(radio.getFrequencyError());
      Serial.println(F(" Hz"));
      Serial.print(F("Timestamp: "));
      Serial.println(lastPacketTime);
      
      // Parse and validate message format
      if (receivedMessage.startsWith("SideQuester_Test_#")) {
        Serial.println(F("✓ Valid SideQuester test message format"));
        
        // Extract message counter if possible
        int counterStart = receivedMessage.indexOf('#') + 1;
        int counterEnd = receivedMessage.indexOf('_', counterStart);
        if (counterStart > 0 && counterEnd > counterStart) {
          String counterStr = receivedMessage.substring(counterStart, counterEnd);
          Serial.print(F("Message counter: "));
          Serial.println(counterStr);
        }
        
        // Extract timestamp if possible
        int timestampStart = receivedMessage.lastIndexOf('_') + 1;
        if (timestampStart > 0 && timestampStart < receivedMessage.length()) {
          String timestampStr = receivedMessage.substring(timestampStart);
          Serial.print(F("Sender timestamp: "));
          Serial.println(timestampStr);
        }
      } else {
        Serial.println(F("⚠ Non-SideQuester message received"));
      }
      
      Serial.println(F("============================"));
      Serial.println(F(""));
      
    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      Serial.println(F("✗ CRC error in received packet!"));
    } else {
      Serial.print(F("✗ Failed to read packet, code: "));
      Serial.println(state);
    }
    
    // Start listening for next packet
    radio.startReceive();
  }
  
  // Print statistics every 30 seconds
  static unsigned long lastStatsTime = 0;
  if (millis() - lastStatsTime >= 30000) {
    Serial.println(F("===== RECEPTION STATISTICS ====="));
    Serial.print(F("Total packets received: "));
    Serial.println(totalPacketsReceived);
    Serial.print(F("Uptime: "));
    Serial.print(millis() / 1000);
    Serial.println(F(" seconds"));
    
    if (totalPacketsReceived > 0) {
      Serial.print(F("Last packet received: "));
      Serial.print((millis() - lastPacketTime) / 1000);
      Serial.println(F(" seconds ago"));
      Serial.print(F("Last RSSI: "));
      Serial.print(lastRSSI);
      Serial.println(F(" dBm"));
      Serial.print(F("Last SNR: "));
      Serial.print(lastSNR);
      Serial.println(F(" dB"));
    } else {
      Serial.println(F("No packets received yet"));
    }
    
    Serial.println(F("================================="));
    Serial.println(F(""));
    lastStatsTime = millis();
  }
  
  // Small delay to prevent overwhelming the CPU
  delay(1);
}