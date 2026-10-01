function condFromWmo(code) {
  if (typeof code !== 'number' || code % 1 !== 0) return 1;
  if (code === 0 || code === 1) return 0;
  if (code === 2 || code === 3 || code === 45 || code === 48) return 1;
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return 2;
  if ((code >= 71 && code <= 77) || code === 85 || code === 86) return 4;
  if (code >= 95 && code <= 99) return 3;
  return 1;
}

function buildUrl(lat, lon) {
  if (typeof lat !== 'number' || typeof lon !== 'number') return null;
  if (!isFinite(lat) || !isFinite(lon)) return null;
  if (Math.abs(lat) > 90 || Math.abs(lon) > 180) return null;
  var sLat = lat.toFixed(1);
  var sLon = lon.toFixed(1);
  if (sLat === '-0.0') sLat = '0.0';
  if (sLon === '-0.0') sLon = '0.0';
  return 'https://api.open-meteo.com/v1/forecast?latitude=' + sLat +
         '&longitude=' + sLon + '&current=temperature_2m,weather_code';
}

function parseResponse(json) {
  if (!json || typeof json !== 'object') return null;
  var current = json.current;
  if (!current || typeof current !== 'object') return null;
  var code = current.weather_code;
  var temp = current.temperature_2m;
  if (typeof code !== 'number' || !isFinite(code)) return null;
  if (typeof temp !== 'number' || !isFinite(temp)) return null;
  if (Math.abs(temp) > 150) return null;
  var tempC10 = Math.round(temp * 10);
  if (tempC10 === 0) tempC10 = 0;
  return { cond: condFromWmo(code), tempC10: tempC10 };
}

module.exports = {
  condFromWmo: condFromWmo,
  buildUrl: buildUrl,
  parseResponse: parseResponse
};
