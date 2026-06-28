#include "machine.h"

namespace slot {

Symbol SlotMachine::drawSymbol(Rng& rng) const {
    int roll = rng.intInRange(1, totalWeight());
    for (int i = 0; i < kSymbolCount; ++i) {
        Symbol s = static_cast<Symbol>(i);
        roll -= info(s).weight;
        if (roll <= 0) return s;
    }
    return static_cast<Symbol>(kSymbolCount - 1); // unreachable: weights sum to totalWeight()
}

Grid SlotMachine::spin(Rng& rng) const {
    Grid grid{};
    for (int r = 0; r < kRows; ++r)
        for (int c = 0; c < kReels; ++c)
            grid[r][c] = drawSymbol(rng);
    return grid;
}

} // namespace slot
