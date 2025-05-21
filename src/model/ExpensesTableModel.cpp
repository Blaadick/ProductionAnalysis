#include "model/ExpensesTableModel.hpp"

#include "ProjectData.hpp"

ExpensesTableModel::ExpensesTableModel(QObject* parent) : QObject(parent) {}

void ExpensesTableModel::removeExpense(const int id) {
    ProjectData::removeExpense(id);
    ProjectData::getExpensesSqlTableModel()->select();
}
