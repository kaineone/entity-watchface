var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);
var weather = require('./weather');

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

function fetchWeather() {
  if (!weatherEnabled()) return;

  function onPos(pos) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', weather.buildUrl(pos.coords.latitude, pos.coords.longitude), true);
    xhr.onload = function() {
      if (xhr.status === 200) {
        try {
          var obj = JSON.parse(xhr.responseText);
          var r = weather.parseResponse(obj);
          if (r) {
            Pebble.sendAppMessage({ WeatherCond: r.cond, WeatherTempC10: r.tempC10 });
          }
        } catch (err) {}
      }
    };
    xhr.onerror = function() {
      console.log('weather fetch failed');
    };
    xhr.send();
  }

  function onErr(err) {
    console.log('weather location failed');
  }

  navigator.geolocation.getCurrentPosition(onPos, onErr, {
    timeout: 15000,
    maximumAge: 1800000,
    enableHighAccuracy: false
  });
}

Pebble.addEventListener('ready', function(e) {
  fetchWeather();
  setInterval(fetchWeather, 30 * 60 * 1000);
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && e.payload.WeatherRequest) {
    fetchWeather();
  }
});
