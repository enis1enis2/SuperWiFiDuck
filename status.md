# Project Status

## Snapshot
- Project: `SuperWiFiDuck` (ESP32 S2/S3 BadUSB + WiFi management interface)
- Date reviewed: 2026-02-22 (updated)
- Files tracked in repo tree: 76
- Main firmware entry: `src/main.cpp`
- Build system: PlatformIO (`platformio.ini`)

## What Is Implemented
- USB HID keyboard startup path is active (`duckparser::beginKeyboard()`, `USB.begin()`).
- WiFi AP mode + captive DNS + async web server + websocket command bridge are implemented in `src/webserver.cpp`.
- Dual WiFi mode (AP+STA) is now implemented:
  - AP remains active at `192.168.4.1`
  - optional STA client connection to router network
  - LAN access via STA IP and `http://wifiduck.local`
- OTA update paths exist (ArduinoOTA and web `/update` endpoint).
- Script storage and file operations are implemented on LittleFS (`src/spiffs.cpp`).
- CLI command surface is broad (`help`, `settings`, `set`, `run`, `stop`, `ls`, `write`, `stream`, etc.) in `src/cli.cpp`.
- DuckyScript parsing/execution exists with support for `STRING`, `DELAY`, `DEFAULTDELAY`, `LOCALE`, `KEYCODE`, `LED` (`src/duckparser.cpp`, `src/duckscript.cpp`).
- Web UI pages are present (`web/index.html`, `web/settings.html`, `web/terminal.html`, etc.).

## Configuration Status
- `platformio.ini` has four envs:
  - `esp32-s2-kaluga-1`
  - `esp32-s3-devkitc-1`
  - `esp32-s2-test`
  - `esp32-s3-test`
- S3 env is configured with:
  - `upload_port = COM5`
  - `upload_speed = 115200`
  - `ARDUINO_USB_MODE=1`
  - `ARDUINO_USB_CDC_ON_BOOT=1`
- Default AP credentials in `src/config.h`:
  - SSID: `wifiduck`
  - Password: `wifiduck`

## Current Working Tree
- Modified:
  - `README.md`
  - `custom_usb_descriptors/USB.cpp`
  - `platformio.ini`
  - `src/cli.cpp`
  - `src/duckparser.h`
  - `src/duckparser.cpp`
  - `src/locale/locale_types.h`
  - `src/locales.h`
  - `src/settings.cpp`
  - `src/spiffs.cpp`
  - `src/webserver.cpp`
  - `test/test_parser_locale/test_main.cpp`
  - `web/credits.html`
  - `web/index.html`
- Untracked:
  - `SuperWiFiDuck.code-workspace`
  - `src/locale/locale_tr.h`
  - `status.md`
  - `todo.md`

## Locale Status
- Turkish locale file exists: `src/locale/locale_tr.h`.
- Integrated into runtime locale selection:
  - Include added in `src/locales.h`
  - `LOCALE TR` branch added in `src/duckparser.cpp`
  - Listed in web UI locale help (`web/index.html`)
  - Listed in README keyboard layout section

## Issues Fixed In This Pass
- Removed `#include <dummy.h>` from `src/cli.cpp`.
- Hardened websocket message handling in `src/webserver.cpp` by copying frame payload to owned null-terminated memory before parsing.
- Changed SPIFFS semantics in `src/spiffs.cpp`:
  - `open()` now uses read mode (`"r"`)
  - write paths explicitly use append mode (`"a+"`)
  - removed unnecessary pre-open in `remove()`
- Updated README references for ESP32 usage and flash links.
- Added safer handling for missing `LOCALE` argument in `src/duckparser.cpp`.
- Fixed Turkish locale compile errors in `src/locale/locale_tr.h`:
  - replaced unsupported `KEY_NONUS_BACKSLASH` usages with `KEY_102ND`
  - aligned `hid_locale_t` initialization pointer types with explicit casts
