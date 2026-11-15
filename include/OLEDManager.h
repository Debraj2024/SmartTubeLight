
#ifndef OLED_MANAGER_H
#define OLED_MANAGER_H

#include "WeatherManager.h"

void beginOLED();
void showLightStatus(bool state);
void updateOLED(const WeatherData &weather);
void refreshOLED();
bool isOLEDReady();

#endif
