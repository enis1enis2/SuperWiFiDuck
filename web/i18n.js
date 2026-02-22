/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
*/

(function() {
  var DEFAULT_LANG = "en";
  var STORAGE_KEY = "wifiduck_lang";

  var translations = {
    en: {
      "nav.home": "WiFi Duck",
      "nav.settings": "Settings",
      "nav.terminal": "Terminal",
      "nav.about": "About",
      "lang.label": "Language",

      "status.connected": "connected",
      "status.disconnected": "disconnected",
      "status.connecting": "connecting...",
      "status.error": "error",
      "status.ready": "ready",
      "status.reading": "reading...",
      "status.saving": "saving...",
      "status.running": "running {script}",

      "index.section.status": "Status",
      "index.section.scripts": "Scripts",
      "index.section.editor": "Editor",
      "index.section.docs": "Scripting",
      "index.network.title": "Network Dashboard",
      "index.network.apIp": "AP IP",
      "index.network.staStatus": "STA Status",
      "index.network.staIp": "STA IP",
      "index.network.staRssi": "STA RSSI",
      "index.network.staUptime": "STA Uptime",
      "index.network.mdns": "mDNS",
      "index.network.lastReason": "Last Disconnect Reason",
      "index.network.na": "-",
      "index.network.status.connected": "connected",
      "index.network.status.connecting": "connecting",
      "index.network.status.disconnected": "disconnected",
      "index.network.status.disabled": "disabled",
      "index.storage": "Storage:",
      "index.format": "format",
      "index.stop": "stop",
      "index.reconnect": "reconnect",
      "index.create": "create",
      "index.enableAutorun": "Enable autorun",
      "index.delete": "delete",
      "index.download": "download",
      "index.save": "save",
      "index.run": "run",
      "index.output": "Output:",
      "index.saved": "saved",
      "index.unsaved": "unsaved changes",
      "index.filenamePlaceholder": "Filename /<name>",
      "index.fileFilterPlaceholder": "Filter files...",
      "index.table.file": "File",
      "index.table.size": "Byte",
      "index.table.actions": "Actions",
      "index.table.edit": "edit",
      "index.table.run": "run",
      "index.confirm.format": "Format SPIFFS? This will delete all scripts!",
      "index.alert.format": "Formatting will take a minute.\nYou have to reconnect afterwards.",
      "index.confirm.delete": "Delete {file}?",
      "index.confirm.autorun": "Run this script automatically on startup?\nYou can disable it in the settings.",

      "settings.section.wifi": "WiFi",
      "settings.section.sta": "Station (Client WiFi)",
      "settings.section.input": "Input Mode",
      "settings.section.update": "Update",
      "settings.restartNotice": "Restart the device to apply new settings.",
      "settings.ssid": "SSID:",
      "settings.password": "Password:",
      "settings.channel": "Channel:",
      "settings.autorun": "Autorun Script:",
      "settings.staSsid": "STA SSID:",
      "settings.staPassword": "STA Password:",
      "settings.staAutoconnect": "STA Autoconnect:",
      "settings.staStatus": "STA Status:",
      "settings.staIp": "STA IP:",
      "settings.staMdns": "mDNS:",
      "settings.staActions": "Actions:",
      "settings.inputMode": "Input Mode:",
      "settings.inputModeToggle": "switch",
      "settings.inputModeCable": "Cable",
      "settings.inputModeBluetooth": "Bluetooth",
      "settings.btSupported": "Bluetooth Support:",
      "settings.btConnected": "Bluetooth Connection:",
      "settings.btName": "Bluetooth Device Name:",
      "settings.btUnsupportedS2": "Bluetooth input mode is supported only on ESP32-S3.",
      "settings.edit": "edit",
      "settings.enable": "enable",
      "settings.disable": "disable",
      "settings.connect": "connect",
      "settings.disconnect": "disconnect",
      "settings.autoconnectOn": "on",
      "settings.autoconnectOff": "off",
      "settings.staStatus.connected": "connected",
      "settings.staStatus.connecting": "connecting",
      "settings.staStatus.disconnected": "disconnected",
      "settings.staStatus.disabled": "disabled",
      "settings.revealPasswords": "Reveal passwords",
      "settings.hidePasswords": "Hide passwords",
      "settings.reset": "Reset settings",
      "settings.updateHelp": "Go to <a href=\"https://github.com/spacehuhntech/WiFiDuck/releases\" target=\"_blank\">releases</a> to check for updates.<br>Select a .bin file and press upload to update the device.<br>",
      "settings.upload": "Upload",
      "settings.prompt.ssid": "SSID (1-32 chars)",
      "settings.prompt.password": "Password (8-64 chars)",
      "settings.prompt.channel": "Channel (1-13)",
      "settings.prompt.staSsid": "STA SSID (0-32 chars, empty to disable)",
      "settings.prompt.staPassword": "STA Password (empty or 8-64 chars)",
      "settings.error.invalidLength": "ERROR: Invalid length",
      "settings.error.invalidStaPassword": "ERROR: STA password must be empty or 8-64 characters",
      "settings.error.btNotAvailable": "ERROR: Bluetooth transport is not available on this device",
      "settings.error.invalidChannel": "ERROR: Invalid channel number",
      "settings.confirm.reset": "Reset all settings to default?",
      "settings.state.connected": "connected",
      "settings.state.disconnected": "disconnected",
      "settings.state.supported": "supported",
      "settings.state.unsupported": "unsupported",

      "terminal.section.title": "Terminal",
      "terminal.send": "send",
      "terminal.clear": "clear",
      "terminal.reconnect": "reconnect",
      "terminal.inputPrefix": "# ",

      "error.404.title": "404",
      "error.404.message": "Page not found :(",
      "error.404.back": "Back to Homepage",

      "footer.source": "Source",
      "footer.copyright": "Copyright (c) Enis Polat"
    },
    tr: {
      "nav.home": "WiFi Duck",
      "nav.settings": "Ayarlar",
      "nav.terminal": "Terminal",
      "nav.about": "Hakkinda",
      "lang.label": "Dil",

      "status.connected": "baglandi",
      "status.disconnected": "baglanti kesildi",
      "status.connecting": "baglaniyor...",
      "status.error": "hata",
      "status.ready": "hazir",
      "status.reading": "okunuyor...",
      "status.saving": "kaydediliyor...",
      "status.running": "calisiyor {script}",

      "index.section.status": "Durum",
      "index.section.scripts": "Scriptler",
      "index.section.editor": "Duzenleyici",
      "index.section.docs": "Betikleme",
      "index.network.title": "Ag Durum Paneli",
      "index.network.apIp": "AP IP",
      "index.network.staStatus": "STA Durumu",
      "index.network.staIp": "STA IP",
      "index.network.staRssi": "STA RSSI",
      "index.network.staUptime": "STA Calisma Suresi",
      "index.network.mdns": "mDNS",
      "index.network.lastReason": "Son Ayrilma Nedeni",
      "index.network.na": "-",
      "index.network.status.connected": "baglandi",
      "index.network.status.connecting": "baglaniyor",
      "index.network.status.disconnected": "bagli degil",
      "index.network.status.disabled": "devre disi",
      "index.storage": "Depolama:",
      "index.format": "formatla",
      "index.stop": "durdur",
      "index.reconnect": "yeniden baglan",
      "index.create": "olustur",
      "index.enableAutorun": "Otomatik calistirmayi ac",
      "index.delete": "sil",
      "index.download": "indir",
      "index.save": "kaydet",
      "index.run": "calistir",
      "index.output": "Cikti:",
      "index.saved": "kaydedildi",
      "index.unsaved": "kaydedilmemis degisiklikler",
      "index.filenamePlaceholder": "Dosya /<isim>",
      "index.fileFilterPlaceholder": "Dosyalari filtrele...",
      "index.table.file": "Dosya",
      "index.table.size": "Bayt",
      "index.table.actions": "Islemler",
      "index.table.edit": "duzenle",
      "index.table.run": "calistir",
      "index.confirm.format": "SPIFFS formatlansin mi? Tum scriptler silinecek!",
      "index.alert.format": "Formatlama bir dakika surebilir.\nSonrasinda tekrar baglanmaniz gerekir.",
      "index.confirm.delete": "{file} silinsin mi?",
      "index.confirm.autorun": "Bu script acilista otomatik calissin mi?\nAyarlardan kapatabilirsiniz.",

      "settings.section.wifi": "WiFi",
      "settings.section.sta": "Istasyon (Musteri WiFi)",
      "settings.section.input": "Giris Modu",
      "settings.section.update": "Guncelleme",
      "settings.restartNotice": "Yeni ayarlarin uygulanmasi icin cihazı yeniden baslatin.",
      "settings.ssid": "SSID:",
      "settings.password": "Sifre:",
      "settings.channel": "Kanal:",
      "settings.autorun": "Otomatik Calisan Script:",
      "settings.staSsid": "STA SSID:",
      "settings.staPassword": "STA Sifresi:",
      "settings.staAutoconnect": "STA Otomatik Baglanti:",
      "settings.staStatus": "STA Durumu:",
      "settings.staIp": "STA IP:",
      "settings.staMdns": "mDNS:",
      "settings.staActions": "Islemler:",
      "settings.inputMode": "Giris Modu:",
      "settings.inputModeToggle": "degistir",
      "settings.inputModeCable": "Kablo",
      "settings.inputModeBluetooth": "Bluetooth",
      "settings.btSupported": "Bluetooth Destegi:",
      "settings.btConnected": "Bluetooth Baglantisi:",
      "settings.btName": "Bluetooth Cihaz Adi:",
      "settings.btUnsupportedS2": "Bluetooth giris modu yalnizca ESP32-S3 icin desteklenir.",
      "settings.edit": "duzenle",
      "settings.enable": "ac",
      "settings.disable": "kapat",
      "settings.connect": "baglan",
      "settings.disconnect": "ayril",
      "settings.autoconnectOn": "acik",
      "settings.autoconnectOff": "kapali",
      "settings.staStatus.connected": "baglandi",
      "settings.staStatus.connecting": "baglaniyor",
      "settings.staStatus.disconnected": "bagli degil",
      "settings.staStatus.disabled": "devre disi",
      "settings.revealPasswords": "Sifreleri goster",
      "settings.hidePasswords": "Sifreleri gizle",
      "settings.reset": "Ayarlari sifirla",
      "settings.updateHelp": "Guncellemeleri kontrol etmek icin <a href=\"https://github.com/spacehuhntech/WiFiDuck/releases\" target=\"_blank\">releases</a> sayfasina gidin.<br>.bin dosyasini secip yukleye basin.<br>",
      "settings.upload": "Yukle",
      "settings.prompt.ssid": "SSID (1-32 karakter)",
      "settings.prompt.password": "Sifre (8-64 karakter)",
      "settings.prompt.channel": "Kanal (1-13)",
      "settings.prompt.staSsid": "STA SSID (0-32 karakter, kapatmak icin bos birak)",
      "settings.prompt.staPassword": "STA Sifresi (bos veya 8-64 karakter)",
      "settings.error.invalidLength": "HATA: Gecersiz uzunluk",
      "settings.error.invalidStaPassword": "HATA: STA sifresi bos veya 8-64 karakter olmali",
      "settings.error.btNotAvailable": "HATA: Bu cihazda Bluetooth tasima destegi yok",
      "settings.error.invalidChannel": "HATA: Gecersiz kanal numarasi",
      "settings.confirm.reset": "Tum ayarlar varsayilana sifirlansin mi?",
      "settings.state.connected": "baglandi",
      "settings.state.disconnected": "bagli degil",
      "settings.state.supported": "destekleniyor",
      "settings.state.unsupported": "desteklenmiyor",

      "terminal.section.title": "Terminal",
      "terminal.send": "gonder",
      "terminal.clear": "temizle",
      "terminal.reconnect": "yeniden baglan",
      "terminal.inputPrefix": "# ",

      "error.404.title": "404",
      "error.404.message": "Sayfa bulunamadi :(",
      "error.404.back": "Ana sayfaya don",

      "footer.source": "Kaynak",
      "footer.copyright": "Telif Hakki (c) Enis Polat"
    }
  };

  function normalizeLang(lang) {
    if (!lang) return DEFAULT_LANG;
    lang = String(lang).toLowerCase();
    if (translations[lang]) return lang;
    if (lang.indexOf("tr") === 0) return "tr";
    return DEFAULT_LANG;
  }

  function getLanguage() {
    var stored = localStorage.getItem(STORAGE_KEY);
    if (stored) return normalizeLang(stored);
    return normalizeLang(navigator.language || navigator.userLanguage || DEFAULT_LANG);
  }

  function setLanguage(lang) {
    var normalized = normalizeLang(lang);
    localStorage.setItem(STORAGE_KEY, normalized);
    window.__lang = normalized;
    applyTranslations(document);
  }

  function formatTemplate(template, vars) {
    if (!vars) return template;
    return template.replace(/\{([a-zA-Z0-9_]+)\}/g, function(_, key) {
      if (Object.prototype.hasOwnProperty.call(vars, key)) {
        return String(vars[key]);
      }
      return "";
    });
  }

  function tr(key, vars) {
    var lang = window.__lang || getLanguage();
    var langMap = translations[lang] || translations[DEFAULT_LANG];
    var baseMap = translations[DEFAULT_LANG];
    var value = langMap[key];
    if (typeof value === "undefined") value = baseMap[key];
    if (typeof value === "undefined") return key;
    return formatTemplate(value, vars);
  }

  function applyTranslations(root) {
    var node = root || document;
    var textNodes = node.querySelectorAll("[data-i18n]");
    for (var i = 0; i < textNodes.length; i++) {
      var key = textNodes[i].getAttribute("data-i18n");
      textNodes[i].textContent = tr(key);
    }

    var htmlNodes = node.querySelectorAll("[data-i18n-html]");
    for (var j = 0; j < htmlNodes.length; j++) {
      var htmlKey = htmlNodes[j].getAttribute("data-i18n-html");
      htmlNodes[j].innerHTML = tr(htmlKey);
    }

    var placeholderNodes = node.querySelectorAll("[data-i18n-placeholder]");
    for (var k = 0; k < placeholderNodes.length; k++) {
      var placeholderKey = placeholderNodes[k].getAttribute("data-i18n-placeholder");
      placeholderNodes[k].setAttribute("placeholder", tr(placeholderKey));
    }

    var valueNodes = node.querySelectorAll("[data-i18n-value]");
    for (var l = 0; l < valueNodes.length; l++) {
      var valueKey = valueNodes[l].getAttribute("data-i18n-value");
      valueNodes[l].setAttribute("value", tr(valueKey));
    }

    var selector = document.getElementById("languageSelect");
    if (selector && selector.value !== (window.__lang || getLanguage())) {
      selector.value = window.__lang || getLanguage();
    }
  }

  function initLanguageSelector() {
    var selector = document.getElementById("languageSelect");
    if (!selector) return;
    selector.value = window.__lang || getLanguage();
    selector.onchange = function() {
      setLanguage(selector.value);
    };
    selector.setAttribute("aria-label", tr("lang.label"));
  }

  window.tr = tr;
  window.applyTranslations = applyTranslations;
  window.getLanguage = getLanguage;
  window.setLanguage = setLanguage;

  window.__lang = getLanguage();

  document.addEventListener("DOMContentLoaded", function() {
    initLanguageSelector();
    applyTranslations(document);
  });
})();
