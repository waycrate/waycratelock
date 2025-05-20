#include "DBusBackend.h"

LockBackend::LockBackend(QObject *parent)
  : QObject(parent)
{
}

int
LockBackend::version() const
{
    return 1;
}
