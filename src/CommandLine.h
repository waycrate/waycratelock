#pragma once

#include <QObject>
#include <QQmlEngine>

class CommandLine final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit CommandLine(QObject *parent = nullptr);

    Q_INVOKABLE void UnLock();
};
