#pragma once

#include <QSqlTableModel>

class ExpensesTableModel final : public QObject {
    Q_OBJECT

public:
    explicit ExpensesTableModel(QObject* parent = nullptr);

public slots:
    void removeExpense(int id);
};
