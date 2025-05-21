#include "ProjectData.hpp"

#include <FileManager.hpp>
#include <QtSql>

ProjectData::ProjectData() {
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
        CREATE TABLE IF NOT EXISTS Expenses(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Vendor TEXT NOT NULL,
            Type INTEGER NOT NULL,
            Description TEXT,
            PlanedDate DATE NOT NULL,
            ActualDate DATE,
            PlanedCost REAL NOT NULL,
            ActualCost REAL,
            FOREIGN KEY (Type) REFERENCES ExpensesTypes(Id)
        );
    )");

    query.exec("PRAGMA foreign_keys = ON;");
}

ProjectData::~ProjectData() {
    delete expensesTableModel;
    db.close();
}

void ProjectData::addExpense(
    const QString& vendor,
    const int type,
    const QString& description,
    const QDate& planedDate,
    const double planedCost
) {
    QSqlQuery query;

    query.prepare("INSERT INTO Expenses (Vendor, Type, Description, PlanedDate, PlanedCost) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(vendor);
    query.addBindValue(type);
    query.addBindValue(description);
    query.addBindValue(planedDate.toString("yyyy-MM-dd"));
    query.addBindValue(planedCost);
    query.exec();
}

void ProjectData::removeExpense(const int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Expenses WHERE Id = ?;");
    query.addBindValue(id);
    query.exec();
}

QList<ExpenseType> ProjectData::getExpensesTypes() {
    QSqlQuery query;
    QList<ExpenseType> types;

    query.exec("SELECT Name, Id FROM ExpensesTypes ORDER BY Name ASC");

    while(query.next()) {
        types += {query.value(0).toString(), query.value(1).toInt()};
    }

    return types;
}

/**
 * Better use QT signals, but whatever.
 */
QSqlTableModel* ProjectData::getExpensesSqlTableModel() {
    if(!expensesTableModel) {
        expensesTableModel = new QSqlRelationalTableModel(nullptr, db);
        expensesTableModel->setTable("Expenses");
        expensesTableModel->setHeaderData(1, Qt::Horizontal, tr("Vendor"));
        expensesTableModel->setHeaderData(2, Qt::Horizontal, tr("Type"));
        expensesTableModel->setHeaderData(3, Qt::Horizontal, tr("Description"));
        expensesTableModel->setHeaderData(4, Qt::Horizontal, tr("Planed Date"));
        expensesTableModel->setHeaderData(5, Qt::Horizontal, tr("Actual Date"));
        expensesTableModel->setHeaderData(6, Qt::Horizontal, tr("Planed Cost"));
        expensesTableModel->setHeaderData(7, Qt::Horizontal, tr("Actual Cost"));
        expensesTableModel->setRelation(2, QSqlRelation("ExpensesTypes", "Id", "Name"));

        expensesTableModel->select();
    }

    return expensesTableModel;
}

QSqlDatabase ProjectData::db;
QSqlRelationalTableModel* ProjectData::expensesTableModel;
