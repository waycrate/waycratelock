#include <SessionLockQt/command.h>
#include <SessionLockQt/shell.h>
#include <SessionLockQt/window.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <cstdlib>

int
main(int argc, char *argv[])
{
    ExtSessionLockV1Qt::Shell::useExtSessionLock();

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    auto screens = QGuiApplication::screens();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/MimeApp/qml/main.qml"_qs);
    QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreated,
      &app,
      [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
              QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

    for (auto screen : screens) {
        engine.load(url);
        if (QWindow *window = qobject_cast<QWindow *>(engine.rootObjects().last())) {
            ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(window, screen);
            window->show();
        } else {
            qDebug() << "Cannot get window";
            return 0;
        }
    }
    QObject::connect(&app, &QGuiApplication::screenAdded, &app, [&engine, url](auto screen) {
        engine.load(url);
        if (QWindow *window = qobject_cast<QWindow *>(engine.rootObjects().last())) {
            ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(window, screen);
            window->show();
        } else {
            qDebug() << "Cannot get window";
            exit(0);
        }
        ExtSessionLockV1Qt::Command::instance()->LockScreen();
    });
    ExtSessionLockV1Qt::Command::instance()->LockScreen();

    return app.exec();
}
