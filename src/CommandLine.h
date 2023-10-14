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

    void setPassword(const QString &password);

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

    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    inline QString errorMessage() { return m_errorMessage; }

    Q_PROPERTY(bool usePam READ usePam NOTIFY usePamChanged)
    inline bool usePam() { return m_usePam; }

    Q_INVOKABLE void UnLock();
    Q_INVOKABLE void RequestUnlock();

private:
    void readConfig();

signals:
    void passwordChanged();
    void userNameChanged();
    void errorMessageChanged();
    void usePamChanged();

private:
    QString m_password;
    QString m_userName;
    QString m_errorMessage;
    pam_handle_t *m_handle;
    bool m_usePam;
};
