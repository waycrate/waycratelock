#include <private/qwaylandinputdevice_p.h>
#include <private/qwaylandsurface_p.h>
#include <private/qwaylandwindow_p.h>
#ifndef DEBUG_MODE
#include <SessionLockQt/command.h>
#include <SessionLockQt/window.h>
#endif

#include "config.h"
#include "lockadaptor.h"
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickStyle>
#include <QQuickWindow>

using namespace Qt::StringLiterals;

int
main(int argc, char *argv[])
{
    ensureConfigFile();
    QGuiApplication app(argc, argv);

    LockBackend backend;
    LockAdaptor adaptor(&backend);

    bool registerServerSucceed =
      QDBusConnection::sessionBus().registerService(u"org.waycrate.lock"_s);

    bool registerObjectSucceed =
      QDBusConnection::sessionBus().registerObject(u"/org/waycrate/lock"_s, &backend);

    if (!registerObjectSucceed || !registerServerSucceed) {
        qWarning() << "another waycratelock is running";
        return -1;
    }
    QQuickStyle::setStyle("Material");
    auto screens = QGuiApplication::screens();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/WayCrateLock/qml/main.qml"_s);
    QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreated,
      &app,
      [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
              QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);


    engine.load(url);
    return app.exec();
}
