#pragma once

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace uc11_golden {

inline constexpr const char* kCliPrompt =
    "Insert value for converting (ex: meter:2.5): ";

inline const std::vector<std::string> kScenarios = {
    "meter:2.5",
    "feet:1.0",
    "yard:1.0",
    "meter:0.0",
};

inline std::string readFile(const std::filesystem::path& path) {
    std::ifstream in(path);
    if (!in) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

inline void writeFile(const std::filesystem::path& path, const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::trunc);
    out << content;
}

inline std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

inline std::string stripPromptPrefix(std::string line) {
    const auto pos = line.find(kCliPrompt);
    if (pos != std::string::npos) {
        line.erase(0, pos + std::string(kCliPrompt).size());
    }
    return line;
}

inline bool isIdentityConversionLine(const std::string& line) {
    const auto eqPos = line.find(" = ");
    if (eqPos == std::string::npos) {
        return false;
    }
    const std::string left = line.substr(0, eqPos);
    const std::string right = line.substr(eqPos + 3);
    const auto leftSpace = left.rfind(' ');
    const auto rightSpace = right.rfind(' ');
    if (leftSpace == std::string::npos || rightSpace == std::string::npos) {
        return false;
    }
    return left.substr(leftSpace + 1) == right.substr(rightSpace + 1);
}

inline std::vector<std::string> extractConversionLines(const std::string& rawStdout) {
    std::vector<std::string> conversions;
    for (const auto& rawLine : splitLines(rawStdout)) {
        std::string line = stripPromptPrefix(rawLine);
        if (line.find(" = ") == std::string::npos) {
            continue;
        }
        if (isIdentityConversionLine(line)) {
            continue;
        }
        conversions.push_back(line);
    }
    return conversions;
}

inline std::string runCliCaptureStdout(const std::filesystem::path& exePath,
                                       const std::string& inputLine) {
    const auto tempDir = std::filesystem::temp_directory_path() / "uc11_golden";
    std::filesystem::create_directories(tempDir);
    const auto inputPath = tempDir / "input.txt";
    const auto outputPath = tempDir / "stdout.txt";

    {
        std::ofstream in(inputPath, std::ios::trunc);
        in << inputLine << '\n';
    }

    std::ostringstream cmd;
#ifdef _WIN32
    cmd << "cmd /c \"\"" << exePath.string() << "\" < \""
        << inputPath.string() << "\" > \"" << outputPath.string() << "\" 2>nul\"";
#else
    cmd << "\"" << exePath.string() << "\" < \"" << inputPath.string() << "\" > \""
        << outputPath.string() << "\" 2>/dev/null";
#endif
    const int rc = std::system(cmd.str().c_str());
    if (rc != 0) {
        return {};
    }
    return readFile(outputPath);
}

inline std::string buildGoldenDocument(const std::filesystem::path& exePath) {
    std::ostringstream doc;
    for (std::size_t i = 0; i < kScenarios.size(); ++i) {
        const auto& scenario = kScenarios[i];
        doc << '[' << scenario << "]\n";
        const auto stdoutText = runCliCaptureStdout(exePath, scenario);
        for (const auto& line : extractConversionLines(stdoutText)) {
            doc << line << '\n';
        }
        if (i + 1 < kScenarios.size()) {
            doc << "---\n";
        }
    }
    return doc.str();
}

inline std::string diffLines(const std::string& expected, const std::string& actual) {
    const auto expectedLines = splitLines(expected);
    const auto actualLines = splitLines(actual);
    const std::size_t maxLines = std::max(expectedLines.size(), actualLines.size());
    std::ostringstream diff;
    diff << "--- expected (tests/golden_master_expected.txt)\n";
    diff << "+++ actual\n";
    bool any = false;
    for (std::size_t i = 0; i < maxLines; ++i) {
        const std::string exp = i < expectedLines.size() ? expectedLines[i] : "<missing>";
        const std::string act = i < actualLines.size() ? actualLines[i] : "<missing>";
        if (exp != act) {
            diff << "@@ line " << (i + 1) << " @@\n";
            diff << "- " << exp << '\n';
            diff << "+ " << act << '\n';
            any = true;
        }
    }
    if (!any && expected == actual) {
        return {};
    }
    if (!any) {
        diff << "(content differs but line-aligned view matched)\n";
    }
    return diff.str();
}

}  // namespace uc11_golden
