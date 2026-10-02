var assert = require('assert');
var startup = require('../../src/pkjs/startup.js');

assert.deepStrictEqual(startup.startupMessage(null), { JsReady: 1 });
assert.deepStrictEqual(startup.startupMessage(''), { JsReady: 1 });
assert.deepStrictEqual(startup.startupMessage('not json'), { JsReady: 1 });
assert.deepStrictEqual(startup.startupMessage('[]'), { JsReady: 1 });
assert.deepStrictEqual(startup.startupMessage('null'), { JsReady: 1 });

var stored = '{"ClockFormat":"24","HourColor":2,"ShowWeather":true,"Fahrenheit":false,"Animate":true,"VibeOnDisconnect":0,"TapSwap":"1","LowBattery":20}';
assert.deepStrictEqual(startup.startupMessage(stored), {
  ClockFormat: "24",
  HourColor: 2,
  ShowWeather: 1,
  Fahrenheit: 0,
  Animate: 1,
  VibeOnDisconnect: 0,
  TapSwap: "1",
  LowBattery: 20,
  JsReady: 1
});

var unknown = JSON.stringify({
  ShowWeather: true,
  Bogus: 'ignored',
  WeatherCond: 5,
  Float: 3.14,
  Falsey: false
});

assert.deepStrictEqual(startup.startupMessage(unknown), {
  ShowWeather: 1,
  JsReady: 1
});

var badValues = '{"HourColor":{},"Animate":[],"TapSwap":null,"LowBattery":"30","Fahrenheit":true}';
assert.deepStrictEqual(startup.startupMessage(badValues), {
  LowBattery: "30",
  Fahrenheit: 1,
  JsReady: 1
});

assert.ok(Array.isArray(startup.SETTING_KEYS));
assert.strictEqual(startup.SETTING_KEYS.length, 8);

console.log('ok: js startup');
