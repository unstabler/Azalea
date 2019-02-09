import QtQuick 2.12
import QtQuick.Layouts 1.11

Rectangle {
    id: statusRoot
    width: parent.width
    height: calculateHeight()
    color: "transparent"

    Row {
        id: row

        topPadding: 4
        bottomPadding: 4
        leftPadding: 8
        rightPadding: 8

        spacing: 8

        Image {
            id: profileImage

            fillMode: Image.PreserveAspectFit
            width: 48
            height: 48
            mipmap: true

            source: 'file:///home/cheesekun/IMG_0724.PNG'
        }
        Column {
            id: column
            width: calculateContentWidth()

            Text {
                id: usernameText
                text: "치즈군★ / @cheesekun@twingyeo.kr"

                font.bold: true

                renderType: Text.NativeRendering
            }

            Text {
                id: statusText
                width: parent.width
                text: modelData
                wrapMode: Text.WrapAnywhere

                renderType: Text.NativeRendering
            }

            Text {
                id: createdAtText
                text: "2099년 99월 99일 화요일 오후 99:99:99"
                renderType: Text.NativeRendering
            }

            function calculateContentWidth() {
                return statusRoot.width - (
                    parent.spacing + parent.leftPadding + parent.rightPadding + profileImage.width
                );
            }

            function calculateContentHeight() {
                return usernameText.contentHeight +
                       statusText.contentHeight +
                       createdAtText.contentHeight;
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                timeline.currentIndex = index;
            } else {
                // right button
                console.log(this);
                console.log("TODO: display context menu");
            }
        }
    }

    function calculateHeight() {
        return (row.topPadding + row.bottomPadding) +
               Math.max(column.calculateContentHeight(), profileImage.height);
    }
}
