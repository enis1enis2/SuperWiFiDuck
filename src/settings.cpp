/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "settings.h"

#include "spiffs.h"
#include "debug.h"
#include "config.h"
#include "eeprom0.h"
#include <ctype.h>

#define SETTINGS_ADDRESS 1
#define SETTINGS_MAGIC_NUM_LEGACY 1234567891
#define SETTINGS_MAGIC_NUM 1234567892

namespace settings {
    // ===== PRIVATE ===== //
    typedef struct legacy_settings_t {
        uint32_t magic_num;

        char ssid[33];
        char password[65];
        char channel[5];
        char autorun[65];
    } legacy_settings_t;

    typedef struct settings_t {
        uint32_t magic_num;

        char ssid[33];
        char password[65];
        char channel[5];
        char autorun[65];
        char sta_ssid[33];
        char sta_password[65];
        uint8_t sta_autoconnect;
    } settings_t;

    settings_t data;

    bool equalsIgnoreCase(const char* a, const char* b) {
        if (!a || !b) return false;
        while (*a && *b) {
            if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
            ++a;
            ++b;
        }
        return *a == '\0' && *b == '\0';
    }

    void migrateLegacySettings() {
        legacy_settings_t oldData;
        memset(&oldData, 0, sizeof(oldData));
        eeprom::getObject(SETTINGS_ADDRESS, oldData);

        if (oldData.magic_num != SETTINGS_MAGIC_NUM_LEGACY) {
            reset();
            return;
        }

        memset(&data, 0, sizeof(data));
        data.magic_num = SETTINGS_MAGIC_NUM;

        if ((oldData.ssid[32] == 0) && (strlen(oldData.ssid) > 0)) setSSID(oldData.ssid);
        else setSSID(WIFI_SSID);

        if ((oldData.password[64] == 0) && (strlen(oldData.password) >= 8)) setPassword(oldData.password);
        else setPassword(WIFI_PASSWORD);

        if ((oldData.channel[4] == 0) && ((strcmp(oldData.channel, "auto") == 0) || ((atoi(oldData.channel) >= 1) && (atoi(oldData.channel) <= 13)))) setChannel(oldData.channel);
        else setChannel(WIFI_CHANNEL);

        if (oldData.autorun[64] == 0) setAutorun(oldData.autorun);
        else setAutorun("");

        setStaSSID("");
        setStaPassword("");
        setStaAutoconnectBool(false);
    }

    // ===== PUBLIC ====== //
    void begin() {
        eeprom::begin();
        load();
    }

    void load() {
        eeprom::getObject(SETTINGS_ADDRESS, data);
        if (data.magic_num == SETTINGS_MAGIC_NUM_LEGACY) {
            migrateLegacySettings();
            return;
        }
        if (data.magic_num != SETTINGS_MAGIC_NUM) {
            reset();
            return;
        }

        if (data.ssid[32] != 0) setSSID(WIFI_SSID);
        if (data.password[64] != 0) setPassword(WIFI_PASSWORD);
        if (data.channel[4] != 0) setChannel(WIFI_CHANNEL);
        if (data.autorun[64] != 0) setAutorun("");
        if (data.sta_ssid[32] != 0) setStaSSID("");
        if (data.sta_password[64] != 0) setStaPassword("");
        if (data.sta_autoconnect > 1) setStaAutoconnectBool(false);
    }

    void reset() {
        debugln("Resetting Settings");
        data.magic_num = SETTINGS_MAGIC_NUM;
        setSSID(WIFI_SSID);
        setPassword(WIFI_PASSWORD);
        setChannel(WIFI_CHANNEL);
        setAutorun("");
        setStaSSID("");
        setStaPassword("");
        setStaAutoconnectBool(false);
    }

    void save() {
        debugln("Saving Settings");
        eeprom::saveObject(SETTINGS_ADDRESS, data);
    }

