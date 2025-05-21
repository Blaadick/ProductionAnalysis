#include "view/MainWindow.hpp"

#include <QMenuBar>
#include <QToolBar>
#include <QWidget>
#include "view/ExpensesTableWindow.hpp"

MainWindow::MainWindow() {
    mdiArea = new QMdiArea(this);

    setupMenuBar();
    setupToolBar();

    setCentralWidget(mdiArea);
    setContextMenuPolicy(Qt::NoContextMenu);

    show();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenuBar() {
    auto* menuBar = new QMenuBar(this);
    auto* menuFile = new QMenu(tr("&File"), menuBar);
    auto* actionNewProject = new QAction(tr("New Project"), menuFile);
    auto* actionOpenProject = new QAction(tr("Open Project"), menuFile);
    auto* actionExit = new QAction(tr("Exit"), menuFile);
    menuBar->addMenu(menuFile);
    menuFile->addAction(actionNewProject);
    menuFile->addSeparator();
    menuFile->addAction(actionOpenProject);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    auto* menuView = new QMenu(tr("&View"), menuBar);
    menuBar->addMenu(menuView);

    auto* menuWindow = new QMenu(tr("&Window"), menuBar);
    auto* actionFullScreen = new QAction(tr("Full Screen"), menuWindow);
    auto* actionCascadeWindows = new QAction(tr("Cascade Windows"), menuWindow);
    auto* actionTileWindows = new QAction(tr("Tile Windows"), menuWindow);
    auto* actionCloseAll = new QAction(tr("Close All"), menuWindow);
    menuBar->addMenu(menuWindow);
    menuWindow->addAction(actionFullScreen);
    menuWindow->addSeparator();
    menuWindow->addAction(actionCascadeWindows);
    menuWindow->addAction(actionTileWindows);
    menuWindow->addSeparator();
    menuWindow->addAction(actionCloseAll);

    actionFullScreen->setShortcut(Qt::Key_F11);

    setMenuBar(menuBar);

    connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(actionFullScreen, &QAction::triggered, this, [this] {
        if(isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
    connect(actionCascadeWindows, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);
    connect(actionTileWindows, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);
    connect(actionCloseAll, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);
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

    connect(action1, &QAction::triggered, this, [this] {
        auto* expensesTable = new ExpensesTableWindow(mdiArea);
        mdiArea->addSubWindow(expensesTable);
        expensesTable->show();
    });
}
