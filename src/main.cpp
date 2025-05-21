#include <QApplication>

#include <QLocale>
#include <qsettings.h>
#include <QTranslator>
#include "FileManager.hpp"
#include "ProjectData.hpp"
#include "view/MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);
    // QSettings settings("ProductionAnalysis");

    QTranslator translator;
    if(translator.load(":/i18n/ProductionAnalysis_en_US")) {
        QApplication::installTranslator(&translator);
    }

    MainWindow mainWindow;
    FileManager fileManager;
    ProjectData project;

    return QApplication::exec();
}
