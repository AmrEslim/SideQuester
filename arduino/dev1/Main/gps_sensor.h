#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <TinyGPS++.h>

// External GPS object - can be accessed from main sketch if needed
extern TinyGPSPlus gps;

// GPS data variables
extern float latitude;
extern float longitude;
extern bool gpsFixed;

// Timing variables
extern unsigned long lastGPSRead;
extern const unsigned long GPS_READ_INTERVAL; // Read GPS every 0.5 seconds

// Function declarations
void readGPSData();

#endif // GPS_SENSOR_H