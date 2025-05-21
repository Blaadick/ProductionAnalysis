#include "gui/ExpenseCreationWindow.hpp"

#include <QPushButton>
#include <QVBoxLayout>
#include "ProjectData.hpp"

ExpenseCreationWindow::ExpenseCreationWindow(QWidget* parent) : QMdiSubWindow(parent) {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout();
    centralWidget->setLayout(centralLayout);

    auto* gridLayout = new QGridLayout();
    centralLayout->addLayout(gridLayout);

    vendorEditLine = new QLineEdit(this);
    descriptionEditBox = new QPlainTextEdit(this);

    planedDateEditLine = new QDateEdit(this);
    planedDateEditLine->setDate(QDate::currentDate());
    planedDateEditLine->setCalendarPopup(true);
    planedDateEditLine->setDisplayFormat("yyyy.MM.dd");

    planedCostEditLine = new QDoubleSpinBox(this);
    planedCostEditLine->setMaximum(std::numeric_limits<double>::max());

    vendorLabel = new QLabel(tr("Vendor"), this);
    vendorLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto* descriptionLabel = new QLabel(tr("Description"), this);
    descriptionLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    auto* planedDateLabel = new QLabel(tr("Planed Date"), this);
    planedDateLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto* planedCostLabel = new QLabel(tr("Planed Cost"), this);
    planedCostLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    gridLayout->addWidget(vendorLabel, 0, 0, 1, 1);
    gridLayout->addWidget(vendorEditLine, 0, 1, 1, 1);
    gridLayout->addWidget(descriptionLabel, 1, 0, 1, 1);
    gridLayout->addWidget(descriptionEditBox, 1, 1, 1, 1);
    gridLayout->addWidget(planedDateLabel, 2, 0, 1, 1);
    gridLayout->addWidget(planedDateEditLine, 2, 1, 1, 1);
    gridLayout->addWidget(planedCostLabel, 4, 0, 1, 1);
    gridLayout->addWidget(planedCostEditLine, 4, 1, 1, 1);

    auto* horizontalLayout = new QHBoxLayout();
    centralLayout->addLayout(horizontalLayout);

    auto* confirmButton = new QPushButton(tr("Confirm"), this);
    auto* cancelButton = new QPushButton(tr("Cancel"), this);

    horizontalLayout->addWidget(confirmButton);
    horizontalLayout->addWidget(cancelButton);

    setWidget(centralWidget);
    setWindowTitle(tr("Create Expense"));
    resize(400, 300);

    connect(confirmButton, &QPushButton::clicked, this, &ExpenseCreationWindow::confirmCreation);
    connect(cancelButton, &QPushButton::clicked, this, &QMdiSubWindow::close);
}

void ExpenseCreationWindow::confirmCreation() {
    if(vendorEditLine->text().isEmpty()) {
        vendorLabel->setStyleSheet("font-weight: bold; color: red;");
        return;
    }

    ProjectData::addExpense(
        vendorEditLine->text(),
        descriptionEditBox->toPlainText(),
        planedDateEditLine->date(),
        planedCostEditLine->value()
    );

    ProjectData::getExpensesSqlTableModel()->select();
    close();
}
