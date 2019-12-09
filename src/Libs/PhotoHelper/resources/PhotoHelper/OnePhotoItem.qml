import QtQuick 2.5
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  property int mainCurrentIndex: 0

  property int outsideIndex: -1

  function copyPhoto(path) {
    fileOperationHandler.copyFile(
          photoModel.getFilePath(mainCurrentIndex),
          path)
  }

  function deletePhoto() {
    fileOperationHandler.deleteFile(photoModel.getFilePath(mainCurrentIndex))
    photoModel.deleteItem(mainCurrentIndex)
  }

  ColumnLayout {
    anchors.fill: parent

    ListView {
      id: photoListView

      Layout.fillWidth: true
      Layout.fillHeight: true

      clip: true
      interactive: false
      orientation: ListView.Horizontal

      model: photoModel

      delegate: Item {
        id: photoWrapper

        width: ListView.view.width === 0 ? 600 : ListView.view.width
        height: ListView.view.height

        clip: true

        Image {
          id: photo

          anchors.fill: parent

          cache: false
          fillMode: Image.PreserveAspectFit
          source: "file:" + model.path

          rotation: 0
        }
      }

      Keys.onLeftPressed: {
        if(photoListView.count > 0 && mainCurrentIndex >= 1) {
          mainCurrentIndex--
          photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
        }
      }
      Keys.onRightPressed: {
        if(mainCurrentIndex < photoListView.count - 1) {
          mainCurrentIndex++
          photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
        }
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

          visible: mainCurrentIndex >= 0 ? true : false
          text: (mainCurrentIndex + 1) + " / " + photoListView.count
        }
      }

      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Text {
          anchors.fill: parent
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignRight

          visible: mainCurrentIndex >= 0
          text: photoModel.getFileName(mainCurrentIndex)
        }
      }
    }
  }
  Component.onCompleted: {
    photoListView.forceActiveFocus()

    if(outsideIndex > 0) {
      mainCurrentIndex = outsideIndex
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
    }
  }
}
