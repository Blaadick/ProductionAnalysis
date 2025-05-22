#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QModelIndex>

template <typename T>
concept SubWindow = std::is_base_of_v<QMdiSubWindow, T>;

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

    ~MainWindow() override;

    template <SubWindow T>
    static void openWindow();

    static void openExpenseEditWindow(const QModelIndex& index);

private:
    static QMdiArea* mdiArea;

    void setupMenuBar();

    void setupToolBar();

    void toggleFullScreen();
};
