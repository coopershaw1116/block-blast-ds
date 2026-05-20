# Block Blast! DS

A faithful and feature-rich Nintendo DS recreation of the hit mobile puzzle game **Block Blast!** by Hungry Studio, designed specifically for the Nintendo DS and DSi family of systems.

Built from scratch in C with native DS hardware support, optimized controls, and desktop test builds for rapid development.

---

## Features

### Gameplay
- Classic Block Blast-style gameplay
- Place blocks onto the board to clear rows and columns
- 3-piece rotation system before refreshing selections
- Smart block generation based on current board state
- Score tracking system
- Persistent local high score saving
- Smooth clearing animations inspired by the original game

### Nintendo DSi Enhancements
- Improved performance on DSi hardware
- Expanded memory support
- Faster loading and rendering

### Planned Features
- **Extra Blocks Mode**
  - 25 unique block types
  - Experimental gameplay variations
  - More difficult late-game strategies
  - Versus gamemode (1v1 local using DS wireless capabilities)

---

# Screenshots

> Screenshots coming soon.

---

# Running on Nintendo DS Hardware

## Using a Flashcart

1. Download the `.nds` release file
2. Copy it to your flashcart SD card
3. Launch the game from your flashcart menu

Compatible with:
- R4 cards
- Ace3DS
- DSTT
- Most standard Nintendo DS flashcarts

---

# Running on PC (Testing Build)

Desktop builds are intended for testing and development.

## Prebuilt Version

1. Download the PC executable
2. Run the executable directly

---

# Building for Nintendo DS

## Requirements

- devkitPro
- `nds-dev`
- Make
- libnds
- Blocksds

Install devkitPro:

- https://devkitpro.org/

---

## Build Instructions

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM

make
```

### Output

```text
BLOCK-BLAST-DS.nds
```

Copy the generated `.nds` file to your flashcart SD card.

---

# Nintendo DS Controls

| Button | Action |
|--------|--------|
| D-Pad | Move cursor |
| L / R | Cycle selected piece |
| A | Place piece |

---

# Desktop Test Build (SDL2)

Useful for rapid debugging and gameplay testing without DS hardware.

## Using Makefile

```bash
make -f Makefile.pc
./build-pc/BLOCK-BLAST-DS
```

## Using CMake

```bash
cmake -B build-pc -S .
cmake --build build-pc

./build-pc/BLOCK-BLAST-DS
```

---

# PC Controls

| Key | Action |
|-----|--------|
| Arrow Keys | Move cursor |
| Q / E | Cycle piece |
| Space | Place piece |

---

# Project Structure

```text
BLOCK-BLAST-DS/
├── data/                # Game assets
│   ├── platform/
│   │   ├── nds/                # Nintendo DS specific backend
│   │   └── pc/                 # PC (Desktop/SDL) backend
```

### Directory Overview

| Directory | Purpose |
|----------|---------|
| `src/core/` | Game rules, board logic, scoring, line clearing |
| `src/platform/nds/` | Rendering, input, and hardware-specific DS code |
| `src/platform/pc/` | SDL2 desktop testing implementation |

---

# Development Stack

| Technology | Usage |
|------------|------|
| C | Primary game code |
| Make | Nintendo DS compilation |
| SDL2 | PC testing build |

---

# Goals

- Create the most accurate Nintendo DS adaptation of Block Blast possible
- Maintain authentic gameplay feel
- Keep performance stable on original DS hardware
- Add optional DS-exclusive enhancements without changing core gameplay
- Introduce a native DS game feel
- Add new fun twists on the original game to enhance replayablility

---

# Disclaimer

**Block Blast! DS is a fan-made recreation and is not affiliated with or endorsed by Hungry Studio.**

**All original trademarks and intellectual property belong to their respective owners.**

**All assets are faithful recreations of their original counterparts.**

**Drafton (Me) does not endorse the game piracy, illegal game distribution or flashcarts.**

**Software is released "as is" with no guarrenty or warrenty. All forks must remain under the license "GPL 3.0" to remain legally compliant of the rules of the license.**

---

**_Made with ❤️ by Drafton_**
