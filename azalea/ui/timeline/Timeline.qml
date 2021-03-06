import QtQuick 2.3
import QtQuick.Controls 2.3

import "qrc:/components" as Components

Rectangle {
    id: root
    signal rightClicked(var status)

    ListView {
        anchors.fill: parent

        SystemPalette {
            id: systemPalette
            colorGroup: SystemPalette.Active
        }

        id: timeline
        objectName: "timeline"
        model: timelineModel
        focus: true
        highlight: Rectangle { color: systemPalette.highlight }
        highlightFollowsCurrentItem: true
        highlightResizeDuration: 0
        highlightMoveDuration: 0

        delegate: Components.Status {
            Component.onCompleted: {
                rightClicked.connect(root.rightClicked);
            }
        }

        ScrollBar.vertical: ScrollBar {}


    }
}
