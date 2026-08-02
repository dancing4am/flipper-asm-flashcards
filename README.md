# ASM Cards

**ASM Cards** is a Flipper Zero flashcard app for learning x86-64 assembly — both
instructions and registers. Built for reverse-engineering practice: recognizing
instructions, register roles, and calling conventions quickly is a core skill when
reading disassembly, and this drills that on the go.

## Screen flow

Title -> Category (Instruction / Register) -> Deck -> Mode (Cards / Quiz) -> cards or quiz.

## Decks

**Instruction** category:

- **Easy** — the everyday basics: MOV, ADD, PUSH, CALL, JMP, CMP...
- **Medium** — flags, conditional jumps, multiply/divide, shifts, extends.
- **Hard** — rotates, bit scans, atomics, anti-debug staples (CPUID, RDTSC), string ops.
- **Float** — SSE scalar/packed float and double instructions and conversions.
- **Cond** — the conditional-jump family and the flags each one reads.
- **Rule** — naming rules: signed vs unsigned mnemonics, the N prefix, SS/SD/PS/PD suffixes.

**Register** category:

- **GP** — the general-purpose 64-bit registers (RAX-RSP, R8-R15, RIP) and what each conventionally holds.
- **Args** — calling-convention argument and return registers, covering both System V AMD64 (Linux/macOS) and Windows x64.
- **Flags** — the key RFLAGS bits (ZF, SF, CF, OF, PF, DF): what sets them and which jumps read them.

## Modes

- **Study mode** — flip through cards showing a name and its meaning. OK opens a
  detail screen with a longer explanation; OK (or Back) closes it.
- **Quiz mode** — two-option multiple choice, in both directions: name to meaning
  (`Quiz N->M`) or meaning to name (`Quiz M->N`).
- **Shuffled order** — cards appear in a random sequence with no repeats until the
  whole deck has been seen, then it reshuffles.

## Controls

- **Up / Down** — move the cursor in menus and quiz options
- **OK** — select; in study mode, open or close the detail screen
- **Left / Right** — previous / next card (study mode)
- **Back** — go up one level (exits from the title or category screen)

## Building

This project is built with [uFBT](https://github.com/flipperdevices/flipperzero-ufbt).

```sh
ufbt          # build
ufbt launch   # build, install to a connected Flipper, and run
```

The compiled `asmcards.fap` lands in the `dist/` folder — you can also install it
by copying that file into the `apps/` folder on your Flipper's SD card.
