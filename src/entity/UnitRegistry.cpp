#include "entity/UnitRegistry.hpp"

#include "entity/ConversionConstants.hpp"
#include "entity/DomainErrors.hpp"

#include <stdexcept>

namespace uc11 {

UnitRegistry::UnitRegistry() = default;

UnitRegistry UnitRegistry::withBuiltins() {
    UnitRegistry reg;
    reg.registerUnit("meter", 1.0);
    reg.registerUnit("feet", kFeetFactorToMeter);
    reg.registerUnit("yard", kYardFactorToMeter);
    return reg;
}

void UnitRegistry::registerUnit(const std::string& name, double factorToMeter) {
    if (name.empty()) {
        throw std::invalid_argument(DomainErr::kInvalidUnitNameEmpty);
    }
    if (factorToMeter <= 0.0) {
        throw std::invalid_argument(DomainErr::kFactorMustBePositive);
    }
    if (factors_.count(name) != 0) {
        throw std::invalid_argument(DomainErr::duplicateUnit(name));
    }
    factors_[name] = factorToMeter;
}

bool UnitRegistry::hasUnit(const std::string& name) const {
    return factors_.count(name) != 0;
}

double UnitRegistry::factorToMeter(const std::string& name) const {
    auto it = factors_.find(name);
    if (it == factors_.end()) {
        throw std::invalid_argument(DomainErr::unknownUnit(name));
    }
    return it->second;
}

std::vector<std::string> UnitRegistry::listUnits() const {
    std::vector<std::string> names;
    names.reserve(factors_.size());
    for (const auto& entry : factors_) {
        names.push_back(entry.first);
    }
    return names;
}

std::size_t UnitRegistry::size() const { return factors_.size(); }

}  // namespace uc11
