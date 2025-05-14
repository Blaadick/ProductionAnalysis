#include <QApplication>
#include "FileManager.hpp"
#include "gui/MainWindow.hpp"

int main(int argc, char* argv[]) {
    FileManager fileManager;
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
