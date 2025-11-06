#ifndef BOOT_SCREEN_H
#define BOOT_SCREEN_H

#include <TFT_eSPI.h>

// Define background color if not already defined
#ifndef COLOR_BG
#define COLOR_BG TFT_BLACK
#endif

// External TFT object - can be accessed from main sketch if needed
extern TFT_eSPI tft;

// Function declarations
void initBootScreen();
void drawBootScreen();
void drawWaitScreen();
void drawConnectedSign(int p1, int p2);
void drawCrossSign(int p1, int p2);
void drawCompassBase();
void drawCardinalMarkers();
void drawTick(float angle, int length, uint16_t color);
void drawStaticNeedle();
void drawRotatingCompass(float heading);
void drawRotatedCardinalMarkers(float heading);
void drawRotatedLabel(const char* label, float angle, uint16_t color);
void drawRotatedTick(float angle, int length, uint16_t color);
void displayDistance(float distanceMeters);
void drawRemoteDevice(float bearing, float heading);
void drawRemoteDeviceAnimated(float bearing, float heading, int pulsePhase);


#endif // BOOT_SCREEN_H
