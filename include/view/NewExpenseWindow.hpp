#pragma once

#include <QDateEdit>
#include <QLineEdit>
#include <QSpinBox>
#include "view/ExpensesTableWindow.hpp"

class NewExpenseWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit NewExpenseWindow(QWidget* parent = nullptr);

private:
    ExpensesTableModel* expensesTableModel;
    QLineEdit* vendorEditLine;
    QPlainTextEdit* descriptionEditBox;
    QDateEdit* planedDateEditLine;
    QDoubleSpinBox* planedCostEditLine;

    void confirm();
};
