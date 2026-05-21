#include "control/ConvertLengthUseCase.hpp"

#include "entity/UnitRegistry.hpp"

namespace uc11 {

ConvertLengthResult ConvertLengthUseCase::execute(const std::string& fromUnit,
                                                  double value) const {
    const UnitRegistry registry = UnitRegistry::withBuiltins();
    ConvertLengthResult result;
    result.fromUnit = fromUnit;
    result.inputValue = value;
    result.conversions = convertAll(registry, fromUnit, value);
    return result;
}

}  // namespace uc11
