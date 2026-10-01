function condFromWmo(code) {
  if (typeof code !== 'number' || isNaN(code)) return 1;
  if (code === 0 || code === 1) return 0;
  if (code === 2 || code === 3 || code === 45 || code === 48) return 1;
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return 2;
  if ((code >= 71 && code <= 77) || code === 85 || code === 86) return 4;
  if (code >= 95 && code <= 99) return 3;
  return 1;
}

function buildUrl(lat, lon) {
  return 'https://api.open-meteo.com/v1/forecast?latitude=' +
         Number(lat).toFixed(2) +
         '&longitude=' +
         Number(lon).toFixed(2) +
         '&current=temperature_2m,weather_code';
}

function parseResponse(json) {
  if (!json || typeof json !== 'object') return null;
  var current = json.current;
  if (!current || typeof current !== 'object') return null;
  var code = current.weather_code;
  var temp = current.temperature_2m;
  if (typeof code !== 'number' || !isFinite(code)) return null;
  if (typeof temp !== 'number' || !isFinite(temp)) return null;
  return { cond: condFromWmo(code), tempC10: Math.round(temp * 10) };
}

module.exports = {
  condFromWmo: condFromWmo,
  buildUrl: buildUrl,
  parseResponse: parseResponse
};
