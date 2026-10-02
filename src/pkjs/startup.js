var SETTING_KEYS = [
  "ClockFormat",
  "HourColor",
  "ShowWeather",
  "Fahrenheit",
  "Animate",
  "VibeOnDisconnect",
  "TapSwap",
  "LowBattery"
];

function startupMessage(storedJson) {
  var out = {};
  var parsed = null;

  if (storedJson) {
    try {
      parsed = JSON.parse(storedJson);
    } catch (e) {}
  }

  if (parsed && typeof parsed === "object" && !Array.isArray(parsed)) {
    for (var i = 0; i < SETTING_KEYS.length; i++) {
      var k = SETTING_KEYS[i];
      if (Object.prototype.hasOwnProperty.call(parsed, k)) {
        var v = parsed[k];
        if (typeof v === "boolean") {
          out[k] = v ? 1 : 0;
        } else if (typeof v === "number" && isFinite(v)) {
          out[k] = v;
        } else if (typeof v === "string") {
          out[k] = v;
        }
      }
    }
  }

  out.JsReady = 1;
  return out;
}

module.exports = {
  startupMessage: startupMessage,
  SETTING_KEYS: SETTING_KEYS
};
