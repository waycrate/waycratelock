#include "WindowElement.h"

WindowElement::WindowElement(QWindow *parent)
  : QQuickView(parent)
{
    setResizeMode(QQuickView::SizeRootObjectToView);

    setSource(u"qrc:/MimeApp/qml/main.qml"_qs);
}
