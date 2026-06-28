#include "wallet.h"

namespace slot {

void Wallet::spend(long amount) {
    spent_ += amount;
    ++rolls_;
}

void Wallet::win(long amount) {
    won_ += amount;
}

} // namespace slot
