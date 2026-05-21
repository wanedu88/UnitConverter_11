#pragma once

#include "entity/ConversionRule.hpp"

namespace uc11 {

class UnitRegistry;

class RegisterUnitUseCase {
public:
    void execute(UnitRegistry& registry, const ConversionRule& rule) const;
};

}  // namespace uc11
