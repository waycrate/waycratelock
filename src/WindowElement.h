#pragma once

#include <QObject>

#include <QQuickView>

class WindowElement : public QQuickView
{
    Q_OBJECT

public:
    explicit WindowElement(QWindow *parent = nullptr);
};
