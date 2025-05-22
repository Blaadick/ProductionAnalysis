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

    static void addExpenseType(const QString& name);

    static void addExpense(
        const QString& vendor,
        int type,
        const QString& description,
        const QDate& planedDate,
        double planedCost
    );

    static void editExpenseType(int id, const QString& name);

    static void editExpense(
        int editableExpenseId,
        const QString& vendor,
        int type,
        const QString& description,
        const QDate& planedDate,
        const QDate& actualDate,
        double planedCost,
        double actualCost
    );

    static void removeExpenseType(int id);

    static void removeExpense(int id);

    static QList<ExpenseType> getExpensesTypes();

    static QSqlTableModel* getExpenseTypesTableModel();

    static QSqlRelationalTableModel* getExpensesTableModel();

private:
    static QSqlDatabase db;
    static QSqlRelationalTableModel* expensesTableModel;
    static QSqlTableModel* expenseTypesTableModel;
};
