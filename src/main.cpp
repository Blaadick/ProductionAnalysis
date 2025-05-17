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
    for(const QString& locale : QLocale::system().uiLanguages()) {
        const QString baseName = "untitled_" + QLocale(locale).name();
        if(translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
