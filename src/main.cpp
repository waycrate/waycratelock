#include "src/WindowElement.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <SessionLockQt/command.h>
#include <SessionLockQt/shell.h>
#include <SessionLockQt/window.h>

int
main(int argc, char *argv[])
{
    ExtSessionLockV1Qt::Shell::useExtSessionLock();

    QGuiApplication app(argc, argv);
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
    });

    return app.exec();
}
