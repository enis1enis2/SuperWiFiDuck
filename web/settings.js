/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

var current_settings_map = {};
var current_wifi_map = {};

function parse_key_value_lines(msg) {
  var map = {};
  if (!msg) return map;

  var lines = msg.split(/\n/);
  for (var i = 0; i < lines.length; i++) {
    var line = lines[i];
    if (!line) continue;

    var sep = line.indexOf("=");
    if (sep <= 0) continue;

    var key = line.substring(0, sep).trim();
    var value = line.substring(sep + 1).trim();
    map[key] = value;
  }

  return map;
}

function cli_escape(value) {
  return String(value).replace(/\\/g, "\\\\").replace(/"/g, "\\\"");
}

function set_text(id, value) {
  var el = E(id);
  if (!el) return;
  el.innerHTML = (value === undefined || value === null || value === "") ? "-" : value;
}

function bind_click(id, handler) {
  var el = E(id);
  if (!el) return;
  el.onclick = handler;
}

function sta_autoconnect_is_enabled() {
  return current_settings_map.sta_autoconnect === "1";
}

function refresh_sta_autoconnect_ui() {
  var enabled = sta_autoconnect_is_enabled();
  set_text("sta_autoconnect", enabled ? tr("settings.autoconnectOn") : tr("settings.autoconnectOff"));

  if (E("toggle_sta_autoconnect")) {
    E("toggle_sta_autoconnect").innerHTML = enabled ? tr("settings.disable") : tr("settings.enable");
  }
}

function translate_sta_status(status) {
  if (!status) return tr("settings.staStatus.disconnected");
  var key = "settings.staStatus." + status;
  var translated = tr(key);
  return translated === key ? status : translated;
}

function load_wifi_info() {
  ws_send("wifi", function(msg) {
    current_wifi_map = parse_key_value_lines(msg);
    set_text("sta_status", translate_sta_status(current_wifi_map.sta_status || "disconnected"));
    set_text("sta_ip", current_wifi_map.sta_ip || "0.0.0.0");
    set_text("sta_mdns", current_wifi_map.mdns || ("http://" + window.location.hostname + ".local"));
  });
}

// ===== WebSocket Actions ===== //
function load_settings() {
  ws_send("settings", function(msg) {
    current_settings_map = parse_key_value_lines(msg);

    set_text("ssid", current_settings_map.ssid || "");
    set_text("password", current_settings_map.password || "");
    set_text("channel", current_settings_map.channel || "");
    set_text("autorun", current_settings_map.autorun || "");

    set_text("sta_ssid", current_settings_map.sta_ssid || "");
    set_text("sta_password", current_settings_map.sta_password || "");
    refresh_sta_autoconnect_ui();

    load_wifi_info();
  });
}

function ws_connected() {
  load_settings();
}

// ===== Startup ===== //
window.addEventListener("load", function() {
  bind_click("edit_ssid", function() {
    var newssid = prompt(tr("settings.prompt.ssid"), current_settings_map.ssid || "");

    if (newssid !== null) {
      if (newssid.length >= 1 && newssid.length <= 32) {
        ws_send("set ssid \"" + cli_escape(newssid) + "\"", function() {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidLength"));
      }
    }
  });

  bind_click("edit_password", function() {
    var newpassword = prompt(tr("settings.prompt.password"), current_settings_map.password || "");

    if (newpassword !== null) {
      if (newpassword.length >= 8 && newpassword.length <= 64) {
        ws_send("set password \"" + cli_escape(newpassword) + "\"", function() {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidLength"));
      }
    }
  });

  bind_click("edit_channel", function() {
    var newchannel = prompt(tr("settings.prompt.channel"), current_settings_map.channel || "");

    if (newchannel !== null) {
      if (parseInt(newchannel, 10) >= 1 && parseInt(newchannel, 10) <= 13) {
        ws_send("set channel " + newchannel, function() {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidChannel"));
      }
    }
  });

  bind_click("disable_autorun", function() {
    ws_send("set autorun \"\"", function() {
      load_settings();
    });
  });

  bind_click("edit_sta_ssid", function() {
    var newStaSsid = prompt(tr("settings.prompt.staSsid"), current_settings_map.sta_ssid || "");

    if (newStaSsid !== null) {
      if (newStaSsid.length <= 32) {
        ws_send("set sta_ssid \"" + cli_escape(newStaSsid) + "\"", function() {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidLength"));
      }
    }
  });

  bind_click("edit_sta_password", function() {
    var newStaPassword = prompt(tr("settings.prompt.staPassword"), current_settings_map.sta_password || "");

    if (newStaPassword !== null) {
      if (newStaPassword.length <= 64) {
        ws_send("set sta_password \"" + cli_escape(newStaPassword) + "\"", function() {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidLength"));
      }
    }
  });

  bind_click("toggle_sta_autoconnect", function() {
    var nextValue = sta_autoconnect_is_enabled() ? "0" : "1";
    ws_send("set sta_autoconnect " + nextValue, function() {
      load_settings();
    });
  });

  bind_click("sta_connect", function() {
    ws_send("sta_connect", function() {
      load_settings();
    });
  });

  bind_click("sta_disconnect", function() {
    ws_send("sta_disconnect", function() {
      load_settings();
    });
  });

  bind_click("reset", function() {
    if (confirm(tr("settings.confirm.reset"))) {
      ws_send("reset", function() {
        load_settings();
      });
    }
  });

  ws_init();
}, false);
