import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import MimeApp

Pane {
    id: root
    property bool isIn: false

    property int year
    property int month
    property int day

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
        year = date.getFullYear();
        month = date.getMonth() + 1;
        day = date.getDate();
        hours = date.getHours();
        minutes = date.getMinutes();
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            root.isIn = true;
            input.focus = true
        }
        onExited: {
            root.isIn = false;
            input.focus = false
        }

        ColumnLayout {
            anchors.fill: parent

            Item {
                Layout.preferredHeight: 30
            }

            Label {
                text: root.year + " " + root.month + " " + root.day
                Layout.alignment: Qt.AlignHCenter
                font.pointSize: 30
                font.bold: true
            }

            Item {
                Layout.fillHeight: true
            }

            TimePanel {
                Layout.alignment: Qt.AlignHCenter
            }

            Item {
                Layout.preferredHeight: 30
            }

            TextField {
                id: input
                visible: root.isIn
                Layout.alignment: Qt.AlignHCenter
                text: CommandLine.password
                placeholderText: "Password"
                echoMode: TextInput.Password
                Layout.preferredWidth: 250
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

    Binding {
        target: CommandLine
        property: "password"
        value: input.text
    }
}
