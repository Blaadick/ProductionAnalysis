#include <QApplication>
#include <QTranslator>
#include "FileManager.hpp"
#include "ProjectData.hpp"
#include "gui/MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);

    QTranslator translator;
    if(translator.load(":/i18n/ProductionAnalysis_en_US")) {
        QApplication::installTranslator(&translator);
    }

    MainWindow mainWindow;
    FileManager fileManager;
    ProjectData projectData;

    return QApplication::exec();
}
