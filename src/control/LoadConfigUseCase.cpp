#include "control/LoadConfigUseCase.hpp"

#include "data/ConfigLoader.hpp"

namespace uc11 {

LoadConfigResult LoadConfigUseCase::execute(const std::string& path) const {
    LoadConfigResult result;
    result.ok = loadConfig(path, result.registry, result.errorMessage);
    return result;
}

}  // namespace uc11
