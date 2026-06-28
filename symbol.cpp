#include "symbol.h"

namespace slot {
namespace {

// Row order must match the Symbol enum. Rarer symbols (lower weight) carry the
// bigger payout multipliers, so the expected return is non-uniform by design.
const std::array<SymbolInfo, kSymbolCount> kTable = {{
    //  glyph  name         weight   3x   4x    5x
    {u8"🍒", "Cherry",      30, {{ 2,   5,  10}}},
    {u8"🍋", "Lemon",       25, {{ 3,   8,  20}}},
    {u8"🔔", "Bell",        18, {{ 5,  15,  40}}},
    {u8"⭐", "Star",        12, {{10,  30,  80}}},
    {u8"💎", "Diamond",      8, {{20,  60, 150}}},
    {u8"💰", "Jackpot",      4, {{50, 200, 500}}},
}};

} // namespace

const SymbolInfo& info(Symbol s) {
    return kTable[static_cast<int>(s)];
}

int payoutMultiplier(Symbol s, int count) {
    if (count < 3 || count > 5) return 0;
    return info(s).payouts[count - 3];
}

int totalWeight() {
    int sum = 0;
    for (const auto& entry : kTable) sum += entry.weight;
    return sum;
}

} // namespace slot
