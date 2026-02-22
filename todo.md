# TODO

## Completed In This Pass
- [x] Add Turkish GUI support for Web UI pages with runtime language switcher (`EN`/`TR`) via new `web/i18n.js`.
- [x] Localize key UI labels, status text, prompts and actions in:
  - [x] `web/index.html`, `web/index.js`
  - [x] `web/settings.html`, `web/settings.js`
  - [x] `web/terminal.html`, `web/terminal.js`
  - [x] `web/error404.html`, `web/credits.html`
- [x] Change Web UI footer copyright to `Enis Polat`.
- [x] Add dynamic WebSocket endpoint resolution in `web/script.js` (uses current host, fallback to `192.168.4.1`).
- [x] Add script list filter to the Web UI (`web/index.html`, `web/index.js`).
- [x] Add terminal command history + Enter/Arrow key UX (`web/terminal.js`).
- [x] Add new CLI `health` command in `src/cli.cpp` (uptime, heap, SPIFFS usage, script state).
- [x] Fix `webconverter.py` output path bug (`src/webfiles.h`) and regenerate embedded assets.
- [x] Rebuild firmware after updates:
  - [x] `pio run -e esp32-s2-kaluga-1` success
  - [x] `pio run -e esp32-s3-devkitc-1` success
- [x] Re-run test firmware build:
  - [x] `pio test --without-uploading --without-testing` success
- [x] Final verification refresh:
  - [x] `pio run -e esp32-s2-kaluga-1` success
  - [x] `pio run -e esp32-s3-devkitc-1` success
  - [x] `pio test --without-uploading --without-testing` success

## Remaining External Blocker
- [ ] On-device test upload still blocked by serial port lock on `COM5` (`PermissionError(13)`).
