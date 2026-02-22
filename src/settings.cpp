/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "settings.h"

#include "debug.h"
#include "config.h"
#include "eeprom0.h"
#include <ctype.h>
#include <cstring>
#include <sdkconfig.h>

#define SETTINGS_ADDRESS 1
#define SETTINGS_MAGIC_NUM_LEGACY 1234567891
#define SETTINGS_MAGIC_NUM_PREVIOUS 1234567892
#define SETTINGS_MAGIC_NUM 1234567893

namespace settings {
    // ===== PRIVATE ===== //
    enum InputTransport : uint8_t {
        INPUT_TRANSPORT_USB = 0,
        INPUT_TRANSPORT_BLUETOOTH = 1
    };

    typedef struct legacy_settings_t {
        uint32_t magic_num;
        char ssid[33];
        char password[65];
        char channel[5];
        char autorun[65];
    } legacy_settings_t;

    typedef struct previous_settings_t {
        uint32_t magic_num;
        char ssid[33];
        char password[65];
        char channel[5];
        char autorun[65];
        char sta_ssid[33];
        char sta_password[65];
        uint8_t sta_autoconnect;
    } previous_settings_t;

    typedef struct settings_t {
        uint32_t magic_num;
        char ssid[33];
        char password[65];
        char channel[5];
        char autorun[65];
        char sta_ssid[33];
        char sta_password[65];
        uint8_t sta_autoconnect;
        uint8_t input_transport;
    } settings_t;

    settings_t data;

    bool isBluetoothTransportSupported() {
#if defined(CONFIG_IDF_TARGET_ESP32S3) && defined(CONFIG_BT_ENABLED)
        return true;
#else
        return false;
#endif
    }

    size_t safeStrLen(const char* str, size_t maxLen) {
        size_t len = 0;
        while ((len <= maxLen) && (str[len] != '\0')) ++len;
        return len;
    }

    bool equalsIgnoreCase(const char* a, const char* b) {
        if (!a || !b) return false;
        while (*a && *b) {
            if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
            ++a;
            ++b;
        }
        return *a == '\0' && *b == '\0';
    }

    bool isDigitsOnly(const char* str) {
        if (!str || *str == '\0') return false;
        while (*str) {
            if (!isdigit((unsigned char)*str)) return false;
            ++str;
        }
        return true;
    }

    bool isValidChannelValue(const char* channel) {
        if (!channel) return false;
        if (strcmp(channel, "auto") == 0) return true;
        if (!isDigitsOnly(channel)) return false;

        int ch = atoi(channel);
        return (ch >= 1) && (ch <= 13);
    }

    bool parseTransportValue(const char* value, uint8_t* transportOut) {
        if (!value || !transportOut) return false;

        if (equalsIgnoreCase(value, "usb") ||
            equalsIgnoreCase(value, "cable") ||
            equalsIgnoreCase(value, "kablo")) {
            *transportOut = INPUT_TRANSPORT_USB;
            return true;
        }

        if (equalsIgnoreCase(value, "bluetooth") ||
            equalsIgnoreCase(value, "ble")) {
            *transportOut = INPUT_TRANSPORT_BLUETOOTH;
            return true;
        }

        return false;
    }

    bool setSSIDInternal(const char* ssid, bool persist) {
        if (!ssid) return false;
        size_t len = strlen(ssid);
        if ((len < 1) || (len > 32)) return false;

        memset(data.ssid, 0, sizeof(data.ssid));
        strncpy(data.ssid, ssid, sizeof(data.ssid) - 1);
        if (persist) save();
        return true;
    }

    bool setPasswordInternal(const char* password, bool persist) {
        if (!password) return false;
        size_t len = strlen(password);
        if ((len < 8) || (len > 64)) return false;

        memset(data.password, 0, sizeof(data.password));
        strncpy(data.password, password, sizeof(data.password) - 1);
        if (persist) save();
        return true;
    }

    bool setChannelInternal(const char* channel, bool persist) {
        if (!isValidChannelValue(channel)) return false;

        memset(data.channel, 0, sizeof(data.channel));
        strncpy(data.channel, channel, sizeof(data.channel) - 1);
        if (persist) save();
        return true;
    }

    bool setAutorunInternal(const char* autorun, bool persist) {
        if (!autorun) return false;
        if (strlen(autorun) > 64) return false;

        memset(data.autorun, 0, sizeof(data.autorun));
        strncpy(data.autorun, autorun, sizeof(data.autorun) - 1);
        if (persist) save();
        return true;
    }

    bool setStaSSIDInternal(const char* ssid, bool persist) {
        if (!ssid) return false;
        if (strlen(ssid) > 32) return false;

        memset(data.sta_ssid, 0, sizeof(data.sta_ssid));
        strncpy(data.sta_ssid, ssid, sizeof(data.sta_ssid) - 1);
        if (persist) save();
        return true;
    }

