#pragma once

#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMdiSubWindow>
#include <QPlainTextEdit>
#include <QSpinBox>

class ExpenseEditWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseEditWindow(const QModelIndex& index, QWidget* parent = nullptr);

private:
    QLineEdit* vendorEditLine;
    QLabel* vendorLabel;
    QComboBox* typeComboBox;
    QPlainTextEdit* descriptionEditBox;
    QDateEdit* planedDateEditLine;
    QDateEdit* actualDateEditLine;
    QDoubleSpinBox* planedCostEditLine;
    QDoubleSpinBox* actualCostEditLine;

    void confirmEdit(int expenseId);
};
