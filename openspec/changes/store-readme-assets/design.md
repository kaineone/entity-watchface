# Design

## Decisions
- **Staged captures**: a throwaway recording build pins the temperature (26°), stretches the
  double-tap window for the emulator and stages disconnects from battery hooks; it also stamps a
  100 ms frame counter into two always-black pixels, which the assembler decodes and blacks out.
  The build is never committed or installed on a real watch.
- **On-glass colour**: VNC frames carry raw framebuffer levels (lit, or dimmed on two different
  scales on emery and gabbro); they are mapped per frame through Core's panel LUT so captures look
  as the watch shows them.
- **Frame-exact video**: each grab is placed by the watch's own frame counter rather than VNC
  arrival time, then demonstrations are joined with a 0.4 s eased slide at 20 fps.
- **README art** is generated from the store screenshots by the same script, 2× nearest-neighbour.
- Older Pebbles report battery in 10% steps, so their shots read 90%.
