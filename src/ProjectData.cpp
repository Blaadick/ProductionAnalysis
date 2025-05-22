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
    delete expenseTypesTableModel;
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

void ProjectData::editExpense(
    const int editableExpenseId,
    const QString& vendor,
    const int type,
    const QString& description,
    const QDate& planedDate,
    const QDate& actualDate,
    const double planedCost,
    const double actualCost
) {
    QSqlQuery query;
    query.prepare(R"(
        UPDATE Expenses SET
            Vendor = ?,
            Type = ?,
            Description = ?,
            PlanedDate = ?,
            ActualDate = ?,
            PlanedCost = ?,
            ActualCost = ?
        WHERE Id = ?
    )");

    query.addBindValue(vendor);
    query.addBindValue(type);
    query.addBindValue(description);
    query.addBindValue(planedDate.toString("yyyy-MM-dd"));
    query.addBindValue(actualDate.toString("yyyy-MM-dd"));
    query.addBindValue(planedCost);
    query.addBindValue(actualCost);
    query.addBindValue(editableExpenseId);
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
QSqlRelationalTableModel* ProjectData::getExpensesTableModel() {
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

void ProjectData::addExpenseType(const QString& name) {
    QSqlQuery query;
    query.prepare("INSERT INTO ExpensesTypes (Name) VALUES (?)");
    query.addBindValue(name);
    query.exec();
}

void ProjectData::editExpenseType(const int id, const QString& name) {
    QSqlQuery query;
    query.prepare("UPDATE ExpensesTypes SET Name = ? WHERE Id = ?");
    query.addBindValue(name);
    query.addBindValue(id);
    query.exec();
}

void ProjectData::removeExpenseType(const int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM ExpensesTypes WHERE Id = ?");
    query.addBindValue(id);
    query.exec();
}

QSqlTableModel* ProjectData::getExpenseTypesTableModel() {
    if(!expenseTypesTableModel) {
        expenseTypesTableModel = new QSqlTableModel(nullptr, db);
        expenseTypesTableModel->setTable("ExpensesTypes");
        expenseTypesTableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        expenseTypesTableModel->select();
    }
    return expenseTypesTableModel;
}

QSqlDatabase ProjectData::db;
QSqlRelationalTableModel* ProjectData::expensesTableModel;
QSqlTableModel* ProjectData::expenseTypesTableModel;
