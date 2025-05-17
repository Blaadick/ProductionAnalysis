#include "view/MainWindow.hpp"

#include <QMdiSubWindow>
#include <QMenuBar>
#include <QTableView>
#include <QToolBar>
#include <QWidget>
#include "Project.hpp"
#include "model/CreationModel.hpp"

MainWindow::MainWindow() {
    creationModel = new CreationModel();

    setupMenuBar();
    setupToolBar();

    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
}

MainWindow::~MainWindow() {
    delete creationModel;
}

void MainWindow::setupMenuBar() {
    auto* menuBar = new QMenuBar(this);
    auto* menuFile = new QMenu(tr("File"), menuBar);
    auto* menuNew = new QMenu(tr("New"), menuFile);
    auto* actionProject = new QAction(tr("Project"), menuNew);
    auto* actionRow = new QAction(tr("Row"), menuNew);
    auto* actionExit = new QAction(tr("Exit"), menuFile);
    auto* menuWindow = new QMenu(tr("View"), menuBar);
    menuBar->addMenu(menuFile);
    menuFile->addMenu(menuNew);
    menuNew->addAction(actionProject);
    menuNew->addSeparator();
    menuNew->addAction(actionRow);
    menuFile->addAction(actionExit);
    menuBar->addMenu(menuWindow);
    setMenuBar(menuBar);

    connect(actionProject, &QAction::triggered, creationModel, &CreationModel::increment);
    connect(creationModel, &CreationModel::incremented, actionRow,
        [actionRow](const int newCount) {
            actionRow->setText(QString::number(newCount));
        }
    );
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::newDocument() const {
    auto* tableView = new QTableView();
    tableView->setModel(Project::getPlanedExpensesTableModel());
    tableView->hideColumn(0);

    QMdiSubWindow* subWindow = mdiArea->addSubWindow(tableView);
    subWindow->setWindowTitle(tr("Document"));
    subWindow->setWindowIcon(style()->standardIcon(QStyle::SP_FileIcon));
    subWindow->show();
}

void MainWindow::setupToolBar() {
    auto* toolBar = new QToolBar(this);
    auto* action1 = new QAction(QIcon::fromTheme("aseprite"), "Aseprite", toolBar);
    auto* action2 = new QAction(QIcon::fromTheme("firefox"), "Firefox", toolBar);
    auto* action3 = new QAction(QIcon::fromTheme("org.prismlauncher.PrismLauncher"), "PrismLauncher", toolBar);
    auto* action4 = new QAction(QIcon::fromTheme("kitty"), "Kitty", toolBar);
    toolBar->setIconSize(QSize(32, 32));
    toolBar->setFloatable(false);
    toolBar->addAction(action1);
    toolBar->addAction(action2);
    toolBar->addAction(action3);
    toolBar->addAction(action4);
    addToolBar(toolBar);

    connect(action1, &QAction::triggered, this, &MainWindow::newDocument);
}
