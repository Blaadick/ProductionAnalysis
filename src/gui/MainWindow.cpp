#include "gui/MainWindow.hpp"
#include "gui/MainWindow.hpp"

#include <QMenuBar>
#include <QPushButton>
#include <QWidget>

MainWindow::MainWindow() {
    auto* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* button = new QPushButton("Hello world!", centralWidget);
}
