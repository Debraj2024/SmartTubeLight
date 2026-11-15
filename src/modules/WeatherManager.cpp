
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#include "Config.h"
#include "WeatherManager.h"

static WeatherData currentWeather = {0.0, 0, "Loading...", false};

void beginWeatherManager() {
    Serial.println("Weather Manager Initialized");
}

bool fetchWeather(WeatherData &data) {

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Weather: WiFi not connected");
        return false;
    }

    BearSSL::WiFiClientSecure client;
    client.setInsecure();
    client.setBufferSizes(512, 512);
    
    HTTPClient http;
    http.setReuse(false);
    http.setTimeout(15000);

    String url = "https://api.openweathermap.org/data/2.5/weather?q=";
    url += WEATHER_CITY;
    url += ",";
    url += WEATHER_COUNTRY;
    url += "&appid=";
    url += WEATHER_API_KEY;
    url += "&units=metric";

    Serial.println("Fetching weather...");

    http.setTimeout(5000);

    if (!http.begin(client, url)) {
        Serial.println("Weather: HTTP initialization failed");
        return false;
    }

    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("Weather HTTP Error: %d\n", httpCode);
        Serial.println("Reason: " + http.errorToString(httpCode));

        http.end();
        client.stop();

        return false;
    }

    String response = http.getString();
    http.end();
    client.stop();

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        Serial.print("JSON Error: ");
        Serial.println(error.c_str());
        return false;
    }

    data.temperature = doc["main"]["temp"] | 0.0;
    data.humidity = doc["main"]["humidity"] | 0;
    data.condition = doc["weather"][0]["description"] | "Unknown";
    data.valid = true;

    currentWeather = data;

    Serial.println("Weather Updated Successfully");
    Serial.printf("Temperature: %.1f C\n", data.temperature);
    Serial.printf("Humidity: %d%%\n", data.humidity);
    Serial.printf("Condition: %s\n", data.condition.c_str());

    return true;
}

WeatherData getWeatherData() {
    return currentWeather;
}
