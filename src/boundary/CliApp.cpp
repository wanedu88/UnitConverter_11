#include "boundary/CliApp.hpp"

#include "boundary/ErrorMessages.hpp"
#include "boundary/InputParser.hpp"
#include "boundary/InputValidator.hpp"
#include "boundary/OutputFormatter.hpp"
#include "control/ConvertLengthUseCase.hpp"

#include <exception>
#include <iostream>
#include <string>

namespace uc11 {

int CliApp::run() {
    std::cout << "Insert value for converting (ex: meter:2.5): ";

    std::string input;
    if (!std::getline(std::cin, input)) {
        std::cerr << ErrMsg::kInputReadFailed << std::endl;
        return 1;
    }

    try {
        const ParsedInput parsed = parseInputLine(input);
        validateInput(parsed);
        const ConvertLengthResult result =
            ConvertLengthUseCase{}.execute(parsed.unit, parsed.value);
        for (const auto& row : result.conversions) {
            std::cout << formatTableLine(parsed.value, parsed.unit, row.value, row.unit)
                      << std::endl;
        }
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

}  // namespace uc11
