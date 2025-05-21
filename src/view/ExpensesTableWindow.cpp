#include "view/ExpensesTableWindow.hpp"

#include <qdatetime.h>
#include <QHeaderView>
#include <QPainter>
#include "ProjectData.hpp"

ExpensesTableWindow::ExpensesTableWindow(QWidget* parent) : QMdiSubWindow(parent) {
    tableView = new QTableView(this);
    expensesTableModel = new ExpensesTableModel(this);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setModel(ProjectData::getExpensesSqlTableModel());
    tableView->hideColumn(0);
    tableView->sortByColumn(3, Qt::DescendingOrder);
    tableView->setItemDelegate(new ExpensesTableDelegate(tableView));
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    setWidget(tableView);
    setWindowTitle(tr("Expenses Table"));
    resize(800, 400);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(tableView, &QTableView::customContextMenuRequested, this, &ExpensesTableWindow::showContextMenu);
}

ExpensesTableWindow::ExpensesTableDelegate::ExpensesTableDelegate(QWidget* parent) : QStyledItemDelegate(parent) {}

void ExpensesTableWindow::ExpensesTableDelegate::paint(
    QPainter* painter,
    const QStyleOptionViewItem& option,
    const QModelIndex& index
) const {
    if(index.column() == 6) {
        const auto planedCost = index.sibling(index.row(), 5).data().toDouble();
        const auto actualCost = index.sibling(index.row(), 6).data().toDouble();

        if(planedCost < actualCost) {
            painter->fillRect(option.rect, QColor("#CCFFCC"));
        }

        if(planedCost > actualCost) {
            painter->fillRect(option.rect, QColor("#FFCCCC"));
        }
    }

    if(index.column() == 4) {
        const auto planedDate = index.sibling(index.row(), 3).data().toDate();
        const auto actualDate = index.sibling(index.row(), 4).data().toDate();

        if(planedDate > actualDate) {
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
    const auto* actionEdit = menu.addAction("Edit");
    const auto* actionDelete = menu.addAction("Delete");

    connect(actionDelete, &QAction::triggered, expensesTableModel, [this, index] {
        expensesTableModel->removeExpense(index.sibling(index.row(), 0).data().toInt());
    });

    menu.exec(QCursor::pos());
}
