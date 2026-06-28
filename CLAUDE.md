# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

A terminal slot machine in C++17. The player rolls for $10 a spin; reels stream
down and settle on a 5×3 grid; matching symbols on a payline pay out, with a
flashy celebration on a win. Play is unlimited (no bankroll cap) — the session
tracks total spent vs. won and the net result.

## Build & Run

```sh
make        # builds the ./slot binary (separate compilation, -Wall -Wextra)
make run    # build then run
make clean  # remove object files and the binary
```

Build artifacts (`*.o`, `slot`) are git-ignored; do not commit them. The game is
interactive (reads stdin); for a non-interactive smoke test, pipe input:
`printf '\n\nq\n' | ./slot` (two rolls, then quit).

There is no test framework wired into the Makefile. Core logic is engine-free and
easy to test directly: compile a small driver against the relevant `.cpp` files,
e.g. `c++ -std=c++17 -I. mytest.cpp symbol.cpp paylines.cpp -o mytest`. The most
valuable target is `evaluate()` in `paylines.cpp` — construct a `Grid`, call it
with a known `lineBet`, and assert on the returned `LineWin`s.

## Architecture

Conventional C++: each module is a `.h`/`.cpp` pair, separately compiled and
linked (see the Makefile). Everything lives in `namespace slot`. Dependencies flow
one direction — the model knows nothing about rendering:

- **`symbol`** — `enum class Symbol` plus the static paytable: each symbol's glyph,
  name, reel **weight**, and 3×/4×/5× payout multipliers. Rarer symbols (lower
  weight) carry higher multipliers, so expected return is non-uniform by design.
- **`rng`** — `Rng` wraps one `std::mt19937` **seeded once** at construction.
  An explicit-seed constructor exists for deterministic sessions/tests.
- **`machine`** — defines `kReels=5`, `kRows=3`, and `Grid` (`[row][reel]`).
  `SlotMachine::spin` draws each cell from the weighted distribution.
- **`paylines`** — the fixed payline set (3 rows + 2 diagonals) and `evaluate()`,
  which scores **3+ identical symbols counted left-to-right from reel 0** on each
  line and returns the winning lines.
- **`wallet`** — running session totals (spent / won / net / roll count).
- **`display`** — all terminal output via ANSI escape codes: welcome + paytable,
  the reel animation (reels lock left-to-right by redrawing the 7-line grid in
  place), the flashy win banner, and the session summary. Takes an `Rng&` only for
  the animation's flicker symbols.
- **`game`** — `Game` owns the model and view and runs the loop: prompt → charge
  `kCostPerRoll` → spin → animate → evaluate → pay out → report. The `Rng` is
  injected (constructed in `main`) so a whole session can be seeded.
- **`main.cpp`** — minimal: construct `Rng`, construct `Game`, `run()`.

## Conventions & invariants

- **Bet math:** one $10 spin covers all paylines; the per-line wager is
  `kCostPerRoll / paylines().size()`. A `LineWin.payout` is `multiplier * lineBet`.
  If you change the number of paylines or `kCostPerRoll`, keep the division exact.
- **Symbol table ordering:** the array in `symbol.cpp` must stay parallel to the
  `Symbol` enum; likewise the payline names array must stay parallel to the
  payline list in `paylines.cpp`.
- **Money** is integer dollars (`long`), never floating point.
- **Symbols are emoji** rendered assuming a display width of 2 columns; the grid
  borders are sized to match. Swapping in symbols of a different width will
  misalign the grid.
- `DesignProcess.txt` holds the original brief (weighted/non-uniform odds, an
  unimplemented "mercy" mechanic that raises odds after losing streaks).
