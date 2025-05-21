#include "gui/MainWindow.hpp"

#include <QMenuBar>
#include <QToolBar>
#include <QWidget>
#include "gui/ExpenseCreationWindow.hpp"
#include "gui/ExpensesTableWindow.hpp"

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

    auto* menuFile = menuBar->addMenu(tr("&File"));
    menuFile->addAction(tr("New Project"));
    const auto* actionNewExpense = menuFile->addAction(tr("New Expense"));
    menuFile->addSeparator();
    menuFile->addAction(tr("Open Project"));
    menuFile->addSeparator();
    const auto* actionExit = menuFile->addAction(tr("Exit"));

    menuBar->addMenu(tr("&View"));

    auto* menuWindow = menuBar->addMenu(tr("&Window"));
    auto* actionFullScreen = menuWindow->addAction(tr("Full Screen"));
    menuWindow->addSeparator();
    const auto* actionCascadeWindows = menuWindow->addAction(tr("Cascade Windows"));
    const auto* actionTileWindows = menuWindow->addAction(tr("Tile Windows"));
    menuWindow->addSeparator();
    const auto* actionCloseAll = menuWindow->addAction(tr("Close All"));

    actionFullScreen->setShortcut(Qt::Key_F11);

    setMenuBar(menuBar);

    connect(actionNewExpense, &QAction::triggered, this, &MainWindow::openExpenseCreationWindow);

    connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(actionFullScreen, &QAction::triggered, this, &MainWindow::toggleFullScreen);
    connect(actionCascadeWindows, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);
    connect(actionTileWindows, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);
    connect(actionCloseAll, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);
}

void MainWindow::setupToolBar() {
    auto* toolBar = new QToolBar(this);
    const auto* action1 = toolBar->addAction(QIcon::fromTheme("aseprite"), "Aseprite");
    toolBar->addAction(QIcon::fromTheme("firefox"), "Firefox");
    toolBar->addAction(QIcon::fromTheme("org.prismlauncher.PrismLauncher"), "PrismLauncher");
    toolBar->addAction(QIcon::fromTheme("kitty"), "Kitty");
    toolBar->setIconSize(QSize(32, 32));
    toolBar->setFloatable(false);
    addToolBar(toolBar);

    connect(action1, &QAction::triggered, this, &MainWindow::openExpensesTableWindow);
}

void MainWindow::toggleFullScreen() {
    isFullScreen() ? showNormal() : showFullScreen();
}

void MainWindow::openExpensesTableWindow() const {
    auto* expensesTable = new ExpensesTableWindow(mdiArea);
    mdiArea->addSubWindow(expensesTable);
    expensesTable->show();
}

void MainWindow::openExpenseCreationWindow() const {
    auto* expenseCreationWindow = new ExpenseCreationWindow(mdiArea);
    mdiArea->addSubWindow(expenseCreationWindow);
    expenseCreationWindow->show();
}
