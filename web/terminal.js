/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */
// ===== WebSocket Actions ===== //
function ws_connected() {}

var terminal_history = [];
var terminal_history_index = -1;

function load_terminal_history() {
  try {
    var raw = localStorage.getItem("wifiduck_terminal_history");
    if (!raw) return;
    var parsed = JSON.parse(raw);
    if (Array.isArray(parsed)) {
      terminal_history = parsed.slice(-100);
    }
  } catch (e) {
    terminal_history = [];
  }
}

function save_terminal_history() {
  localStorage.setItem("wifiduck_terminal_history", JSON.stringify(terminal_history.slice(-100)));
}

function push_terminal_history(cmd) {
  if (!cmd || cmd.trim().length === 0) return;
  if (terminal_history.length === 0 || terminal_history[terminal_history.length - 1] !== cmd) {
    terminal_history.push(cmd);
    save_terminal_history();
  }
  terminal_history_index = terminal_history.length;
}

// ===== Startup ===== //
window.addEventListener("load", function() {
  load_terminal_history();

  function send_terminal_command() {
    var input = E("input").value;

    E("output").innerHTML += tr("terminal.inputPrefix") + input + "<br>";

    E("reconnect").onclick = ws_init;

    ws_send(input, function(msg) {
      log(msg);
      E("output").innerHTML += msg.replace(/\n/g, "<br>");
      E("output").scrollTop = E("output").scrollHeight;
    });

    push_terminal_history(input);
  }

  E("send").onclick = function() {
    send_terminal_command();
  };

  E("clear").onclick = function() {
    E("output").innerHTML = "";
  };

  E("input").addEventListener("keydown", function(event) {
    if (event.key === "Enter") {
      event.preventDefault();
      send_terminal_command();
      return;
    }

    if (event.key === "ArrowUp") {
      event.preventDefault();
      if (terminal_history.length === 0) return;
      terminal_history_index = Math.max(0, terminal_history_index - 1);
      E("input").value = terminal_history[terminal_history_index];
      return;
    }

    if (event.key === "ArrowDown") {
      event.preventDefault();
      if (terminal_history.length === 0) return;
      terminal_history_index = Math.min(terminal_history.length, terminal_history_index + 1);
      if (terminal_history_index >= terminal_history.length) {
        E("input").value = "";
      } else {
        E("input").value = terminal_history[terminal_history_index];
      }
    }
  });

  ws_init();
}, false);
