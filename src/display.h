#ifndef SLOT_DISPLAY_H
#define SLOT_DISPLAY_H

#include <vector>

#include "machine.h"
#include "paylines.h"
#include "rng.h"
#include "wallet.h"

namespace slot {

// Everything the player sees in the terminal: the welcome screen and paytable,
// the reel "streaming down" animation, the settled grid, the flashy win banner,
// and the end-of-session summary. Rendering uses ANSI escape codes.
class Display {
public:
    explicit Display(Rng& rng) : rng_(rng) {}

    void welcome(long costPerRoll, long lineBet) const;

    // Show the player's running total (net, with spent/won breakdown) before a roll.
    void showRunningTotal(const Wallet& wallet) const;

    // Animate the reels streaming and settling, reel by reel, onto `result`.
    void animateSpin(const Grid& result) const;

    // Flashy celebration listing each winning line and the total won.
    void celebrate(const std::vector<LineWin>& wins, long totalWon) const;

    // Quiet message for a losing spin.
    void showLoss() const;

    void showSessionSummary(const Wallet& wallet) const;

private:
    Rng& rng_;

    Symbol randomSymbol() const;
    void drawGrid(const Grid& grid) const; // prints the 7-line bordered grid in place
};

} // namespace slot

#endif // SLOT_DISPLAY_H
