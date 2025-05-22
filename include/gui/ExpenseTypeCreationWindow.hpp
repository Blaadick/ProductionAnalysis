#pragma once

#include <QLineEdit>
#include <QLabel>
#include <QMdiSubWindow>

class ExpenseTypeCreationWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseTypeCreationWindow(QWidget* parent = nullptr);

private:
    QLineEdit* nameEditLine;
    QLabel* nameLabel;

    void confirmCreation();
}; 