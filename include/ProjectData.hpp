#pragma once

#include <QSqlRelationalTableModel>

struct ExpenseType {
    QString name;
    int id;
};

class ProjectData final : public QObject {
    Q_OBJECT

public:
    ProjectData();

    ~ProjectData() override;

    static void addExpense(
        const QString& vendor,
        int type,
        const QString& description,
        const QDate& planedDate,
        double planedCost
    );

    static void removeExpense(int id);

    static QList<ExpenseType> getExpensesTypes();

    static QSqlTableModel* getExpensesSqlTableModel();

private:
    static QSqlDatabase db;
    static QSqlRelationalTableModel* expensesTableModel;
};
