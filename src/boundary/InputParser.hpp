#pragma once

#include <string>

namespace uc11 {

struct ParsedInput {
    std::string unit;
    double value;
};

ParsedInput parseInputLine(const std::string& line);

}  // namespace uc11
