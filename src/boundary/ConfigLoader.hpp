#pragma once

#include <string>

namespace uc11 {

class UnitRegistry;

bool loadConfig(const std::string& path, UnitRegistry& registry, std::string& errorMessage);

}  // namespace uc11
