
#include <Arduino.h>
#include "Config.h"
#include "LEDManager.h"

// Timing variables
static unsigned long lastBeat = 0;
static unsigned long lastBlink = 0;

static int beatStep = 0;

static bool previousConnection = false;
static bool ledInitialized = false;
static bool normalLedState = false;

// Initialize LED
void beginLEDManager() {

    pinMode(LED_PIN, OUTPUT);

    // Built-in LED OFF (active LOW)
    digitalWrite(LED_PIN, HIGH);

    Serial.println("LED Manager Initialized");
}

// Update LED according to connection status
void updateLED(bool connected) {

    unsigned long now = millis();

    // Detect connection status change
    if (!ledInitialized || connected != previousConnection) {

        previousConnection = connected;
        ledInitialized = true;

        if (connected) {

            Serial.println("Internet Connected - Heartbeat Mode");

            beatStep = 0;
            lastBeat = now;

            digitalWrite(LED_PIN, HIGH);

        } else {

            Serial.println("Internet Disconnected - Normal Blink Mode");

            normalLedState = true;
            lastBlink = now;

            digitalWrite(LED_PIN, LOW);
        }
    }

    // DISCONNECTED: Normal blinking
    if (!connected) {

        if (now - lastBlink >= 500) {

            lastBlink = now;
            normalLedState = !normalLedState;

            digitalWrite(
                LED_PIN,
                normalLedState ? LOW : HIGH
            );
        }

        return;
    }

    // CONNECTED: Slow double heartbeat
    switch (beatStep) {

        case 0:
            if (now - lastBeat >= 1200) {

                digitalWrite(LED_PIN, LOW);

                lastBeat = now;
                beatStep = 1;
            }
            break;

        case 1:
            if (now - lastBeat >= 120) {

                digitalWrite(LED_PIN, HIGH);

                lastBeat = now;
                beatStep = 2;
            }
            break;

        case 2:
            if (now - lastBeat >= 200) {

                digitalWrite(LED_PIN, LOW);

                lastBeat = now;
                beatStep = 3;
            }
            break;

        case 3:
            if (now - lastBeat >= 120) {

                digitalWrite(LED_PIN, HIGH);

                lastBeat = now;
                beatStep = 0;
            }
            break;
    }
}