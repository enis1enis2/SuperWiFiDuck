/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String

namespace settings {
    void begin();
    void load();

    void reset();
    void save();

    String toString(bool includeSecrets = false);

    const char* getSSID();
    const char* getPassword();
    const char* getChannel();
    const char* getAutorun();
    const char* getStaSSID();
    const char* getStaPassword();
    bool getStaAutoconnect();

    int getChannelNum();

    bool set(const char* name, const char* value);

    bool setSSID(const char* ssid);
    bool setPassword(const char* password);
    bool setChannel(const char* channel);
    bool setAutorun(const char* autorun);
    bool setStaSSID(const char* ssid);
    bool setStaPassword(const char* password);
    bool setStaAutoconnect(const char* autoconnect);
    void setStaAutoconnectBool(bool autoconnect);
}
