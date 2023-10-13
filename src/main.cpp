#include "src/Command.h"
#include "src/WindowElement.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <SessionLockQt/command.h>
#include <SessionLockQt/shell.h>
#include <SessionLockQt/window.h>

void
registGlobTypes()
{
    qmlRegisterSingletonType<CommandLine>(
      "MimeApp.Command", 1, 0, "CommandLine", [](QQmlEngine *, QJSEngine *) -> QObject * {
          return new CommandLine;
      });
}

int
main(int argc, char *argv[])
{
    ExtSessionLockV1Qt::Shell::useExtSessionLock();

    QGuiApplication app(argc, argv);
    auto screens = QGuiApplication::screens();

    registGlobTypes();

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
