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
    photoListView.forceActiveFocus()
  }

  function deletePhoto() {
    fileOperationHandler.deleteFile(photoModel.getFilePath(mainCurrentIndex))
    photoModel.deleteItem(mainCurrentIndex)
    photoListView.forceActiveFocus()
    if(mainCurrentIndex == elementsCount - 1)
      mainCurrentIndex--
    updateCurrentNameLabel()
  }

  function rotatePhoto() {
    var oldOrientation = photoModel.getOrientation(mainCurrentIndex)
    var newOrientation = 0;

    if(oldOrientation < 3)
      newOrientation = oldOrientation + 1;

    photoModel.setOrientation(mainCurrentIndex, newOrientation)
    fileOperationHandler.setImageOrientation(photoModel.getFilePath(mainCurrentIndex),
                                             newOrientation)
    photoListView.forceActiveFocus()
  }

  function updateCurrentNameLabel() {
    photoNameLabel.text = photoModel.getFileName(mainCurrentIndex)
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

          anchors.centerIn: parent

          width: (model.orientation === 0) || (model.orientation === 2) ?
                   parent.width : parent.height
          height: (model.orientation === 0) || (model.orientation === 2) ?
                   parent.height : parent.width

          cache: false
          fillMode: Image.PreserveAspectFit
          source: "file:" + model.path

          rotation: 90 * model.orientation
        }
      }

      Keys.onLeftPressed: {
        if(elementsCount > 0 && mainCurrentIndex >= 1) {
          mainCurrentIndex--
          photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
          updateCurrentNameLabel()
        }
      }
      Keys.onRightPressed: {
        if(mainCurrentIndex < elementsCount - 1) {
          mainCurrentIndex++
          photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
          updateCurrentNameLabel()
        }
      }
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Layout.maximumHeight: 30

      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Text {
          anchors.fill: parent
          verticalAlignment: Text.AlignVCenter

          visible: mainCurrentIndex >= 0 ? true : false
          text: (mainCurrentIndex + 1) + " / " + elementsCount + " " + qsTr("фото")
        }
      }

      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Text {
          id: photoNameLabel

          anchors.fill: parent
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignRight

          visible: elementsCount > 0 && mainCurrentIndex >= 0
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
    updateCurrentNameLabel()
  }
}
