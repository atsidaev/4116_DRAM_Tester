#ifndef STATE_REPORTER_H
#define STATE_REPORTER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pinout.h"

#define SCREEN_WIDTH   128  // OLED display width, in pixels
#define SCREEN_HEIGHT  32   // OLED display height, in pixels
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

class StateReporter : public Print {
private:
    Adafruit_SSD1306 display;
    bool displayInitialized = false;
public:
    StateReporter() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) { }

    void init();

    // Text
    size_t write(uint8_t c) override;
    bool logTestResult(bool result, const __FlashStringHelper* msg_ok, const __FlashStringHelper* msg_fail);
    bool logTestResult(bool result);
    void refresh();

    // LED
    void ledOn() { digitalWrite(LED, HIGH); }
    void ledOff() { digitalWrite(LED, LOW); }
    void ledBlink();
};

#endif /* STATE_REPORTER_H */
