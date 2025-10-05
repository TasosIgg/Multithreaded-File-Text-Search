#include "SearchWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

SearchWindow::SearchWindow(QWidget *parent)
    : QMainWindow(parent), manager(nullptr) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    dirInput = new QLineEdit();
    keywordInput = new QLineEdit();
    threadsInput = new QLineEdit("4");

    regexCheck = new QCheckBox("Regex");
    caseCheck = new QCheckBox("Ignore Case");
    QPushButton *searchBtn = new QPushButton("Start Search");

    table = new QTableWidget(0, 3);
    table->setHorizontalHeaderLabels(QStringList() << "File" << "Line" << "Match");
    table->horizontalHeader()->setStretchLastSection(true);

    progress = new QProgressBar();
    statusLabel = new QLabel("Idle");

    layout->addWidget(new QLabel("Directory:")); layout->addWidget(dirInput);
    layout->addWidget(new QLabel("Keyword:")); layout->addWidget(keywordInput);
    layout->addWidget(new QLabel("Threads:")); layout->addWidget(threadsInput);
    layout->addWidget(regexCheck);
    layout->addWidget(caseCheck);
    layout->addWidget(searchBtn);
    layout->addWidget(table);
    layout->addWidget(progress);
    layout->addWidget(statusLabel);

    setCentralWidget(central);

    connect(searchBtn, &QPushButton::clicked, this, &SearchWindow::startSearch);
}

SearchWindow::~SearchWindow() {
    if (manager) {
        workerThread.quit();
        workerThread.wait();
        delete manager;
    }
}

void SearchWindow::startSearch() {
    table->setRowCount(0);
    progress->setValue(0);
    statusLabel->setText("Searching...");

    // Create SearchManager and move to thread
    manager = new SearchManager(
        dirInput->text().toStdString(),
        keywordInput->text().toStdString(),
        threadsInput->text().toInt(),
        regexCheck->isChecked(),
        caseCheck->isChecked(),
        true, // colorOutput ignored in GUI
        0,
        "" // filterExt
    );
    manager->moveToThread(&workerThread);

    connect(&workerThread, &QThread::started, manager, &SearchManager::run);
    connect(manager, &SearchManager::matchFound, this, &SearchWindow::onMatchFound);
    connect(manager, &SearchManager::progressUpdate, this, &SearchWindow::onProgressUpdate);
    connect(manager, &SearchManager::searchFinished, this, &SearchWindow::onSearchFinished);

    workerThread.start();
}

void SearchWindow::onMatchFound(const QString& file, int lineNum, const QString& line) {
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(file));
    table->setItem(row, 1, new QTableWidgetItem(QString::number(lineNum)));
    table->setItem(row, 2, new QTableWidgetItem(line));
}

void SearchWindow::onProgressUpdate(int processed, int total) {
    progress->setMaximum(total);
    progress->setValue(processed);
}

void SearchWindow::onSearchFinished() {
    statusLabel->setText("Search complete.");
    workerThread.quit();
}
