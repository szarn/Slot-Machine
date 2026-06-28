#include "rng.h"

namespace slot {

Rng::Rng() : engine_(std::random_device{}()) {}

Rng::Rng(std::uint32_t seed) : engine_(seed) {}

int Rng::intInRange(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(engine_);
}

} // namespace slot
