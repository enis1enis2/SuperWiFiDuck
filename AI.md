# AI.md — Super WiFi Duck

> **Purpose:** This file is the authoritative instruction manual for any AI assistant working on this codebase. Read it in full before making any changes. Keep it updated whenever the project evolves.

---

## 1. Project Identity

| Field | Value |
|---|---|
| **Name** | Super WiFi Duck |
| **Version** | 1.3.0 (see `src/config.h`) |
| **Type** | Embedded firmware + web UI |
| **Platform** | ESP32-S2 / ESP32-S3 (Espressif, Arduino framework) |
| **Build system** | PlatformIO (`platformio.ini`) |
| **License** | MIT |
| **Repo** | https://github.com/enis1enis2/SuperWiFiDuck |

**What it does:** A BadUSB / keystroke-injection tool. The device plugs into a USB port, emulates a HID keyboard, and executes Ducky Script payloads delivered via a WiFi web interface. No extra hardware required.

---

## 2. Repository Layout

```
/
├── src/                    Firmware source (C++17 + C)
│   ├── main.cpp            Entry point — setup() / loop()
│   ├── config.h            Compile-time constants (VERSION, pins, EEPROM addresses)
│   ├── debug.h             Conditional debug macros (ENABLE_DEBUG guard)
│   ├── keyboard.cpp/.h     HIDKeyboard class — USB + BLE transport
│   ├── duckparser.cpp/.h   Ducky Script interpreter + transport bridge
│   ├── duckscript.cpp/.h   Script runner (FreeRTOS task)
│   ├── settings.cpp/.h     EEPROM-backed settings with migration
│   ├── cli.cpp/.h          SimpleCLI command set
│   ├── webserver.cpp/.h    AsyncWebServer, WiFi AP+STA, OTA, WebSocket
│   ├── spiffs.cpp/.h       LittleFS file storage
│   ├── eeprom.cpp          EEPROM init wrapper
│   ├── eeprom0.h           EEPROM template helpers
│   ├── led.cpp/.h          RGB LED abstraction (NeoPixel / DotStar / RGB / stub)
│   ├── locales.h           Aggregates all locale headers
│   ├── parser.c/.h         C line/word parser (shared with SimpleCLI)
│   ├── locale/             Per-language HID key maps (19 locales)
│   ├── NeoPixel.cpp/.h     WS2812 driver
│   ├── Adafruit_DotStar.*  DotStar driver
│   └── webfiles.h          Gzip-embedded web assets (auto-generated)
├── web/                    Web UI source (HTML + CSS + Vanilla JS)
│   ├── index.html/.js      Script editor + network dashboard
│   ├── settings.html/.js   Settings page (WiFi, STA, Input Mode)
│   ├── terminal.html/.js   WebSocket CLI terminal
│   ├── script.js           Shared WebSocket + helper utilities
│   ├── i18n.js             EN/TR translation engine
│   └── style.css           Shared stylesheet
├── custom_usb_descriptors/ Patched USB.cpp for custom VID/PID
├── test/                   PlatformIO unit tests (native + ESP32)
├── testscript/             Sample Ducky Script files
├── webconverter.py         Converts web/ → src/webfiles.h (gzip PROGMEM)
├── platformio.ini          Build environments
├── README.md               User-facing documentation (Turkish)
├── todo.md                 Task tracking
├── status.md               Development pass log
└── AI.md                   ← This file
```

---

## 3. Target Hardware & Build Environments

| PlatformIO env | Board | USB HID | BLE HID | Notes |
|---|---|---|---|---|
| `esp32-s2-kaluga-1` | ESP32-S2 | ✅ | ❌ | USB-only |
| `esp32-s3-devkitc-1` | ESP32-S3 | ✅ | ✅ (`BLSWD`) | Full dual-transport |
| `esp32-s2-test` | ESP32-S2 | ✅ | ❌ | Test build |
| `esp32-s3-test` | ESP32-S3 | ✅ | ✅ | Test build |
| `native` | Host PC | — | — | Parser unit tests only |

