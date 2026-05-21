#pragma once

#include "entity/ConversionRule.hpp"

#include <map>
#include <string>
#include <vector>

namespace uc11 {

class UnitRegistry {
public:
    UnitRegistry();

    void registerUnit(const std::string& name, double factorToMeter);
    void registerUnit(const ConversionRule& rule);
    bool hasUnit(const std::string& name) const;
    double factorToMeter(const std::string& name) const;
    std::vector<std::string> listUnits() const;
    std::size_t size() const;

    static UnitRegistry withBuiltins();

private:
    std::map<std::string, double> factors_;
};

}  // namespace uc11
