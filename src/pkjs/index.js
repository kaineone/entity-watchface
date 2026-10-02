var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);
var weather = require('./weather');
var startup = require('./startup');

function weatherEnabled() {
  if (typeof localStorage === 'undefined') return true;
  var raw = localStorage.getItem('clay-settings');
  if (!raw) return true;
  try {
    var cfg = JSON.parse(raw);
    if (cfg && (cfg.ShowWeather === false || cfg.ShowWeather === 0)) return false;
  } catch (e) {}
  return true;
}

var inflight = false;
var retryTimer = null;

function fetchWeather(isRetry) {
  if (!weatherEnabled()) return;
  if (inflight) return;
  inflight = true;

  function done(ok) {
    inflight = false;
    // One retry per failed scheduled fetch; a failed retry waits for the next interval.
    if (!ok && !isRetry && retryTimer === null) {
      retryTimer = setTimeout(function () {
        retryTimer = null;
        fetchWeather(true);
      }, 3 * 60 * 1000);
    }
  }

  function onPos(pos) {
    var url = weather.buildUrl(pos.coords.latitude, pos.coords.longitude);
    if (!url) {
      done(false);
      return;
    }
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.timeout = 10000;
    xhr.onload = function () {
      if (xhr.status === 200) {
        try {
          var obj = JSON.parse(xhr.responseText);
          var r = weather.parseResponse(obj);
          if (r) {
            Pebble.sendAppMessage(
              { WeatherCond: r.cond, WeatherTempC10: r.tempC10 },
              function () {
                done(true);
              },
              function () {
                done(false);
              }
            );
            return;
          }
        } catch (err) {}
      }
      done(false);
    };
    xhr.onerror = function () {
      done(false);
    };
    xhr.ontimeout = function () {
      done(false);
    };
    xhr.send();
  }

  function onErr(err) {
    done(false);
  }

  navigator.geolocation.getCurrentPosition(onPos, onErr, {
    timeout: 15000,
    maximumAge: 1800000,
    enableHighAccuracy: false
  });
}

function sendStartup(isRetry) {
  var raw = null;
  try {
    if (typeof localStorage !== 'undefined') {
      raw = localStorage.getItem('clay-settings');
    }
  } catch (e) {}
  Pebble.sendAppMessage(
    startup.startupMessage(raw),
    function () {},
    function () {
      if (!isRetry) {
        setTimeout(function () {
          sendStartup(true);
        }, 3000);
      }
    }
  );
}

Pebble.addEventListener('ready', function(e) {
  sendStartup(false);
  setInterval(function () {
    fetchWeather(false);
  }, 30 * 60 * 1000);
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && e.payload.WeatherRequest) {
    fetchWeather(false);
  }
});
