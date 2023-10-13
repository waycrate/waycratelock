import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import MimeApp

Pane {
    id: root
    property bool isIn: false

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            root.isIn = true;
        }
        onExited: {
            root.isIn = false;
        }

        ColumnLayout {
            anchors.fill: parent
            Item {
                Layout.fillHeight: true
            }
            TimePanel {
                Layout.alignment: Qt.AlignHCenter
            }
            Button {
                id: loginBtn
                visible: root.isIn
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
}
