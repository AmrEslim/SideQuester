#ifndef COMPASS_SENSOR_H
#define COMPASS_SENSOR_H

#include <Wire.h>

// Compass data structure
struct CompassData
{
    int16_t x;
    int16_t y;
    int16_t z;
    float heading;
    float smoothedHeading;
};

extern CompassData compass;

bool checkSensor();
bool initSensor();
bool readCompass();
void calculateHeading();
void smoothHeading();
String getDirection();

#endif // COMPASS_SENSOR_H