- Removed dead/noisy code and warnings:
  - replaced deprecated `beginResponse_P` with `beginResponse` in `src/webserver.cpp`
  - removed redundant `if (true)` branch in `src/cli.cpp` `status` command
  - removed unused local variable in `src/cli.cpp` stream read path
  - removed noisy SPIFFS write logs in `src/spiffs.cpp`
  - resolved S3 `USB_PID` macro redefinition warnings by switching to `USB_CUSTOM_PID`
    in `platformio.ini` and `custom_usb_descriptors/USB.cpp`
- Performed full code scan pass and completed all items from `todo.md`:
  - fixed OTA typo in `src/webserver.cpp` (`Recieve` -> `Receive`)
  - removed dead `repeatNum/getRepeats` path in duck parser (`src/duckparser.h`, `src/duckparser.cpp`)
  - corrected settings constant spelling to `SETTINGS_ADDRESS` in `src/settings.cpp`
  - changed SPIFFS startup success/failure logs from error level to debug text in `src/spiffs.cpp`
  - corrected README typos/inaccuracies:
    - `Expressif` -> `Espressif`
    - `platform.ini` -> `platformio.ini`
    - updated old core reference to ESP32 core
  - aligned web credits core reference with ESP32 (`web/credits.html`)
- Implemented AP+STA feature set end-to-end:
  - extended persistent settings with `sta_ssid`, `sta_password`, `sta_autoconnect` (`src/settings.cpp`, `src/settings.h`)
  - added settings migration path from legacy struct to new struct (`src/settings.cpp`)
  - added runtime STA controls/info API: `staConnect()`, `staDisconnect()`, `wifiInfo()` (`src/webserver.h`, `src/webserver.cpp`)
  - switched WiFi startup to `WIFI_AP_STA` and added non-blocking reconnect loop with 10s backoff (`src/webserver.cpp`)
  - enabled mDNS startup/retry and exposed endpoint info (`src/webserver.cpp`)
  - added CLI commands: `wifi`, `sta_connect`, `sta_disconnect` (`src/cli.cpp`)
  - updated Settings web UI with STA controls/status (`web/settings.html`, `web/settings.js`)
  - added EN/TR translations for STA GUI strings (`web/i18n.js`)
  - regenerated embedded web assets (`src/webfiles.h`) via `python webconverter.py`
  - updated README usage with AP+STA LAN access note (`README.md`)

## Remaining Items
1. Run on-device tests once target board/serial port is available (current `pio test` upload step fails with `Error 2` on both test envs).

## Notes
- PlatformIO CLI is available via `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe`.
- Verified builds:
  - `pio run -e esp32-s2-kaluga-1`: SUCCESS
  - `pio run -e esp32-s3-devkitc-1`: SUCCESS
- Full rebuild pass after TODO execution:
  - `pio run -e esp32-s2-kaluga-1`: SUCCESS
  - `pio run -e esp32-s3-devkitc-1`: SUCCESS
- Remaining-items pass (this turn):
  - Added real test suite: `test/test_parser_locale/test_main.cpp`
  - Added dedicated test environment: `env:esp32-s2-test` in `platformio.ini`
  - Added dedicated test environment: `env:esp32-s3-test` in `platformio.ini`
  - `pio test -e esp32-s2-test --without-uploading --without-testing`: PASSED (test firmware build successful)
  - `pio test -e esp32-s3-test --without-uploading --without-testing`: PASSED (test firmware build successful)
  - `pio test -e esp32-s2-test`: upload failed (`*** [upload] Error 2`)
  - `pio test -e esp32-s3-test`: upload failed (`*** [upload] Error 2`)
  - Verified API cleanup safety: no remaining `getRepeats()` references in codebase
  - Reviewed `src/locale/locale_types.h`: added `<stdint.h>` and `<stddef.h>` includes are valid and consistent with type usage
- Runtime/flash-on-device tests were not executed in this review.
- Latest verification pass (2026-02-22, AP+STA implementation):
  - `python webconverter.py`: SUCCESS (web assets regenerated)
  - `pio run -e esp32-s2-kaluga-1`: SUCCESS
  - `pio run -e esp32-s3-devkitc-1`: SUCCESS
  - `pio test --without-uploading --without-testing`: PASSED (`esp32-s2-test`, `esp32-s3-test`)
