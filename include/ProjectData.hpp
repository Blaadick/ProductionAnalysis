#pragma once

#include <QSqlTableModel>

class ProjectData final : public QObject {
public:
    ProjectData();

    ~ProjectData() override;

    static QSqlTableModel* getExpensesSqlTableModel();

    static void addExpense(
        const QString& vendor,
        const QString& description,
        const QDate& planedDate,
        double planedCost
    );

    static void removeExpense(int id);

private:
    static QSqlDatabase db;
    static QSqlTableModel* expensesTableModel;
};
