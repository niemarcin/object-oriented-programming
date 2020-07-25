#include "Store.hpp"

#include "Alcohol.hpp"
#include "Fruit.hpp"
#include "Item.hpp"

#include <algorithm>
#include <iomanip>
#include <random>

Store::Store() {
    defineStoreEconomy();
    generateCargo();
}

Store::Store(std::shared_ptr<Time>& time)
    : time_(time) {
    defineStoreEconomy();
    generateCargo();
    time_->addObserver(this);
}

Response Store::buy(Cargo* cargo, size_t amount, Player* player) {
    //cargo is a product selected from vector of cargo which are available in this store
    const size_t price = amount * this->calculateBuyPrice(cargo);   //buy price is calculated based on Store economy
    if (cargo->getAmount() < amount) {
        return Response::lack_of_cargo;
    } else if (player->getMoney() < price) {
        return Response::lack_of_money;
    } else if (player->getAvailableSpace() < amount) {
        return Response::lack_of_space;
    }
    *cargo -= amount;   // when player buys the product then we should decrease amount of this product in this store
    player->purchaseCargo(CargoPtr(cargo), amount, price);
    return Response::done;
}

Response Store::sell(Cargo* cargo, size_t amount, Player* player) {
    const size_t price = amount * cargo->getPrice();
    *cargo += amount; // when player sells the product then we should increace amount of this product in this store
    player->sellCargo(CargoPtr(cargo), amount, price);
    return Response::done;
}

void Store::nextDay() {
    //randomizeCargoAmount();
    cargo_.clear();
    generateCargo();
}

void Store::generateCargo() { //TODO generate more Cargo in Store or maybe even make it in different way
    generateAlcohol();
    generateFruits();
    generateItems();

    // cargo_.emplace_back(std::make_shared<Alcohol>("Rum", 150, 100, 60));
    // cargo_.emplace_back(std::make_shared<Alcohol>("Vodka", 20, 85, 70));
    // cargo_.emplace_back(std::make_shared<Fruit>("Apple", 10, 5));
    // cargo_.emplace_back(std::make_shared<Item>("Best item ever", 1, 1000, Rarity::legendary));
    
//    randomizeCargoAmount();
}

void Store::generateAlcohol() {
    for (auto name : alcoholNames_) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> randAmount(0, 20);
        std::uniform_int_distribution<> randBasePrice(100, 200);
        std::uniform_int_distribution<> randAlcoholContent(1, maximumAlcoholContent);
        cargo_.emplace_back(std::make_shared<Alcohol>(name, randAmount(gen), randBasePrice(gen), randAlcoholContent(gen)));
    }
}

void Store::generateFruits() {
    for (auto name : fruitNames_) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> randAmount(0, 15);
        std::uniform_int_distribution<> randBasePrice(30, 50);
        cargo_.emplace_back(std::make_shared<Fruit>(name, randAmount(gen), randBasePrice(gen)));
    }
}

void Store::generateItems() {
    for (auto name : itemNames_) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> randAmount(0, 20);
        std::uniform_int_distribution<> randBasePrice(50, 100);
        std::uniform_int_distribution<> randRarity(0, rarityAmount - 1);
        cargo_.emplace_back(std::make_shared<Item>(name, randAmount(gen), randBasePrice(gen), possibleRarities[randRarity(gen)]));
    }
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

size_t Store::calculateBuyPrice(Cargo* const cargo) const {
    size_t cargoPrice = cargo->getPrice();
    double buyPriceMultiplier = (cargoPrice <= cargoPriceThreshold_ ? belowThreshMultiplier_ : aboveThreshMultiplier_);

    return cargoPrice * buyPriceMultiplier;
}

void Store::defineStoreEconomy() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> priceThresh(10, 20);
    std::uniform_real_distribution<> below(1.5, 1.9);
    std::uniform_real_distribution<> above(1.2, 1.5);

    cargoPriceThreshold_ = priceThresh(gen);
    belowThreshMultiplier_ = below(gen);
    aboveThreshMultiplier_ = above(gen);
}
