

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

    if (WiFi.status() == WL_CONNECTED) {

        if (lastWeatherUpdate == 0 ||
            currentTime - lastWeatherUpdate >= WEATHER_INTERVAL) {

            Serial.println("Starting weather refresh...");

            lastWeatherUpdate = currentTime;

            WeatherData weather;

            if (fetchWeather(weather)) {
                Serial.println("Weather refresh successful");

                refreshOLED();
            } else {
                Serial.println("Weather refresh failed");
            }
        }

    } else {
        Serial.println("Waiting for WiFi...");
    }

    updateOLED(getWeatherData());
}


