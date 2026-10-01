# Entity

A watchface for the Pebble Time 2 and the Pebble Round 2.

The hour sits above the minute in Zen Dots. Under them, 21 bars track the watch's Bluetooth
link to your phone. While connected, a red cursor sweeps the bars four times a second and
leaves a short orange trail. When the link drops, the bars go flat and grey. The top row shows
the date and weather. Tap the watch and it switches to steps and heart rate for ten seconds.

On the Round 2 the bars become 60 ticks around the rim, and the cursor moves once per second.

## Battery

The animation stops when the battery falls to 20% (you can change the threshold), during
quiet time, while a timeline peek covers the screen, or if you turn it off in settings. Time
updates once a minute. Weather is fetched by the phone every 30 minutes.

## Building

Install the Pebble tool and SDK 4.33.1:

```sh
uv tool install pebble-tool
pebble sdk install 4.33.1
```

Then build and run it in the emulator:

```sh
pebble build
pebble install --emulator emery --vnc
```

`make -C tests/host` runs the unit tests for the drawing and formatting logic on your machine.

## Fonts

Zen Dots by The Dots Project Authors and JetBrains Mono by JetBrains, both under the SIL Open
Font License 1.1. The license texts are in `resources/fonts/`.
