
#ifndef WEATHER_MANAGER_H
#define WEATHER_MANAGER_H

#include <Arduino.h>

struct WeatherData {
    float temperature;
    int humidity;
    String condition;
    bool valid;
};

void beginWeatherManager();
bool fetchWeather(WeatherData &data);
WeatherData getWeatherData();

#endif
