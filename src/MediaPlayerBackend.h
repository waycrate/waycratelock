#pragma once

#include <QObject>

#include <QDBusAbstractInterface>
#include <QDBusPendingReply>
#include <QMetaType>
#include <QQmlEngine>
#include <QVariant>

typedef QMap<QString, QVariant> Dict;
Q_DECLARE_METATYPE(Dict)

class MediaPlayerBackend final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit MediaPlayerBackend(QObject *parent = nullptr);

    Q_PROPERTY(bool hasMedia READ hasMedia NOTIFY hasMediaChanged)
    inline bool hasMedia() { return !m_service.isEmpty(); }

    Q_PROPERTY(bool canPause READ canPause NOTIFY canPlayChanged)
    inline bool canPause() { return m_canPause; }
    Q_PROPERTY(bool canPlay READ canPlay NOTIFY canPlayChanged)
    inline bool canPlay() { return m_canPlay; }

    Q_PROPERTY(bool canGoNext READ canGoNext NOTIFY canPlayChanged)
    inline bool canGoNext() { return m_canGoNext; }

    Q_PROPERTY(bool canGoPrevious READ canGoPrevious NOTIFY canGoPreviousChanged)
    inline bool canGoPrevious() { return m_canGoPre; }

    Q_PROPERTY(QString currentDisplayName READ currentDisplayName NOTIFY currentDisplayNameChanged)
    inline QString currentDisplayName() { return m_currentDisplayName; }

    Q_PROPERTY(QString playbackStatus READ playbackStatus NOTIFY playbackStatusChanged)

    inline QString playbackStatus() { return m_playBackStatus; }

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void goPre();
    Q_INVOKABLE void goNext();

signals:
    void hasMediaChanged();
    void canPlayChanged();
    void CanPauseChanged();
    void canGoNextChanged();
    void canGoPreviousChanged();
    void currentDisplayNameChanged();
    void playbackStatusChanged();

    void serviceDeleted(const QString &server);

    void requestPlay();
    void requestPause();
    void requestGoPre();
    void requestGoNext();

private:
    void initMediaPlayer();
    void initSignalServer(const QString &serverPath);

private:
    QString m_service;
    bool m_canPlay;
    bool m_canGoNext;
    bool m_canGoPre;
    bool m_canPause;
    QString m_currentDisplayName;
    QString m_playBackStatus;
};

class MediaPlayerInterface : public QDBusAbstractInterface
{
    Q_OBJECT

public:
    MediaPlayerInterface(const QString &service,
                         const QString &path,
                         const QDBusConnection &connection,
                         QObject *parent = nullptr);
    ~MediaPlayerInterface();

public:
    inline QDBusPendingReply<> Play()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Play"), argumentList);
    }

    inline QDBusPendingReply<> Stop()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Stop"), argumentList);
    }

    inline QDBusPendingReply<> Pause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Pause"), argumentList);
    }

    inline QDBusPendingReply<> Next()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Next"), argumentList);
    }

    inline QDBusPendingReply<> Previous()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Previous"), argumentList);
    }

    Q_PROPERTY(Dict Metadata READ metadata NOTIFY MetadataChanged)
    inline Dict metadata() const { return qvariant_cast<Dict>(property("Metadata")); }

    Q_PROPERTY(bool CanPlay READ canPlay NOTIFY CanPlayChanged)
    inline bool canPlay() const { return qvariant_cast<bool>(property("CanPlay")); }

    Q_PROPERTY(bool CanGoNext READ canGoNext NOTIFY CanGoNextChanged)
    inline bool canGoNext() const { return qvariant_cast<bool>(property("CanGoNext")); }

    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious NOTIFY CanGoPreviousChanged)
    inline bool canGoPrevious() const { return qvariant_cast<bool>(property("CanGoPrevious")); }

    Q_PROPERTY(bool CanPause READ canPause NOTIFY CanPauseChanged)
    inline bool canPause() const { return qvariant_cast<bool>(property("CanPause")); }

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY PlaybackStatusChanged)
    inline QString playbackStatus() const
    {
        return qvariant_cast<QString>(property("PlaybackStatus"));
    }

Q_SIGNALS:
    void MetadataChanged();
    void CanPlayChanged();
    void CanGoNextChanged();
    void CanGoPreviousChanged();
    void CanPauseChanged();
    void PlaybackStatusChanged();


private Q_SLOTS:
    void onPropertyChanged(const QDBusMessage &msg);
};
