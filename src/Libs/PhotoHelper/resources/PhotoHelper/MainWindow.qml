import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.13

import DestinationFolderModel 1.0
import FileOperationHandler 1.0
import FolderSet 1.0
import PhotoModel 1.0

import Proxy 1.0

Window {
  id: root

  property FolderSet folderSet
  property FileOperationHandler fileOperationHandler
  property Proxy proxy

  property int buttonWidth: 100
  property int buttonMargin: 50

  property int elementsCount:  photoModel.elementsCount

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
    photoModel.clear()

    destinationButtonModel.init()

    photoModel.setLastOperatedIndex(folderSet.getLastOperatedIndex())
    photoModel.setDestinationPathList(folderSet.getDestinationPathListAsList())
    photoModel.setDestinationPathNameList(folderSet.getDestinationPathNameListAsList())

    proxy.startLoading(folderSet.sourcePath, folderSet.getDestinationPathListAsList())
  }

  width: 800
  height: 800

  PhotoModel {
    id: photoModel
  }

  RowLayout {
    anchors.fill: parent
    anchors.margins: 10

    ColumnLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.maximumWidth: buttonWidth

      z: 10 // чтобы нижний текст был поверх фото

      Button {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/one_photo"

        enabled: (elementsCount > 0) && isCurrentAllPhotoItem()
        onClicked: loader.setSource("OnePhotoItem.qml",
                                    {"photoModel": photoModel,
                                     "fileOperationHandler": fileOperationHandler,
                                     "outsideIndex": loader.item.mainCurrentIndex1
                                    })
      }

      Button {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/all_photos"

        enabled: (elementsCount > 0) && isCurrentOnePhotoItem()
        onClicked: loader.setSource("AllPhotoItem.qml",
                                    {"photoModel": photoModel,
                                     "fileOperationHandler": fileOperationHandler,
                                     "outsideIndex": loader.item.mainCurrentIndex
                                    })
      }

      Button {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/forward"

        enabled: elementsCount > 0
        onClicked: loader.item.forwardClicked()
      }

      Button {
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/back"

        enabled: elementsCount > 0
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

      Text {
        id: replacingText

        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignCenter

        visible: elementsCount == 0
        color: "gray"
        font.pointSize: 20
        textFormat: Text.PlainText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
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

      Button {
        id: rotateButton

        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/turn_right"

        enabled: elementsCount > 0
        onClicked: loader.item.rotateRightPhoto()
      }

      Button {
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

      ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        clip: true

        delegate: Button {
          implicitWidth: buttonWidth
          implicitHeight: buttonWidth

          text: model.name
          enabled: elementsCount > 0
          action: Action {
            text: model.name
            shortcut: "Ctrl+"+(index+1)
          }
          onClicked: loader.item.copyPhoto(model.path)

          Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 5

            width: 15
            height: width

            border.width: 1
            color: model.color
          }
        }
        model: DestinationFolderModel {
          id: destinationButtonModel
        }
      }      

      Button {
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
    target: proxy
    onLoadingFinished: {
      replacingText.text = qsTr("Нет изображений")
      photoModel.setData(proxy.getImagesPathList())
      photoModel.setDestinationPathFilesCache(proxy.getDestinationPathFilesCache())

      if (elementsCount > 0) {
        loader.setSource("OnePhotoItem.qml",
                         {"photoModel": photoModel,
                          "fileOperationHandler": fileOperationHandler,
                          "outsideIndex": folderSet.getLastOperatedIndex()})
      }
    }
  }

  Component.onCompleted: {
    reloadData()
  }

  onVisibilityChanged: if(!visible)
                         folderSet.setLastOperatedIndex(loader.item.mainCurrentIndex)
}
