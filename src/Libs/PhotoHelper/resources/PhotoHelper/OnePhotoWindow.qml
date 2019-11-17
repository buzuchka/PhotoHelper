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
  property int currentIndex: 0

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

    Image {
      id: photo

      Layout.fillWidth: true
      Layout.fillHeight: true

      cache: false
      fillMode: Image.PreserveAspectFit;
      Component.onCompleted: source = "file:" + listPhotoView.model[currentIndex]
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
        onClicked: currentIndex++
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
        onClicked: currentIndex--
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
          cppFileOperationHandler.deleteFile(photoModel.getFilePath(currentIndex))
          photoModel.deleteItem(currentIndex)

          if(currentIndex < listPhotoView.count-1)
            currentIndex++
          else if(listPhotoView.count > 0)
            currentIndex--
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
  }
}
