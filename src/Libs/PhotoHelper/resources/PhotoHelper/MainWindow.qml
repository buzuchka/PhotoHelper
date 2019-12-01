import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.13

import DestinationFolderModel 1.0
import FileOperationHandler 1.0
import FolderSet 1.0
import PhotoModel 1.0

Window {
  id: root

  property FolderSet folderSet
  property FileOperationHandler fileOperationHandler

  property int buttonWidth: 200
  property int buttonMargin: 50
  property int currentIndex: -1

  function updatePhotoByIndex (){
    if(!loader.source.toString().includes("OnePhotoItem.qml"))
      return;
    if(currentIndex >=0)
      loader.item.photoFilePath = listPhotoView.model[currentIndex]
  }

  function resetRotation() {
    if(!loader.source.toString().includes("OnePhotoItem.qml"))
      return;
    if(currentIndex >=0)
      loader.item.rotation = 0
  }

  width: 800
  height: 800

  PhotoModel {
    id: photoModel
  }

  ListView {
    id: listPhotoView
    model: photoModel.data
    delegate: Text {}
    visible: false
  }

  RowLayout {
    anchors.fill: parent
    anchors.margins: 10

    ColumnLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Loader {
        id: loader

        Layout.fillWidth: true
        Layout.fillHeight: true
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
            text: (currentIndex + 1) + " / " + listPhotoView.count
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

    ColumnLayout {
      id: buttonsLayout

      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.maximumWidth: buttonWidth

      RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.maximumWidth: buttonWidth

        Button {
          Layout.fillWidth: true

          text: "One"
          enabled: !loader.source.toString().includes("OnePhotoItem.qml")
//            onClicked: loader.setSource("Fragment3.qml", {"opacity": 0.5})
          onClicked: {
            loader.setSource("OnePhotoItem.qml")
            updatePhotoByIndex()
          }
        }

        Button {
          Layout.fillWidth: true

          text: "All"
          enabled: !loader.source.toString().includes("AllPhotoItem.qml")
          onClicked: loader.setSource("AllPhotoItem.qml")
        }
      }

      Button {
        id: rotateButton

        Layout.fillWidth: true

        text: qsTr("Повернуть")
        enabled: currentIndex > 0
        onClicked: {
          photo.rotation += 90
        }
      }

      Button {
        id: deleteButton

        Layout.fillWidth: true

        text: qsTr("Удалить")
        enabled: currentIndex >= 0
        action: Action {
          text: qsTr("Удалить")
          shortcut: StandardKey.Delete
        }
        onClicked: {
          resetRotation()
          cppFileOperationHandler.deleteFile(photoModel.getFilePath(currentIndex))
          photoModel.deleteItem(currentIndex)
          updatePhotoByIndex()
        }
      }

      ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        clip: true

        delegate: Button {
          implicitWidth: buttonWidth
          text: model.name
          action: Action {
            text: model.name
            shortcut: "Ctrl+"+(index+1)
          }
          onClicked: {fileOperationHandler.copyFile(
                       photoModel.getFilePath(currentIndex),
                       model.path)}
        }
        model: DestinationFolderModel {
          id: destinationModel
        }
      }

      Button {
        id: forwardButton

        Layout.fillWidth: true

        text: qsTr("Вперед")
        enabled: currentIndex < listPhotoView.count-1
        action: Action {
          text: qsTr("Вперед")
          shortcut: StandardKey.MoveToNextChar
        }
        onClicked: {
          resetRotation()
          currentIndex++
          updatePhotoByIndex()
        }
      }

      Button {
        id: backButton

        Layout.fillWidth: true

        text: qsTr("Назад")
        enabled: currentIndex > 0
        action: Action {
          text: qsTr("Назад")
          shortcut: StandardKey.MoveToPreviousChar
        }
        onClicked: {
          resetRotation()
          currentIndex--
          updatePhotoByIndex()
        }
      }
    }
  }
  Component.onCompleted: {
    photoModel.setData(fileOperationHandler.getImagesPathList(folderSet.sourcePath))
    destinationModel.init()

    if (listPhotoView.count > 0) {
      loader.setSource("OnePhotoItem.qml")
      currentIndex = 0
      updatePhotoByIndex()
    }
  }
}
