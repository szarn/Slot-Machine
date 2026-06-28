#include "paylines.h"

namespace slot {
namespace {

// Names parallel the lines below; keep the two in sync.
const std::vector<Payline> kLines = {
    {{1, 1, 1, 1, 1}}, // middle row
    {{0, 0, 0, 0, 0}}, // top row
    {{2, 2, 2, 2, 2}}, // bottom row
    {{0, 1, 2, 1, 0}}, // V-shape
    {{2, 1, 0, 1, 2}}, // inverted-V
};

const char* kNames[] = {
    "middle row",
    "top row",
    "bottom row",
    "V-shape",
    "inverted-V",
};

} // namespace

const std::vector<Payline>& paylines() {
    return kLines;
}

const char* paylineName(int index) {
    if (index < 0 || index >= static_cast<int>(kLines.size())) return "?";
    return kNames[index];
}

std::vector<LineWin> evaluate(const Grid& grid, long lineBet) {
    std::vector<LineWin> wins;
    for (std::size_t i = 0; i < kLines.size(); ++i) {
        const Payline& line = kLines[i];
        Symbol first = grid[line[0]][0];
        int count = 1;
        for (int reel = 1; reel < kReels; ++reel) {
            if (grid[line[reel]][reel] != first) break;
            ++count;
        }
        int mult = payoutMultiplier(first, count);
        if (mult > 0) {
            wins.push_back(LineWin{static_cast<int>(i), first, count, mult * lineBet});
        }
    }
    return wins;
}

} // namespace slot
