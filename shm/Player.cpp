#include "Player.hpp"

#include <numeric>

Player::Player(std::unique_ptr<Ship>& ship, size_t money) {
    ship_ = std::move(ship);  //it might be good to let the Player class to construct Ship for itself
    ship_->setDelegate(std::bind(&Player::payCrew, this, std::placeholders::_1));
    money_ = money;
    avaliableSpace_ = countAvailableSpace();
};

void Player::payCrew(size_t money) {
    if (money > money_) {
        money_ = SIZE_MAX;
    } else {
        money_ -= money;
    }
}

size_t Player::countAvailableSpace() {
    auto occupiedSpace = std::accumulate(begin(ship_->getAllCargos()),
                                         end(ship_->getAllCargos()),
                                         0,
                                         [](int accumulated, const auto& cargo) {
                                             return accumulated + cargo->getAmount();
                                         });

    if (ship_->getCapacity() <= occupiedSpace) {
        return 0;
    }
    return ship_->getCapacity() - occupiedSpace;
}
