import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import ESPDevice 1.0

Rectangle {
    color: "#97500e"
    radius: 5

    signal leave

    Dialog {
        id: dialogOk
        title: "Сообщение"
        standardButtons: Dialog.Ok

        onVisibleChanged:   {
            if(Qt.platform.os == "windows")
                return

            if(!visible)
                return
            x = (parent.width - width) / 2
            y = (parent.height - height) / 2
        }

        Text {
            id: messageString
            anchors.centerIn: parent
            text: qsTr("Данные обновлены")
        }

        onAccepted: close()
    }

    SerialAdapter {
        id: adapter
    }

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
                text: qsTr("Сервисные настройки")
                font.pixelSize: 18
                color: "white"
            }
        }

        Row {

            spacing: 10
            ComboBox {
                id: serialNames
                height: 25
                width: 150
                enabled: !adapter.opened

                model: adapter.serials
            }
            Button {
                text: adapter.opened ? "Stop" : "Start"
                height: 25
                width: 75

                onPressed: {
                    if(adapter)
                    {

                        if(adapter.opened)
                        {
                            adapter.close()
                        }
                        else
                        {
                            adapter.open(serialNames.currentText)
                            var settings = adapter.requestSettings()
                            if(settings) {
                                apName.text = settings.apName
                                apPass.text = settings.apPass
                                deviceName.text = settings.devName
                                apType.currentIndex = settings.apType === 2 ? 0 : 1
                            }

                            var mqttData = adapter.requestMQTTSettings()
                            if(mqttData) {
                                mqttUri.text = mqttData.uri
                                mqttName.text = mqttData.name
                                mqttPass.text = mqttData.passwd
                                mqttTopic.text = mqttData.topic
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Имя устройства:")
                }
            }
            TextField {
                id: deviceName
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                selectByMouse: true

                onEditingFinished: {
                    focus = false
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Имя WIFI:")
                }
            }
            TextField {
                id: apName
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                selectByMouse: true
                onEditingFinished: {
                    focus = false
                }
            }
        }


        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Пароль WIFI:")
                }
            }
            TextField {
                id: apPass
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                echoMode: TextInput.PasswordEchoOnEdit
                selectByMouse: true

                onEditingFinished: {
                    focus = false
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 180

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Тип точки доступа:")
                }
            }
            ComboBox {
                id: apType

                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18

                model: ["Точка доступа", "Клиент"]

                currentIndex: 0
            }
        }

        RowLayout {
            Button {
                height: 25
                Layout.fillWidth: true
                enabled: adapter.opened
                text: qsTr("Установить настройки")
                font.pixelSize: 15

                onPressed: {
                    var settings = {
                        apName:apName.text,
                        apPass:apPass.text,
                        apType: apType.currentIndex == 0 ? 2 : 1,
                        devName: deviceName.text,
                    }
                    if(adapter.saveSettings(settings) === 0)
                    {
                        messageString.text = "Данные обновлены"
                        dialogOk.open()
                    }
                    else
                    {
                        messageString.text = "Устройство не ответило"
                        dialogOk.open()
                    }
                }
            }

            Button {
                height: 25
                Layout.fillWidth: true
                enabled: adapter.opened
                text: qsTr("Запросить настройки")
                font.pixelSize: 15

                onPressed: {
                    var settings = adapter.requestSettings()
                    if(!settings)
                    {
                        messageString.text = "Устройство не ответило"
                        dialogOk.open()
                        return
                    }
                    apName.text = settings.apName
                    apPass.text = settings.apPass
                    deviceName.text = settings.devName
                    apType.currentIndex = settings.apType === 2 ? 0 : 1
                    messageString.text = "Данные обновлены"
                    dialogOk.open()
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("MQTT uri:")
                }
            }
            TextField {
                id: mqttUri
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                selectByMouse: true

                onEditingFinished: {
                    focus = false
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Логин MQTT:")
                }
            }
            TextField {
                id: mqttName
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                selectByMouse: true

                onEditingFinished: {
                    focus = false
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Пароль MQTT:")
                }
            }
            TextField {
                id: mqttPass
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                selectByMouse: true
                echoMode: TextInput.PasswordEchoOnEdit

                onEditingFinished: {
                    focus = false
                }
            }
        }

        RowLayout {
            Item {
                height: 25
                width: 160

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: qsTr("Topic MQTT:")
                }
            }
            TextField {
                id: mqttTopic
                height: 25
                Layout.fillWidth: true
                font.pixelSize: 18
                selectByMouse: true

                onEditingFinished: {
                    focus = false
                }
            }
        }

        RowLayout {
            Button {
                height: 25
                Layout.fillWidth: true
                enabled: adapter.opened
                text: qsTr("Установить настройки MQTT")
                font.pixelSize: 15

                onPressed: {
                    var settings = {
                        name: mqttName.text,
                        passwd: mqttPass.text,
                        uri: mqttUri.text,
                        topic: mqttTopic.text,
                    }
                    if(adapter.saveMQTTSettings(settings) === 0)
                    {
                        messageString.text = "Данные обновлены"
                        dialogOk.open()
                    }
                    else
                    {
                        messageString.text = "Устройство не ответило"
                        dialogOk.open()
                    }
                }
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
            color: "#c05d06"

            RowLayout {
                anchors.fill: parent
                anchors.rightMargin: 3
                anchors.leftMargin: 3

                BackButton {
                    height: 35
                    width: 35
                    onPress: {
                        adapter.close()
                        leave()
                    }
                }
                Text {
                    id: statusText
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
    }
}