    String toString() {
        String s;

        s += "ssid=";
        s += getSSID();
        s += "\n";
        s += "password=";
        s += getPassword();
        s += "\n";
        s += "channel=";
        s += getChannel();
        s += "\n";
        s += "autorun=";
        s += getAutorun();
        s += "\n";
        s += "sta_ssid=";
        s += getStaSSID();
        s += "\n";
        s += "sta_password=";
        s += getStaPassword();
        s += "\n";
        s += "sta_autoconnect=";
        s += getStaAutoconnect() ? "1" : "0";
        s += "\n";

        return s;
    }

    const char* getSSID() {
        return data.ssid;
    }

    const char* getPassword() {
        return data.password;
    }

    const char* getChannel() {
        return data.channel;
    }

    int getChannelNum() {
        if (strcmp(data.channel, "auto") != 0) return atoi(data.channel);
        return 1;
    }

    const char* getAutorun() {
        return data.autorun;
    }

    const char* getStaSSID() {
        return data.sta_ssid;
    }

    const char* getStaPassword() {
        return data.sta_password;
    }

    bool getStaAutoconnect() {
        return data.sta_autoconnect == 1;
    }

    void set(const char* name, const char* value) {
        if (strcmp(name, "ssid") == 0) {
            setSSID(value);
        } else if (strcmp(name, "password") == 0) {
            setPassword(value);
        } else if (strcmp(name, "channel") == 0) {
            setChannel(value);
        } else if (strcmp(name, "autorun") == 0) {
            setAutorun(value);
        } else if (strcmp(name, "sta_ssid") == 0) {
            setStaSSID(value);
        } else if (strcmp(name, "sta_password") == 0) {
            setStaPassword(value);
        } else if (strcmp(name, "sta_autoconnect") == 0) {
            setStaAutoconnect(value);
        }
    }

    void setSSID(const char* ssid) {
        if (ssid && (strlen(ssid) <= 32)) {
            memset(data.ssid, 0, 33);
            strncpy(data.ssid, ssid, 32);

            save();
        }
    }

    void setPassword(const char* password) {
        if (password && (strlen(password) >= 8) && (strlen(password) <= 64)) {
            memset(data.password, 0, 65);
            strncpy(data.password, password, 64);

            save();
        }
    }

    void setChannel(const char* channel) {
        if (channel && ((strcmp(channel, "auto") == 0) || ((atoi(channel) >= 1) && (atoi(channel) <= 13)))) {
            memset(data.channel, 0, 5);
            strncpy(data.channel, channel, 4);

            save();
        }
    }

    void setAutorun(const char* autorun) {
        if (autorun && (strlen(autorun) <= 64)) {
            memset(data.autorun, 0, 65);
            strncpy(data.autorun, autorun, 64);

            save();
        }
    }

    void setStaSSID(const char* ssid) {
        if (ssid && (strlen(ssid) <= 32)) {
            memset(data.sta_ssid, 0, 33);
            strncpy(data.sta_ssid, ssid, 32);

            save();
        }
    }

    void setStaPassword(const char* password) {
        if (password && (strlen(password) <= 64)) {
            memset(data.sta_password, 0, 65);
            strncpy(data.sta_password, password, 64);

            save();
        }
    }

    void setStaAutoconnect(const char* autoconnect) {
        if (!autoconnect) return;

        if ((strcmp(autoconnect, "1") == 0) ||
            equalsIgnoreCase(autoconnect, "on") ||
            equalsIgnoreCase(autoconnect, "true")) {
            setStaAutoconnectBool(true);
        } else if ((strcmp(autoconnect, "0") == 0) ||
                   equalsIgnoreCase(autoconnect, "off") ||
                   equalsIgnoreCase(autoconnect, "false")) {
            setStaAutoconnectBool(false);
        }
    }

    void setStaAutoconnectBool(bool autoconnect) {
        data.sta_autoconnect = autoconnect ? 1 : 0;
        save();
    }
}
