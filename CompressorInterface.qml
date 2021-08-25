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
            var status = device.updateStatus()
        }
    }

    CompressorAdapter {
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
                ColumnLayout {
                    anchors.fill: parent
                    anchors.bottomMargin: 5
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Rectangle {
                            color: "#c05d06"
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 10

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.verticalCenter: parent.verticalCenter

                                Text {
                                    color: "#ffffff"
                                    text: qsTr("Давление")
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 25
                                    Layout.leftMargin: 10
                                }

                                RowLayout {
                                    Layout.alignment: Qt.AlignHCenter
                                    Text {
                                        width: 150
                                        color: "#ffffff"
                                        text: device.pressure.toFixed(1)
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 30
                                        font.bold: true
                                    }
                                    Text {
                                        color: "#ffffff"
                                        text: qsTr("(")
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 30
                                    }
                                    Text {
                                        color: "#ffffff"
                                        text: device.pressureSelected.toFixed(1)
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 30
                                    }
                                    Text {
                                        color: "#ffffff"
                                        text: qsTr(")")
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 30
                                    }

                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#c05d06"
                            radius: 10

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.verticalCenter: parent.verticalCenter
                                Text {
                                    color: "#ffffff"
                                    text: qsTr("Температура")
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 25
                                    Layout.leftMargin: 10
                                }

                                RowLayout {
                                    Layout.alignment: Qt.AlignHCenter
                                    Text {
                                        width: 150
                                        color: "#ffffff"
                                        text: device.temperature.toFixed(1)
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 30
                                        font.bold: true
                                    }
                                    Text {
                                        width: 120
                                        color: "#ffffff"
                                        text: qsTr("°C")
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        opacity: 0.8
                                        font.pixelSize: 30
                                    }
                                }
                            }
                        }

                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#c05d06"
                        radius: 10

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.verticalCenter: parent.verticalCenter
                            RowLayout {
                                Layout.leftMargin: 10

                                Text {
                                    color: "#ffffff"
                                    text: qsTr("Заданное давление")
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 25
                                }
                            }

                            Slider {
                                orientation: Qt.Horizontal
                                Layout.fillWidth: true
                                from: 2
                                to: 10
                                stepSize: 0.1
                                value: device.pressureSelected
                                snapMode: Slider.SnapOnRelease


                                onValueChanged: {
                                    device.pressureSelected = value
                                }
                            }

                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ColumnLayout {
                            anchors.fill: parent
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                color: "#ffffff"
                                text: qsTr("Масло")
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: 25
                            }

                            Rectangle {
                                Layout.fillHeight: true

                                Layout.alignment: Qt.AlignHCenter
                                width: 100

                                radius: height / 3

                                //width: height
                                color: device.oilPressure ? "green" : "red"

                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                color: "#ffffff"
                                text: device.compressorState == CompressorAdapterData.ENSTATE_IDLE ?
                                          "Остановлен" :
                                          device.compressorState == CompressorAdapterData.ENSTATE_STARTING ?
                                            "Запуск" :
                                              device.compressorState == CompressorAdapterData.ENSTATE_STOPING ?
                                                "Остановка" :
                                                  device.compressorState == CompressorAdapterData.ENSTATE_COMPRESSING ?
                                                    "Работа" : "Неизвестно"


                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: 25
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: 5
                        Layout.rightMargin: 5


                        Rectangle {
                            color: "green"
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            radius: 10

                            Text {
                                color: "#ffffff"
                                text: qsTr("CТАРТ")
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                anchors.centerIn: parent
                                font.pixelSize: 35
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: device.enabled = true
                            }
                        }

                        Rectangle {
                            color: "red"
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 10

                            Text {
                                color: "#ffffff"
                                text: qsTr("CТОП")
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                anchors.centerIn: parent
                                font.pixelSize: 35
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: device.enabled = false
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

