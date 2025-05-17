#pragma once

#include <QSqlTableModel>

class Project final {
public:
    Project();

    ~Project();

    static QSqlTableModel* getPlanedExpensesTableModel();

private:
    static QSqlDatabase db;
    static QSqlTableModel* planedExpensesTableModel;
};
