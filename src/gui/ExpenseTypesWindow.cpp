#include "gui/ExpenseTypesWindow.hpp"
#include "ProjectData.hpp"
#include "gui/ExpenseTypeEditWindow.hpp"
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QVBoxLayout>
#include <QWidget>

ExpenseTypesWindow::ExpenseTypesWindow(QWidget* parent) : QMdiSubWindow(parent) {
    setupUI();
    setupConnections();
    refreshTable();
}

void ExpenseTypesWindow::setupUI() {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    tableView = new QTableView(this);
    tableView->setModel(ProjectData::getExpenseTypesTableModel());
    tableView->hideColumn(0);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    centralLayout->addWidget(tableView);

    setWidget(centralWidget);
    setWindowTitle(tr("Expense Types"));
    resize(400, 300);
}

void ExpenseTypesWindow::setupConnections() {
    connect(tableView, &QTableView::doubleClicked, this, &ExpenseTypesWindow::editType);
    connect(tableView, &QTableView::customContextMenuRequested, this, &ExpenseTypesWindow::showContextMenu);
}

void ExpenseTypesWindow::refreshTable() {
    if(auto* model = qobject_cast<QSqlTableModel*>(tableView->model())) {
        model->select();
    }
}

void ExpenseTypesWindow::editType() {
    const auto index = tableView->currentIndex();
    if(!index.isValid()) return;

    const auto id = tableView->model()->data(tableView->model()->index(index.row(), 0)).toInt();
    const auto name = tableView->model()->data(tableView->model()->index(index.row(), 1)).toString();

    auto* window = new ExpenseTypeEditWindow(id, name, this);
    
    // Center the edit window relative to the table window
    const auto tableGeometry = geometry();
    const auto editGeometry = window->geometry();
    const auto x = tableGeometry.x() + (tableGeometry.width() - editGeometry.width()) / 2;
    const auto y = tableGeometry.y() + (tableGeometry.height() - editGeometry.height()) / 2;
    window->move(x, y);
    
    window->show();
    connect(window, &QMdiSubWindow::destroyed, this, &ExpenseTypesWindow::refreshTable);
}

void ExpenseTypesWindow::deleteType() {
    const auto index = tableView->currentIndex();
    if(!index.isValid()) return;

    const auto id = tableView->model()->data(tableView->model()->index(index.row(), 0)).toInt();
    ProjectData::removeExpenseType(id);
    refreshTable();
}

void ExpenseTypesWindow::showContextMenu(const QPoint& pos) {
    const auto index = tableView->indexAt(pos);
    if(!index.isValid()) return;

    QMenu contextMenu(this);
    contextMenu.addAction(tr("Edit"), this, &ExpenseTypesWindow::editType);
    contextMenu.addAction(tr("Delete"), this, &ExpenseTypesWindow::deleteType);
    contextMenu.exec(tableView->viewport()->mapToGlobal(pos));
}
