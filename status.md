# Project Status

## Snapshot
- Date: 2026-02-22
- Project: `SuperWiFiDuck`
- Focus of this pass: `GUI Kablo/Bluetooth HID output switch`
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

## Remaining Items
1. Run on-device acceptance checks when COM port is free:
   - GUI toggle behavior (S3 + S2)
   - BLE host connected/disconnected report behavior
   - Script no-op behavior when Bluetooth mode is active without BLE host
2. Optional security hardening:
   - Add access control gate for `settings_secrets`.
