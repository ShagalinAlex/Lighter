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

    BME280Adapter {
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
                                font.pixelSize: 35
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
                                    font.pixelSize: 50
                                    font.bold: true
                                }
                                Text {
                                    width: 120
                                    color: "#ffffff"
                                    text: qsTr("°C")
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    opacity: 0.8
                                    font.pixelSize: 50
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
                                text: qsTr("Влажность")
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: 35
                                Layout.leftMargin: 10
                            }

                            RowLayout {
                                Layout.alignment: Qt.AlignHCenter
                                Text {
                                    width: 150
                                    color: "#ffffff"
                                    text: device.humidity.toFixed(1)
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 50
                                    font.bold: true
                                }
                                Text {
                                    width: 120
                                    color: "#ffffff"
                                    text: qsTr("%")
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    opacity: 0.8
                                    font.pixelSize: 50
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
                                text: qsTr("Давление")
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: 35
                                Layout.leftMargin: 10
                            }

                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    width: 150
                                    color: "#ffffff"
                                    text: device.pressure.toFixed(0)
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 50
                                    font.bold: true
                                }

                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
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

