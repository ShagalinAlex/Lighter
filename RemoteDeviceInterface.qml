import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

import Lighter 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    LighterAdapterSerial {
        id: lighter
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        //interactive: false
        Rectangle {
            color: "#0dbad1"
            ColumnLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillWidth: true
                    height: headerText.implicitHeight + 10
                    radius: 6
                    color: "#0d495c"

                    Text {
                        id: headerText
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Управление светом")
                        font.pixelSize: 18
                        color: "white"
                    }
                }



                MyConicalGradient {
                    id: colorPicker

                    property color selectesColor: "white"

                    Layout.fillWidth: true
                    height: 300

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            var current = colorPicker.getColor(mouse.x, mouse.y);


                            lighter.color = current === Qt.black ? lighter.color : colorPicker.getColor(mouse.x, mouse.y)
                            lighter.updateColor(lighter.color)
                        }

                        onPositionChanged: {
                            if(pressed && containsMouse)
                            {
                                var current = colorPicker.getColor(mouse.x, mouse.y);
                                lighter.color = current === Qt.black ? lighter.color : colorPicker.getColor(mouse.x, mouse.y)
                                lighter.updateColor(lighter.color)
                            }
                        }
                    }
                }
                Rectangle {
                    id: example
                    height: 30
                    Layout.fillWidth: true
                    color: lighter.color
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Text {
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

                    onValueChanged: {
                        lighter.brightness = value
                        lighter.updateColor(lighter.color)
                    }
                }



                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    radius: 6
                    color: "#0d495c"

                    RowLayout {
                        anchors.fill: parent
                        anchors.rightMargin: 3
                        anchors.leftMargin: 3
                        spacing: 3
                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                        CustomButton {
                            height: 35
                            width: 35

                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                sourceSize.width: 30
                                sourceSize.height: 30

                                source: "qrc:/Images/icon_settings.png"
                            }

                            onPress: {
                                swipeView.currentIndex = 1
                            }
                        }
                    }
                }
            }
        }


        Rectangle {
            color: "#0dbad1"
            ColumnLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillWidth: true
                    height: settingsHeaderText.implicitHeight + 10
                    radius: 6
                    color: "#0d495c"

                    Text {
                        id: settingsHeaderText
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Настройки")
                        font.pixelSize: 18
                        color: "white"
                    }
                }

                Settings { Layout.fillHeight: true; Layout.fillWidth: true; adapter: lighter }

                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    radius: 6
                    color: "#0d495c"

                    RowLayout {
                        anchors.fill: parent
                        anchors.rightMargin: 3
                        anchors.leftMargin: 3
                        spacing: 3
                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                        CustomButton {
                            height: 35
                            width: 35

                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                sourceSize.width: 30
                                sourceSize.height: 30

                                source: "qrc:/Images/icon_back.png"
                            }

                            onPress: swipeView.currentIndex = 0
                        }
                    }
                }

            }
        }


        AvaInterface {
            id: avaInterface
        }

    }

}
