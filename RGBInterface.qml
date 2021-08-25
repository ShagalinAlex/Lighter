import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.0

import ESPDevice 1.0

Rectangle {
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

    color: "#e87910"

    Settings {
        id: settingsAdapter
    }

    RBGControl {
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

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainView

        ColumnLayout {
            id: mainView

            MyConicalGradient {
                id: colorPicker

                property color selectedColor: "white"

                Layout.fillWidth: true
                height: 400

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        var current = colorPicker.getColor(mouse.x, mouse.y);


                        device.color = current === Qt.black ? device.color : colorPicker.getColor(mouse.x, mouse.y)
                    }

                    onPositionChanged: {
                        if(pressed && containsMouse)
                        {
                            var current = colorPicker.getColor(mouse.x, mouse.y);
                            device.color = current === Qt.black ? device.color : colorPicker.getColor(mouse.x, mouse.y)
                            colorPicker.selectedColor = current
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                //Layout.margins: 5
                height: 35

                ListView {
                    id: colorSelector
                    anchors.fill: parent
                    model: 5
                    orientation: ListView.Horizontal
                    interactive: false

                    delegate: MouseArea {
                        id: savedColorItem
                        height: 40
                        width: root.width / 5
                        property bool selected: ListView.isCurrentItem


                        onClicked: {
                            colorSelector.currentIndex = model.index
                            //console.log(savedColorRect.color)
                            device.color = savedColorRect.color
                        }

                        Rectangle {
                            id: savedColorRect
                            anchors.fill: parent
                            anchors.margins: 2


                            property int index: 0
                            color: "white"
                            border.width: 1
                            border.color: savedColorItem.selected ? "steelblue" : "black"

                            Component.onCompleted: {
                                var settingsName = "RGBColor#" + String(model.index)
                                savedColorRect.color = settingsAdapter.value(settingsName, "white")
                            }

                            Connections {
                                target: colorPicker
                                onSelectedColorChanged: {
                                    if(!savedColorItem.selected)
                                        return
                                    savedColorRect.color = colorPicker.selectedColor
                                    var settingsName = "RGBColor#" + String(model.index)
                                    settingsAdapter.setValue(settingsName, savedColorRect.color)
                                }
                            }
                        }
                    }
                }
            }

            Text {
                height: 35
                Layout.fillWidth: true

                font.pixelSize: 18
                color: "white"
                text: qsTr("Яркость")
            }
            Slider {
                Layout.fillWidth: true
                height: 30
                from: 0.0
                to: 1.0
                value: 1.0
                stepSize: 0.05
                snapMode: Slider.SnapAlways

                onValueChanged: {
                    device.brightness = value
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Rectangle {
                height: 40
                Layout.fillWidth: true
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

