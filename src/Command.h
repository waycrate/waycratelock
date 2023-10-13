#pragma once

#include <QObject>

class CommandLine final : public QObject
{
    Q_OBJECT

public:
    explicit CommandLine(QObject *parent = nullptr);

    Q_INVOKABLE void UnLock();
};
