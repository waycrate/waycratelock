#include <QObject>

class LockBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int version READ version)
public:
    explicit LockBackend(QObject *parent = nullptr);
    int version() const;
};
