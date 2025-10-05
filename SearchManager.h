#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QObject>
#include <string>

enum LogLevel {
    QUIET = 0,
    NORMAL = 1,
    VERBOSE = 2
};


class SearchManager : public QObject {
    Q_OBJECT

public:
    SearchManager(const std::string& rootDir, const std::string& keyword,
                  int numThreads, bool useRegex, bool caseInsensitive,
                  bool colorOutput, int logLevel,
                  const std::string& filterExt = "");

public slots:
    void run();

signals:
    void matchFound(QString file, int lineNum, QString line);
    void progressUpdate(int processed, int total);
    void searchFinished();

private:
    std::string rootDir, keyword, filterExt;
    int numThreads;
    bool useRegex, caseInsensitive, colorOutput;
    int logLevel;
};

#endif