**BLE dependency** (`t-vk/ESP32 BLE Keyboard@^0.3.2`) is added **only** to S3 environments. Never add it to S2 environments.

**USB VID/PID spoofing** is done via `platformio.ini` build flags:
```ini
-D USB_VID=0x05ac
-D USB_CUSTOM_PID=0x0267
-D USB_MANUFACTURER='"Apple Inc."'
-D USB_PRODUCT='"Apple Magic Keyboard"'
```

---

## 4. Language Stack & Coding Standards

### 4.1 Firmware (C++17)

- **Standard:** C++17 (`-std=c++17` in native env; Arduino ESP32 core uses C++17 by default).
- **Namespaces:** Every module uses a `namespace` matching its filename (e.g., `namespace settings`, `namespace spiffs`). Do not use global functions outside a namespace.
- **Headers:** Use `#pragma once`. Never use include guards.
- **Includes inside namespaces:** **Forbidden.** Always place `#include` at file scope.
- **Arduino types:** Use `String` (Arduino) for user-facing strings. Use `const char*` / `char[]` for internal buffers.
- **Memory:**
  - Prefer stack allocation for small fixed-size buffers.
  - When heap-allocating (`malloc`/`new`), always check for null and always free/delete on all exit paths.
  - `BUFFER_SIZE` (256) is the maximum line buffer. Always null-terminate before passing to C string functions.
- **Debug logging:**
  - Use `debugln(...)` / `debugf(...)` macros from `debug.h` — compiled out unless `ENABLE_DEBUG` is defined.
  - **Never** use `ESP_LOGI` / `ESP_LOGE` in production paths — these are always compiled in and produce serial noise.
  - `Serial.print` is acceptable only inside `#ifdef ENABLE_DEBUG` blocks.
- **FreeRTOS:** The script runner (`duckscript.cpp`) uses `xTaskCreate`. Any access to shared state (`File f`, `bool running`, `prevMessage`) from multiple tasks requires a mutex. Currently there is no mutex — this is a known risk documented in `status.md`. Do not add new shared state without a mutex.
- **Conditional compilation:**
  - BLE code: `#if defined(CONFIG_IDF_TARGET_ESP32S3) && defined(CONFIG_BT_ENABLED)`
  - S2/S3 detection: `CONFIG_IDF_TARGET_ESP32S2` / `CONFIG_IDF_TARGET_ESP32S3`
  - LED type: `#if defined(NEOPIXEL)` / `#elif defined(DOTSTAR)` / `#elif defined(LED_RGB)`

### 4.2 C (parser)

- `src/parser.c` and `src/parser.h` are pure C (no C++ features).
- All functions are suffixed `1` (e.g., `parse_lines1`, `compare1`) to avoid name collisions.
- When iterating with `stri <= len`, always guard `str[stri+1]` as `(stri+1 <= len) ? str[stri+1] : 0`.

### 4.3 Web UI (Vanilla JS, no frameworks)

- **No build step** for JS/HTML/CSS — files are plain and served gzip-compressed from PROGMEM.
- **No ES6 modules, no `import`/`export`** — all scripts are loaded via `<script src="...">` tags.
- **Global helpers** defined in `script.js`: `E(id)`, `ws_send()`, `ws_send_raw()`, `ws_init()`, `status()`, `fixFileName()`, `download_txt()`.
- **i18n:** Use `tr("key")` for all user-visible strings. Add keys to both `en` and `tr` objects in `web/i18n.js`. Use `data-i18n="key"` attributes in HTML for static text.
- **WebSocket protocol:** Single-channel request/response. One message sent at a time; next message sent only after response received (`cts` flag in `script.js`). Do not bypass this queue.
- **DOM manipulation:** Use `el.textContent` for plain text, `el.innerHTML` only when HTML is intentional. Never inject unsanitized user input into `innerHTML`.
- **Memory display:** Always guard against division by zero when computing percentages from firmware-reported sizes.

### 4.4 Python (webconverter)

- Python 3 only.
- Use `rsplit('.', 1)` with a length check when extracting file extensions — never `split('.')[1]`.
- The script reads all files in `web/` and embeds them as gzip PROGMEM arrays in `src/webfiles.h`. Run it after any web file change.

