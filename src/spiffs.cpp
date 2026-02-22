/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "spiffs.h"

#include "config.h"
#include "debug.h"


namespace spiffs {
    File streamFile;

    // ===== PRIVATE ===== //
    void fixPath(String& path) {
        if (!path.startsWith("/")) {
            path = "/" + path;
        }
    }

    // ===== PUBLIC ====== //
    void begin() {
        debug("Initializing SPIFFS...");
        LittleFS.begin(true);
        debugln("OK");

        String FILE_NAME = "/startup_spiffs_test";

        remove(FILE_NAME);
        create(FILE_NAME);
        File f = open(FILE_NAME);
        if (!f) {
            debugln("Startup FS test failed");
            format();
        } else {
            f.close();
            debugln("Startup FS test passed");
            remove(FILE_NAME);
        }
    }

    void format() {
        debug("Formatting SPIFFS...");
        LittleFS.format();
        debugln("OK");
    }

    size_t size() {
        return LittleFS.totalBytes();
    }

    size_t usedBytes() {
        return LittleFS.usedBytes();
    }

    size_t freeBytes() {
        return LittleFS.totalBytes() - LittleFS.usedBytes();
    }

    size_t size(String fileName) {
        fixPath(fileName);

        File f = LittleFS.open(fileName, "r");
        if (!f) return 0;
        size_t s = f.size();
        f.close();
        return s;
    }

    bool exists(String fileName) {
        return LittleFS.exists(fileName);
    }

    File open(String fileName) {
        fixPath(fileName);

        File f = LittleFS.open(fileName, "r");

        if (f) f.seek(0);

        return f;
    }

    void create(String fileName) {
        fixPath(fileName);

        File f = LittleFS.open(fileName, "a+");
        if (f) f.close();
    }

    void remove(String fileName) {
        fixPath(fileName);

        LittleFS.remove(fileName);
    }

    void rename(String oldName, String newName) {
        fixPath(oldName);
        fixPath(newName);

        LittleFS.rename(oldName, newName);
    }

    void write(String fileName, const char* str) {
        fixPath(fileName);
        File f = LittleFS.open(fileName, "a+");

        if (f) {
            f.println(str);
            f.close();
            debugln("Wrote file");
        } else {
            debugln("File error");
        }
    }

    void write(String fileName, const uint8_t* buf, size_t len) {
        fixPath(fileName);
        File f = LittleFS.open(fileName, "a+");

        if (f) {
            f.write(buf, len);
            f.close();
            debugln("Wrote file");
        } else {
            debugln("File error");
        }
    }

    String listDir(String dirName) {

        String res;

        fixPath(dirName);

        File root = LittleFS.open(dirName, "r");

        File file = root.openNextFile();

        while (file) {
            res += file.name();
            res += ' ';
            res += size(file.name());
            res += '\n';
            file = root.openNextFile();
        }

        if (res.length() == 0) {
            res += "\n";
        }

        return res;
    }

    void streamOpen(String fileName) {
        streamClose();
        fixPath(fileName);
        streamFile = LittleFS.open(fileName, "a+");
        if (streamFile) streamFile.seek(0);
        if (!streamFile) debugln("ERROR: No stream file open");
    }

    void streamWrite(const char* buf, size_t len) {
        if (streamFile) streamFile.write((uint8_t*)buf, len);
        else debugln("ERROR: No stream file open");
    }

    size_t streamRead(char* buf, size_t len) {
        if (streamFile) {
            size_t i;

            for (i = 0; i<len; ++i) {
                if (!streamFile.available() || (i == len-1)) {
                    buf[i] = '\0';
                    break;
                } else {
                    buf[i] = streamFile.read();
                }
            }

            return i;
        } else {
            debugln("ERROR: No stream file open");
            return 0;
        }
    }

    void streamClose() {
        streamFile.close();
    }

    bool streaming() {
        return streamFile;
    }

    size_t streamAvailable() {
        if (!streamFile) return 0;
        return streamFile.available();
    }
}
