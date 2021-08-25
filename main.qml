import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Lighter 1.0


Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

//    LighterAdapter {
//        id: lighter
//    }




//    ColumnLayout {
//        anchors.fill: parent
//        Row {
//            spacing: 10
//            ComboBox {
//                id: serialNames
//                height: 25
//                width: 150
//                enabled: !lighter.opened

//                model: lighter.serials
//            }
//            Button {
//                text: lighter.opened ? "Stop" : "Start"
//                height: 25
//                width: 75

//                onPressed: {
//                    lighter.opened ? lighter.close() : lighter.open(serialNames.currentText);
//                }
//            }
//        }

//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Красный")
//            }
//            Slider {
//                id: redValue
//                from: 0
//                value: 1
//                to: 1

//                onMoved: {
//                    var col = lighter.color
//                    col.r = redValue.value
//                    lighter.updateColor(col)
//                }
//            }
//        }

//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Зеленый")
//            }
//            Slider {
//                id: greenValue
//                from: 0
//                value: 1
//                to: 1
//                onMoved: {
//                    var col = lighter.color
//                    col.g = greenValue.value
//                    lighter.updateColor(col)
//                }
//            }
//        }

//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Синий")
//            }
//            Slider {
//                id: blueValue
//                from: 0
//                value: 1
//                to: 1

//                onMoved: {
//                    var col = lighter.color
//                    col.b = blueValue.value
//                    lighter.updateColor(col)
//                }
//            }
//        }

//        Item {
//            Layout.fillWidth: true
//            height: 50
//        }

//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Имя:")
//            }
//            TextField {
//                id: apName
//                height: 25
//                width: 250

//                onEditingFinished: {
//                    focus = false
//                }
//            }

//        }


//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Пароль:")
//            }
//            TextField {
//                id: apPass
//                text: lighter.apPassword
//                height: 25
//                width: 250

//                onEditingFinished: {
//                    focus = false
//                }

//            }
//        }

//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Тип точки доступа:")
//            }
//            ComboBox {
//                id: apType

//                height: 25
//                width: 250
//                model: ["Точка доступа", "Клиент"]

//                currentIndex: lighter.apType == LighterAdapter.SOFTAP ? 0 : 1
//            }
//        }

//        Row {
//            Text {
//                height: 25
//                width: 150
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Идентификатор:")
//            }
//            TextField {
//                id: apId
//                text: lighter.id
//                height: 25
//                width: 250
//                validator: IntValidator{ bottom: 0; top: 255; }
//                onEditingFinished: {
//                    focus = false
//                }
//            }
//        }

//        Row {
//            Rectangle {
//                height: 25
//                width: 75
//                color: lighter.color


//            }

//            Button {
//                height: 25
//                enabled: lighter.opened
//                text: qsTr("Установить настройки")

//                onPressed: {

//                    lighter.apName = apName.text
//                    lighter.apPassword = apPass.text
//                    lighter.apType = apType.currentIndex == 1 ? LighterAdapter.SOFTAP : LighterAdapter.STATION
//                    lighter.id = Number(apId.text)

//                    lighter.updateProperty()
//                }
//            }

//            Button {
//                height: 25
//                enabled: lighter.opened
//                text: qsTr("Запросить настройки")

//                onPressed: {
//                    lighter.requestProperty()
//                }
//            }

//        }


//        Item {
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//        }

//        Text {
//            id: statusText
//        }

//        Connections {
//            target: lighter

//            onPropertyRecieved: {
//                apName.text = lighter.apName
//                apPass.text = lighter.apPassword
//                apType.currentIndex = lighter.apType == LighterAdapter.SOFTAP ?  0 : 1
//                apId.text = String(lighter.id)

//            }

//            onCurrentRequestChanged: {
//                //console.log(lighter.currentRequest)

//                switch (lighter.currentRequest)
//                {
//                    case LighterAdapter.Timeout_Request:
//                        statusText.text = qsTr("Превышено время ожидания ответа")
//                        break
//                    case LighterAdapter.None_Request:
//                        statusText.text = qsTr("Готов")
//                        break
//                    default:
//                        statusText.text = qsTr("В состоянии запроса")
//                        break

//                }
//            }
//        }
//    }
}
