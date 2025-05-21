#pragma once

#include <QSqlTableModel>

class ProjectData final : public QObject {
public:
    ProjectData();

    ~ProjectData() override;

    static void removeExpense(int id);

    static QSqlTableModel* getExpensesSqlTableModel();

private:
    static QSqlDatabase db;
    static QSqlTableModel* expensesTableModel;
};
