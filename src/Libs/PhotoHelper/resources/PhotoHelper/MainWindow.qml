import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.13

import DataLoader 1.0
import DestinationFolderModel 1.0
import FolderSet 1.0
import PhotoController 1.0
import PhotoModel 1.0

Window {
  id: root

  property FolderSet folderSet

  property int buttonWidth: 100
  property int buttonMargin: 50

  property int elementsCount:  photoController.elementsCount

  function isCurrentOnePhotoItem() {
    return loader.source.toString().includes("OnePhotoItem.qml")
  }

  function isCurrentAllPhotoItem() {
    return loader.source.toString().includes("AllPhotoItem.qml")
  }

  function updateFocus() {
    loader.item.updateFocus()
  }

  function reloadData() {
    replacingText.text = qsTr("Загрузка изображений")
    progressBar.visible = true

    photoController.onStartReloadData(folderSet)

    dataLoader.startLoading(folderSet.sourcePath,
                            folderSet.getDestinationPathListAsList())
  }

  width: 800
  height: 800

  DataLoader {
    id: dataLoader
  }

  PhotoController {
    id: photoController
  }

  RowLayout {
    anchors.fill: parent
    anchors.margins: 10

    ColumnLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.maximumWidth: buttonWidth

      z: 10 // чтобы нижний текст был поверх фото

      CustomButton {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: isCurrentOnePhotoItem() ?
                       "qrc:/icons/all_photos" :
                       "qrc:/icons/one_photo"

        enabled: elementsCount > 0
        onClicked:
            if(isCurrentOnePhotoItem())
              loader.setSource("AllPhotoItem.qml",
                               {"photoController": photoController})
            else
              loader.setSource("OnePhotoItem.qml",
                               {"photoController": photoController})
      }

      CustomButton {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/forward"

        enabled: elementsCount > 0 &&
                 photoController.currentIndex > 0
        onClicked: loader.item.forwardClicked()
      }

      CustomButton {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/back"

        enabled: elementsCount > 0 &&
                 photoController.currentIndex  < elementsCount-1
        onClicked: loader.item.backClicked()
      }

      Text {
         Layout.fillHeight: true
      }

      // Имя текущего файла фотографии
      Text {
        visible: elementsCount > 0
        text: loader.item ? loader.item.currentPhotoNameText : ""
      }

      // Текущий индекс / Количество фотографий
      Text {
        visible: elementsCount > 0
        text: loader.item ? loader.item.currentPhotoIndexText : ""
      }
    }

    ColumnLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        visible: elementsCount == 0

        Text {
          id: replacingText

          anchors.centerIn: parent

          color: "gray"
          font.pointSize: 20
          textFormat: Text.PlainText
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
        }

        ProgressBar {
          id: progressBar

          anchors.topMargin: 20
          anchors.top: replacingText.bottom
          anchors.horizontalCenter: parent.horizontalCenter
          indeterminate: true

        }
      }

      Loader {
        id: loader

        Layout.fillWidth: true
        Layout.fillHeight: true

        visible: elementsCount != 0
      }      
    }

    ColumnLayout {
      id: buttonsLayout

      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.maximumWidth: buttonWidth

      CustomButton {
        id: deleteButton

        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/trash"

        enabled: elementsCount > 0
        action: Action {
          text: qsTr("Удалить")
          shortcut: StandardKey.Delete
        }
        onClicked: loader.item.deletePhoto()
      }

      CustomButton {
        id: rotateButton

        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/turn_right"

        enabled: elementsCount > 0 &&
                 photoController.isCurrentPhotoOrientationCorrect
        onClicked: loader.item.rotateRightPhoto()
      }

      ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        clip: true

        delegate: CustomToggleButton {
          implicitWidth: buttonWidth
          implicitHeight: buttonWidth

          checked: model.contains
          specialColor: model.color

          text: model.name
          enabled: elementsCount > 0
          action: Action {
            text: model.name
          }

          onCheckedChanged: {
            if(checked)
              loader.item.copyPhoto(model.path)
            else
              loader.item.deletePhotoFromFolder(model.path)
          }
        }
        model: photoController.getDestinationFolderModel()
      }      

      CustomButton {
        id: settingsButton

        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: "transparent"
        icon.source: "qrc:/icons/settings"

        onClicked: settingsDialog.show()
      }
    }
  }

  SettingsDialog {
    id: settingsDialog
    folderSet: root.folderSet
    title: root.title
    onSaveButtonClicked: reloadData()
    onCloseButtonClicked: updateFocus()
  }

  Connections {
    target: dataLoader
    onLoadingFinished: {
      replacingText.text = qsTr("Нет изображений")
      progressBar.visible = false

      photoController.onFinishReloadData(dataLoader.getSourcePhotoPathList(),
                                         dataLoader.getDestinationPathPhotosCache())

      if (elementsCount > 0) {
        loader.setSource("OnePhotoItem.qml",
                         {"photoController": photoController})
      }
    }
  }

  Component.onCompleted: {
    reloadData()
  }

  // Сохранение индекса фото при закрытии программы
  onVisibilityChanged: if(!visible)
                         folderSet.setLastOperatedIndex(
                               photoController.currentIndex)
}
