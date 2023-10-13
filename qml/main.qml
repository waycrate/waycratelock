import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import MimeApp

Pane {
    id: root
    property bool isIn: false

    property int hours
    property int minutes

    Item {
        Timer {
            interval: 100
            running: true
            repeat: true
            onTriggered: root.timeChanged()
        }
    }

    function timeChanged() {
        var date = new Date;
        hours = date.getHours();
        minutes = date.getMinutes();
    }

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

            Item {
                Layout.preferredHeight: 30
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

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: root.hours
                    font.pointSize: 25
                    font.bold: true
                    Layout.preferredWidth: 80
                }
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: ":"
                    font.pointSize: 25
                    font.bold: true
                }
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: root.minutes
                    font.pointSize: 25
                    font.bold: true
                    Layout.preferredWidth: 80
                }
            }

            Item {
                Layout.preferredHeight: 30
            }
        }
    }
}
