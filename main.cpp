#include "game.h"
#include "rng.h"

int main() {
    slot::Rng rng;
    slot::Game game(rng);
    game.run();
    return 0;
}
