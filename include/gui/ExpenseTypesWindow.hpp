#pragma once

#include <QMdiSubWindow>
#include <QTableView>

class ExpenseTypesWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseTypesWindow(QWidget* parent = nullptr);

private:
    QTableView* tableView;

    void setupUI();

    void setupConnections();

    void refreshTable();

private slots:
    void editType();

    void deleteType();

    void showContextMenu(const QPoint& pos);
};
