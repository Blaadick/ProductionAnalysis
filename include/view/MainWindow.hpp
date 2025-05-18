#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QMenuBar>
#include <QPlainTextEdit>

#include "model/CreationModel.hpp"

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

    ~MainWindow() override;

    void document() const;

private:
    CreationModel* creationModel;
    QMdiArea* mdiArea;

    void setupMenuBar();

    void setupToolBar();
};