---

## 5. Module Reference

### 5.1 `settings` module

**EEPROM layout:**
- Address `1`: `settings_t` struct (magic `1234567893`)
- Address `3210`: boot counter (legacy, no longer used)

**Migration chain:** `1234567891` (legacy) → `1234567892` (previous) → `1234567893` (current)

**Settings keys:**

| Key | Type | Constraints |
|---|---|---|
| `ssid` | `char[33]` | 1–32 chars |
| `password` | `char[65]` | 8–64 chars |
| `channel` | `char[5]` | `"auto"` or `"1"`–`"13"` |
| `autorun` | `char[65]` | 0–64 chars, empty = disabled |
| `sta_ssid` | `char[33]` | 0–32 chars, empty = STA disabled |
| `sta_password` | `char[65]` | empty or 8–64 chars |
| `sta_autoconnect` | `uint8_t` | `0` or `1` |
| `input_transport` | `uint8_t` | `0`=USB, `1`=Bluetooth (S3 only) |

**When adding a new setting:**
1. Bump `SETTINGS_MAGIC_NUM` by 1.
2. Add the field to `settings_t`.
3. Add a `previous_settings_t` snapshot of the old struct.
4. Add a `migratePreviousSettings()` function.
5. Add getter, setter, validation, and `toString()` output.
6. Add the key to `set()` dispatch.

### 5.2 `keyboard` module

**Transport enum:**
```cpp
enum class KeyboardOutputTransport : uint8_t {
  USB = 0,
  BLUETOOTH = 1
};
```

**Key rule:** Only one transport is active at a time. Never send to both simultaneously.

**BLE device name** is hardcoded as `"BLSWD"` — do not make it configurable without updating the settings schema.

**`setTransport()` contract:** Returns `false` if the requested transport is unsupported on the current hardware. Caller must handle failure and fall back to USB.

### 5.3 `duckparser` module

**`applyConfiguredTransport()`** — reads `settings::getInputTransport()`, calls `keyboard.setTransport()`, and on failure auto-reverts to USB and persists the change. Called at boot and every 500ms in `loop()`.

**`parse()` contract:** Parses a single chunk of Ducky Script. May be called with multi-line input. Handles `REPEAT`/`REPLAY` by replaying `prevMessage`. `prevMessage` is reset to NULL when `duckscript::stopAll()` is called.

### 5.4 `spiffs` module

**Filesystem:** LittleFS (named `spiffs` for historical reasons — do not rename the namespace).

**File handle rule:** Every `LittleFS.open()` call must be paired with `f.close()` on all exit paths, including error paths.

**Stream mode:** `streamOpen()` → `streamWrite()` / `streamRead()` → `streamClose()`. Only one stream open at a time. The CLI intercepts all input while streaming (except `close` and `read` commands).

### 5.5 `cli` module

**Adding a new command:**
```cpp
cli.addCommand("mycommand", [](cmd* c) {
    print("result");
});
```
For commands with arguments, use `cli.addSingleArgCmd` or create a `Command` object and call `addPosArg`.

**`set` command** — after setting `input_transport`, immediately call `duckparser::applyConfiguredTransport()`.

### 5.6 `webserver` module

**WiFi mode:** Always `WIFI_AP_STA`. AP is always active. STA is optional.

**AP IP:** Fixed at `192.168.4.1`.

**mDNS hostname:** `wifiduck` → `http://wifiduck.local`.

**WebSocket endpoint:** `/ws` — all CLI commands flow through here.

**OTA:** Both Arduino OTA (port 3232) and Web OTA (`POST /update`) are active.

**`wifiInfo()` output format:** `key=value\n` lines. Parsed by `parse_key_value_lines()` in JS.

---

## 6. Ducky Script Reference

### Commands

