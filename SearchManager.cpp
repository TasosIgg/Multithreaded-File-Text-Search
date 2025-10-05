#include "SearchManager.h"
#include "ThreadPool.h"
#include "SearchWorker.h"
#include "FileUtils.h"

#include <filesystem>
#include <atomic>
#include <iostream>
#include <QString>

namespace fs = std::filesystem;

SearchManager::SearchManager(const std::string& rootDir, const std::string& keyword,
                             int numThreads, bool useRegex, bool caseInsensitive,
                             bool colorOutput, int logLevel,
                             const std::string& filterExt)
    : rootDir(rootDir), keyword(keyword), numThreads(numThreads),
      useRegex(useRegex), caseInsensitive(caseInsensitive),
      colorOutput(colorOutput), logLevel(logLevel), filterExt(filterExt) {}

void SearchManager::run() {
    ThreadPool pool(numThreads);
    std::atomic<int> totalFiles{0};
    std::atomic<int> processed{0};

    // Count files first
    for (auto& entry : fs::recursive_directory_iterator(rootDir)) {
        if (entry.is_regular_file()) {
            if (!filterExt.empty() && entry.path().extension() != filterExt) continue;
            totalFiles++;
        }
    }

    // Traverse again and enqueue tasks
    for (auto& entry : fs::recursive_directory_iterator(rootDir)) {
        if (!entry.is_regular_file()) continue;
        if (!filterExt.empty() && entry.path().extension() != filterExt) continue;

        std::string filePath = entry.path().string();

        pool.enqueue([&, filePath]() {
            // Instead of printing directly, capture matches
            auto matches = searchFileCollect(filePath, keyword, useRegex, caseInsensitive);

            for (auto& [lineNum, lineText] : matches) {
                emit matchFound(QString::fromStdString(filePath),
                                lineNum,
                                QString::fromStdString(lineText));
            }

            processed++;
            emit progressUpdate(processed, totalFiles);
        });
    }

    pool.shutdown();

    emit searchFinished();
}
