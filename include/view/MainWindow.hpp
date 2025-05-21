#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QMenuBar>
#include <QPlainTextEdit>

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

    ~MainWindow() override;

private:
    QMdiArea* mdiArea;

    void setupMenuBar();

    void setupToolBar();
};
