import QtQuick 2.7
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import ESPDevice 1.0

StackView {
    id: root
    property alias address: device.address

    property string unavailableColor: "#fb9292"
    property string actionColor: "green"
    property string idleColor: "gray"
    property string halfOpenColor: "#8672b9"
    property string backgroundColor: "#e87910"
    property string controlColor: "#97500e"

    signal leave
    signal error

    initialItem: mainView

    Timer {
        interval: 500
        running: true
        repeat: true
        onTriggered: {
            var status = device.getStatus()
        }
    }

    ButtonAdapter {
        id: device

        onIsConnectedChanged: {
            if(!isConnected)
                error()
        }
    }

    Component {
        id: settings

        SettingsInterface {
            adapter: device

            onLeave: {
                stackView.pop();
            }
        }
    }

    Component {
        id: mainView
        Item {
            Rectangle {
                id: rectangle
                color: backgroundColor
                anchors.fill: parent
                anchors.bottomMargin: 40
                radius: 5

                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 5
                    color: device.status === ButtonAdapter.BUTTON_OFF ? idleColor : actionColor
                    radius: 10
                    border.color: "#ffffff"
                    border.width: 5

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            if(device.status === ButtonAdapter.BUTTON_ON)
                                device.status = ButtonAdapter.BUTTON_OFF
                            else
                                device.status = ButtonAdapter.BUTTON_ON
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        color: "#ffffff"
                        text: device.status === ButtonAdapter.BUTTON_OFF ? qsTr("ВЫКЛ") : qsTr("ВКЛ")
                        font.pixelSize: parent.width / 4
                        elide: Text.ElideRight
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 40
                radius: 6
                color: "#c05d06"

                RowLayout {
                    anchors.fill: parent
                    anchors.rightMargin: 3
                    anchors.leftMargin: 3

                    BackButton {
                        height: 35
                        width: 35
                        onPress: {
                            leave()
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                    SettingsButton {
                        height: 35
                        width: 35

                        onPress: {
                            stackView.push(settings)
                        }
                    }
                }
            }
        }


    }
}

