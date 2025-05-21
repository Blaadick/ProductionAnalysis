#pragma once

#include <QMdiSubWindow>
#include <QStyledItemDelegate>
#include <QTableView>
#include "gui/MainWindow.hpp"

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

private slots:
    void showContextMenu(const QPoint& pos) const;
};
