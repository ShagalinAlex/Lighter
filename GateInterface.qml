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

    GateAdapter {
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
                id: meteoHeader
                height: width / 5
                radius: 5
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#000000"
                    }

                    GradientStop {
                        position: 1
                        color: backgroundColor
                    }
                }

                Image {
                    id: termometer
                    width: height / 2.5
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    opacity: 0.8
                    source: "Images/termometer.png"
                }

                Text {
                    id: text1
                    y: 48
                    width: 52
                    height: 46
                    color: "#ffffff"
                    text: qsTr("°C")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: tempValueText.right
                    anchors.leftMargin: 0
                    opacity: 0.8
                    font.pixelSize: 30
                }

                Text {
                    id: tempValueText
                    y: 48
                    width: 80
                    height: 46
                    color: "#ffffff"
                    text: device.temperature.toFixed(1)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: termometer.right
                    anchors.leftMargin: 7
                    font.pixelSize: 30
                }

                Image {
                    id: hummeter
                    width: height / 1.6
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 7
                    anchors.top: parent.top
                    anchors.topMargin: 7
                    anchors.left: parent.horizontalCenter
                    anchors.leftMargin: 10
                    source: "Images/humidity.png"
                }

                Text {
                    id: text3
                    x: -9
                    y: 39
                    width: 52
                    height: 46
                    color: "#ffffff"
                    text: qsTr("%")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.verticalCenterOffset: 0
                    anchors.leftMargin: 0
                    anchors.left: humValueText.right
                    font.pixelSize: 28
                    opacity: 0.8
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: humValueText
                    y: 39
                    width: 80
                    height: 46
                    color: "#ffffff"
                    text: device.humidity.toFixed(0)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    transformOrigin: Item.Center
                    anchors.verticalCenterOffset: 0
                    anchors.leftMargin: 5
                    anchors.left: hummeter.right
                    font.pixelSize: 30
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle {
                id: rectangle
                color: backgroundColor
                anchors.top: meteoHeader.bottom
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 40
                radius: 5

                Rectangle {
                    id: light
                    height: parent.height / 3.5
                    color: device.lightState == GateAdapter.LIGHT_OFF ? idleColor : actionColor
                    radius: height / 4
                    anchors.right: parent.horizontalCenter
                    anchors.rightMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 25
                    border.color: "#ffffff"
                    border.width: 5

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            if(device.lightState == GateAdapter.LIGHT_ON)
                                device.lightState = GateAdapter.LIGHT_OFF
                            else
                                device.pulseLight()
                        }

                        onPressAndHold: {
                            device.lightState = device.lightState == GateAdapter.LIGHT_ON ? GateAdapter.LIGHT_OFF : GateAdapter.LIGHT_ON

                        }
                    }

                    Text {
                        x: 199
                        y: 48
                        color: "#ffffff"
                        text: qsTr("Свет")
                        font.pixelSize: parent.height / 3
                        elide: Text.ElideRight
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Rectangle {
                    id: door
                    height: parent.height / 3.5
                    color: openDoorTimer.running ? actionColor : idleColor
                    radius: height / 4
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.left: parent.horizontalCenter
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 25
                    border.color: "#ffffff"
                    Text {
                        x: 199
                        y: 48
                        color: "#ffffff"
                        text: qsTr("Дверь")
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: parent.height / 3
                        anchors.verticalCenter: parent.verticalCenter
                        elide: Text.ElideRight
                    }
                    border.width: 5

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            if(device.pulseDoor() === 1)
                                openDoorTimer.start();
                        }
                    }

                    Timer {
                        id: openDoorTimer
                        interval: 1000; running: false; repeat: false
                    }
                }

                Rectangle {
                    id: gateControl
                    color: controlColor
                    radius: 22
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.top: door.bottom
                    anchors.topMargin: 13

                    Text {
                        id: gateHeader
                        x: 196
                        y: -232
                        color: "#ffffff"
                        text: qsTr("Управление воротами")
                        anchors.horizontalCenterOffset: 0
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 25
                    }

                    Item {
                        id: item1
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 10
                        anchors.top: gateHeader.bottom
                        anchors.topMargin: 10

                        Rectangle {
                            id: gateOpen
                            x: 0
                            y: 0
                            color: device.gateState == GateAdapter.GATE_OPEN ? actionColor : idleColor
                            radius: height / 2
                            anchors.bottom: parent.verticalCenter
                            anchors.bottomMargin: 5
                            anchors.top: parent.top
                            border.color: "#ffffff"
                            anchors.topMargin: 10
                            anchors.leftMargin: 10
                            border.width: 5
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.rightMargin: 10

                            MouseArea {
                                anchors.fill: parent

                                onClicked: {
                                    if(device.gateState != GateAdapter.GATE_OPEN)
                                        device.gateState = GateAdapter.GATE_OPEN
                                    else
                                        device.gateState = GateAdapter.GATE_IDLE
                                }
                            }

                            Text {
                                id: text5
                                x: 1
                                y: 77
                                color: "#ffffff"
                                text: qsTr("Открыть")
                                anchors.verticalCenterOffset: 0
                                anchors.horizontalCenterOffset: -62
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pixelSize: parent.height / 3
                                anchors.verticalCenter: parent.verticalCenter
                                elide: Text.ElideRight
                            }

                            Rectangle {
                                id: smallOpenGate
                                x: 299
                                width: parent.width / 2.5
                                color: halfOpenColor
                                radius: height / 2
                                border.width: 5
                                border.color: "#ffffff"
                                anchors.horizontalCenterOffset: parent.horizontalCenter - (smallOpenGate.width)
                                anchors.right: parent.right
                                anchors.rightMargin: 0
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 0
                                anchors.top: parent.top
                                anchors.topMargin: 0

                                MouseArea {
                                    anchors.fill: parent

                                    onClicked: {
                                        device.pulseGate()
                                    }
                                }

                                Text {
                                    color: "#ffffff"
                                    text: qsTr("30 сек")
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: 26
                                }
                            }
                        }

                        Rectangle {
                            id: gateClose
                            color: device.gateState == GateAdapter.GATE_CLOSE ? actionColor : idleColor
                            radius: height / 2
                            anchors.bottomMargin: 10
                            anchors.top: parent.verticalCenter
                            anchors.bottom: parent.bottom
                            border.color: "#ffffff"
                            anchors.leftMargin: 10
                            anchors.topMargin: 5
                            border.width: 5
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            MouseArea {
                                anchors.fill: parent

                                onClicked: {
                                    if(device.gateState != GateAdapter.GATE_CLOSE)
                                        device.gateState = GateAdapter.GATE_CLOSE
                                    else
                                        device.gateState = GateAdapter.GATE_IDLE
                                }
                            }

                            Text {
                                color: "#ffffff"
                                text: qsTr("Закрыть")
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pixelSize: parent.height / 3
                                anchors.verticalCenter: parent.verticalCenter
                                elide: Text.ElideRight
                            }
                        }
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
