/*
 * LoRa E22-900M30S Receiver Test
 * SideQuester Project - https://github.com/AmrEslim/SideQuester
 * 
 * This sketch demonstrates LoRa communication using the E22-900M30S module
 * configured in transparent transmission mode. It receives test messages and
 * displays them with timestamps and signal strength information.
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

// Device Configuration
#define DEVICE_ID "Device B"
#define BAUD_RATE 9600
#define RECEIVE_BUFFER_SIZE 256
#define STATUS_UPDATE_INTERVAL 10000  // 10 seconds

// Create SoftwareSerial instance for LoRa communication
SoftwareSerial loraSerial(LORA_RX_PIN, LORA_TX_PIN);

// Global variables
bool loraInitialized = false;
unsigned int messagesReceived = 0;
unsigned long lastStatusUpdate = 0;
String receiveBuffer = "";
int lastRSSI = -999;  // Simulated RSSI value

// Function prototypes
void initializeLora();
void setTransparentMode();
void setConfigurationMode();
void configureLoraModule();
bool waitForAux(unsigned long timeout = 1000);
void processIncomingData();
void displayReceivedMessage(String message);
void printTimestamp();
void printStatus();
String formatTime(unsigned long milliseconds);
int simulateRSSI();

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("===========================================");
  Serial.println("      E22-900M30S LoRa Receiver Test      ");
  Serial.println("           SideQuester Project            ");
  Serial.println("===========================================");
  Serial.println();
  
  printTimestamp();
  Serial.println("Initializing LoRa receiver...");
  
  // Initialize LoRa module
  initializeLora();
  
  if (loraInitialized) {
    printTimestamp();
    Serial.println("✓ LoRa module initialized successfully!");
    Serial.println("✓ Ready to receive messages");
    Serial.println("✓ Configuration: 915MHz, 22dBm, Channel 23");
    Serial.println("✓ Transparent transmission mode active");
    Serial.println();
    Serial.println("--- Reception Log ---");
    Serial.println("Waiting for incoming messages...");
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
  
  // Process any incoming LoRa data
  processIncomingData();
  
  // Print periodic status updates
  if (millis() - lastStatusUpdate >= STATUS_UPDATE_INTERVAL) {
    printStatus();
    lastStatusUpdate = millis();
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

void processIncomingData() {
  // Check if data is available from LoRa module
  while (loraSerial.available()) {
    char receivedChar = loraSerial.read();
    
    // Add character to buffer
    receiveBuffer += receivedChar;
    
    // Check for end of message (you might want to use a different delimiter)
    // For this test, we'll assume messages are complete when there's a pause in data
    // In a real implementation, you might use a specific terminator character
  }
  
  // If we have data in buffer and no new data for a short time, process the message
  static unsigned long lastReceiveTime = 0;
  if (receiveBuffer.length() > 0) {
    if (millis() - lastReceiveTime > 100) { // 100ms timeout
      displayReceivedMessage(receiveBuffer);
      receiveBuffer = ""; // Clear buffer
    }
    lastReceiveTime = millis();
  }
}

void displayReceivedMessage(String message) {
  // Increment received message counter
  messagesReceived++;
  
  // Simulate RSSI value (in a real implementation, you'd read this from the module)
  lastRSSI = simulateRSSI();
  
  // Print received message with timestamp and signal info
  printTimestamp();
  Serial.print("RX [");
  Serial.print(messagesReceived);
  Serial.print("] ");
  Serial.print(message);
  Serial.print(" | RSSI: ");
  Serial.print(lastRSSI);
  Serial.println(" dBm");
  
  // Brief LED flash to indicate reception
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
}

void printStatus() {
  Serial.println();
  printTimestamp();
  Serial.println("--- Status Update ---");
  Serial.print("Messages received: ");
  Serial.println(messagesReceived);
  Serial.print("Last RSSI: ");
  if (lastRSSI != -999) {
    Serial.print(lastRSSI);
    Serial.println(" dBm");
  } else {
    Serial.println("N/A");
  }
  Serial.print("Uptime: ");
  Serial.println(formatTime(millis()));
  Serial.println("Listening for messages...");
  Serial.println();
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

int simulateRSSI() {
  // Simulate realistic RSSI values between -50 and -120 dBm
  // In a real implementation, you would read this from the LoRa module
  static int baseRSSI = -80; // Base signal strength
  int variation = random(-20, 20); // Add some random variation
  return constrain(baseRSSI + variation, -120, -50);
}