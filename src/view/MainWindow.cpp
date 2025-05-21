#include "view/MainWindow.hpp"

#include <QMenuBar>
#include <QToolBar>
#include <QWidget>
#include "view/ExpensesTableWindow.hpp"
#include "view/NewExpenseWindow.hpp"

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

    connect(actionNewExpense, &QAction::triggered, mdiArea, [this] {
        auto* newExpenseWindow = new NewExpenseWindow(mdiArea);
        mdiArea->addSubWindow(newExpenseWindow);
        newExpenseWindow->show();
    });

    connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(actionFullScreen, &QAction::triggered, this, [this] {
        isFullScreen() ? showNormal() : showFullScreen();
    });
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

    connect(action1, &QAction::triggered, mdiArea, [this] {
        auto* expensesTable = new ExpensesTableWindow(mdiArea);
        mdiArea->addSubWindow(expensesTable);
        expensesTable->show();
    });
}
