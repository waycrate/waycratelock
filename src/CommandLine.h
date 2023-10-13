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

    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    inline QString password() { return m_password; }
    void setPassword(const QString &password)
    {
        m_password = password;
        Q_EMIT passwordChanged();
    }

    Q_INVOKABLE void UnLock();
signals:
    void passwordChanged();

private:
    QString m_password;
};
