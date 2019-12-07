import QtQuick 2.5
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  function copyPhoto(path) {
      fileOperationHandler.copyFile(
            photoModel.getFilePath(currentIndex),
            path)
  }

  function deletePhoto() {
      fileOperationHandler.deleteFile(photoModel.getFilePath(currentIndex))
      photoModel.deleteItem(currentIndex)
  }

  ListView {
    id: photoListView
    model: photoModel
    delegate: Text {}
    visible: false
  }

  ColumnLayout {
    anchors.fill: parent

    Item {
      id: photoWrapper

      Layout.fillWidth: true
      Layout.fillHeight: true

      clip: true

      Image {
        id: photo

        anchors.fill: parent

        cache: false
        fillMode: Image.PreserveAspectFit
        source: "file:" + photoModel.getFilePath(currentIndex)

        rotation: 0
      }
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Layout.maximumHeight: 50

      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Text {
          anchors.fill: parent
          verticalAlignment: Text.AlignVCenter

          visible: currentIndex >= 0 ? true : false
          text: (currentIndex + 1) + " / " + photoListView.count
        }
      }

      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Text {
          anchors.fill: parent
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignRight

          visible: currentIndex >= 0
          text: photoModel.getFileName(currentIndex)
        }
      }
    }
  }
}
