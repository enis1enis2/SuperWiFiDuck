/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once
#include <Arduino.h>

namespace webserver {
    void begin();
    void update();
    void send(const char* str);
    void staConnect();
    void staDisconnect();
    String wifiInfo();
}