    bool setStaPasswordInternal(const char* password, bool persist) {
        if (!password) return false;
        size_t len = strlen(password);
        if (!((len == 0) || ((len >= 8) && (len <= 64)))) return false;

        memset(data.sta_password, 0, sizeof(data.sta_password));
        strncpy(data.sta_password, password, sizeof(data.sta_password) - 1);
        if (persist) save();
        return true;
    }

    bool setStaAutoconnectInternal(bool autoconnect, bool persist) {
        data.sta_autoconnect = autoconnect ? 1 : 0;
        if (persist) save();
        return true;
    }

    bool setInputTransportInternal(uint8_t transport, bool persist) {
        if (transport > INPUT_TRANSPORT_BLUETOOTH) return false;
        if ((transport == INPUT_TRANSPORT_BLUETOOTH) && !isBluetoothTransportSupported()) return false;

        data.input_transport = transport;
        if (persist) save();
        return true;
    }

    bool parseBoolValue(const char* value, bool* result) {
        if (!value || !result) return false;

        if ((strcmp(value, "1") == 0) ||
            equalsIgnoreCase(value, "on") ||
            equalsIgnoreCase(value, "true")) {
            *result = true;
            return true;
        }

        if ((strcmp(value, "0") == 0) ||
            equalsIgnoreCase(value, "off") ||
            equalsIgnoreCase(value, "false")) {
            *result = false;
            return true;
        }

        return false;
    }

    bool isStoredAPSSIDValid() {
        size_t len = safeStrLen(data.ssid, 32);
        return (len >= 1) && (len <= 32);
    }

    bool isStoredAPPasswordValid() {
        size_t len = safeStrLen(data.password, 64);
        return (len >= 8) && (len <= 64);
    }

    bool isStoredChannelValid() {
        size_t len = safeStrLen(data.channel, 4);
        if ((len == 0) || (len > 4)) return false;
        return isValidChannelValue(data.channel);
    }

    bool isStoredAutorunValid() {
        size_t len = safeStrLen(data.autorun, 64);
        return len <= 64;
    }

    bool isStoredStaSSIDValid() {
        size_t len = safeStrLen(data.sta_ssid, 32);
        return len <= 32;
    }

    bool isStoredStaPasswordValid() {
        size_t len = safeStrLen(data.sta_password, 64);
        return (len == 0) || ((len >= 8) && (len <= 64));
    }

    bool isStoredStaAutoconnectValid() {
        return data.sta_autoconnect <= 1;
    }

    bool isStoredInputTransportValid() {
        if (data.input_transport > INPUT_TRANSPORT_BLUETOOTH) return false;
        if ((data.input_transport == INPUT_TRANSPORT_BLUETOOTH) && !isBluetoothTransportSupported()) return false;
        return true;
    }

    String maskSecret(const char* value, bool includeSecrets) {
        if (!value || strlen(value) == 0) return "-";
        if (includeSecrets) return String(value);
        return "********";
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

        if (!(setSSIDInternal(oldData.ssid, false))) setSSIDInternal(WIFI_SSID, false);
        if (!(setPasswordInternal(oldData.password, false))) setPasswordInternal(WIFI_PASSWORD, false);
        if (!(setChannelInternal(oldData.channel, false))) setChannelInternal(WIFI_CHANNEL, false);
        if (!(setAutorunInternal(oldData.autorun, false))) setAutorunInternal("", false);
        setStaSSIDInternal("", false);
        setStaPasswordInternal("", false);
        setStaAutoconnectInternal(false, false);
        setInputTransportInternal(INPUT_TRANSPORT_USB, false);
        save();
    }

