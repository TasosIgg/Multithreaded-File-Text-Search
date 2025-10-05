#include "FileUtils.h"
#include <fstream>
#include <cctype>

bool isBinary(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    char c;
    int nonPrintable = 0, total = 0;
    while (file.get(c) && total < 1024) {
        if (!isprint(static_cast<unsigned char>(c)) && !isspace(static_cast<unsigned char>(c)))
            nonPrintable++;
        total++;
    }
    return (total > 0 && (nonPrintable * 1.0 / total) > 0.3);
}
