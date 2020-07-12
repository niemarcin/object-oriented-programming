#include "Store.hpp"

#include "Alcohol.hpp"
#include "Fruit.hpp"
#include "Item.hpp"

#include <algorithm>
#include <iomanip>
#include <random>

Store::Store() {
    generateCargo();
}

Store::Store(std::shared_ptr<Time>& time)
    : time_(time) {
    generateCargo();
    time_->addObserver(this);
}

Response Store::buy(Cargo* cargo, size_t amount, Player* player) {
    const size_t price = amount * cargo->getBasePrice();
    if (cargo->getAmount() < amount) {
        return Response::lack_of_cargo;
    } else if (player->getMoney() < price) {
        return Response::lack_of_money;
    } else if (player->getAvailableSpace() < amount) {
        return Response::lack_of_space;
    }
    *cargo += amount;
    player->purchaseCargo(CargoPtr (cargo), amount, price);
    return Response::done;
}

Response Store::sell(Cargo* cargo, size_t amount, Player* player) {
    const size_t price = amount * cargo->getPrice();
    *cargo -= amount; 
    player->sellCargo(CargoPtr (cargo), amount, price);
    return Response::done;
}

void Store::nextDay() {
    randomizeCargoAmount();
}

void Store::generateCargo() {
    cargo_.emplace_back(std::make_shared<Alcohol>("Rum", 150, 100, 60));
    cargo_.emplace_back(std::make_shared<Alcohol>("Vodka", 20, 85, 70));
    cargo_.emplace_back(std::make_shared<Fruit>("Apple", 10, 5));
    cargo_.emplace_back(std::make_shared<Item>("Best item ever", 1, 1000, Rarity::legendary));
    
    randomizeCargoAmount();
}

void Store::randomizeCargoAmount() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 20);
    std::uniform_int_distribution<> changeSign(100);

    std::for_each(cargo_.begin(), cargo_.end(), [this, &distrib, &changeSign, &gen](const auto& el){
        if (changeSign(gen) % 2) {
            *el += distrib(gen);
        }
        else {
            *el -= distrib(gen);
        }
    });
}

std::ostream& operator<<(std::ostream& out, const Store& store) {
    std::string horizontalSeparator(61, '=');
    int i = 0;
    out << horizontalSeparator 
              << "\n"
              << "|| NAME"
              << std::setw(27) << "| QTY "
              << std::setw(5) << "| BUY PRICE "
              << std::setw(3) << "| SELL PRICE"
              << std::setw(3) << " ||\n"
              << horizontalSeparator << "\n";

    for (const CargoPtr& el : store.cargo_) {
        out << "||"
                  << std::setw(2) << ++i << ". "
                  << std::setw(21) << std::left << el->getName() << " | "
                  << std::setw(3) << std::right << el->getAmount() << " | "
                  << std::setw(9) << std::right << store.calculateBuyPrice(el.get()) << " | "
                  << std::setw(10) << std::right << el->getPrice() << " ||\n";
    }
    out << horizontalSeparator << "\n";
    
    return out;
}

size_t Store::calculateBuyPrice(Cargo* cargo) const {
    constexpr size_t cargoPriceThreshold{10};
    constexpr double belowThreshMultiplier{1.6};
    constexpr double aboveThreshMultiplier{1.2};

    size_t cargoPrice = cargo->getPrice();
    double buyPriceMultiplier = (cargoPrice <= cargoPriceThreshold ? belowThreshMultiplier : aboveThreshMultiplier);

    return cargoPrice * buyPriceMultiplier;
}
