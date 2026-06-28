# 🎰 Slot Machine

A terminal slot machine written in C++17. Roll the reels, watch the symbols
stream down and settle on a 5×3 grid, and win on classic slot paylines — with a
flashy celebration when you hit. Play is unlimited; the game tracks how much
you've spent versus won across the session.

```
  ╔══════════════════════════════════╗
  ║        🎰  SLOT  MACHINE  🎰      ║
  ╚══════════════════════════════════╝

  Rolling...
  ┌────┬────┬────┬────┬────┐
  │ 🍒 │ 💎 │ ⭐ │ 🔔 │ 🍋 │
  ├────┼────┼────┼────┼────┤
  │ 💰 │ 💰 │ 💰 │ 🍒 │ ⭐ │   ← middle payline
  ├────┼────┼────┼────┼────┤
  │ 🔔 │ 🍋 │ 🍒 │ 💎 │ 🔔 │
  └────┴────┴────┴────┴────┘

  ✨ ★  W I N N E R !  ★ ✨

  3× 💰 Jackpot  on the middle row   $300

  TOTAL WON: $300
```

## Features

- **Animated reels** that stream random symbols and lock left-to-right, like a
  real machine.
- **5×3 grid with 5 paylines** — three rows plus both diagonals.
- **Weighted odds** — rarer symbols pay more and land less often, so the
  expected return is non-uniform by design (not a flat coin flip).
- **Flashy win celebration** with a per-line breakdown and the total won.
- **Session tracking** — total rolls, spent, won, and net result.
- **Modular, separately-compiled C++** with the game logic fully decoupled from
  terminal rendering.

## Requirements

- A C++17 compiler (`clang++` or `g++`).
- `make`.
- A UTF-8 terminal that supports ANSI escape codes and renders emoji at a width
  of 2 columns (most modern terminals: iTerm2, the macOS Terminal, GNOME
  Terminal, Windows Terminal, etc.).

## Build & Run

```sh
make        # build the ./slot binary
make run    # build, then play
make clean  # remove build artifacts
```

Or run the binary directly after building:

```sh
./slot
```

## How to Play

1. Launch the game. You'll see the title screen and the paytable.
2. At the prompt, press **Enter** to roll. Each roll costs **$10**.
3. The reels spin and settle. If any payline shows **3 or more matching symbols
   starting from the leftmost reel**, you win — the payouts from all winning
   lines are added up and a celebration is shown.
4. Keep rolling as long as you like. Type **`q`** (then Enter) to quit.
5. On exit, a session summary shows your rolls, total spent, total won, and net.

## Rules & Payouts

### Paylines

Every $10 roll plays all **5 paylines** at once ($2 per line). A line wins when
its **leftmost reels** show the same symbol on 3, 4, or 5 reels in a row,
counted from left to right.

| Payline       | Shape across the 5 reels        |
| ------------- | ------------------------------- |
| Middle row    | `─ ─ ─ ─ ─` (center)            |
| Top row       | `▔ ▔ ▔ ▔ ▔`                     |
| Bottom row    | `▁ ▁ ▁ ▁ ▁`                     |
| V-shape       | top → bottom → top              |
| Inverted-V    | bottom → top → bottom           |

### Paytable

Payouts below are **dollars per winning line** (at $2 per line). Rarer symbols
appear less often and pay more.

| Symbol       | Rarity      | 3 in a row | 4 in a row | 5 in a row |
| ------------ | ----------- | ---------- | ---------- | ---------- |
| 🍒 Cherry    | most common | $10        | $30        | $80        |
| 🍋 Lemon     | common      | $16        | $50        | $150       |
| 🔔 Bell      | uncommon    | $30        | $100       | $300       |
| ⭐ Star       | rare        | $60        | $200       | $600       |
| 💎 Diamond   | very rare   | $120       | $400       | $1,500     |
| 💰 Jackpot   | rarest      | $300       | $1,000     | **$5,000** |

Wins on multiple paylines in the same spin stack — the top prize is five
💰 Jackpots on every line at once.

## Project Structure

The code is conventional C++: each module is a `.h`/`.cpp` pair, separately
compiled and linked. All sources live in `src/`; everything is in
`namespace slot`, and dependencies flow one way — the game logic knows nothing
about how it's drawn.

```
.
├── src/                  # all C++ sources and headers
│   ├── main.cpp          Entry point: construct the RNG and game, run
│   ├── symbol.{h,cpp}    Symbol set + paytable: glyphs, weights, payout multipliers
│   ├── rng.{h,cpp}       Rng — a single Mersenne Twister engine, seeded once
│   ├── machine.{h,cpp}   Grid type and SlotMachine::spin (weighted draws)
│   ├── paylines.{h,cpp}  The payline set and evaluate() (scores winning lines)
│   ├── wallet.{h,cpp}    Running session totals (spent / won / net)
│   ├── display.{h,cpp}   All terminal output: animation, win banner, summary
│   └── game.{h,cpp}      Game — the play loop tying model and view together
├── docs/
│   └── DesignProcess.txt Original design brief and notes
├── Makefile
├── README.md
└── CLAUDE.md
```

## Testing

The core logic is engine-free and easy to test in isolation. Compile a small
driver against the relevant sources — for example, to test payline scoring:

```sh
c++ -std=c++17 -Isrc mytest.cpp src/symbol.cpp src/paylines.cpp -o mytest && ./mytest
```

Construct a `Grid`, call `evaluate(grid, lineBet)`, and assert on the returned
`LineWin`s. Because `Rng` accepts an explicit seed, whole spins can also be made
deterministic for reproducible tests.

## Implementation Notes

- **Money** is tracked as integer dollars (`long`) — never floating point.
- **Bet math:** the per-line wager is `costPerRoll / number-of-paylines`
  ($10 / 5 = $2). A line's payout is `multiplier × lineBet`.
- **Symbols are emoji** rendered assuming a 2-column display width; the grid
  borders are sized to match. Substituting symbols of a different width will
  misalign the grid.
- A **"mercy" mechanic** (raising win odds after a losing streak), sketched in
  `docs/DesignProcess.txt`, is noted but not yet implemented.
