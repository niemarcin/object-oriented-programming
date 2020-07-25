#pragma once

#include "Cargo.hpp"
#include "Observer.hpp"
#include "Player.hpp"

enum class Response {
    done,
    lack_of_money,
    lack_of_cargo,
    lack_of_space
};

class Store : public Observer {
private:
    std::shared_ptr<Time> time_{nullptr};
    CargoStock cargo_;
    size_t cargoPriceThreshold_{10};
    double belowThreshMultiplier_{1.6};
    double aboveThreshMultiplier_{1.2};
    const std::array<std::string, 4> alcoholNames_{"beer", "rum", "vodka", "whiskey"};
    const std::array<std::string, 4> fruitNames_{"apple", "banana", "stawberry", "orange"};
    const std::array<std::string, 4> itemNames_{"sword", "hammer", "knife", "wood"};

    void generateCargo();
    void generateAlcohol();
    void generateFruits();
    void generateItems();
    void randomizeCargoAmount();
    size_t calculateBuyPrice(Cargo* const cargo) const;
    void defineStoreEconomy();
    

public:
    Store();
    Store(std::shared_ptr<Time>& time);
    Cargo* getCargo(const size_t pos) { return cargo_[pos].get(); }
    Response buy(Cargo* cargo, size_t amount, Player* player);
    Response sell(Cargo* cargo, size_t amount, Player* player);

    //Override from Observer
    void nextDay() override;

    friend std::ostream& operator<<(std::ostream& out, const Store& store);
};
