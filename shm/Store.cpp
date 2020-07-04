#include "Store.hpp"
#include <algorithm>
#include <iomanip>

std::vector<std::tuple<std::string, int, int, int>> products{
        {"Lisc laurowy", 15, 100, 84},
        {"Pieprzowe whisky", 100, 1254, 1150},
        {"Sznur konopny", 20, 249, 210},
        {"Cyrkonie", 10, 300, 180},
        {"Kukurydza", 50, 15, 8},
        {"Rum", 150, 139, 112},
        {"Slodkie ziemniaki", 25, 36, 25},
        {"Liscie miety", 35, 122, 100},
        {"Owoce kakaowca", 25, 485, 429}};

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

    for (const auto& el : products) {
        out << "||"
                  << std::setw(2) << ++i << ". "
                  << std::setw(21) << std::left << std::get<0>(el) << " | "
                  << std::setw(3) << std::right << std::get<1>(el) << " | "
                  << std::setw(9) << std::right << std::get<2>(el) << " | "
                  << std::setw(10) << std::right << std::get<3>(el) << " ||\n";
    }
    out << horizontalSeparator << "\n";
    
    return out;
}

Response Store::buy(std::shared_ptr<Cargo> cargo, size_t amount, Player* player) {
    const size_t price = amount * cargo->getBasePrice();
    if (cargo -> getAmount() < amount){
        return Response::lack_of_cargo;
    }
    else if (player -> getMoney() < price){
        return Response::lack_of_money;
    }
    else if (player -> getAvailableSpace() < amount){
        return Response::lack_of_space;
    }
    *cargo += amount;
    player->buyCargo(cargo, amount, price);
    return Response::done;
}

Response Store::sell(std::shared_ptr<Cargo> cargo, size_t amount, Player* player) {
    const size_t price = amount * cargo->getPrice();
    *cargo -= amount; 
    player->sellCargo(cargo, amount, price);
    return Response::done;
}

