import QtQuick 2.12
import QtQuick.Layouts 1.11

Rectangle {
    id: statusRoot
    width: parent.width
    height: calculateHeight()
    color: "transparent"

    property var status: model.display

    Row {
        id: row

        topPadding: 4
        bottomPadding: 4
        leftPadding: 8
        rightPadding: 8

        spacing: 8

        Rectangle {
            width: 48
            height: 48

            Image {
                id: profileImage

                fillMode: Image.PreserveAspectFit
                width: 48
                height: 48
                mipmap: true

                source: model.display.avatarUrl
            }

            Image {
                id: interactProfileImage

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 24

                fillMode: Image.PreserveAspectFit
                width: 32
                height: 32
                mipmap: true

                source: model.display.interactAvatarUrl
            }
        }
        Column {
            id: column
            width: calculateContentWidth()

            Text {
                id: usernameText
                text: status.formattedAuthor

                font.bold: true

                renderType: Text.NativeRendering
            }

            Text {
                id: statusText
                width: parent.width
                text: status.content
                textFormat: Text.RichText // 그냥 두면 Text.StyledText로 되는 듯 함
                wrapMode: Text.WrapAnywhere

                renderType: Text.NativeRendering
            }

            Text {
                id: createdAtText
                text: status.createdAt
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
