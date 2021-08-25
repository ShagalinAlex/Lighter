import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

import Lighter 1.0
import ESPDevice 1.0


Window {
    visible: true
    width: 480
    height: 640
    title: qsTr("Управление")

    AvaService {
        id: avaService
    }

    //Component.onCompleted: stackView.push(compressorInterface, {"address": "127.0.0.1", "name": "name"})

    Rectangle {
        anchors.fill: parent
        color: "lightgray"

        StackView {
            id: stackView
            anchors.fill: parent
            initialItem: mainView

            Component {
                id: contactDelegate
                Item {
                    width: contentItem.width
                    height: stackView.height / 4
                    Rectangle {
                        color: "#e87910"
                        anchors.fill: parent
                        anchors.margins: 5
                        radius: 3

                        Text { text: '<b>Имя:</b> ' + modelData.name; color: "white"; font.pixelSize: 24; anchors.centerIn: parent; }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                if(modelData.type === DataAdapter.DEV_TYPE_LIGHTER )
                                {
                                    stackView.push(rgbInterface, {"address": modelData.address})
                                }
                                if(modelData.type === DataAdapter.DEV_TYPE_GATE)
                                {
                                    stackView.push(gateInterface, {"address": modelData.address})
                                }
                                if(modelData.type === DataAdapter.DEV_TYPE_ONOFF)
                                {
                                    stackView.push(buttonInterface, {"address": modelData.address})
                                }
                                if(modelData.type === DataAdapter.DEV_TYPE_SENSOR_BME280)
                                {
                                    stackView.push(bme280Interface, {"address": modelData.address})
                                }
                                if(modelData.type === DataAdapter.DEV_TYPE_COMPRESSOR)
                                {
                                    stackView.push(compressorInterface, {"address": modelData.address})
                                }

                            }
                        }
                    }
                }
            }

            Component {
                id: settingsSerial

                SettingsSerial {
                    onLeave: {
                        stackView.pop()
                    }
                }
            }

            Component {
                id: mainView
                Rectangle {
                    color: "#97500e"

                    ColumnLayout {
                        anchors.fill: parent

                        Rectangle {
                            Layout.fillWidth: true
                            height: headerText.implicitHeight + 10
                            radius: 6
                            color: "#c05d06"

                            Text {
                                id: headerText
                                anchors.centerIn: parent
                                text: qsTr("Список устройств")
                                font.pixelSize: 18
                                color: "white"
                            }
                        }

                        ListView {
                            id: abonentList
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            model: avaService.abonents
                            delegate: contactDelegate

                            highlight: Item {}
                            focus: true
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 40
                            radius: 6
                            color: "#c05d06"
                            visible: Qt.platform.os != "android"

                            RowLayout {
                                anchors.fill: parent
                                anchors.rightMargin: 3
                                anchors.leftMargin: 3
                                spacing: 3
                                Item {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                                SettingsButton {
                                    height: 35
                                    width: 35

                                    onPress: {
                                        stackView.push(settingsSerial)
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: gateInterface
                GateInterface {
                    onLeave:
                        stackView.pop()
                    onError: {
                        console.log("Transfer error")
                        avaService.resetAbonents()
                        stackView.pop()
                    }
                }
            }

            Component {
                id: buttonInterface
                ONOFFInterface {
                    onLeave:
                        stackView.pop()
                    onError: {
                        console.log("Transfer error")
                        avaService.resetAbonents()
                        stackView.pop()
                    }
                }
            }
            Component {
                id: rgbInterface
                RGBInterface {
                    onLeave:
                        stackView.pop()
                    onError: {
                        console.log("Transfer error")
                        avaService.resetAbonents()
                        stackView.pop()
                    }
                }
            }

            Component {
                id: bme280Interface
                BME280Interface {
                    onLeave:
                        stackView.pop()
                    onError: {
                        console.log("Transfer error")
                        avaService.resetAbonents()
                        stackView.pop()
                    }
                }
            }

            Component {
                id: compressorInterface
                CompressorInterface {
                    onLeave:
                        stackView.pop()
                    onError: {
                        console.log("Transfer error")
                        avaService.resetAbonents()
                        stackView.pop()
                    }
                }
            }
        }
    }
}


