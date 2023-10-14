import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import WayCrateLock

Page {
    id: root
    anchors.fill: parent
    property bool isIn: false

    property int year
    property int month
    property int day

    property int hours
    property int minutes

    function leftPad(number) {
        var output = number + '';
        while (output.length < 2) {
            output = '0' + output;
        }
        return output;
    }

    background: Image {
        anchors.fill: root
        source: CommandLine.background
        opacity: 0.6
    }
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
            input.focus = true;
            root.forceActiveFocus();
        }
        onExited: {
            root.isIn = false;
            input.focus = false;
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
                visible: root.isIn && CommandLine.usePam
                Layout.alignment: Qt.AlignHCenter
                text: CommandLine.password
                placeholderText: "Password"
                onEditingFinished: {
                    CommandLine.password = input.text;
                }
                echoMode: TextInput.Password
                Layout.preferredWidth: 250
                onAccepted: {
                    loginBtn.checked = true;
                    CommandLine.RequestUnlock();
                }
                onReleased: {
                    loginBtn.checked = false;
                }
            }

            Label {
                visible: CommandLine.errorMessage !== "" && root.isIn
                text: CommandLine.errorMessage
                Layout.alignment: Qt.AlignHCenter
                font.pointSize: 15
                color: "red"
            }

            Button {
                id: loginBtn
                visible: root.isIn
                Layout.alignment: Qt.AlignHCenter
                text: "Unlock"
                onClicked: {
                    CommandLine.RequestUnlock();
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
                    text: root.leftPad(root.hours)
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
                    text: root.leftPad(root.minutes)
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
