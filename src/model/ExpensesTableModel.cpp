#include "model/ExpensesTableModel.hpp"

#include <QDate>
#include "ProjectData.hpp"

ExpensesTableModel::ExpensesTableModel(QObject* parent) : QObject(parent) {}

void ExpensesTableModel::removeExpense(const int id) {
    ProjectData::removeExpense(id);
    ProjectData::getExpensesSqlTableModel()->select();
}

void ExpensesTableModel::addExpense(
    const QString& vendor,
    const QString& description,
    const QDate& planedDate,
    const double planedCost
) {
    ProjectData::addExpense(vendor, description, planedDate, planedCost);
    ProjectData::getExpensesSqlTableModel()->select();
}
