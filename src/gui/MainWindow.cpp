#include "gui/MainWindow.hpp"

#include <QPushButton>
#include <QWidget>

MainWindow::MainWindow() {
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* button = new QPushButton("Hello world!", centralWidget);
    button->resize(200, 100);
}
