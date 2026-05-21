#include "data/ConfigLoader.hpp"

#include "data/ConfigErrors.hpp"
#include "entity/UnitRegistry.hpp"

#include <cctype>
#include <fstream>
#include <sstream>
#include <string>

namespace uc11 {

namespace {

std::string readFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

bool parseUnitsFromContent(const std::string& content, UnitRegistry& registry,
                           std::string& errorMessage) {
    registry = UnitRegistry{};
    std::size_t pos = 0;
    while (pos < content.size()) {
        const auto nameKey = content.find("\"name\"", pos);
        if (nameKey == std::string::npos) {
            const auto yamlName = content.find("name:", pos);
            if (yamlName == std::string::npos) {
                break;
            }
            pos = yamlName + 5;
            while (pos < content.size() && std::isspace(static_cast<unsigned char>(content[pos]))) {
                ++pos;
            }
            std::string name;
            if (pos < content.size() && content[pos] == '"') {
                ++pos;
                while (pos < content.size() && content[pos] != '"') {
                    name += content[pos++];
                }
                ++pos;
            } else {
                while (pos < content.size() &&
                       (std::isalnum(static_cast<unsigned char>(content[pos])) || content[pos] == '_')) {
                    name += content[pos++];
                }
            }

            const auto factorKey = content.find("factor_to_meter", pos);
            if (factorKey == std::string::npos) {
                errorMessage = ConfigErr::kMissingFactor;
                return false;
            }
            pos = factorKey + 15;
            while (pos < content.size() && (content[pos] == ':' || content[pos] == ' ')) {
                ++pos;
            }
            try {
                std::size_t consumed = 0;
                const double factor = std::stod(content.substr(pos), &consumed);
                registry.registerUnit(name, factor);
                pos += consumed;
            } catch (...) {
                errorMessage = ConfigErr::kInvalidFactor;
                return false;
            }
            continue;
        }

        pos = nameKey + 6;
        const auto quote1 = content.find('"', pos);
        if (quote1 == std::string::npos) {
            errorMessage = ConfigErr::kMalformedName;
            return false;
        }
        const auto quote2 = content.find('"', quote1 + 1);
        if (quote2 == std::string::npos) {
            errorMessage = ConfigErr::kMalformedName;
            return false;
        }
        const std::string name = content.substr(quote1 + 1, quote2 - quote1 - 1);

        const auto factorKey = content.find("factor_to_meter", quote2);
        if (factorKey == std::string::npos) {
            errorMessage = ConfigErr::kMissingFactor;
            return false;
        }
        pos = factorKey + 15;
        const auto colon = content.find(':', pos);
        pos = (colon == std::string::npos) ? pos : colon + 1;
        while (pos < content.size() && (content[pos] == ' ' || content[pos] == '\t')) {
            ++pos;
        }
        try {
            std::size_t consumed = 0;
            const double factor = std::stod(content.substr(pos), &consumed);
            if (factor <= 0.0) {
                errorMessage = ConfigErr::kFactorMustBePositive;
                return false;
            }
            registry.registerUnit(name, factor);
            pos += consumed;
        } catch (...) {
            errorMessage = ConfigErr::kInvalidFactor;
            return false;
        }
    }

    if (registry.size() == 0) {
        errorMessage = ConfigErr::kNoUnitsDefined;
        return false;
    }
    return true;
}

}  // namespace

bool loadConfig(const std::string& path, UnitRegistry& registry, std::string& errorMessage) {
    const std::string content = readFile(path);
    if (content.empty()) {
        registry = UnitRegistry::withBuiltins();
        return true;
    }
    return parseUnitsFromContent(content, registry, errorMessage);
}

}  // namespace uc11
