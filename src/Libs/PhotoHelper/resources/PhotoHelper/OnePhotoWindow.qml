import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.13

import DestinationFolderModel 1.0
import FileOperationHandler 1.0
import FolderSet 1.0
import PhotoModel 1.0

Window {
  id: root

  width: 800
  height: 800

  property FolderSet folderSet
  property FileOperationHandler fileOperationHandler

  property int buttonWidth: 200
  property int currentIndex: -1

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

      Image {
        id: photo

        Layout.fillWidth: true
        height: parent.height - 50
        Layout.fillHeight: true

        cache: false
        fillMode: Image.PreserveAspectFit;
        source: if(currentIndex >= 0)
                  "file:" + listPhotoView.model[currentIndex]

        rotation: 0

        clip: true
      }

      RowLayout {
        anchors.margins: 10
        spacing: 50

        Layout.fillWidth: true

        Text {
          visible: currentIndex >= 0 ? true : false
          text: (currentIndex + 1) + " / " + listPhotoView.count
        }

        Text {
          visible: currentIndex >= 0 ? true : false
          text: photoModel.getFileName(currentIndex)
        }
      }
    }

    ColumnLayout {
      Layout.fillHeight: true

      Button {
        id: forwardButton
        text: qsTr("Вперед")
        implicitWidth: buttonWidth
        enabled: currentIndex < listPhotoView.count-1
        action: Action {
          text: qsTr("Вперед")
          shortcut: StandardKey.MoveToNextChar
        }
        onClicked: {
          photo.rotation = 0
          currentIndex++
        }
      }

      Button {
        id: backButton
        text: qsTr("Назад")
        implicitWidth: buttonWidth
        enabled: currentIndex != 0
        action: Action {
          text: qsTr("Назад")
          shortcut: StandardKey.MoveToPreviousChar
        }
        onClicked: {
          photo.rotation = 0
          currentIndex--
        }
      }

      Button {
        id: deleteButton
        text: qsTr("Удалить")
        implicitWidth: buttonWidth
        action: Action {
          text: qsTr("Удалить")
          shortcut: StandardKey.Delete
        }
        onClicked: {
          photo.rotation = 0
          cppFileOperationHandler.deleteFile(photoModel.getFilePath(currentIndex))
          photoModel.deleteItem(currentIndex)
        }
      }

      Button {
        id: rotateButton
        Layout.topMargin: 30
        text: qsTr("Повернуть")
        implicitWidth: buttonWidth
        onClicked: {
          photo.rotation += 90
        }
      }

      ListView {
        topMargin: 50
        Layout.fillHeight: true
        Layout.fillWidth: true

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
    }
  }

  Component.onCompleted: {
    photoModel.setData(fileOperationHandler.getImagesPathList(folderSet.sourcePath))
    destinationModel.init()

    if (listPhotoView.count > 0)
      currentIndex = 0
  }
}
