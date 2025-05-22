#include "gui/ExpenseTypeCreationWindow.hpp"
#include "ProjectData.hpp"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

ExpenseTypeCreationWindow::ExpenseTypeCreationWindow(QWidget* parent) : QMdiSubWindow(parent) {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout();
    centralWidget->setLayout(centralLayout);

    auto* gridLayout = new QGridLayout();
    centralLayout->addLayout(gridLayout);

    nameEditLine = new QLineEdit(this);
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
    setWindowTitle(tr("New Expense Type"));
    resize(300, 150);

    connect(confirmButton, &QPushButton::clicked, this, &ExpenseTypeCreationWindow::confirmCreation);
    connect(cancelButton, &QPushButton::clicked, this, &QMdiSubWindow::close);
}

void ExpenseTypeCreationWindow::confirmCreation() {
    if(nameEditLine->text().isEmpty()) {
        nameLabel->setStyleSheet("font-weight: bold; color: red;");
        return;
    }

    ProjectData::addExpenseType(nameEditLine->text());
    ProjectData::getExpenseTypesTableModel()->select();
    close();
} 