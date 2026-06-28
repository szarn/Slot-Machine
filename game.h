#ifndef SLOT_GAME_H
#define SLOT_GAME_H

#include "display.h"
#include "machine.h"
#include "rng.h"
#include "wallet.h"

namespace slot {

// Orchestrates the play loop: prompt, charge, spin, animate, evaluate, pay out,
// and report. Owns the model (machine, wallet) and view (display); the RNG is
// injected so the whole session can be seeded deterministically.
class Game {
public:
    explicit Game(Rng& rng);
    void run();

private:
    static constexpr long kCostPerRoll = 10;

    long lineBet() const;     // wager apportioned to each payline
    bool promptRoll() const;  // true = roll, false = quit/EOF

    Rng& rng_;
    SlotMachine machine_;
    Display display_;
    Wallet wallet_;
};

} // namespace slot

#endif // SLOT_GAME_H
