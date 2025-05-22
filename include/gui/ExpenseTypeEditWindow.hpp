#pragma once

#include <QLineEdit>
#include <QLabel>
#include <QMdiSubWindow>

class ExpenseTypeEditWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ExpenseTypeEditWindow(int id, const QString& name, QWidget* parent = nullptr);

private:
    QLineEdit* nameEditLine;
    QLabel* nameLabel;
    int typeId;

    void confirmEdit();
};
