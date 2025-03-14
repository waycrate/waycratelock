#include <private/qwaylandinputdevice_p.h>
#include <private/qwaylandsurface_p.h>
#include <private/qwaylandwindow_p.h>
#ifndef DEBUG_MODE
#include <SessionLockQt/command.h>
#include <SessionLockQt/shell.h>
#include <SessionLockQt/window.h>
#endif

#include "config.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickStyle>
#include <QQuickWindow>
#include <cstdlib>

using namespace Qt::StringLiterals;

QtWaylandClient::QWaylandWindow *oldWindow = nullptr;

int
main(int argc, char *argv[])
{
#ifndef DEBUG_MODE
    ExtSessionLockV1Qt::Shell::useExtSessionLock();
#endif

    ensureConfigFile();
    QGuiApplication app(argc, argv);
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

    auto connectScreen = [&engine, url](auto screen) -> void {
        engine.load(url);
        QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().last());
        if (!window) {
            qDebug() << "Cannot get window";
            exit(0);
        }
        auto input = window->findChild<QQuickItem *>("input");
        QObject::connect(input, &QQuickItem::focusChanged, input, [input](auto focus) {
            if (!focus)
                return;
            auto focusWindow = input->window();
            auto wFocusWindow =
              dynamic_cast<QtWaylandClient::QWaylandWindow *>(focusWindow->handle());
            wFocusWindow->display()->handleWindowActivated(wFocusWindow);
            if (wFocusWindow->display()->defaultInputDevice() &&
                wFocusWindow->display()->defaultInputDevice()->keyboard()) {
                wFocusWindow->display()->defaultInputDevice()->keyboard()->mFocus =
                  wFocusWindow->waylandSurface();
            }
            if (oldWindow && oldWindow != wFocusWindow) {
                oldWindow->display()->handleWindowDeactivated(oldWindow);
            }
            oldWindow = wFocusWindow;
        });
#ifndef DEBUG_MODE
        ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(window, screen);
#endif
        window->show();
    };

    for (auto screen : screens) {
        connectScreen(screen);
    }
    QObject::connect(&app, &QGuiApplication::screenAdded, &app, [connectScreen](auto screen) {
        connectScreen(screen);
#ifndef DEBUG_MODE
        ExtSessionLockV1Qt::Command::instance()->LockScreen();
#endif
    });
#ifndef DEBUG_MODE
    ExtSessionLockV1Qt::Command::instance()->LockScreen();
#endif

    return app.exec();
}
