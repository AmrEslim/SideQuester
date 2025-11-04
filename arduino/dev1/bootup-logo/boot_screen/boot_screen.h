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

#endif // BOOT_SCREEN_H
