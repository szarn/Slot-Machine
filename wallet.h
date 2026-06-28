#ifndef SLOT_WALLET_H
#define SLOT_WALLET_H

namespace slot {

// Tracks lifetime money flow for the session. Unlimited play: there is no
// balance cap, only running totals of money in vs. money out.
class Wallet {
public:
    void spend(long amount); // money put in for a roll (also counts the roll)
    void win(long amount);   // money paid out

    long spent() const { return spent_; }
    long won() const { return won_; }
    long net() const { return won_ - spent_; }
    int rolls() const { return rolls_; }

private:
    long spent_ = 0;
    long won_ = 0;
    int rolls_ = 0;
};

} // namespace slot

#endif // SLOT_WALLET_H
