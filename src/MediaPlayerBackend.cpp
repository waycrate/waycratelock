#include "MediaPlayerBackend.h"

#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QMetaProperty>
#include <qdbuspendingcall.h>

MediaPlayerBackend::MediaPlayerBackend(QObject *parent)
  : QObject(parent)
  , m_service(QString())
  , m_canPlay(false)
  , m_canPause(false)
  , m_canGoNext(false)
  , m_canGoPre(false)
  , m_currentDisplayName(QString())
  , m_playBackStatus(QString())
{
    initMediaPlayer();
}

void
MediaPlayerBackend::initMediaPlayer()
{
    QDBusConnectionInterface *interface = QDBusConnection::sessionBus().interface();
    QDBusPendingCall call               = interface->asyncCall("ListNames");
    QDBusPendingCallWatcher *watcher    = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [this, call] {
        if (call.isError()) {
            return;
        }
        QDBusReply<QStringList> reply  = call.reply();
        const QStringList &serviceList = reply.value();

        for (const QString &serv : serviceList) {
            if (!serv.startsWith("org.mpris.MediaPlayer2"))
                continue;

            initSignalServer(serv);
        }
    });
    connect(interface,
            &QDBusConnectionInterface::serviceRegistered,
            this,
            [this](const QString &service) {
                if (service.startsWith("org.mpris.MediaPlayer2")) {
                    initSignalServer(service);
                }
            });
    connect(interface,
            &QDBusConnectionInterface::serviceUnregistered,
            this,
            [this](const QString &service) {
                if (service.startsWith("org.mpris.MediaPlayer2")) {
                    Q_EMIT serviceDeleted(service);
                }
            });
    connect(
      watcher, &QDBusPendingCallWatcher::finished, watcher, &QDBusPendingCallWatcher::deleteLater);
}

void
MediaPlayerBackend::initSignalServer(const QString &server)
{
    MediaPlayerInterface *interface = new MediaPlayerInterface(
      server, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);
    if ((m_service.isEmpty() && m_playBackStatus != "Playing") ||
        m_playBackStatus != "Playing" && interface->playbackStatus() == "Playing") {
        m_playBackStatus     = interface->playbackStatus();
        m_service            = server;
        m_canPlay            = interface->canPlay();
        m_canPause           = interface->canPause();
        m_canGoPre           = interface->canGoPrevious();
        m_canGoNext          = interface->canGoNext();
        m_currentDisplayName = interface->metadata().value("xesam:title").toString();
        Q_EMIT playbackStatusChanged();
        Q_EMIT hasMediaChanged();
        Q_EMIT canPlayChanged();
        Q_EMIT canGoPreviousChanged();
        Q_EMIT canGoNextChanged();
        Q_EMIT currentDisplayNameChanged();
    }

    connect(this,
            &MediaPlayerBackend::serviceDeleted,
            interface,
            [interface, server, this](const QString &name) {
                if (name != server) {
                    return;
                }
                if (m_service == name) {
                    m_service = QString();
                    Q_EMIT hasMediaChanged();
                }
                interface->deleteLater();
            });
    connect(interface, &MediaPlayerInterface::CanGoNextChanged, this, [this, server, interface] {
        if (m_service != server) {
            return;
        }
        m_canGoNext = interface->canGoNext();
        Q_EMIT canGoNextChanged();
    });
    connect(
      interface, &MediaPlayerInterface::CanGoPreviousChanged, this, [this, server, interface] {
          if (m_service != server) {
              return;
          }
          m_canGoPre = interface->canGoPrevious();
          Q_EMIT canGoPreviousChanged();
      });

    connect(interface, &MediaPlayerInterface::CanPauseChanged, this, [this, server, interface] {
        if (m_service != server) {
            return;
        }
        m_canPause = interface->canPause();
        Q_EMIT CanPauseChanged();
    });

    connect(interface, &MediaPlayerInterface::MetadataChanged, this, [this, server, interface] {
        if (m_service != server) {
            return;
        }
        m_currentDisplayName = interface->metadata().value("xesam:title").toString();
        Q_EMIT currentDisplayNameChanged();
    });
    connect(
      interface, &MediaPlayerInterface::PlaybackStatusChanged, this, [this, server, interface] {
          if (m_service.isEmpty() && interface->playbackStatus() == "Playing") {
              m_service = server;
              Q_EMIT hasMediaChanged();
          } else if (m_playBackStatus != "Playing" && interface->playbackStatus() == "Playing") {
              m_service = server;
              Q_EMIT hasMediaChanged();
              m_currentDisplayName = interface->metadata().value("xesam:title").toString();
              Q_EMIT currentDisplayNameChanged();
              m_canGoPre = interface->canGoPrevious();
              Q_EMIT canGoPreviousChanged();
              m_canPause = interface->canPause();
              Q_EMIT CanPauseChanged();
          } else if (m_service != server) {
              return;
          }
          m_playBackStatus = interface->playbackStatus();

          Q_EMIT playbackStatusChanged();
      });

    connect(this, &MediaPlayerBackend::requestPlay, interface, [interface, this, server] {
        if (m_service != server) {
            return;
        }
        interface->Play();
    });

    connect(this, &MediaPlayerBackend::requestPause, interface, [interface, this, server] {
        if (m_service != server) {
            return;
        }
        interface->Pause();
    });

    connect(this, &MediaPlayerBackend::requestGoPre, interface, [interface, this, server] {
        if (m_service != server) {
            return;
        }
        interface->Previous();
    });

    connect(this, &MediaPlayerBackend::requestGoNext, interface, [interface, this, server] {
        if (m_service != server) {
            return;
        }
        interface->Next();
    });
}

void
MediaPlayerBackend::play()
{
    Q_EMIT requestPlay();
}

void
MediaPlayerBackend::pause()
{
    Q_EMIT requestPause();
}

void
MediaPlayerBackend::goPre()
{
    Q_EMIT requestGoPre();
}

void
MediaPlayerBackend::goNext()

{
    Q_EMIT requestGoNext();
}

MediaPlayerInterface::MediaPlayerInterface(const QString &service,
                                           const QString &path,
                                           const QDBusConnection &connection,
                                           QObject *parent)
  : QDBusAbstractInterface(service, path, "org.mpris.MediaPlayer2.Player", connection, parent)
{
    QDBusConnection::sessionBus().connect(this->service(),
                                          this->path(),
                                          "org.freedesktop.DBus.Properties",
                                          "PropertiesChanged",
                                          "sa{sv}as",
                                          this,
                                          SLOT(onPropertyChanged(const QDBusMessage &)));
}

MediaPlayerInterface::~MediaPlayerInterface()
{
    QDBusConnection::sessionBus().disconnect(this->service(),
                                             this->path(),
                                             "org.freedesktop.DBus.Properties",
                                             "PropertiesChanged",
                                             "sa{sv}as",
                                             this,
                                             SLOT(onPropertyChanged(const QDBusMessage &)));
}

void
MediaPlayerInterface::onPropertyChanged(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != "org.mpris.MediaPlayer2.Player")
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys         = changedProps.keys();
    foreach (const QString &prop, keys) {
        const QMetaObject *self = metaObject();
        for (int i = self->propertyOffset(); i < self->propertyCount(); ++i) {
            QMetaProperty p = self->property(i);
            if (p.name() == prop) {
                Q_EMIT p.notifySignal().invoke(this);
            }
        }
    }
}
