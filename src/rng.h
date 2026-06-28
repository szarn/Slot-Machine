#ifndef SLOT_RNG_H
#define SLOT_RNG_H

#include <cstdint>
#include <random>

namespace slot {

// A single Mersenne Twister engine, seeded once on construction. (The starter
// code reseeded a fresh engine on every draw, which is both slow and biased;
// this is the fix.)
class Rng {
public:
    Rng();                              // seed from std::random_device
    explicit Rng(std::uint32_t seed);   // deterministic seed for tests/replays

    // Uniform integer in [lo, hi], inclusive.
    int intInRange(int lo, int hi);

private:
    std::mt19937 engine_;
};

} // namespace slot

#endif // SLOT_RNG_H
