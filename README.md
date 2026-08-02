# Register Cards

A Flipper Zero app for learning x86-64 assembly instructions through flashcards.
Built for reverse-engineering practice: recognizing instructions quickly is a core
skill when reading disassembly, and this drills that on the go.

## Features

- **Six decks** grouped by difficulty and topic: Easy, Medium, Hard, Float (SSE/floating-point), Cond (conditional jumps), and Rule (instruction-suffix rules).
- **Study mode** — flip through cards showing an instruction and its meaning. Press OK on any card for a longer explanation.
- **Quiz mode** — multiple-choice practice in both directions: name to meaning, or meaning to name.
- **Shuffled order** — cards appear in a random sequence with no repeats until the whole deck has been seen, then it reshuffles.

## Controls

- **Up / Down** — move the cursor in menus and quiz options
- **OK** — select; in study mode, open or close the detail screen
- **Left / Right** — previous / next card (study mode)
- **Back** — return to the previous screen

## Building

This project is built with [uFBT](https://github.com/flipperdevices/flipperzero-ufbt).

Run `ufbt` to build, or `ufbt launch` to build, install to a connected Flipper, and run it. The compiled `.fap` lands in the `dist/` folder — you can also install it by copying that file into the `apps/` folder on your Flipper's SD card.
