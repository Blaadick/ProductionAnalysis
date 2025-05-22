#pragma once

#include <QMdiSubWindow>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

class ExpensesTableWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpensesTableWindow(QWidget* parent = nullptr);

private:
    class ExpensesTableDelegate final : public QStyledItemDelegate {
    public:
        explicit ExpensesTableDelegate(QWidget* parent = nullptr);

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    };

    QTableView* tableView;
    QComboBox* typeFilter;
    QDoubleSpinBox* minCostFilter;
    QDoubleSpinBox* maxCostFilter;
    QHBoxLayout* filterLayout;

    void setupFilters();
    void applyFilters();

private slots:
    void showContextMenu(const QPoint& pos);
    void editExpense(const QModelIndex& index);
    void onFilterChanged();
};
