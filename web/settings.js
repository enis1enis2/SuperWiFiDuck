/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */
// ===== WebSocket Actions ===== //
function load_settings() {
  ws_send("settings", function(msg) {
    var lines = msg.split(/\n/);

    var ssid = lines[0].split("=")[1];
    var password = lines[1].split("=")[1];
    var channel = lines[2].split("=")[1];
    var autorun = lines[3].split("=")[1];

    E("ssid").innerHTML = ssid;
    E("password").innerHTML = password;
    E("channel").innerHTML = channel;
    E("autorun").innerHTML = autorun;
  });
}

function ws_connected() {
  load_settings();
}


// ===== Startup ===== //
window.addEventListener("load", function() {

  E("edit_ssid").onclick = function() {
    var newssid = prompt(tr("settings.prompt.ssid"), E("ssid").innerHTML);

    if (newssid) {
      if (newssid.length >= 1 && newssid.length <= 32) {
        ws_send("set ssid \"" + newssid + "\"", function(msg) {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidLength"));
      }
    }
  };

  E("edit_password").onclick = function() {
    var newpassword = prompt(tr("settings.prompt.password"), E("password").innerHTML);

    if (newpassword) {
      if (newpassword.length >= 8 && newpassword.length <= 64) {
        ws_send("set password \"" + newpassword + "\"", function(msg) {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidLength"));
      }
    }
  };

  E("edit_channel").onclick = function() {
    var newchannel = prompt(tr("settings.prompt.channel"), E("channel").innerHTML);

    if (newchannel) {
      if (parseInt(newchannel) >= 1 && parseInt(newchannel) <= 13) {
        ws_send("set channel " + newchannel, function(msg) {
          load_settings();
        });
      } else {
        alert(tr("settings.error.invalidChannel"));
      }
    }
  };

  E("disable_autorun").onclick = function() {
    ws_send("set autorun \"\"", function(msg) {
      load_settings();
    });
  };

  E("reset").onclick = function() {
    if (confirm(tr("settings.confirm.reset"))) {
      ws_send("reset", function(msg) {
        load_settings();
      });
    }
  };

  ws_init();
}, false);
