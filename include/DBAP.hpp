#pragma once

#include <QCoreApplication>
#include <QtSql>

class DBAP final {
public:
    DBAP();

    ~DBAP();
private:
    QSqlDatabase db;
};
