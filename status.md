# Project Status

## Snapshot
- Date: 2026-02-22
- Project: `SuperWiFiDuck`
- Focus of latest pass: `Code Audit & Bug Fixes`
- Previous pass: `GUI Kablo/Bluetooth HID output switch`
- Firmware targets verified: `esp32-s2-kaluga-1`, `esp32-s3-devkitc-1`

## Implemented in This Pass

### 1) HID transport switching (USB/Bluetooth)
- Added transport abstraction in `HIDKeyboard`:
  - `KeyboardOutputTransport` enum (`USB`, `BLUETOOTH`)
  - `setTransport`, `getTransport`, `isBluetoothSupported`, `isBluetoothConnected`, `getBluetoothName`, `transportInfo`
  - Files: `src/keyboard.h`, `src/keyboard.cpp`
- Added BLE HID keyboard routing on ESP32-S3 only:
  - Uses `t-vk/ESP32 BLE Keyboard`
  - Device name fixed to `BLSWD`
  - If BLE not connected in Bluetooth mode, key reports are dropped (no-op) with throttled debug note.

### 2) Persistent settings model extension
- Added persistent key `input_transport` to settings schema:
  - Stored as enum (`0=usb`, `1=bluetooth`)
  - String aliases accepted: `usb|cable|kablo|bluetooth|ble`
  - Files: `src/settings.h`, `src/settings.cpp`
- Added new settings APIs:
  - `getInputTransport()`
  - `setInputTransport(const char*)`
  - `setInputTransportEnum(uint8_t)`
- Added hardware validation:
  - S2 rejects Bluetooth transport values.
  - S3 accepts Bluetooth transport values.
- Added migration path:
  - New settings magic: `1234567893`
  - Migrates previous struct (`1234567892`) with default `input_transport=usb`.

### 3) Runtime transport sync
- Added `duckparser` runtime bridge:
  - `applyConfiguredTransport()`
  - `inputInfo()`
  - Files: `src/duckparser.h`, `src/duckparser.cpp`
- Behavior:
  - Applies configured transport at startup.
  - Re-syncs periodically in loop (500ms check) for immediate GUI/CLI mode changes.
  - On apply failure, auto-fallback to USB and persists `input_transport=usb`.
- `main.cpp` updated to invoke sync at boot and runtime.

### 4) CLI extension
- New CLI command:
  - `input` -> prints:
    - `transport=...`
    - `ble_supported=...`
    - `ble_connected=...`
    - `ble_name=BLSWD`
  - File: `src/cli.cpp`
- `set input_transport ...` now triggers immediate runtime apply.

### 5) GUI extension (Settings page)
- Added new section:
  - Input Mode
  - Bluetooth Support
  - Bluetooth Connection
  - Bluetooth Device Name
  - Toggle button for `Kablo <-> Bluetooth`
  - S2 unsupported warning and disabled control
  - Files: `web/settings.html`, `web/settings.js`
- Added EN/TR i18n keys for input mode and BT status:
  - File: `web/i18n.js`

### 6) Build configuration
- Added BLE keyboard dependency only to S3 environments:
  - `env:esp32-s3-devkitc-1`
  - `env:esp32-s3-test`
  - File: `platformio.ini`

### 7) Documentation and embedded web assets
- README updated with Kablo/Bluetooth usage notes and `input` command.
  - File: `README.md`
- Regenerated embedded web assets:
  - `python webconverter.py`
  - `src/webfiles.h` updated.

## Verification Matrix
| Command | Result |
|---|---|
| `python webconverter.py` | PASS |
| `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe run -e esp32-s2-kaluga-1` | PASS |
| `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe run -e esp32-s3-devkitc-1` | PASS |
| `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe test --without-uploading --without-testing` | PASS (`esp32-s2-test`, `esp32-s3-test`) |

## Runtime Expectations (Post-Change)
- S3:
  - GUI/CLI `input_transport=bluetooth` switches HID output to BLE immediately.
  - `input` command reports BLE support and connection state.
  - BLE name is always `BLSWD`.
- S2:
  - Bluetooth input mode is blocked by validation.
  - GUI shows unsupported warning and disabled toggle.
  - USB HID behavior remains unchanged.

## Implemented in This Pass (Code Audit)

### 1) Critical bug fixes
- `src/parser.c:parse_lines1` — fixed out-of-bounds read `str[stri+1]` at end of loop.
- `src/duckscript.cpp:nextLine` — fixed `strtok` on non-null-terminated buffer in REPEAT branch; buffer now always null-terminated; loop guard changed to `buf_i < BUFFER_SIZE - 1`.
- `src/spiffs.cpp:size(String)` — fixed file handle leak; file now closed before return.
- `src/spiffs.cpp:create()` — added null-check before calling `.close()` on File.
- `src/duckscript.cpp:stopAll()` — `prevMessage` now freed and reset to NULL on stop, preventing stale REPEAT replay across script runs.

### 2) Dead code removal
- Removed `runTest()` and `repeat()` ghost declarations from `src/duckscript.h`.
- Removed `checkBootNum()`, `resetBootNum()`, `end()` from `src/eeprom.cpp` and `src/eeprom0.h`.
- Removed `streamReadUntil()` from `src/spiffs.cpp` and `src/spiffs.h`.
- Removed unused `bleInitialized` field from `src/keyboard.h` and `src/keyboard.cpp`.

### 3) Production noise / logging fixes
- Removed 4× `ESP_LOGI` calls from `src/spiffs.cpp` (always-on serial output in production).
- Changed spurious blank `debugln()` on buffer overflow to `debugln("WARNING: line truncated at BUFFER_SIZE")`.

### 4) Off-by-one / logic fixes
- `src/settings.cpp:safeStrLen` — changed `<= maxLen` to `< maxLen` to prevent reading one byte past field boundary.
- `src/led.cpp` — moved `#include <Arduino.h>` out of namespace body to file scope.

### 5) Web / tooling fixes
- `web/index.js` — fixed division-by-zero in memory display (`percent = byte/100` → proper percentage formula with zero-guard).
- `web/script.js` — fixed misleading `'\-'` escape in `fixFileName()` to plain `'-'`.
- `webconverter.py` — fixed `IndexError` crash on files without an extension (`split('.')[1]` → `rsplit('.', 1)` with length check).

## Remaining Items
1. Run on-device acceptance checks when COM port is free:
   - GUI toggle behavior (S3 + S2)
   - BLE host connected/disconnected report behavior
   - Script no-op behavior when Bluetooth mode is active without BLE host
2. Optional security hardening:
   - Add access control gate for `settings_secrets`.
3. Known architectural risk (not yet fixed — requires mutex/refactor):
   - FreeRTOS task in `duckscript::run()` accesses module-level `File f` and `bool running` without a mutex. Safe for single-script use but fragile under concurrent stop/run calls.
