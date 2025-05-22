#include "gui/ExpensesTableWindow.hpp"

#include <qdatetime.h>
#include <QHeaderView>
#include <QMenu>
#include <QPainter>
#include "ProjectData.hpp"
#include "gui/ExpenseEditWindow.hpp"
#include "gui/MainWindow.hpp"

ExpensesTableWindow::ExpensesTableWindow(QWidget* parent) : QMdiSubWindow(parent) {
    tableView = new QTableView(this);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setModel(ProjectData::getExpensesSqlTableModel());
    tableView->hideColumn(0);
    tableView->sortByColumn(4, Qt::DescendingOrder);
    tableView->setItemDelegate(new ExpensesTableDelegate(tableView));
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    setWidget(tableView);
    setWindowTitle(tr("Expenses Table"));
    resize(800, 400);

    connect(tableView, &QTableView::customContextMenuRequested, this, &ExpensesTableWindow::showContextMenu);
}

ExpensesTableWindow::ExpensesTableDelegate::ExpensesTableDelegate(QWidget* parent) : QStyledItemDelegate(parent) {}

void ExpensesTableWindow::ExpensesTableDelegate::paint(
    QPainter* painter,
    const QStyleOptionViewItem& option,
    const QModelIndex& index
) const {
    if(index.column() == 7) {
        const auto planedCost = index.sibling(index.row(), 6).data().toDouble();
        const auto actualCostData = index.sibling(index.row(), 7).data();
        const auto actualCost = actualCostData.toDouble();

        if(!actualCostData.isNull() && planedCost > actualCost) {
            painter->fillRect(option.rect, QColor("#CCFFCC"));
        }

        if(planedCost < actualCost) {
            painter->fillRect(option.rect, QColor("#FFCCCC"));
        }
    }

    if(index.column() == 5) {
        const auto planedDate = index.sibling(index.row(), 4).data().toDate();
        const auto actualDate = index.sibling(index.row(), 5).data().toDate();

        if(actualDate.isValid() && planedDate > actualDate) {
            painter->fillRect(option.rect, QColor("#CCFFCC"));
        }

        if(planedDate < actualDate) {
            painter->fillRect(option.rect, QColor("#FFCCCC"));
        }
    }

    QStyledItemDelegate::paint(painter, option, index);
}

void ExpensesTableWindow::showContextMenu(const QPoint& pos) const {
    const auto index = tableView->indexAt(pos);
    if(!index.isValid()) return;

    QMenu menu(tableView);
    menu.addAction("Details");
    const auto* actionEdit = menu.addAction("Edit");
    menu.addSeparator();
    const auto* actionDelete = menu.addAction("Delete");

    connect(actionEdit, &QAction::triggered, this, [index] {
        MainWindow::openExpenseEditWindow(index);
    });
    connect(actionDelete, &QAction::triggered, this, [this, index] {
        ProjectData::removeExpense(index.sibling(index.row(), 0).data().toInt());
        ProjectData::getExpensesSqlTableModel()->select();
    });

    menu.exec(QCursor::pos());
}
