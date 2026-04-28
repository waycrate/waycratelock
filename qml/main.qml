import QtQuick
import WayCrateLock

WlSessionLock {
    id: w

    WlSessionLockSurface {
        MainView {
            anchors.fill: parent
        }
    }
}
