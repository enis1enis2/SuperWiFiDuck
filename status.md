# Project Status

## Snapshot
- Date: 2026-02-22
- Project: `SuperWiFiDuck`
- Focus of this pass: `Risk Remediation + Network Dashboard`
- Firmware targets verified: `esp32-s2-kaluga-1`, `esp32-s3-devkitc-1`

## Findings and Risks (Pre-Fix)
### High Severity
1. Plaintext credential exposure in default settings output (`src/settings.cpp`, surfaced via `src/cli.cpp`, `web/settings.js`).
2. AP boot log leaked AP password (`src/webserver.cpp`).

### Medium Severity
1. STA password validation accepted `1..7` chars (`src/settings.cpp`, `web/settings.js`).
2. CLI `set` echoed success even when validation failed (`src/cli.cpp`, `src/settings.cpp`).
3. Reset/migration performed multiple EEPROM commits (`src/settings.cpp`).

## Implemented Mitigations
### A1. Secret masking by default
- `settings::toString(bool includeSecrets = false)` added (`src/settings.h`, `src/settings.cpp`).
- Default `settings` output now masks `password` and `sta_password` as `********` (or `-` if empty).
- New CLI command `settings_secrets` returns unmasked values on explicit request (`src/cli.cpp`).
- Settings GUI now has reveal/hide flow:
  - `Reveal passwords` action fetches `settings_secrets` once.
  - Auto-remask after 20 seconds.
  - Reveal state is runtime-only (`web/settings.html`, `web/settings.js`, `web/i18n.js`).

### A2. Log hardening
- AP startup log no longer prints AP password (`src/webserver.cpp`).

### A3. STA password policy hardening
- STA password is now valid only when:
  - empty string (open network), or
  - length `8..64`.
- `1..7` is rejected in firmware and UI (`src/settings.cpp`, `web/settings.js`, `web/i18n.js`).

### A4. Accurate `set` result behavior
- Settings setters and dispatcher now return `bool` success:
  - `set`, `setSSID`, `setPassword`, `setChannel`, `setAutorun`, `setStaSSID`, `setStaPassword`, `setStaAutoconnect`.
- CLI `set` now prints:
  - success only on valid write,
  - `ERROR: invalid value or setting name` on failure.
- CLI output masks secret values in success echo (`src/settings.h`, `src/settings.cpp`, `src/cli.cpp`).

### A5. EEPROM durability improvements
- Added internal setter flow with `persist` control.
- `reset()` and legacy migration now batch in memory and commit once.
- `load()` only calls `save()` when correction is required (`src/settings.cpp`).

## New Features Added
### B1. Expanded WiFi telemetry
`wifi` output now includes:
- `ap_ip`
- `ap_ssid`
- `sta_ssid`
- `sta_status`
- `sta_ip`
- `sta_autoconnect`
- `sta_rssi`
- `sta_uptime_s`
- `sta_last_disconnect_reason`
- `mdns`

Implementation details:
- STA connect uptime tracking
- STA disconnect reason tracking via WiFi events
- RSSI and uptime reporting (`src/webserver.cpp`, `src/webserver.h`, `src/cli.cpp`).

### B2. Index network dashboard (live)
- Added Network Dashboard block to Index Status section (`web/index.html`).
- Added 2-second polling of `wifi` telemetry and safe key=value parsing (`web/index.js`).
- Dashboard shows AP IP, STA status/IP, RSSI, uptime, mDNS, and last disconnect reason.
- Added EN/TR i18n labels and status translations (`web/i18n.js`).

## Files Updated In This Pass
- `src/settings.h`
- `src/settings.cpp`
- `src/cli.cpp`
- `src/webserver.cpp`
- `web/settings.html`
- `web/settings.js`
- `web/index.html`
- `web/index.js`
- `web/i18n.js`
- `src/webfiles.h` (regenerated)

## Verification Matrix
| Command | Result |
|---|---|
| `python webconverter.py` | PASS |
| `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe run -e esp32-s2-kaluga-1` | PASS |
| `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe run -e esp32-s3-devkitc-1` | PASS |
| `C:\Users\lenovo\.platformio\penv\Scripts\pio.exe test --without-uploading --without-testing` | PASS (`esp32-s2-test`, `esp32-s3-test`) |

## On-Device Validation Status
- Not executed in this pass.
- Existing external blocker remains: serial/COM lock can prevent upload/runtime tests (`PermissionError(13) / access denied`).

## Remaining Items
1. Run on-device acceptance checks when COM port is free:
   - `settings` masked by default
   - `settings_secrets` explicit reveal behavior
   - settings page reveal auto-remask after 20s
   - network dashboard live refresh and STA telemetry transitions
2. Optional hardening follow-up: restrict `settings_secrets` behind optional unlock/auth model.
