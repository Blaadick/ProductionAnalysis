#pragma once

#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMdiSubWindow>
#include <QSpinBox>

class ExpenseCreationWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseCreationWindow(QWidget* parent = nullptr);

private:
    QLineEdit* vendorEditLine;
    QLabel* vendorLabel;
    QPlainTextEdit* descriptionEditBox;
    QDateEdit* planedDateEditLine;
    QDoubleSpinBox* planedCostEditLine;

    void confirmCreation();
};
