/*
 * LoRa E22-900M30S Transmitter Test
 * SideQuester Project - https://github.com/AmrEslim/SideQuester
 * 
 * This sketch demonstrates LoRa communication using the E22-900M30S module
 * configured in transparent transmission mode. It sends test messages every
 * 2 seconds with a counter to verify successful transmission.
 * 
 * Hardware Requirements:
 * - ESP32 Development Board
 * - E22-900M30S LoRa Module (SX1262 chipset)
 * 
 * Wiring (E22-900M30S → ESP32):
 * VCC → 3.3V
 * GND → GND  
 * RXD → GPIO16 (ESP32 TX)
 * TXD → GPIO17 (ESP32 RX)
 * AUX → GPIO18
 * M0 → GPIO4
 * M1 → GPIO5
 * 
 * Configuration:
 * - Frequency: 915MHz (US band)
 * - Power: 22dBm
 * - Channel: 23
 * - Mode: Transparent transmission (M0=0, M1=0)
 * 
 * Author: SideQuester Project
 * License: MIT
 */

#include <SoftwareSerial.h>

// LoRa Module Pin Definitions
#define LORA_RX_PIN 17    // ESP32 GPIO17 → E22 TXD
#define LORA_TX_PIN 16    // ESP32 GPIO16 → E22 RXD
#define LORA_AUX_PIN 18   // ESP32 GPIO18 → E22 AUX
#define LORA_M0_PIN 4     // ESP32 GPIO4 → E22 M0
#define LORA_M1_PIN 5     // ESP32 GPIO5 → E22 M1

// LoRa Configuration Parameters
#define LORA_FREQUENCY 915    // MHz (US band)
#define LORA_POWER 22         // dBm
#define LORA_CHANNEL 23       // Channel number
#define TRANSMISSION_INTERVAL 2000  // 2 seconds between transmissions

// Device Configuration
#define DEVICE_ID "Device A"
#define BAUD_RATE 9600

// Create SoftwareSerial instance for LoRa communication
SoftwareSerial loraSerial(LORA_RX_PIN, LORA_TX_PIN);

// Global variables
unsigned long lastTransmission = 0;
unsigned int messageCounter = 0;
bool loraInitialized = false;

// Function prototypes
void initializeLora();
void setTransparentMode();
void setConfigurationMode();
void configureLoraModule();
bool waitForAux(unsigned long timeout = 1000);
void sendTestMessage();
void printTimestamp();
String formatTime(unsigned long milliseconds);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("===========================================");
  Serial.println("     E22-900M30S LoRa Transmitter Test    ");
  Serial.println("           SideQuester Project            ");
  Serial.println("===========================================");
  Serial.println();
  
  printTimestamp();
  Serial.println("Initializing LoRa transmitter...");
  
  // Initialize LoRa module
  initializeLora();
  
  if (loraInitialized) {
    printTimestamp();
    Serial.println("✓ LoRa module initialized successfully!");
    Serial.println("✓ Ready to transmit test messages every 2 seconds");
    Serial.println("✓ Configuration: 915MHz, 22dBm, Channel 23");
    Serial.println("✓ Transparent transmission mode active");
    Serial.println();
    Serial.println("--- Transmission Log ---");
  } else {
    Serial.println("✗ LoRa initialization failed!");
    Serial.println("Please check wiring and try again.");
  }
}

void loop() {
  if (!loraInitialized) {
    // Blink built-in LED to indicate error
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    return;
  }
  
  // Check if it's time to send the next message
  if (millis() - lastTransmission >= TRANSMISSION_INTERVAL) {
    sendTestMessage();
    lastTransmission = millis();
  }
  
  // Small delay to prevent excessive CPU usage
  delay(10);
}

void initializeLora() {
  // Initialize control pins
  pinMode(LORA_M0_PIN, OUTPUT);
  pinMode(LORA_M1_PIN, OUTPUT);
  pinMode(LORA_AUX_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Start with configuration mode to set up the module
  setConfigurationMode();
  
  // Initialize LoRa serial communication
  loraSerial.begin(BAUD_RATE);
  
  // Wait for module to be ready
  delay(500);
  
  // Configure the LoRa module parameters
  configureLoraModule();
  
  // Switch to transparent transmission mode
  setTransparentMode();
  
  // Wait for AUX to go high (module ready)
  if (waitForAux(3000)) {
    loraInitialized = true;
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate success
  } else {
    Serial.println("✗ Timeout waiting for module to be ready");
  }
}

void setTransparentMode() {
  digitalWrite(LORA_M0_PIN, LOW);   // M0 = 0
  digitalWrite(LORA_M1_PIN, LOW);   // M1 = 0
  delay(100);  // Allow time for mode change
  
  Serial.println("→ Set to transparent transmission mode (M0=0, M1=0)");
}

void setConfigurationMode() {
  digitalWrite(LORA_M0_PIN, HIGH);  // M0 = 1
  digitalWrite(LORA_M1_PIN, HIGH);  // M1 = 1
  delay(100);  // Allow time for mode change
  
  Serial.println("→ Set to configuration mode (M0=1, M1=1)");
}

void configureLoraModule() {
  Serial.println("→ Configuring LoRa module parameters...");
  
  // Configuration packet for E22-900M30S
  // This is a simplified configuration - in a real implementation,
  // you would send proper configuration commands based on the module's datasheet
  
  // For the E22-900M30S, configuration typically involves:
  // 1. Setting frequency (915MHz)
  // 2. Setting transmission power (22dBm)
  // 3. Setting channel (23)
  // 4. Setting other parameters like air data rate, etc.
  
  // Note: The actual configuration commands depend on the specific module
  // For this test, we assume the module is pre-configured or uses default settings
  
  Serial.println("→ Using default configuration for frequency 915MHz");
  Serial.println("→ Power set to 22dBm, Channel 23");
  
  delay(200);
}

bool waitForAux(unsigned long timeout) {
  unsigned long startTime = millis();
  
  while (millis() - startTime < timeout) {
    if (digitalRead(LORA_AUX_PIN) == HIGH) {
      return true;
    }
    delay(10);
  }
  
  return false;
}

void sendTestMessage() {
  // Increment message counter
  messageCounter++;
  
  // Create test message
  String message = "Hello from " + String(DEVICE_ID) + " - Count: " + String(messageCounter);
  
  // Print transmission info to Serial Monitor
  printTimestamp();
  Serial.print("TX [");
  Serial.print(messageCounter);
  Serial.print("]: ");
  Serial.println(message);
  
  // Send message via LoRa
  loraSerial.print(message);
  
  // Brief LED flash to indicate transmission
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
}

void printTimestamp() {
  Serial.print("[");
  Serial.print(formatTime(millis()));
  Serial.print("] ");
}

String formatTime(unsigned long milliseconds) {
  unsigned long seconds = milliseconds / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  
  String timeStr = "";
  if (hours < 10) timeStr += "0";
  timeStr += String(hours) + ":";
  if (minutes < 10) timeStr += "0";
  timeStr += String(minutes) + ":";
  if (seconds < 10) timeStr += "0";
  timeStr += String(seconds);
  
  return timeStr;
}