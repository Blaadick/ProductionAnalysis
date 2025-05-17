#include "Project.hpp"

#include <FileManager.hpp>
#include <QtSql>

Project::Project() {
    const auto isFirstCreation = !exists(FileManager::getLocalDir() / "data.db");
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName((FileManager::getLocalDir() / "data.db").c_str());
    db.open();

    QSqlQuery query;

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
            FOREIGN KEY (Type) REFERENCES ExpensesTypes(Id)
        );
    )");

    query.exec("PRAGMA foreign_keys = ON;");

    if(isFirstCreation) {
        //TODO Add base expense types
    }
}

Project::~Project() {
    db.close();
}

QSqlTableModel* Project::getPlanedExpensesTableModel() {
    if(!planedExpensesTableModel) {
        planedExpensesTableModel = new QSqlTableModel(nullptr, db);
        planedExpensesTableModel->setTable("PlanedExpenses");
        planedExpensesTableModel->select();
    }

    return planedExpensesTableModel;
}

QSqlDatabase Project::db;
QSqlTableModel* Project::planedExpensesTableModel;