| Command | Syntax | Notes |
|---|---|---|
| `REM` | `REM comment` | Comment, ignored |
| `DEFAULTDELAY` / `DEFAULT_DELAY` | `DEFAULTDELAY 200` | ms between commands |
| `DELAY` | `DELAY 1000` | One-shot delay in ms |
| `STRING` | `STRING Hello World` | Types each character |
| `REPEAT` / `REPLAY` | `REPEAT 3` | Repeats last non-REPEAT command |
| `LOCALE` | `LOCALE TR` | Sets keyboard layout |
| `KEYCODE` | `KEYCODE 0x02 0x04` | Raw HID report |
| `LED` | `LED 40 20 10` | Sets RGB LED (0–255 each) |

### Supported Locales

`US`, `DE`, `GB`, `ES`, `FR`, `RU`, `TR`, `DK`, `BE`, `PT`, `IT`, `SK`, `CZ`, `SI`, `BG`, `CA-FR`, `CH-DE`, `CH-FR`, `HU`

### Adding a New Locale

1. Copy `src/locale/locale_us.h` → `src/locale/locale_XX.h`.
2. Rename all identifiers to `locale_xx`.
3. Edit ASCII and UTF-8 key maps.
4. Add `#include "locale/locale_xx.h"` to `src/locales.h`.
5. Add `else if (compare1(w->str, w->len, "XX", CASE_INSENSETIVE))` branch to `duckparser.cpp`.
6. Verify with a test script covering all characters.

---

## 7. CLI Command Reference

| Command | Description |
|---|---|
| `help` | List all commands |
| `ram` | Free heap bytes |
| `health` | Uptime, heap, SPIFFS, script state |
| `version` | Firmware version |
| `settings` | All settings (passwords masked) |
| `settings_secrets` | All settings including raw passwords |
| `input` | Transport, BLE support/connection/name |
| `wifi` | AP/STA network state |
| `sta_connect` | Trigger STA connection attempt |
| `sta_disconnect` | Disconnect STA |
| `set <name> <value>` | Change a setting |
| `reset` | Reset all settings to defaults |
| `status` | Script running state |
| `run <file>` | Start a script |
| `stop <file>` | Stop a script |
| `ls <path>` | List SPIFFS directory |
| `mem` | SPIFFS size/usage |
| `cat <file>` | Print file contents |
| `create <file>` | Create empty file |
| `remove <file>` | Delete file |
| `rename <a> <b>` | Rename file |
| `write <file> <content>` | Append to file |
| `format` | Format SPIFFS |
| `stream <file>` | Open write stream |
| `close` | Close stream |
| `read` | Read 1024 bytes from stream |

---

## 8. Web UI Conventions

### WebSocket Message Flow

```
Browser                    Firmware
  |-- "command\n" -------->|
  |<-- "response text" ----|
```

- All commands end with `\n` (added by `ws_send()`).
- Response is a single text frame.
- Queue is strictly FIFO; `cts` flag prevents sending before response received.

### i18n Keys Convention

- `nav.*` — navigation bar
- `status.*` — status bar states
- `index.*` — index page
- `settings.*` — settings page
- `terminal.*` — terminal page
- `error.*` — error pages
- `footer.*` — footer

### Adding a New Settings Field to the UI

1. Add the key/value row to `web/settings.html`.
2. Add `data-i18n` attributes for labels.
3. Add EN and TR translations to `web/i18n.js`.
4. Add load/display logic to `web/settings.js`.
5. Add a button handler if the field is editable.
6. Run `python webconverter.py` to regenerate `src/webfiles.h`.
7. Rebuild firmware.

---

## 9. Build & Deploy Procedures

### Build firmware

```bash
# ESP32-S3
pio run -e esp32-s3-devkitc-1

# ESP32-S2
pio run -e esp32-s2-kaluga-1
```

### Upload firmware

```bash
pio run -e esp32-s3-devkitc-1 -t upload
pio run -e esp32-s2-kaluga-1 -t upload
```

### Run unit tests (no device required)

```bash
pio test --without-uploading --without-testing
# or for native parser tests only:
pio test -e native
```

### Regenerate embedded web assets

```bash
python webconverter.py
```

**Must be run after any change to `web/` files.** The output `src/webfiles.h` must be committed alongside web changes.

### OTA update (device on network)

