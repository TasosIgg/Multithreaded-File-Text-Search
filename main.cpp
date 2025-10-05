#include "SearchWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SearchWindow w;
    w.show();
    return app.exec();
}
