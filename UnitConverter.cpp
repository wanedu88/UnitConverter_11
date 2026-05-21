#include "boundary/InputParser.hpp"
#include "boundary/InputValidator.hpp"
#include "boundary/OutputFormatter.hpp"
#include "control/ConvertLengthUseCase.hpp"

#include <iostream>
#include <string>

int main() {
    std::cout << "Insert value for converting (ex: meter:2.5): ";

    std::string input;
    if (!std::getline(std::cin, input)) {
        return 1;
    }

    try {
        const uc11::ParsedInput parsed = uc11::parseInputLine(input);
        uc11::validateInput(parsed);
        const uc11::ConvertLengthResult result =
            uc11::ConvertLengthUseCase{}.execute(parsed.unit, parsed.value);
        for (const auto& row : result.conversions) {
            std::cout << uc11::formatTableLine(parsed.value, parsed.unit, row.value, row.unit)
                      << std::endl;
        }
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
