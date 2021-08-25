import QtQuick 2.0

Rectangle {
    id: root
    radius: 6
    color: "#c05d06"
    height: 35
    width: 35

    signal press

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: press()
    }

    states: [
        State {
            name: "pressed"
            when: mouseArea.pressed
            PropertyChanges {
                target: root
                color: "#e87910"
            }
        },
        State {
            name: "hovered"
            when: mouseArea.containsMouse
            PropertyChanges {
                target: root
                color: "#e87910"
            }
        }
    ]
}
