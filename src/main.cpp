#include <Arduino.h>

#include "config.h"
#include "debug.h"
#include "duckscript.h"
#include "duckparser.h"
#include "webserver.h"
#include "spiffs.h"
#include "settings.h"
#include "cli.h"
#include "led.h"
#include "USB.h"

namespace {
    enum class DeviceState {
        Booting,
        Ready,
        RunningScript
    };

    DeviceState currentState = DeviceState::Booting;
    unsigned long lastBlinkMs = 0;
    bool builtinLedOn = false;
    unsigned long lastTransportSyncMs = 0;

    void setRgbStatus(int r, int g, int b) {
#if defined(ARDUINO_ARCH_ESP32) && defined(STATUS_RGB_PIN)
        neopixelWrite(STATUS_RGB_PIN, r, g, b);
#else
        led::setColor(r, g, b);
#endif
    }

    void setBuiltinLed(bool on) {
#ifdef LED_BUILTIN
        digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
#else
        (void)on;
#endif
    }

    void applyState(DeviceState state) {
        switch (state) {
            case DeviceState::Booting:
                setRgbStatus(0, 0, 40);  // Blue
                setBuiltinLed(false);
                break;
            case DeviceState::Ready:
                setRgbStatus(0, 30, 0);  // Green
                setBuiltinLed(true);
                break;
            case DeviceState::RunningScript:
                setRgbStatus(40, 0, 0);  // Red
                setBuiltinLed(builtinLedOn);
                break;
        }
    }

    void updateStatusIndicator() {
        DeviceState targetState = duckscript::isRunning() ? DeviceState::RunningScript : DeviceState::Ready;

        if (targetState != currentState) {
            currentState = targetState;
            applyState(currentState);
        }

        if (currentState == DeviceState::RunningScript) {
            unsigned long now = millis();
            if (now - lastBlinkMs >= 250) {
                lastBlinkMs = now;
                builtinLedOn = !builtinLedOn;
                setBuiltinLed(builtinLedOn);
            }
        }
    }
}


void setup() {
    debug_init();
    duckparser::beginKeyboard();
    led::begin();
#ifdef LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
#endif
    applyState(DeviceState::Booting);

    USB.begin();
    delay(200);
    spiffs::begin();
    settings::begin();
    duckparser::applyConfiguredTransport();
    cli::begin();
    webserver::begin();

    duckscript::run(settings::getAutorun());
    currentState = DeviceState::Ready;
    applyState(currentState);
}

void loop() {
    webserver::update();
    debug_update();

    unsigned long now = millis();
    if ((now - lastTransportSyncMs) >= 500) {
        lastTransportSyncMs = now;
        duckparser::applyConfiguredTransport();
    }

    updateStatusIndicator();
}
