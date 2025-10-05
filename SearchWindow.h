#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QThread>

#include "SearchManager.h"

class SearchWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();

private slots:
    void startSearch();
    void onMatchFound(const QString& file, int lineNum, const QString& line);
    void onProgressUpdate(int processed, int total);
    void onSearchFinished();

private:
    QLineEdit *dirInput;
    QLineEdit *keywordInput;
    QLineEdit *threadsInput;
    QCheckBox *regexCheck;
    QCheckBox *caseCheck;
    QTableWidget *table;
    QProgressBar *progress;
    QLabel *statusLabel;

    QThread workerThread;
    SearchManager *manager;
};

#endif
