#include "control/RegisterUnitUseCase.hpp"

#include "entity/UnitRegistry.hpp"

namespace uc11 {

void RegisterUnitUseCase::execute(UnitRegistry& registry, const ConversionRule& rule) const {
    registry.registerUnit(rule);
}

}  // namespace uc11
