var assert = require('assert');
var weather = require('../../src/pkjs/weather.js');

assert.strictEqual(weather.condFromWmo(0), 0);
assert.strictEqual(weather.condFromWmo(1), 0);
assert.strictEqual(weather.condFromWmo(2), 1);
assert.strictEqual(weather.condFromWmo(3), 1);
assert.strictEqual(weather.condFromWmo(45), 1);
assert.strictEqual(weather.condFromWmo(48), 1);
assert.strictEqual(weather.condFromWmo(50), 1);
assert.strictEqual(weather.condFromWmo(51), 2);
assert.strictEqual(weather.condFromWmo(67), 2);
assert.strictEqual(weather.condFromWmo(68), 1);
assert.strictEqual(weather.condFromWmo(71), 4);
assert.strictEqual(weather.condFromWmo(77), 4);
assert.strictEqual(weather.condFromWmo(78), 1);
assert.strictEqual(weather.condFromWmo(80), 2);
assert.strictEqual(weather.condFromWmo(82), 2);
assert.strictEqual(weather.condFromWmo(83), 1);
assert.strictEqual(weather.condFromWmo(85), 4);
assert.strictEqual(weather.condFromWmo(86), 4);
assert.strictEqual(weather.condFromWmo(95), 3);
assert.strictEqual(weather.condFromWmo(99), 3);
assert.strictEqual(weather.condFromWmo(100), 1);
assert.strictEqual(weather.condFromWmo('x'), 1);
assert.strictEqual(weather.condFromWmo(undefined), 1);

assert.strictEqual(
  weather.buildUrl(51.50734, -0.12776),
  'https://api.open-meteo.com/v1/forecast?latitude=51.51&longitude=-0.13&current=temperature_2m,weather_code'
);

var parsed = weather.parseResponse({ current: { weather_code: 1, temperature_2m: 18.26 } });
assert.strictEqual(parsed.cond, 0);
assert.strictEqual(parsed.tempC10, 183);

parsed = weather.parseResponse({ current: { weather_code: 71, temperature_2m: -3.44 } });
assert.strictEqual(parsed.cond, 4);
assert.strictEqual(parsed.tempC10, -34);

assert.strictEqual(weather.parseResponse(null), null);
assert.strictEqual(weather.parseResponse({}), null);
assert.strictEqual(weather.parseResponse({ current: { weather_code: 1 } }), null);
assert.strictEqual(weather.parseResponse({ current: { weather_code: 1, temperature_2m: NaN } }), null);

console.log('ok: js weather');
