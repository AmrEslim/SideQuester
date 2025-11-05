#ifndef LORA_TRANSMITTER_H
#define LORA_TRANSMITTER_H

#include <HardwareSerial.h>

// Object
extern HardwareSerial E220; // Use UART2 for LoRa
// Timing variables
extern unsigned long lastLoRaTransmit;
extern const unsigned long LORA_TRANSMIT_INTERVAL; // Transmit every 5 seconds

struct RemoteLocationData
{
    bool isValid;
    float latitude;
    float longitude;
    int satellites;
    float hdop;
};

extern RemoteLocationData remoteLocation;

// Function declarations
void transmitLocationData();
void checkIncomingMessages();
double calculateDistance(double lat1, double lon1, double lat2, double lon2);
RemoteLocationData parseLocationMessage(String message);

#endif // LORA_TRANSMITTER_H
