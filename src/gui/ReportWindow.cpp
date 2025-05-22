#include "gui/ReportWindow.hpp"
#include "ProjectData.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextDocumentWriter>

ReportWindow::ReportWindow(QWidget* parent) : QMdiSubWindow(parent) {
    setupUI();
    setupConnections();
}

void ReportWindow::setupUI() {
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(10, 10, 10, 10);

    // Date range selection
    auto* dateLayout = new QHBoxLayout();
    auto* startDateLabel = new QLabel(tr("Start Date:"), this);
    startDateEdit = new QDateEdit(this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("yyyy.MM.dd");
    startDateEdit->setDate(QDate::currentDate().addMonths(-1));

    auto* endDateLabel = new QLabel(tr("End Date:"), this);
    endDateEdit = new QDateEdit(this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("yyyy.MM.dd");
    endDateEdit->setDate(QDate::currentDate());

    dateLayout->addWidget(startDateLabel);
    dateLayout->addWidget(startDateEdit);
    dateLayout->addWidget(endDateLabel);
    dateLayout->addWidget(endDateEdit);
    dateLayout->addStretch();

    // Type filter
    auto* typeLayout = new QHBoxLayout();
    auto* typeLabel = new QLabel(tr("Type:"), this);
    typeFilter = new QComboBox(this);
    typeFilter->addItem(tr("All"), -1);
    for(const auto& [name, id] : ProjectData::getExpensesTypes()) {
        typeFilter->addItem(name, id);
    }

    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeFilter);
    typeLayout->addStretch();

    // Buttons
    auto* buttonLayout = new QHBoxLayout();
    generateButton = new QPushButton(tr("Generate Report"), this);
    exportButton = new QPushButton(tr("Export to ODF"), this);
    exportButton->setEnabled(false);

    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();

    // Report table
    reportTable = new QTableView(this);
    reportTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    reportTable->setSelectionMode(QAbstractItemView::SingleSelection);
    reportTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    reportTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    centralLayout->addLayout(dateLayout);
    centralLayout->addLayout(typeLayout);
    centralLayout->addLayout(buttonLayout);
    centralLayout->addWidget(reportTable);

    setWidget(centralWidget);
    setWindowTitle(tr("Production Cost Report"));
    resize(800, 600);
}

void ReportWindow::setupConnections() {
    connect(generateButton, &QPushButton::clicked, this, &ReportWindow::generateReport);
    connect(exportButton, &QPushButton::clicked, this, &ReportWindow::exportToODF);
}

void ReportWindow::generateReport() {
    QSqlQuery query;
    QString queryStr = "SELECT e.Vendor, et.Name as Type, "
        "e.ActualDate, e.ActualCost "
        "FROM Expenses e "
        "LEFT JOIN ExpensesTypes et ON e.Type = et.Id "
        "WHERE e.PlanedDate BETWEEN ? AND ? "
        "AND e.ActualDate IS NOT NULL "
        "AND e.ActualCost IS NOT NULL";

    if(typeFilter->currentData().toInt() != -1) {
        queryStr += " AND e.Type = ?";
    }

    queryStr += " ORDER BY e.ActualDate DESC";

    query.prepare(queryStr);
    query.addBindValue(startDateEdit->date());
    query.addBindValue(endDateEdit->date());

    if(typeFilter->currentData().toInt() != -1) {
        query.addBindValue(typeFilter->currentData().toInt());
    }

    if(!query.exec()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to generate report: %1").arg(query.lastError().text()));
        return;
    }

    auto* model = new QSqlQueryModel(this);
    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, tr("Vendor"));
    model->setHeaderData(1, Qt::Horizontal, tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, tr("Cost"));

    reportTable->setModel(model);
    reportTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    exportButton->setEnabled(true);
}

