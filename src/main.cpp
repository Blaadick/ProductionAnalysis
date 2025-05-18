#include <QApplication>

#include <QLocale>
#include <QTranslator>
#include "FileManager.hpp"
#include "Project.hpp"
#include "view/MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);
    FileManager fileManager;
    Project project;

    QTranslator translator;
    if(translator.load(":/i18n/ProductionAnalysis_ru_RU")) {
        QApplication::installTranslator(&translator);
    }

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
