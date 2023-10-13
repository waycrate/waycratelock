#include "WindowElement.h"

#include <SessionLockQt/command.h>
#include <SessionLockQt/shell.h>
#include <SessionLockQt/window.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int
main(int argc, char *argv[])
{
    ExtSessionLockV1Qt::Shell::useExtSessionLock();

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    auto screens = QGuiApplication::screens();

    for (auto screen : screens) {
        WindowElement *element = new WindowElement;
        ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(element, screen);
        element->show();
    }

    QObject::connect(&app, &QGuiApplication::screenAdded, &app, [](auto screen) {
        WindowElement *element = new WindowElement;
        ExtSessionLockV1Qt::Window::registerWindowFromQtScreen(element, screen);
        element->show();
        ExtSessionLockV1Qt::Command::instance()->LockScreen();
    });
    ExtSessionLockV1Qt::Command::instance()->LockScreen();

    return app.exec();
}
