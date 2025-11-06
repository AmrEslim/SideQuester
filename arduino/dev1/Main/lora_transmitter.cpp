#include <HardwareSerial.h>
#include "lora_transmitter.h"
#include "gps_sensor.h"

// Objects
HardwareSerial E220(2); // Use UART2 for LoRa
RemoteLocationData result;

// Timing variables
unsigned long lastLoRaTransmit = 0;
const unsigned long LORA_TRANSMIT_INTERVAL = 5000; // Transmit every 5 seconds



RemoteLocationData parseLocationMessage(String message)
{
    result.isValid = false;

    // Remove "LOC:" prefix
    String data = message.substring(4);

    // Parse comma-separated values
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);

    if (firstComma > 0 && secondComma > 0 && thirdComma > 0)
    {
        result.latitude = data.substring(0, firstComma).toFloat();
        result.longitude = data.substring(firstComma + 1, secondComma).toFloat();
        result.satellites = data.substring(secondComma + 1, thirdComma).toInt();
        result.hdop = data.substring(thirdComma + 1).toFloat();
        result.isValid = true;
    }

    return result;
}

void transmitLocationData()
{
    // Create location message in JSON-like format for easy parsing
    String locationMessage = "LOC:";
    locationMessage += String(latitude, 6);
    locationMessage += ",";
    locationMessage += String(longitude, 6);
    locationMessage += ",";
    locationMessage += String(gps.satellites.value());
    locationMessage += ",";
    locationMessage += String(gps.hdop.hdop(), 1);

    // Send via LoRa
    E220.println(locationMessage);

    // Log to serial
    Serial.print("📡 Transmitted: ");
    Serial.println(locationMessage);
    Serial.print("   Format: LOC:latitude,longitude,satellites,hdop");
    Serial.println();
}

void checkIncomingMessages()
{
    if (E220.available())
    {
        String receivedMessage = E220.readStringUntil('\n');
        receivedMessage.trim(); // Remove any trailing whitespace

        Serial.print("📨 Received: ");
        Serial.println(receivedMessage);

        // Parse incoming location messages
        if (receivedMessage.startsWith("LOC:"))
        {
            parseLocationMessage(receivedMessage);
        }
    }
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    // Haversine formula to calculate distance between two points on Earth
    const double R = 6371.0; // Earth's radius in kilometers

    double lat1Rad = lat1 * PI / 180.0;
    double lon1Rad = lon1 * PI / 180.0;
    double lat2Rad = lat2 * PI / 180.0;
    double lon2Rad = lon2 * PI / 180.0;

    double dLat = lat2Rad - lat1Rad;
    double dLon = lon2Rad - lon1Rad;

    double a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1Rad) * cos(lat2Rad) * sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c;
}

double calculateBearing(double lat1, double lon1, double lat2, double lon2)
{
    // Calculate bearing from point 1 to point 2
    double lat1Rad = lat1 * PI / 180.0;
    double lon1Rad = lon1 * PI / 180.0;
    double lat2Rad = lat2 * PI / 180.0;
    double lon2Rad = lon2 * PI / 180.0;

    double dLon = lon2Rad - lon1Rad;

    double y = sin(dLon) * cos(lat2Rad);
    double x = cos(lat1Rad) * sin(lat2Rad) - sin(lat1Rad) * cos(lat2Rad) * cos(dLon);
    
    double bearingRad = atan2(y, x);
    double bearingDeg = bearingRad * 180.0 / PI;
    
    // Normalize to 0-360 degrees
    bearingDeg = fmod((bearingDeg + 360.0), 360.0);
    
    return bearingDeg;
}