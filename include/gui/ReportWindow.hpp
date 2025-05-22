#pragma once

#include <QMdiSubWindow>
#include <QDateEdit>
#include <QPushButton>
#include <QTableView>
#include <QComboBox>
#include <QGridLayout>

class ReportWindow final : public QMdiSubWindow {
    Q_OBJECT

public:
    explicit ReportWindow(QWidget* parent = nullptr);

private:
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QComboBox* typeFilter;
    QTableView* reportTable;
    QPushButton* generateButton;
    QPushButton* exportButton;

    void setupUI();

    void setupConnections();

    void generateReport();

    void exportToODF();
};