1. Build `.bin`: `pio run -e esp32-s3-devkitc-1`
2. Open `http://192.168.4.1/settings.html` → Update section → upload `.bin`.

---

## 10. Testing Requirements

### Unit tests (`test/test_parser_locale/`)

- Tests run on native platform (no hardware needed).
- Cover `parse_lines1`, `compare1`, and locale key mappings.
- Run before every PR merge.

### Manual acceptance tests (requires device)

- GUI toggle `Kablo ↔ Bluetooth` on S3 and S2.
- BLE host connected/disconnected report behavior.
- Script no-op behavior when Bluetooth mode active without BLE host.
- REPEAT command across script boundaries (verify stale replay is gone).
- SPIFFS file operations: create, write, read, rename, delete.
- Settings persistence across reboot.
- STA autoconnect behavior.

---

## 11. Known Architectural Risks

| Risk | Location | Severity | Status |
|---|---|---|---|
| FreeRTOS task accesses `File f` and `bool running` without mutex | `src/duckscript.cpp` | High | Open — requires refactor |
| Single WebSocket client model — `currentClient` is a raw pointer | `src/webserver.cpp` | Medium | Acceptable for single-user device |
| `settings_secrets` has no access control | `src/cli.cpp` | Medium | Optional hardening item |

---

## 12. Project-Specific Terminology

| Term | Meaning |
|---|---|
| **Ducky Script** | The scripting language used to define keystroke payloads |
| **BadUSB** | Attack technique where a USB device impersonates a keyboard |
| **HID** | Human Interface Device — USB/BLE keyboard protocol |
| **Transport** | The physical channel for HID output: `usb` or `bluetooth` |
| **BLSWD** | Fixed BLE device name for the Bluetooth keyboard |
| **SPIFFS** | The namespace name for the LittleFS file storage module |
| **AP** | Access Point — the device's own WiFi network (`192.168.4.1`) |
| **STA** | Station — the device connected to an external router |
| **Locale** | A keyboard layout mapping (e.g., `TR`, `DE`, `US`) |
| **webfiles.h** | Auto-generated header containing gzip-compressed web assets |
| **webconverter.py** | Script that generates `webfiles.h` from `web/` directory |
| **PROGMEM** | ESP32 flash storage attribute for read-only data |

---

## 13. AI Behavior Guidelines

### Before making any change

1. Read `README.md` for user-facing context.
2. Read `status.md` for the current development state and known risks.
3. Read `todo.md` for pending tasks.
4. Search for all call sites of any function/variable you plan to remove or rename.
5. Check `platformio.ini` for conditional compilation differences between S2 and S3.

### Code change rules

- **Never** add `ESP_LOGI` / `ESP_LOGE` to production code paths. Use `debugln` / `debugf` macros.
- **Never** include headers inside a namespace body.
- **Never** call `.close()` on a `File` object without first checking it is valid (`if (f) f.close()`).
- **Never** pass a non-null-terminated buffer to `strtok`, `strlen`, `strcmp`, or any C string function.
- **Always** null-terminate char buffers before passing to C string functions.
- **Always** close files opened with `LittleFS.open()` on all exit paths.
- **Always** run `python webconverter.py` after changing any file in `web/`.
- **Always** bump `SETTINGS_MAGIC_NUM` and add a migration function when changing `settings_t`.
- **Always** add both EN and TR translations when adding new UI strings.
- **Always** verify with `search_files` before removing any function or declaration.

### Commit message format

```
<type>: <short description>

<body explaining what and why>
```

Types: `feat`, `fix`, `refactor`, `docs`, `test`, `chore`

### PR requirements

- All unit tests pass (`pio test --without-uploading --without-testing`).
- Both S2 and S3 firmware builds succeed.
- `src/webfiles.h` is regenerated if any `web/` file changed.
- `status.md` and `todo.md` updated to reflect the change.

---

## 14. Maintenance

Update this file whenever:
- A new module is added.
- The settings schema changes (new magic number).
- A new locale is added.
- A new CLI command is added.
- A new web page or major UI section is added.
- A known architectural risk is resolved or a new one is identified.
- Build environments change in `platformio.ini`.
