#pragma once

#include <QDateEdit>
#include <QLineEdit>
#include <QSpinBox>
#include "gui/ExpensesTableWindow.hpp"

class ExpenseCreationWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseCreationWindow(QWidget* parent = nullptr);

private:
    QLineEdit* vendorEditLine;
    QPlainTextEdit* descriptionEditBox;
    QDateEdit* planedDateEditLine;
    QDoubleSpinBox* planedCostEditLine;

    void confirm();
};
