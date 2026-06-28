#include "game.h"

#include <iostream>
#include <string>
#include <vector>

#include "paylines.h"

namespace slot {

Game::Game(Rng& rng) : rng_(rng), display_(rng) {}

long Game::lineBet() const {
    return kCostPerRoll / static_cast<long>(paylines().size());
}

bool Game::promptRoll() const {
    std::cout << "  Press [Enter] to roll ($" << kCostPerRoll
              << "), or type q to quit: " << std::flush;
    std::string input;
    if (!std::getline(std::cin, input)) return false; // EOF / stream closed
    return input.empty() || (input[0] != 'q' && input[0] != 'Q');
}

void Game::run() {
    display_.welcome(kCostPerRoll, lineBet());

    while (true) {
        display_.showRunningTotal(wallet_); // current total is visible before each spin
        if (!promptRoll()) break;

        wallet_.spend(kCostPerRoll);

        const Grid grid = machine_.spin(rng_);
        display_.animateSpin(grid);

        const std::vector<LineWin> wins = evaluate(grid, lineBet());
        long won = 0;
        for (const auto& w : wins) won += w.payout;

        if (won > 0) {
            wallet_.win(won);
            display_.celebrate(wins, won);
        } else {
            display_.showLoss();
        }
    }

    display_.showSessionSummary(wallet_);
}

} // namespace slot
