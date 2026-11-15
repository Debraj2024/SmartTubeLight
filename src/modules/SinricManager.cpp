
#include <Arduino.h>

#include <SinricPro.h>
#include <SinricProSwitch.h>

#include "Config.h"
#include "SinricManager.h"
#include "LightController.h"

// =====================================
// POWER STATE CALLBACK
// =====================================

bool onPowerState(const String &deviceId, bool &state) {

    if (deviceId != DEVICE_ID) {
        return false;
    }

    setLightState(state);

    Serial.println(
        state ? "Tube Light ON" : "Tube Light OFF"
    );

    return true;
}

// =====================================
// INITIALIZE SINRICPRO
// =====================================

void beginSinricManager() {

    SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];

    mySwitch.onPowerState(onPowerState);

    SinricPro.onConnected([]() {
        Serial.println("Connected to SinricPro");
    });

    SinricPro.onDisconnected([]() {
        Serial.println("Disconnected from SinricPro");
    });

    SinricPro.begin(APP_KEY, APP_SECRET);

    SinricPro.restoreDeviceStates(true);

    Serial.println("SinricPro Initialized");
}

// =====================================
// HANDLE SINRICPRO
// =====================================

void handleSinric() {

    SinricPro.handle();
}

// =====================================
// CONNECTION STATUS
// =====================================

bool isSinricConnected() {

    return SinricPro.isConnected();
}
