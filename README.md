# Entity

Entity is a watchface for the Pebble Time 2 and Pebble Round 2. It also runs on the original
Pebble Time, Time Steel and Time Round, and on the Pebble 2 Duo.

It isn't in the Pebble appstore yet. Once it's finished and we've tried it on real watches, you'll
be able to install it straight from the Pebble app on your phone, and a link will go here.

Most of the screen is the time, in big bold numerals with the hour stacked over the minute. Along
the bottom is a row of bars that shows whether your watch can reach your phone. Flick your wrist
and a red bar glides side to side across them, trailing a little orange, then settles after half
a minute. If the connection drops, the bars flatten out and turn grey, so you can tell at a glance.

The top of the screen has the date and the temperature. Double tap the watch and it switches over
to your steps and heart rate for a few seconds, then flips back.

On the round watches the bars wrap around the edge of the screen as tick marks, and the red one
travels around the rim.

## Battery

The moving bar is the one thing here that really costs power, so it only moves when you're likely
to be looking: briefly at each new minute, and for a while after you flick your wrist. The rest of
the time the bars sit still. It won't move at all when your battery is low (20% unless you pick a
different number in the settings), during quiet time, while a timeline peek covers the screen, or
if you switch the animation off.

## Type

The numerals are drawn as blocky shapes, so the face carries no font files. Labels use
Pebble's own Gothic, which is built into the watch.

## Development

To build it you need the Pebble tool and SDK 4.33.1:

```sh
uv tool install pebble-tool
pebble sdk install 4.33.1
pebble build
pebble install --emulator emery --vnc
```

`make -C tests/host` runs the unit tests for the formatting and drawing logic.
