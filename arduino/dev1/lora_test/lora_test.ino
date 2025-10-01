#include <HardwareSerial.h>

HardwareSerial E220(2); // Use UART2 on ESP32

#define RX 16
#define TX 17

void setup() {
  Serial.begin(115200);
  E220.begin(9600, SERIAL_8N1, RX, TX);
  delay(1000);
  Serial.println("E220 LoRa test started");
}

void loop() {
  // Send a message every 2 seconds
  E220.println("Hey you");
  Serial.println("Sent: Hey you");
  delay(2000);

  // Check if something received
  if (E220.available()) {
    String msg = E220.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(msg);
  }
}
