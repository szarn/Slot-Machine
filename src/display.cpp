#include "display.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

namespace slot {
namespace {

// ANSI styling.
constexpr const char* kReset = "\033[0m";
constexpr const char* kBold = "\033[1m";
constexpr const char* kDim = "\033[2m";
constexpr const char* kCyan = "\033[1;36m";
constexpr const char* kGreen = "\033[1;32m";
constexpr const char* kRed = "\033[1;31m";
constexpr const char* kYellow = "\033[1;93m";
constexpr const char* kMagenta = "\033[1;95m";

constexpr int kGridHeight = 7;            // top border + 3 rows + 2 separators + bottom border
constexpr auto kFrameDelay = std::chrono::milliseconds(70);

void sleepFor(std::chrono::milliseconds d) { std::this_thread::sleep_for(d); }

// One bordered horizontal rule, e.g. ┌────┬────┬...┐. Each cell interior is 4
// columns wide (" X " where X is a width-2 emoji).
std::string borderLine(const char* left, const char* mid, const char* right) {
    std::string s = left;
    for (int c = 0; c < kReels; ++c) {
        s += u8"────";
        s += (c + 1 < kReels) ? mid : right;
    }
    return s;
}

// One row of symbols, e.g. │ 🍒 │ 🍋 │ ...
std::string symbolRow(const Grid& grid, int row) {
    std::string s;
    for (int c = 0; c < kReels; ++c) {
        s += u8"│ ";
        s += info(grid[row][c]).glyph;
        s += " ";
    }
    s += u8"│";
    return s;
}

// Print a line and clear anything left over from a previous (longer) frame.
void putLine(const std::string& line) {
    std::cout << line << "\033[K\n";
}

} // namespace

Symbol Display::randomSymbol() const {
    return static_cast<Symbol>(rng_.intInRange(0, kSymbolCount - 1));
}

void Display::drawGrid(const Grid& grid) const {
    putLine(borderLine(u8"┌", u8"┬", u8"┐"));
    putLine(symbolRow(grid, 0));
    putLine(borderLine(u8"├", u8"┼", u8"┤"));
    putLine(symbolRow(grid, 1));
    putLine(borderLine(u8"├", u8"┼", u8"┤"));
    putLine(symbolRow(grid, 2));
    putLine(borderLine(u8"└", u8"┴", u8"┘"));
}

void Display::animateSpin(const Grid& result) const {
    // Each reel locks onto its final symbols a few frames after the previous
    // one, so the columns settle left-to-right like a real machine.
    std::array<int, kReels> lockAt{};
    for (int c = 0; c < kReels; ++c) lockAt[c] = 8 + c * 4;
    const int lastFrame = lockAt[kReels - 1] + 2;

    std::cout << "\n" << kCyan << "  Rolling..." << kReset << "\n";
    for (int frame = 0; frame <= lastFrame; ++frame) {
        Grid view{};
        for (int c = 0; c < kReels; ++c) {
            const bool locked = frame >= lockAt[c];
            for (int r = 0; r < kRows; ++r)
                view[r][c] = locked ? result[r][c] : randomSymbol();
        }
        if (frame > 0) std::cout << "\033[" << kGridHeight << "A"; // overwrite previous grid
        drawGrid(view);
        std::cout.flush();
        sleepFor(kFrameDelay);
    }
}

void Display::celebrate(const std::vector<LineWin>& wins, long totalWon) const {
    const std::string banner = u8"  ✨ ★  W I N N E R !  ★ ✨";

    // Flash the banner in place a few times.
    for (int i = 0; i < 6; ++i) {
        const char* color = (i % 2 == 0) ? kYellow : kMagenta;
        std::cout << "\r" << color << kBold << banner << kReset << "\033[K" << std::flush;
        sleepFor(std::chrono::milliseconds(140));
    }
    std::cout << "\n\n";

    for (const auto& w : wins) {
        std::cout << "  " << kBold << w.count << u8"× " << info(w.symbol).glyph << " "
                  << info(w.symbol).name << kReset
                  << kDim << "  on the " << paylineName(w.lineIndex) << kReset
                  << "   " << kGreen << "$" << w.payout << kReset << "\n";
    }
    std::cout << "\n  " << kGreen << kBold << "TOTAL WON: $" << totalWon << kReset << "\n";
}

void Display::showLoss() const {
    std::cout << "\n  " << kDim << u8"— no win this spin —" << kReset << "\n";
}

void Display::showRunningTotal(const Wallet& wallet) const {
    const long net = wallet.net();
    const char* netColor = (net >= 0) ? kGreen : kRed;
    const char* sign = (net >= 0) ? "+" : "-";
    std::cout << "\n  " << kBold << "Total: " << netColor << sign << "$" << std::abs(net)
              << kReset << kDim << "   (spent $" << wallet.spent()
              << " · won $" << wallet.won() << ")" << kReset << "\n";
}

void Display::welcome(long costPerRoll, long lineBet) const {
    std::cout << "\033[2J\033[H"; // clear screen, cursor home
    std::cout << kCyan << kBold
              << "  ╔══════════════════════════════════╗\n"
              << u8"  ║        🎰  SLOT  MACHINE  🎰      ║\n"
              << "  ╚══════════════════════════════════╝\n"
              << kReset << "\n";

    std::cout << "  " << kReels << " reels × " << kRows << " rows, "
              << paylines().size() << " paylines.\n";
    std::cout << "  Each roll costs " << kBold << "$" << costPerRoll << kReset
              << " (" << "$" << lineBet << " per line).\n";
    std::cout << "  Match 3+ identical symbols from the leftmost reel on a payline to win.\n\n";

    std::cout << "  " << kBold << "PAYTABLE" << kReset << kDim
              << "  (payout in $ per line: 3× / 4× / 5×)" << kReset << "\n";
    for (int i = 0; i < kSymbolCount; ++i) {
        const SymbolInfo& si = info(static_cast<Symbol>(i));
        std::cout << "    " << si.glyph << "  " << si.name;
        // Pad the name column to roughly align the figures.
        for (int pad = static_cast<int>(std::string(si.name).size()); pad < 9; ++pad)
            std::cout << ' ';
        std::cout << "$" << si.payouts[0] * lineBet
                  << "  /  $" << si.payouts[1] * lineBet
                  << "  /  $" << si.payouts[2] * lineBet << "\n";
    }
    std::cout << "\n";
}

void Display::showSessionSummary(const Wallet& wallet) const {
    const long net = wallet.net();
    const char* netColor = (net >= 0) ? kGreen : kRed;
    const char* sign = (net >= 0) ? "+" : "-";

    std::cout << "\n  " << kCyan << kBold << "──── SESSION SUMMARY ────" << kReset << "\n";
    std::cout << "    Rolls: " << wallet.rolls() << "\n";
    std::cout << "    Spent: $" << wallet.spent() << "\n";
    std::cout << "    Won:   $" << wallet.won() << "\n";
    std::cout << "    Net:   " << netColor << kBold << sign << "$" << std::abs(net)
              << kReset << "\n\n";
    std::cout << "  Thanks for playing!\n";
}

} // namespace slot
