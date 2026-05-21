#pragma once

#include "entity/UnitRegistry.hpp"

#include <string>

namespace uc11 {

struct LoadConfigResult {
    UnitRegistry registry;
    bool ok{false};
    std::string errorMessage;
};

class LoadConfigUseCase {
public:
    LoadConfigResult execute(const std::string& path) const;
};

}  // namespace uc11
