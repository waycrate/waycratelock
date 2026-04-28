import QtQuick
import WayCrate.SessionLock

WlSessionLock {
    id: w

    WlSessionLockSurface {
        MainView {
            anchors.fill: parent
        }
    }
}
