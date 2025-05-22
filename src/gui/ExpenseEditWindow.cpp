#include "gui/ExpenseEditWindow.hpp"

#include <QPushButton>
#include <qsqlrecord.h>
#include <QVBoxLayout>
#include "ProjectData.hpp"

inline QVariant getData(const QModelIndex& index, const int column) {
    return index.sibling(index.row(), column).data();
}

ExpenseEditWindow::ExpenseEditWindow(const QModelIndex& index, QWidget* parent) : QMdiSubWindow(parent) {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout();
    centralWidget->setLayout(centralLayout);

    auto* gridLayout = new QGridLayout();
    centralLayout->addLayout(gridLayout);

    vendorEditLine = new QLineEdit(this);
    vendorEditLine->setText(getData(index, 1).toString());

    typeComboBox = new QComboBox(this);
    for(const auto& [name, id] : ProjectData::getExpensesTypes()) {
        typeComboBox->addItem(name, id);
    }

    descriptionEditBox = new QPlainTextEdit(this);
    descriptionEditBox->setPlainText(getData(index, 3).toString());

    planedDateEditLine = new QDateEdit(this);
    planedDateEditLine->setDate(getData(index, 4).toDate());
    planedDateEditLine->setCalendarPopup(true);
    planedDateEditLine->setDisplayFormat("yyyy.MM.dd");

    actualDateEditLine = new QDateEdit(this);
    actualDateEditLine->setDate(getData(index, 5).isNull() ? planedDateEditLine->date() : getData(index, 5).toDate());
    actualDateEditLine->setCalendarPopup(true);
    actualDateEditLine->setDisplayFormat("yyyy.MM.dd");

    auto* dateLayout = new QHBoxLayout();
    dateLayout->addWidget(planedDateEditLine);
    dateLayout->addWidget(actualDateEditLine);

    planedCostEditLine = new QDoubleSpinBox(this);
    planedCostEditLine->setMaximum(std::numeric_limits<double>::max());
    planedCostEditLine->setValue(getData(index, 6).toDouble());

    actualCostEditLine = new QDoubleSpinBox(this);
    actualCostEditLine->setMaximum(std::numeric_limits<double>::max());
    actualCostEditLine->setValue(getData(index, 7).isNull() ? planedCostEditLine->value() : getData(index, 7).toDouble());

    auto* costLayout = new QHBoxLayout();
    costLayout->addWidget(planedCostEditLine);
    costLayout->addWidget(actualCostEditLine);

    vendorLabel = new QLabel(tr("Vendor"), this);
    vendorLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto* typeLabel = new QLabel(tr("Type"), this);
    typeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto* descriptionLabel = new QLabel(tr("Description"), this);
    descriptionLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    auto* dateLabel = new QLabel(tr("Date"), this);
    dateLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto* costLabel = new QLabel(tr("Cost"), this);
    costLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    gridLayout->addWidget(vendorLabel, 0, 0, 1, 1);
    gridLayout->addWidget(vendorEditLine, 0, 1, 1, 1);
    gridLayout->addWidget(typeLabel, 1, 0, 1, 1);
    gridLayout->addWidget(typeComboBox, 1, 1, 1, 1);
    gridLayout->addWidget(descriptionLabel, 2, 0, 1, 1);
    gridLayout->addWidget(descriptionEditBox, 2, 1, 1, 1);
    gridLayout->addWidget(dateLabel, 3, 0, 1, 1);
    gridLayout->addLayout(dateLayout, 3, 1, 1, 1);
    gridLayout->addWidget(costLabel, 4, 0, 1, 1);
    gridLayout->addLayout(costLayout, 4, 1, 1, 1);

    auto* horizontalLayout = new QHBoxLayout();
    centralLayout->addLayout(horizontalLayout);

    auto* confirmButton = new QPushButton(tr("Confirm"), this);
    auto* cancelButton = new QPushButton(tr("Cancel"), this);

    horizontalLayout->addWidget(confirmButton);
    horizontalLayout->addWidget(cancelButton);

    setWidget(centralWidget);
    setWindowTitle(tr("Expense Edit"));
    resize(400, 300);

    connect(confirmButton, &QPushButton::clicked, this, [this, index] {
        confirmEdit(getData(index, 0).toInt());
    });
    connect(cancelButton, &QPushButton::clicked, this, &QMdiSubWindow::close);
}

void ExpenseEditWindow::confirmEdit(const int expenseId) {
    if(vendorEditLine->text().isEmpty()) {
        vendorLabel->setStyleSheet("font-weight: bold; color: red;");
        return;
    }

    ProjectData::editExpense(
        expenseId,
        vendorEditLine->text(),
        typeComboBox->currentData().toInt(),
        descriptionEditBox->toPlainText(),
        planedDateEditLine->date(),
        actualDateEditLine->date(),
        planedCostEditLine->value(),
        actualCostEditLine->value()
    );

    ProjectData::getExpensesTableModel()->select();
    close();
}
