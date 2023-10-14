#pragma once

#include <QObject>
#include <QQmlEngine>
#include <security/pam_appl.h>

class PassWordInfo final : public QObject
{
    Q_OBJECT

public:
    static PassWordInfo *instance();

    inline QString password() { return m_password; }

    void setPassword(const QString& password);

private:
    explicit PassWordInfo(QObject *parent = nullptr);

private:
    QString m_password;
};

class CommandLine final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit CommandLine(QObject *parent = nullptr);

    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    inline QString password() { return m_password; }
    void setPassword(const QString &password);

    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    inline QString userName() { return m_userName; }

    Q_INVOKABLE void UnLock();
    Q_INVOKABLE void RequestUnlock();

signals:
    void passwordChanged();
    void userNameChanged();

private:
    QString m_password;
    QString m_userName;
    pam_handle_t *m_handle;
};
