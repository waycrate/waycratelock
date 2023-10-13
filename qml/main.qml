import QtQuick
import QtQuick.Controls
import MimeApp.Command

Pane {
    Button {
        text: "Unlock"
        anchors.fill: parent
        onClicked: {
            CommandLine.UnLock();
        }
    }
}
