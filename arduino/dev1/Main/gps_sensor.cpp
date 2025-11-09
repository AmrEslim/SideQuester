#include "gps_sensor.h"
#include <TinyGPS++.h>

// Objects
TinyGPSPlus gps;

// GPS data variables
float latitude = 0.0;
float longitude = 0.0;
bool gpsFixed = false;

// Timing variables
unsigned long lastGPSRead = 0;
const unsigned long GPS_READ_INTERVAL = 1000; // Read GPS every 1 second

void readGPSData()
{
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
    if (gps.location.isValid()){
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        gpsFixed = true;
        Serial.print("gpsFixed");
    }
    else{
        gpsFixed = false;
        Serial.print("gps not fixed");
    }
}