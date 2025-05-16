#include <QApplication>

#include "DBAP.hpp"
#include "FileManager.hpp"
#include "gui/MainWindow.hpp"

int main(int argc, char* argv[]) {
    FileManager fileManager;
    QApplication a(argc, argv);
    DBAP dbap;

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
