# TODO

## Security Hardening
- [x] Mask secrets in default settings output (`settings` command + Settings GUI).
- [x] Add explicit secret reveal command (`settings_secrets`).
- [x] Add GUI reveal/hide password control with 20s auto-remask.
- [x] Remove AP password from startup logs.
- [x] Enforce STA password policy: empty or 8-64 characters.
- [x] Make CLI `set` report validation failures correctly.

## Reliability and Durability
- [x] Convert settings setters/dispatcher to `bool` success API.
- [x] Batch EEPROM writes in migration/reset paths.
- [x] Save corrected settings only when needed during load.

## Network Dashboard Feature
- [x] Extend `wifi` telemetry with RSSI, uptime, last disconnect reason.
- [x] Add Index network dashboard UI block.
- [x] Add 2-second polling and runtime rendering in `web/index.js`.
- [x] Add EN/TR translation keys for network dashboard and reveal controls.

## Build and Packaging
- [x] Regenerate embedded web assets (`python webconverter.py`).
- [x] Rebuild firmware for S2 (`esp32-s2-kaluga-1`).
- [x] Rebuild firmware for S3 (`esp32-s3-devkitc-1`).
- [x] Run test firmware build (`pio test --without-uploading --without-testing`).

## External Blockers
- [ ] Run full on-device acceptance tests (blocked when COM port is locked/busy).

## Optional Next Improvements
- [ ] Add optional auth/unlock gate for `settings_secrets` command.
- [ ] Add STA scan/list feature in GUI (SSID discovery) without changing AP-first behavior.