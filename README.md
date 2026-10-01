# Entity

Entity is a watchface for the Pebble Time 2 and the Pebble Round 2.

Most of the screen is the time, set in big Zen Dots numerals with the hour sitting above the
minute. Along the bottom is a row of bars that shows whether your watch can reach your phone.
While it can, a red bar scans side to side with a little orange trail behind it. If the
connection drops, the bars flatten out and turn grey, so you can tell at a glance.

The top of the screen has the date and the weather. Tap the watch and it switches over to your
steps and heart rate for a few seconds, then flips back.

On the Round 2 the bars wrap around the edge of the screen as tick marks, and the red one
travels around the rim.

## Battery

The moving bar is the one thing here that really costs power, so Entity holds it still when it
isn't earning its keep. That happens when your battery gets low (20% unless you pick a different
number in the settings), during quiet time, while a timeline peek covers the screen, or any time
you switch the animation off.

## Building it yourself

You'll need the Pebble tool and SDK 4.33.1:

```sh
uv tool install pebble-tool
pebble sdk install 4.33.1
```

Then build it and try it in the emulator:

```sh
pebble build
pebble install --emulator emery --vnc
```

There are a few unit tests for the formatting and drawing logic. Run them with
`make -C tests/host`.

## Fonts

Zen Dots is by The Dots Project Authors and JetBrains Mono is by JetBrains. Both are under the
SIL Open Font License 1.1, and you'll find the license texts in `resources/fonts/`.
