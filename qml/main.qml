import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import MimeApp

Pane {
    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillHeight: true
        }
        TimePanel {
            Layout.alignment: Qt.AlignHCenter
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Unlock"
            onClicked: {
                CommandLine.UnLock();
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
