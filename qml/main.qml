import QtQuick.Controls
import MimeApp

Pane {
    Button {
        text: "Unlock"
        anchors.fill: parent
        onClicked: {
            CommandLine.UnLock();
        }
    }
}