    void migratePreviousSettings() {
        previous_settings_t oldData;
        memset(&oldData, 0, sizeof(oldData));
        eeprom::getObject(SETTINGS_ADDRESS, oldData);

        if (oldData.magic_num != SETTINGS_MAGIC_NUM_PREVIOUS) {
            reset();
            return;
        }

        memset(&data, 0, sizeof(data));
        data.magic_num = SETTINGS_MAGIC_NUM;

        if (!(setSSIDInternal(oldData.ssid, false))) setSSIDInternal(WIFI_SSID, false);
        if (!(setPasswordInternal(oldData.password, false))) setPasswordInternal(WIFI_PASSWORD, false);
        if (!(setChannelInternal(oldData.channel, false))) setChannelInternal(WIFI_CHANNEL, false);
        if (!(setAutorunInternal(oldData.autorun, false))) setAutorunInternal("", false);
        if (!(setStaSSIDInternal(oldData.sta_ssid, false))) setStaSSIDInternal("", false);
        if (!(setStaPasswordInternal(oldData.sta_password, false))) setStaPasswordInternal("", false);
        setStaAutoconnectInternal(oldData.sta_autoconnect == 1, false);
        setInputTransportInternal(INPUT_TRANSPORT_USB, false);
        save();
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

        if (data.magic_num == SETTINGS_MAGIC_NUM_PREVIOUS) {
            migratePreviousSettings();
            return;
        }

        if (data.magic_num != SETTINGS_MAGIC_NUM) {
            reset();
            return;
        }

        bool changed = false;

        if (!isStoredAPSSIDValid()) changed |= setSSIDInternal(WIFI_SSID, false);
        if (!isStoredAPPasswordValid()) changed |= setPasswordInternal(WIFI_PASSWORD, false);
        if (!isStoredChannelValid()) changed |= setChannelInternal(WIFI_CHANNEL, false);
        if (!isStoredAutorunValid()) changed |= setAutorunInternal("", false);
        if (!isStoredStaSSIDValid()) changed |= setStaSSIDInternal("", false);
        if (!isStoredStaPasswordValid()) changed |= setStaPasswordInternal("", false);
        if (!isStoredStaAutoconnectValid()) changed |= setStaAutoconnectInternal(false, false);
        if (!isStoredInputTransportValid()) changed |= setInputTransportInternal(INPUT_TRANSPORT_USB, false);

        if (changed) save();
    }

    void reset() {
        debugln("Resetting Settings");

        memset(&data, 0, sizeof(data));
        data.magic_num = SETTINGS_MAGIC_NUM;

        setSSIDInternal(WIFI_SSID, false);
        setPasswordInternal(WIFI_PASSWORD, false);
        setChannelInternal(WIFI_CHANNEL, false);
        setAutorunInternal("", false);
        setStaSSIDInternal("", false);
        setStaPasswordInternal("", false);
        setStaAutoconnectInternal(false, false);
        setInputTransportInternal(INPUT_TRANSPORT_USB, false);
        save();
    }

    void save() {
        debugln("Saving Settings");
        eeprom::saveObject(SETTINGS_ADDRESS, data);
    }

    String toString(bool includeSecrets) {
        String s;
        s.reserve(288);

        s += "ssid=";
        s += getSSID();
        s += "\n";
        s += "password=";
        s += maskSecret(getPassword(), includeSecrets);
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
        s += maskSecret(getStaPassword(), includeSecrets);
        s += "\n";
        s += "sta_autoconnect=";
        s += getStaAutoconnect() ? "1" : "0";
        s += "\n";
        s += "input_transport=";
        s += getInputTransport();
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

    const char* getInputTransport() {
        return (data.input_transport == INPUT_TRANSPORT_BLUETOOTH) ? "bluetooth" : "usb";
    }

    bool set(const char* name, const char* value) {
        if (!name || !value) return false;

        if (strcmp(name, "ssid") == 0) return setSSID(value);
        if (strcmp(name, "password") == 0) return setPassword(value);
        if (strcmp(name, "channel") == 0) return setChannel(value);
        if (strcmp(name, "autorun") == 0) return setAutorun(value);
        if (strcmp(name, "sta_ssid") == 0) return setStaSSID(value);
        if (strcmp(name, "sta_password") == 0) return setStaPassword(value);
        if (strcmp(name, "sta_autoconnect") == 0) return setStaAutoconnect(value);
        if (strcmp(name, "input_transport") == 0) return setInputTransport(value);
        return false;
    }

    bool setSSID(const char* ssid) {
        return setSSIDInternal(ssid, true);
    }

    bool setPassword(const char* password) {
        return setPasswordInternal(password, true);
    }

    bool setChannel(const char* channel) {
        return setChannelInternal(channel, true);
    }

    bool setAutorun(const char* autorun) {
        return setAutorunInternal(autorun, true);
    }

    bool setStaSSID(const char* ssid) {
        return setStaSSIDInternal(ssid, true);
    }

    bool setStaPassword(const char* password) {
        return setStaPasswordInternal(password, true);
    }

    bool setStaAutoconnect(const char* autoconnect) {
        bool parsed = false;
        if (!parseBoolValue(autoconnect, &parsed)) return false;
        return setStaAutoconnectInternal(parsed, true);
    }

    bool setInputTransport(const char* transport) {
        uint8_t parsed = INPUT_TRANSPORT_USB;
        if (!parseTransportValue(transport, &parsed)) return false;
        return setInputTransportInternal(parsed, true);
    }

    void setInputTransportEnum(uint8_t transport) {
        setInputTransportInternal(transport, true);
    }

    void setStaAutoconnectBool(bool autoconnect) {
        setStaAutoconnectInternal(autoconnect, true);
    }
}
