#pragma once

#include <QObject>

class CreationModel final : public QObject {
    Q_OBJECT

public slots:
    void increment();

signals:
    void incremented(int newCount);

private:
    int count = 0;
};
