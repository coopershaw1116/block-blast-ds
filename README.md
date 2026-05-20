# Block Blast! DS

An accurate, comprehensive and fun DS port of the hit mobile game **Block Blast!** by **Hungry Studio** (includes DSi enhancements).

## Building for Nintendo DS (primary)

Requires [devkitPro](https://devkitpro.org/) with `nds-dev` installed:

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM
make
```

Output: `BLOCK-BLAST-DS.nds` — copy to your flashcart SD card (R4, etc.).

### Controls (hardware)

| Button | Action |
|--------|--------|
| D-Pad | Move cursor |
| L / R | Cycle piece in hand |
| A | Place piece |

## Desktop test build (optional, SDL2)

For quick testing on PC without a flashcart:

```bash
make -f Makefile.pc
./build-pc/BLOCK-BLAST-DS
```

### Controls (PC)

| Key | Action |
|-----|--------|
| Arrow keys | Move cursor |
| Q / E | Cycle piece |
| Space | Place piece |

## Project layout

- `src/core/` — portable game logic (board, pieces, line clearing)
- `src/platform/nds/` — libnds rendering and input (`.nds` build only)
- `src/platform/pc/` — SDL2 desktop test build (not linked into `.nds`)
