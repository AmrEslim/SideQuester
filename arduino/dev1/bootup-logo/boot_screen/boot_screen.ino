#include <TFT_eSPI.h>
#include "boot_screen.h"


// Arduino setup function
void setup() {
    initBootScreen();
}

// Arduino loop function
void loop() {
    drawBootScreen();
}
