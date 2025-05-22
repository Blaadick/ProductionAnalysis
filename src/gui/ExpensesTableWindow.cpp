#include "gui/ExpensesTableWindow.hpp"

#include <qdatetime.h>
#include <QHeaderView>
#include <QMenu>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include "ProjectData.hpp"
#include "gui/ExpenseEditWindow.hpp"
#include "gui/MainWindow.hpp"

ExpensesTableWindow::ExpensesTableWindow(QWidget* parent) : QMdiSubWindow(parent) {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setupFilters();
    centralLayout->addLayout(filterLayout);

    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setModel(ProjectData::getExpensesTableModel());
    tableView->hideColumn(0);
    tableView->sortByColumn(4, Qt::DescendingOrder);
    tableView->setItemDelegate(new ExpensesTableDelegate(tableView));
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    centralLayout->addWidget(tableView);

    setWidget(centralWidget);
    setWindowTitle(tr("Expenses Table"));
    resize(800, 400);

    connect(tableView, &QTableView::customContextMenuRequested, this, &ExpensesTableWindow::showContextMenu);
    connect(tableView, &QTableView::doubleClicked, this, &ExpensesTableWindow::editExpense);
}

void ExpensesTableWindow::setupFilters() {
    filterLayout = new QHBoxLayout();
    
    // Type filter
    auto* typeLabel = new QLabel(tr("Type:"), this);
    typeFilter = new QComboBox(this);
    typeFilter->addItem(tr("All"), -1);
    for(const auto& [name, id] : ProjectData::getExpensesTypes()) {
        typeFilter->addItem(name, id);
    }
    
    // Cost range filters
    auto* costLabel = new QLabel(tr("Cost range:"), this);
    minCostFilter = new QDoubleSpinBox(this);
    maxCostFilter = new QDoubleSpinBox(this);
    minCostFilter->setMaximum(std::numeric_limits<double>::max());
    maxCostFilter->setMaximum(std::numeric_limits<double>::max());
    
    filterLayout->addWidget(typeLabel);
    filterLayout->addWidget(typeFilter);
    filterLayout->addWidget(costLabel);
    filterLayout->addWidget(minCostFilter);
    filterLayout->addWidget(new QLabel("-", this));
    filterLayout->addWidget(maxCostFilter);
    filterLayout->addStretch();
    
    connect(typeFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ExpensesTableWindow::onFilterChanged);
    connect(minCostFilter, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ExpensesTableWindow::onFilterChanged);
    connect(maxCostFilter, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ExpensesTableWindow::onFilterChanged);
}

void ExpensesTableWindow::applyFilters() {
    auto* model = ProjectData::getExpensesTableModel();
    QString filter;
    
    // Type filter
    const auto typeId = typeFilter->currentData().toInt();
    if(typeId != -1) {
        filter += QString("Type = %1").arg(typeId);
    }
    
    // Cost range filter
    const auto minCost = minCostFilter->value();
    const auto maxCost = maxCostFilter->value();
    
    if(minCost > 0 || (maxCost > 0 && maxCost < std::numeric_limits<double>::max())) {
        if(!filter.isEmpty()) {
            filter += " AND ";
        }
        
        if(minCost > 0 && maxCost > 0 && maxCost < std::numeric_limits<double>::max()) {
            filter += QString("(PlanedCost >= %1 AND PlanedCost <= %2)").arg(minCost).arg(maxCost);
        } else if(minCost > 0) {
            filter += QString("PlanedCost >= %1").arg(minCost);
        } else if(maxCost > 0) {
            filter += QString("PlanedCost <= %1").arg(maxCost);
        }
    }
    
    model->setFilter(filter);
    model->select();
}

void ExpensesTableWindow::onFilterChanged() {
    applyFilters();
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

void ExpensesTableWindow::showContextMenu(const QPoint& pos) {
    const auto index = tableView->indexAt(pos);
    if(!index.isValid()) return;

    QMenu menu(tableView);
    menu.addAction("Details");
    const auto* actionEdit = menu.addAction("Edit");
    menu.addSeparator();
    const auto* actionDelete = menu.addAction("Delete");

    connect(actionEdit, &QAction::triggered, this, [this, index] {
        editExpense(index);
    });
    connect(actionDelete, &QAction::triggered, this, [this, index] {
        ProjectData::removeExpense(index.sibling(index.row(), 0).data().toInt());
        ProjectData::getExpensesTableModel()->select();
    });

    menu.exec(QCursor::pos());
}

void ExpensesTableWindow::editExpense(const QModelIndex& index) {
    if(!index.isValid()) return;

    auto* window = new ExpenseEditWindow(index, this);
    
    // Center the edit window relative to the table window
    const auto tableGeometry = geometry();
    const auto editGeometry = window->geometry();
    const auto x = tableGeometry.x() + (tableGeometry.width() - editGeometry.width()) / 2;
    const auto y = tableGeometry.y() + (tableGeometry.height() - editGeometry.height()) / 2;
    window->move(x, y);
    
    window->show();
    connect(window, &QMdiSubWindow::destroyed, this, [this] {
        ProjectData::getExpensesTableModel()->select();
    });
}
