#include "ProjectData.hpp"

#include <FileManager.hpp>
#include <QtSql>

ProjectData::ProjectData() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName((FileManager::getLocalDir() / "data.db").c_str());
    db.open();

    QSqlQuery query;

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS Expenses(
            Id INTEGER PRIMARY KEY AUTOINCREMENT,
            Vendor TEXT NOT NULL,
            Description TEXT,
            PlanedDate DATE NOT NULL,
            ActualDate DATE,
            PlanedCost REAL NOT NULL,
            ActualCost REAL
        );
    )");
}

ProjectData::~ProjectData() {
    db.close();
}

void ProjectData::removeExpense(const int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Expenses WHERE Id = ?;");
    query.addBindValue(id);
    query.exec();
}

QSqlTableModel* ProjectData::getExpensesSqlTableModel() {
    if(!expensesTableModel) {
        expensesTableModel = new QSqlTableModel(nullptr, db);
        expensesTableModel->setTable("Expenses");
        expensesTableModel->setHeaderData(1, Qt::Horizontal, tr("Vendor"));
        expensesTableModel->setHeaderData(2, Qt::Horizontal, tr("Description"));
        expensesTableModel->setHeaderData(3, Qt::Horizontal, tr("Planed Date"));
        expensesTableModel->setHeaderData(4, Qt::Horizontal, tr("Actual Date"));
        expensesTableModel->setHeaderData(5, Qt::Horizontal, tr("Planed Cost"));
        expensesTableModel->setHeaderData(6, Qt::Horizontal, tr("Actual Cost"));

        expensesTableModel->select();
    }

    return expensesTableModel;
}

QSqlDatabase ProjectData::db;
QSqlTableModel* ProjectData::expensesTableModel;
