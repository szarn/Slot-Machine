#ifndef SLOT_MACHINE_H
#define SLOT_MACHINE_H

#include <array>

#include "rng.h"
#include "symbol.h"

namespace slot {

inline constexpr int kReels = 5; // columns
inline constexpr int kRows  = 3; // rows visible in the window

// The visible window after a spin, indexed [row][reel].
using Grid = std::array<std::array<Symbol, kReels>, kRows>;

class SlotMachine {
public:
    // Spin every position, returning the visible grid. Each cell is drawn
    // independently from the weighted symbol distribution, so high-value
    // symbols land less often.
    Grid spin(Rng& rng) const;

private:
    Symbol drawSymbol(Rng& rng) const;
};

} // namespace slot

#endif // SLOT_MACHINE_H