void ReportWindow::exportToODF() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export Report"), "",
        tr("OpenDocument Text (*.odt);;All Files (*)"));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(".odt", Qt::CaseInsensitive))
        fileName += ".odt";

    QTextDocument document;
    QTextCursor cursor(&document);

    // Set document margins
    QTextFrameFormat frameFormat = document.rootFrame()->frameFormat();
    frameFormat.setMargin(10);
    document.rootFrame()->setFrameFormat(frameFormat);

    // Add title
    QTextCharFormat titleFormat;
    titleFormat.setFontPointSize(16);
    titleFormat.setFontWeight(QFont::Bold);
    cursor.insertText(tr("Отчет по производственным затратам\n"), titleFormat);
    cursor.insertText(tr("Период: %1 - %2\n\n").arg(
        startDateEdit->date().toString("yyyy.MM.dd"),
        endDateEdit->date().toString("yyyy.MM.dd")));

    // Calculate totals and savings
    QSqlQuery query;
    QString queryStr = "SELECT "
        "SUM(e.ActualCost) as TotalCost, "
        "SUM(e.PlanedCost - e.ActualCost) as TotalSavings, "
        "SUM(julianday(e.PlanedDate) - julianday(e.ActualDate)) as TotalTimeSavings "
        "FROM Expenses e "
        "WHERE e.PlanedDate BETWEEN ? AND ? "
        "AND e.ActualDate IS NOT NULL "
        "AND e.ActualCost IS NOT NULL";

    if(typeFilter->currentData().toInt() != -1) {
        queryStr += " AND e.Type = ?";
    }

    query.prepare(queryStr);
    query.addBindValue(startDateEdit->date());
    query.addBindValue(endDateEdit->date());

    if(typeFilter->currentData().toInt() != -1) {
        query.addBindValue(typeFilter->currentData().toInt());
    }

    if(!query.exec() || !query.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to calculate savings"));
        return;
    }

    double totalCost = query.value(0).toDouble();
    double totalSavings = query.value(1).toDouble();
    double totalTimeSavings = query.value(2).toDouble();

    // Add summary
    QTextCharFormat summaryFormat;
    summaryFormat.setFontPointSize(12);
    summaryFormat.setFontWeight(QFont::Bold);
    cursor.insertText(tr("\nИтого:\n"), summaryFormat);
    cursor.insertText(tr("Общая стоимость: %1\n").arg(QString::number(totalCost, 'f', 2)));
    cursor.insertText(tr("Общая экономия: %1\n").arg(QString::number(totalSavings, 'f', 2)));
    cursor.insertText(tr("Общая экономия времени: %1 дней\n\n").arg(QString::number(totalTimeSavings, 'f', 1)));

    // Create table
    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setCellPadding(5);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);

    QTextTable* table = cursor.insertTable(reportTable->model()->rowCount() + 1,
        reportTable->model()->columnCount(),
        tableFormat);

    // Add headers
    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setBackground(QColor("#f0f0f0"));

    for(int col = 0; col < reportTable->model()->columnCount(); ++col) {
        cursor = table->cellAt(0, col).firstCursorPosition();
        cursor.insertText(reportTable->model()->headerData(col, Qt::Horizontal).toString(), headerFormat);
    }

    // Add data
    for(int row = 0; row < reportTable->model()->rowCount(); ++row) {
        for(int col = 0; col < reportTable->model()->columnCount(); ++col) {
            cursor = table->cellAt(row + 1, col).firstCursorPosition();
            QVariant data = reportTable->model()->data(reportTable->model()->index(row, col));

            // Format numbers
            if(col == 3) {
                // Cost column
                cursor.insertText(QString::number(data.toDouble(), 'f', 2));
            } else {
                cursor.insertText(data.toString());
            }
        }
    }

    // Save document
    QTextDocumentWriter writer(fileName);
    if(!writer.write(&document)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to export report to ODF format"));
        return;
    }

    QMessageBox::information(this, tr("Success"), tr("Report exported successfully"));
}
