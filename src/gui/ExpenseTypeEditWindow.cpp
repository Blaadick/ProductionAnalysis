#include "gui/ExpenseTypeEditWindow.hpp"
#include "ProjectData.hpp"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

ExpenseTypeEditWindow::ExpenseTypeEditWindow(int id, const QString& name, QWidget* parent) : QMdiSubWindow(parent), typeId(id) {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout();
    centralWidget->setLayout(centralLayout);

    auto* gridLayout = new QGridLayout();
    centralLayout->addLayout(gridLayout);

    nameEditLine = new QLineEdit(this);
    nameEditLine->setText(name);

    nameLabel = new QLabel(tr("Name"), this);
    nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    gridLayout->addWidget(nameLabel, 0, 0, 1, 1);
    gridLayout->addWidget(nameEditLine, 0, 1, 1, 1);

    auto* horizontalLayout = new QHBoxLayout();
    centralLayout->addLayout(horizontalLayout);

    auto* confirmButton = new QPushButton(tr("Confirm"), this);
    auto* cancelButton = new QPushButton(tr("Cancel"), this);

    horizontalLayout->addWidget(confirmButton);
    horizontalLayout->addWidget(cancelButton);

    setWidget(centralWidget);
    setWindowTitle(tr("Edit Expense Type"));
    resize(300, 150);

    connect(confirmButton, &QPushButton::clicked, this, &ExpenseTypeEditWindow::confirmEdit);
    connect(cancelButton, &QPushButton::clicked, this, &QMdiSubWindow::close);
}

void ExpenseTypeEditWindow::confirmEdit() {
    if(nameEditLine->text().isEmpty()) {
        nameLabel->setStyleSheet("font-weight: bold; color: red;");
        return;
    }

    ProjectData::editExpenseType(typeId, nameEditLine->text());
    ProjectData::getExpenseTypesTableModel()->select();
    close();
}
