

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Config.h"
#include "LightController.h"
#include "LEDManager.h"
#include "SinricManager.h"
#include "OLEDManager.h"
#include "WeatherManager.h"

static unsigned long lastWeatherUpdate = 0;

void setup() {
    Serial.begin(115200);
    Serial.println();

    Serial.println("==============================");
    Serial.println(" SMART TUBE LIGHT STARTING");
    Serial.println("==============================");

    beginLightController();
    beginLEDManager();

    beginOLED();
    beginWeatherManager();

    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Connecting to WiFi...");

    beginSinricManager();
}

void loop() {
    handleSinric();

    updateLED(isSinricConnected());

    unsigned long currentTime = millis();

    // Fetch weather immediately after WiFi connects,
    // then refresh every 10 minutes.
    if (WiFi.status() == WL_CONNECTED &&
        (lastWeatherUpdate == 0 ||
         currentTime - lastWeatherUpdate >= WEATHER_INTERVAL)) {

        lastWeatherUpdate = currentTime;

        WeatherData weather;

        if (!fetchWeather(weather)) {
            Serial.println("Weather update failed");
        }
        refreshOLED();
        updateOLED(getWeatherData());
    }

    // Refresh the OLED display and handle the 3-second notification.
    updateOLED(getWeatherData());
}

