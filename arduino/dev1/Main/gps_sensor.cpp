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
const unsigned long GPS_READ_INTERVAL = 500; // Read GPS every 0.5 seconds

void readGPSData()
{
    // Process GPS data
    while (Serial1.available() > 0)
    {
        char c = Serial1.read();
        if (gps.encode(c))
        {
            if (gps.location.isValid())
            {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                gpsFixed = true;
            }
            else
            {
                gpsFixed = false;
            }
        }
    }
}