#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup()
{
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=== Display Test Starting ===");
    
    tft.init();
    tft.setRotation(1);
    
    Serial.printf("Screen dimensions: %d x %d\n", tft.width(), tft.height());
    
    // Test with bright colors in sequence
    Serial.println("RED...");
    tft.fillScreen(TFT_RED);
    delay(1000);
    
    Serial.println("GREEN...");
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    
    Serial.println("BLUE...");
    tft.fillScreen(TFT_BLUE);
    delay(1000);
    
    Serial.println("WHITE...");
    tft.fillScreen(TFT_WHITE);
    delay(1000);
    
    Serial.println("BLACK...");
    tft.fillScreen(TFT_BLACK);
    delay(1000);
    
    // Now pink with text
    Serial.println("PINK with text...");
    tft.fillScreen(TFT_PINK);
    
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(20, 80);
    tft.println("Hello");
    tft.setCursor(20, 105);
    tft.println("Sidequester");
    
    Serial.println("=== Test Complete ===");
}

void loop()
{
    // Nothing to do in loop
}