import QtQuick 2.12
import QtQuick.Layouts 1.11

Rectangle {
    id: statusRoot
    width: parent.width
    height: calculateHeight()
    color: "transparent"

    property var status: model.display

    signal rightClicked(var status)

    Rectangle {
        // Qt 5.10부터 이런거 사용할 수 있게 되긴 했는데..
        // https://doc.qt.io/archives/qt-5.10/qml-qtquick-shapes-shape.html
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        width: parent.width
        height: 1

        color: "#cacaca"
    }

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 8

        width: 32

        Image {
            id: boostIcon
            fillMode: Image.PreserveAspectFit
            width: 16
            height: 16
            mipmap: true

            visible: model.display.boosted

            source: "qrc:/icons/retweet.svg" // TODO: boost icon
        }
        Image {
            id: favouriteIcon

            anchors.left: boostIcon.right

            fillMode: Image.PreserveAspectFit
            width: 16
            height: 16
            mipmap: true

            visible: model.display.favourited

            source: "qrc:/icons/star.svg" // TODO: boost icon
        }
    }

    Column {
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
                color: "transparent"

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

                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: -4

                    fillMode: Image.PreserveAspectFit
                    width: 24
                    height: 24
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

                Grid {
                    id: attachmentsGrid
                    columns: 4
                    spacing: 4
                    Repeater {
                        model: status.images

                        Image {
                            fillMode: Image.PreserveAspectCrop
                            width: 128
                            height: 128
                            mipmap: true

                            source: modelData
                        }
                    }
                }

                function calculateContentWidth() {
                    return statusRoot.width - (
                        parent.spacing + parent.leftPadding + parent.rightPadding + profileImage.width
                    );
                }

                function calculateContentHeight() {
                    return usernameText.contentHeight +
                           statusText.contentHeight +
                           createdAtText.contentHeight +
                           attachmentsGrid.childrenRect.height;
                }
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
                timeline.currentIndex = index;
                statusRoot.rightClicked(status, mouse.x, mouse.y);
            }
        }
    }

    function calculateHeight() {
        return (row.topPadding + row.bottomPadding) +
               Math.max(column.calculateContentHeight(), profileImage.height);
    }
}
