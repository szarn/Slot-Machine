#ifndef SLOT_PAYLINES_H
#define SLOT_PAYLINES_H

#include <array>
#include <vector>

#include "machine.h"
#include "symbol.h"

namespace slot {

// A payline names the row (0=top, 1=middle, 2=bottom) visited on each reel.
using Payline = std::array<int, kReels>;

// The fixed set of paylines evaluated on every spin (3 rows + 2 diagonals).
const std::vector<Payline>& paylines();

// Human-readable name for paylines()[index] (e.g. "middle row", "V-shape").
const char* paylineName(int index);

struct LineWin {
    int lineIndex;   // index into paylines()
    Symbol symbol;   // the matching symbol
    int count;       // consecutive matches from the leftmost reel (3..5)
    long payout;     // amount won on this line (multiplier * lineBet)
};

// Evaluate every payline against `grid`. A line wins when its leftmost reels
// show 3+ identical symbols in a row (left-to-right). `lineBet` is the wager
// apportioned to each line.
std::vector<LineWin> evaluate(const Grid& grid, long lineBet);

} // namespace slot

#endif // SLOT_PAYLINES_H
