#include "boot_screen.h"
#include "gps_sensor.h"
#include "lora_transmitter.h"
#include "compass_sensor.h"

// GPS setup
#define GPS_TX_PIN 33
#define GPS_RX_PIN 32

// LoRa E220 setup
#define LORA_RX 16
#define LORA_TX 17


//received location data 
String remoteLatStr = "N/A";
String remoteLonStr = "N/A";
float remoteDistance = 0.0;
bool remoteDataReceived = false;

//display update function
void updateDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  
  // Title
  tft.setCursor(70, 10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN);
  tft.println("SideQuester");
  
  // Local GPS Status
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(10, 40);
  tft.println("Local GPS:");
  
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 55);
  if (gpsFixed) {
    tft.print("Lat: ");
    tft.println(latitude, 4);
    tft.setCursor(10, 70);
    tft.print("Lon: ");
    tft.println(longitude, 4);
    tft.setCursor(10, 85);
    tft.print("Sats: ");
    tft.print(gps.satellites.value());
  } else {
    tft.setTextColor(TFT_RED);
    tft.println("No Fix");
  }
  
  // Remote Device Status
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(10, 110);
  tft.println("Remote Device:");
  
  tft.setTextColor(TFT_WHITE);
  if (remoteDataReceived) {
    tft.setCursor(10, 125);
    tft.print("Lat: ");
    tft.println(remoteLatStr);
    tft.setCursor(10, 140);
    tft.print("Lon: ");
    tft.println(remoteLonStr);
    tft.setCursor(10, 155);
    tft.print("Distance: ");
    tft.print(remoteDistance, 2);
    tft.println(" km");
  } else {
    tft.setCursor(10, 125);
    tft.setTextColor(TFT_ORANGE);
    tft.println("Waiting...");
  }
  
  // Status indicator
  tft.fillCircle(120, 220, 8, gpsFixed ? TFT_GREEN : TFT_RED);
}
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== Display initialization ===");
  Serial.println("=== GPS + LoRa Location Transmitter ===");
  Serial.println("Initializing GPS and LoRa modules...");
  // Initialize TFT display
  initBootScreen();
  
  
  // Initialize GPS (Serial1)
  Serial1.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS module initialized on Serial1");
  
  // Initialize LoRa E220 (Serial2)
  E220.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
  delay(500);
  Serial.println("LoRa E220 module initialized on Serial2");
  
  // Show boot screen
  drawBootScreen();
  delay(2000);
  
  Serial.println("Starting main loop...");
  Serial.println("=====================================");
}

void loop() {
  if (!gpsFixed || !remoteDataReceived) {
    drawWaitScreen();
    if (!gpsFixed && !remoteDataReceived) {
      drawCrossSign(41, 158);
      drawCrossSign(170, 158);
    }
    else if (!gpsFixed && remoteDataReceived) {
      drawCrossSign(41, 158);
      drawConnectedSign(170, 158);
    }
    else if (gpsFixed && !remoteDataReceived) {
      drawCrossSign(170, 158);
      drawConnectedSign(41, 158);
    }
  }

  unsigned long currentTime = millis();
  
  // Read GPS data
  if (currentTime - lastGPSRead >= GPS_READ_INTERVAL) {
    readGPSData();
    lastGPSRead = currentTime;
  }
  
  // Transmit location via LoRa if GPS has a fix
  if (currentTime - lastLoRaTransmit >= LORA_TRANSMIT_INTERVAL) {
    if (gpsFixed) {
      transmitLocationData();
    } else {
      Serial.println("⏳ Waiting for GPS fix before transmitting...");
    }
    lastLoRaTransmit = currentTime;
  }
  
  // Check for incoming LoRa messages
  checkIncomingMessages();
  
  // Small delay to prevent excessive CPU usage
  delay(100);
}
 