#include "SearchWorker.h"
#include "FileUtils.h"
#include <fstream>
#include <regex>
#include <algorithm>

std::vector<std::pair<int, std::string>>
searchFileCollect(const std::string& filePath,
                  const std::string& keyword,
                  bool useRegex,
                  bool caseInsensitive) {
    std::vector<std::pair<int, std::string>> results;

    if (isBinary(filePath)) return results;

    std::ifstream file(filePath);
    if (!file.is_open()) return results;

    std::regex pattern;
    if (useRegex) {
        auto flags = std::regex::ECMAScript;
        if (caseInsensitive) flags |= std::regex::icase;
        try {
            pattern = std::regex(keyword, flags);
        } catch (std::regex_error&) {
            return results;
        }
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        bool match = false;

        if (useRegex) {
            if (std::regex_search(line, pattern)) match = true;
        } else {
            std::string text = line;
            std::string key = keyword;

            if (caseInsensitive) {
                std::transform(text.begin(), text.end(), text.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c) { return std::tolower(c); });
            }
            if (text.find(key) != std::string::npos) match = true;
        }

        if (match) {
            results.push_back({lineNum, line});
        }
    }
    return results;
}
