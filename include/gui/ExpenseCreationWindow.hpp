#pragma once

#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMdiSubWindow>
#include <QPlainTextEdit>
#include <QSpinBox>

class ExpenseCreationWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseCreationWindow(QWidget* parent = nullptr);

private:
    QLineEdit* vendorEditLine;
    QLabel* vendorLabel;
    QComboBox* typeComboBox;
    QPlainTextEdit* descriptionEditBox;
    QDateEdit* planedDateEditLine;
    QDoubleSpinBox* planedCostEditLine;

    void confirmCreation();
};
