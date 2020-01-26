import QtQuick 2.5
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  property string currentPhotoIndexText: (mainCurrentIndex + 1) + " / " + elementsCount + " " + qsTr("фото")
  property string currentPhotoNameText

  property int mainCurrentIndex: 0

  property int outsideIndex: -1

  property bool once: true

  function updateFocus() {
    photoListView.forceActiveFocus()
  }

  function forwardClicked() {
    if(mainCurrentIndex < elementsCount - 1) {
      mainCurrentIndex++
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
      updateCurrentNameLabel()
      updateFocus()
    }
  }

  function backClicked() {
    if(elementsCount > 0 && mainCurrentIndex >= 1) {
      mainCurrentIndex--
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
      updateCurrentNameLabel()
      updateFocus()
    }
  }

  function copyPhoto(path) {
    fileOperationHandler.copyFile(
          photoModel.getFilePath(mainCurrentIndex),
          path)
    photoModel.emitUpdateData(mainCurrentIndex)
    updateFocus()
  }

  function deletePhoto() {
    fileOperationHandler.deleteFile(photoModel.getFilePath(mainCurrentIndex))
    photoModel.deleteItem(mainCurrentIndex)
    if(mainCurrentIndex == elementsCount - 1)
      mainCurrentIndex--
    updateCurrentNameLabel()
    updateFocus()
  }

  function rotateRightPhoto() {
    photoModel.rotateRight(mainCurrentIndex)
    updateFocus()
  }

  function updateCurrentNameLabel() {
    currentPhotoNameText = photoModel.getFileName(mainCurrentIndex)
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

        // Изображение
        Image {
          id: photo

          anchors.centerIn: parent

          width: (model.orientation === 0) || (model.orientation === 2) ?
                   parent.width : parent.height
          height: (model.orientation === 0) || (model.orientation === 2) ?
                   parent.height : parent.width

          fillMode: Image.PreserveAspectFit
          source: "file:" + model.path

          rotation: 90 * model.orientation
        }

        // Индикаторы наличия в папках
        Row {
          anchors {
            top: parent.top
            right: parent.right
          }

          spacing: 5

          Repeater {
            model: dircontains

            Rectangle {
              color: modelData
              width: 20
              height: 20
              border.width: 2
            }
          }
        }
      }

      Keys.onLeftPressed: {
        backClicked()
      }
      Keys.onRightPressed: {
        forwardClicked()
      }

      // Костыль для корректного отображения конкретного элемента при загрузке программы
      onCountChanged: {
        if(count > 1) {
          if(once) {
            backClicked()
            forwardClicked()
            once = false
          }
        }
      }
    }
  }
  Component.onCompleted: {
    if(outsideIndex > 0) {
      mainCurrentIndex = outsideIndex
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
    }
    updateCurrentNameLabel()
    updateFocus()
  }
}
