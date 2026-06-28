#ifndef SLOT_SYMBOL_H
#define SLOT_SYMBOL_H

#include <array>

namespace slot {

// The reel symbols, ordered low value -> high value. `Count` is a sentinel used
// to size tables and iterate; it is never a real symbol.
enum class Symbol : int {
    Cherry = 0,
    Lemon,
    Bell,
    Star,
    Diamond,
    Jackpot,
    Count
};

inline constexpr int kSymbolCount = static_cast<int>(Symbol::Count);

struct SymbolInfo {
    const char* glyph;            // emoji shown on the reel
    const char* name;             // human-readable name
    int weight;                   // relative frequency on a reel (higher = more common)
    std::array<int, 3> payouts;   // payout multipliers for 3, 4, 5 of a kind (x line bet)
};

// Static info for a symbol.
const SymbolInfo& info(Symbol s);

// Payout multiplier for `count` identical symbols on a line. Returns 0 for
// counts below 3 (no win) or above 5 (impossible on a 5-reel line).
int payoutMultiplier(Symbol s, int count);

// Sum of every symbol's weight; used by the weighted reel draw.
int totalWeight();

} // namespace slot

#endif // SLOT_SYMBOL_H
