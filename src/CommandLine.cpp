#include "CommandLine.h"

#include <QGuiApplication>
#include <SessionLockQt/command.h>

CommandLine::CommandLine(QObject *parent)
  : QObject(parent)
{
}

void
CommandLine::UnLock()
{
    ExtSessionLockV1Qt::Command::instance()->unLockScreen();
    QGuiApplication::quit();
}
