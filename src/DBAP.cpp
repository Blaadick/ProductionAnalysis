#include "DBAP.hpp"

#include <FileManager.hpp>

DBAP::DBAP() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName((FileManager::getLocalDir() / "data.db").c_str());
    QSqlQuery query(db);

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS Products(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT,
            Price REAL
        );
    )");

    query.exec("PRAGMA foreign_keys = ON;");
}

DBAP::~DBAP() {
    db.close();
}
