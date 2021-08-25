import QtQuick 2.0

CustomButton {
    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        sourceSize.width: parent.width - 2
        sourceSize.height: parent.height - 2

        source: "qrc:/Images/icon_back.png"
    }
}

