# TODO

## Completed: Kablo/Bluetooth HID Output Switch
- [x] Add transport abstraction to keyboard layer (`USB` / `BLUETOOTH`).
- [x] Add S3-only BLE HID keyboard backend with fixed name `BLSWD`.
- [x] Keep single-channel output behavior (no dual send, no fallback send).
- [x] Add runtime transport info output (`transport`, `ble_supported`, `ble_connected`, `ble_name`).

## Completed: Settings and Migration
- [x] Add persistent setting key `input_transport`.
- [x] Add alias parsing (`usb|cable|kablo|bluetooth|ble`).
- [x] Add S2/S3 hardware validation for Bluetooth mode.
- [x] Add settings migration for new schema (`magic=1234567893`).
- [x] Include `input_transport` in `settings` output.

## Completed: Runtime and CLI
- [x] Add `duckparser::applyConfiguredTransport()` and `duckparser::inputInfo()`.
- [x] Apply transport at startup and periodic runtime sync in `loop()`.
- [x] Add CLI command `input`.
- [x] Trigger transport apply after `set input_transport ...`.

## Completed: GUI and i18n
- [x] Add Input Mode section to Settings GUI.
- [x] Add Kablo/Bluetooth toggle control.
- [x] Add Bluetooth support/connection/name rows.
- [x] Disable control on S2 and show unsupported warning.
- [x] Add EN/TR translations for new labels and states.

## Completed: Build and Packaging
- [x] Add BLE keyboard dependency to S3 firmware env.
- [x] Add BLE keyboard dependency to S3 test env.
- [x] Regenerate embedded web assets (`python webconverter.py`).
- [x] Rebuild S2 firmware (`esp32-s2-kaluga-1`).
- [x] Rebuild S3 firmware (`esp32-s3-devkitc-1`).
- [x] Run test firmware build (`pio test --without-uploading --without-testing`).

## External Blockers
- [ ] Run on-device acceptance tests when COM port is available and free.

## Optional Next Steps
- [ ] Add optional pairing/security controls for BLE mode (PIN/passkey policy).
- [ ] Add GUI hint for current BLE host name if available from stack callbacks.

## Completed: Code Audit & Bug Fixes (2026-02-22)
- [x] Fix file handle leak in `spiffs::size(String)` — file was never closed.
- [x] Fix null-dereference in `spiffs::create()` — `.close()` called on invalid File.
- [x] Fix out-of-bounds read in `parser.c:parse_lines1` — `str[stri+1]` read past buffer end.
- [x] Fix `strtok` on non-null-terminated buffer in `duckscript::nextLine()` REPEAT branch.
- [x] Fix `prevMessage` not reset between script runs in `duckscript::stopAll()`.
- [x] Remove dead declarations `runTest()` and `repeat()` from `duckscript.h`.
- [x] Remove dead functions `checkBootNum()`, `resetBootNum()`, `end()` from `eeprom.cpp`/`eeprom0.h`.
- [x] Remove dead function `streamReadUntil()` from `spiffs.cpp`/`spiffs.h`.
- [x] Remove unused field `bleInitialized` from `keyboard.h`/`keyboard.cpp`.
- [x] Fix `#include <Arduino.h>` inside namespace body in `led.cpp`.
- [x] Fix `safeStrLen` off-by-one (`<= maxLen` → `< maxLen`) in `settings.cpp`.
- [x] Fix division-by-zero risk in memory display in `web/index.js`.
- [x] Fix misleading `'\-'` escape in `fixFileName()` in `web/script.js`.
- [x] Fix `IndexError` on extension-less files in `webconverter.py`.
- [x] Remove `ESP_LOGI` production noise from `spiffs.cpp` (open/stream paths).
- [x] Fix spurious blank `debugln()` on buffer overflow in `duckscript.cpp`.
