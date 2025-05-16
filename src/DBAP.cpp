#include "DBAP.hpp"

#include <FileManager.hpp>

DBAP::DBAP() {
    const auto firstCreation = !exists(FileManager::getLocalDir() / "data.db");
    QSqlQuery query;

    db.setDatabaseName((FileManager::getLocalDir() / "data.db").c_str());
    db.open();

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS ExpensesTypes(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL
        );
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS PlanedExpenses(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Description TEXT,
            Type INTEGER NOT NULL,
            ExpenseTime DATETIME NOT NULL,
            Cost REAL NOT NULL,
            FOREIGN KEY (RelatedProject) REFERENCES Projects(Id),
            FOREIGN KEY (Type) REFERENCES ExpensesTypes(Id)
        );
    )");

    query.exec("PRAGMA foreign_keys = ON;");

    if(firstCreation) {
        //TODO Add base expense types
    }
}

DBAP::~DBAP() {
    db.close();
}
