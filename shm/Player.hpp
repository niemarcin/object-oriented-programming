#pragma once

#include <memory>

#include "Cargo.hpp"
#include "Ship.hpp"

class Player {
public:
    Player(std::unique_ptr<Ship> ship, size_t money);

    size_t getMoney() const { return money_; };
    size_t getAvaliableSpace() const { return avaliableSpace_; };
    size_t getSpeed() const { return ship_->getSpeed(); };
    Cargo* getCargo(size_t index) const { return ship_->getCargo(index); };
    void buyCargo(Cargo* cargo, size_t amount, size_t price);
    void sellCargo(Cargo* cargo, size_t amount, size_t price);

private:
    std::unique_ptr<Ship> ship_;
    size_t money_;
    size_t avaliableSpace_;

    size_t countAvailableSpace();
};
