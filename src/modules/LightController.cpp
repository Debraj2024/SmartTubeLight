#include <Arduino.h>
#include "Config.h"
#include "LightController.h"
#include "OLEDManager.h"

static bool lightState = false;

void beginLightController() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);

    lightState = false;

    Serial.println("Light Controller Initialized");
}

void setLightState(bool state) {

    // Ignore duplicate commands.
    if (lightState == state) return;

    lightState = state;

    digitalWrite(RELAY_PIN, state ? LOW : HIGH);

    Serial.println(state ? "Light ON" : "Light OFF");

    showLightStatus(state);
}

bool getLightState() {
    return lightState;
}
