#pragma once

#include <QSqlTableModel>

class ExpensesTableModel final : public QObject {
    Q_OBJECT

public:
    explicit ExpensesTableModel(QObject* parent = nullptr);

public slots:
    void removeExpense(int id);

    void addExpense(
        const QString& vendor,
        const QString& description,
        const QDate& planedDate,
        double planedCost
    );
};
