
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Config.h"
#include "OLEDManager.h"

static Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

static bool oledReady = false;
static bool statusVisible = false;
static bool screenNeedsUpdate = true;

static bool displayedLightState = false;
static unsigned long statusStartTime = 0;

void beginOLED() {
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("OLED initialization failed!");
        oledReady = false;
        return;
    }

    oledReady = true;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();

    Serial.println("OLED initialized successfully");
}

void showLightStatus(bool state) {
    if (!oledReady) return;

    displayedLightState = state;
    statusVisible = true;
    statusStartTime = millis();
    screenNeedsUpdate = true;
}

void updateOLED(const WeatherData &weather) {
    if (!oledReady) return;

    // Return to weather screen after 3 seconds.
    if (statusVisible &&
        millis() - statusStartTime >= LIGHT_DISPLAY_TIME) {
        statusVisible = false;
        screenNeedsUpdate = true;
    }

    if (!screenNeedsUpdate) return;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    if (statusVisible) {
        display.setTextSize(1);
        display.setCursor(10, 12);
        display.println("SMART TUBE LIGHT");

        display.drawLine(0, 27, 127, 27, SSD1306_WHITE);

        display.setTextSize(2);
        display.setCursor(10, 39);

        if (displayedLightState) {
            display.println("LIGHT ON");
        } else {
            display.println("LIGHT OFF");
        }
    } else {
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println(WEATHER_CITY);

        display.drawLine(0, 11, 127, 11, SSD1306_WHITE);

        if (!weather.valid) {
            display.setCursor(0, 25);
            display.println("Loading weather...");
        } else {
            display.setTextSize(2);
            display.setCursor(0, 17);
            display.print(weather.temperature, 1);
            display.print(" C");

            display.setTextSize(1);
            display.setCursor(0, 43);
            display.print("Humidity: ");
            display.print(weather.humidity);
            display.println("%");

            display.setCursor(0, 55);
            display.println(weather.condition);
        }
    }

    display.display();
    screenNeedsUpdate = false;
}

bool isOLEDReady() {
    return oledReady;
}

void refreshOLED() {
    screenNeedsUpdate = true;
}