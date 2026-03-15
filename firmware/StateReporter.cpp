#include "StateReporter.h"

void StateReporter::init()
{
    displayInitialized = false;
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    Serial.begin(9600);
    // Wait for display
    delay(500);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        // Continue as we still have serial and LED
    } else {
        displayInitialized = true;

        display.clearDisplay();
        display.setRotation(1);
        display.setTextSize(1);              // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE); // Draw white text
        display.setCursor(0, 0);              // Start at top-left corner
        refresh();
    }
}

size_t StateReporter::write(uint8_t c)
{
    if (displayInitialized)
        display.write(c);
    return Serial.write(c);
}

void StateReporter::refresh()
{
    if (displayInitialized)
        display.display();
}

bool StateReporter::logTestResult(bool result, const __FlashStringHelper* msg_ok, const __FlashStringHelper* msg_fail)
{
    if (result)
        this->println(msg_ok);
    else
        this->println(msg_fail);
    
    this->refresh();

    return result;
}

bool StateReporter::logTestResult(bool result)
{
    return logTestResult(result, F("OK"), F(" X"));
}

void StateReporter::ledBlink() {
    ledOn();
    delay(100);
    ledOff();
    delay(100);
}