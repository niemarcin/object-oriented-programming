#include "Ship.hpp"

#include <algorithm>
#include <iomanip>

Ship::Ship()
    : id_(0) {
}

Ship::Ship(int capacity, int maxCrew, int speed, const std::string& name, size_t id, std::shared_ptr<Time>& time)
    : capacity_(capacity), maxCrew_(maxCrew), crew_(0), speed_(speed), name_(name), id_(id), time_(time) {
    time_->addObserver(this);
}

Ship::Ship(int maxCrew, int speed, size_t id, std::shared_ptr<Time>& time)
    : Ship(0, maxCrew, speed, "", id, time) {
}

Cargo* Ship::getCargo(size_t index) const {
    if (index > cargo_.size() || index == 0) {
        return nullptr;
    }
    return cargo_[index - 1].get();
}

Ship& Ship::operator-=(const size_t crew) {
    if (crew > crew_) {
        crew_ = 0;
        return *this;
    }
    crew_ -= crew;
    return *this;
}

Ship& Ship::operator+=(const size_t crew) {
    if (crew_ + crew > maxCrew_) {
        crew_ = maxCrew_;
        return *this;
    }
    crew_ += crew;
    return *this;
}

void Ship::load(const CargoPtr& cargo, const size_t amount) {
    Cargo* foundCargo = findGivenCargoInStock(cargo);
    if (foundCargo) {
        foundCargo += amount;
    }
    else {
        cargo_.push_back(cargo);
    }
}

void Ship::unload(Cargo* cargo, const size_t amount) {
    if (!cargo->getAmount()) {
        std::remove_if(cargo_.begin(), cargo_.end(), [cargo](const auto& ptr) {
            return ptr.get() == cargo;
        });
    }
}

void Ship::nextDay() {
    if (delegate_) {
        delegate_->payCrew(crew_ * salaryPerWorker);
    }
    std::for_each(cargo_.begin(), cargo_.end(), [](const auto& el){ el->nextDay(); });
}

std::ostream& operator<<(std::ostream& out, const Ship& ship) {
    std::string horizontalSeparator(61, '=');
    std::string headerSeparator(22, '=');
    int i = 0;
    out << "\n" << headerSeparator << " SHIP'S  STOCK " << headerSeparator
              << "\n"
              << "|| NAME"
              << std::setw(27) << "| QTY "
              << std::setw(5) << "| BASE PRICE "
              << std::setw(3) << " ||\n"
              << horizontalSeparator << "\n";

    for (const CargoPtr& el : ship.cargo_) {
        out << "||"
                  << std::setw(2) << ++i << ". "
                  << std::setw(21) << std::left << el->getName() << " | "
                  << std::setw(3) << std::right << el->getAmount() << " | "
                  << std::setw(10) << std::right << el->getBasePrice() << " ||\n";
    }
    out << horizontalSeparator << "\n";
    
    return out;
}

Cargo* Ship::findGivenCargoInStock(const CargoPtr& cargo) {
    auto cargo_it = std::find_if(cargo_.begin(), cargo_.end(),
                               [&cargo](const auto& crg) {
                                   return *(crg.get()) == *(cargo.get());
                               });
    
    return (cargo_it != cargo_.end()) ? (*cargo_it).get() : nullptr;
}
