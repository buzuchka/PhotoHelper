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

      Text {
        visible: elementsCount > 0
        text: loader.item ? loader.item.currentPhotoNameText : ""
      }

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
        Layout.preferredWidth: buttonWidth
        Layout.preferredHeight: buttonWidth

        icon.height: buttonWidth
        icon.width: buttonWidth
        icon.color: enabled ? "transparent" : "lightgrey"
        icon.source: "qrc:/icons/one_photo"

        enabled: isCurrentAllPhotoItem()
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

        enabled:isCurrentOnePhotoItem()
        onClicked: loader.setSource("AllPhotoItem.qml",
                                    {"photoModel": photoModel,
                                     "fileOperationHandler": fileOperationHandler,
                                     "outsideIndex": loader.item.mainCurrentIndex
                                    })
      }

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
          text: model.name
          enabled: elementsCount > 0
          action: Action {
            text: model.name
            shortcut: "Ctrl+"+(index+1)
          }
          onClicked: loader.item.copyPhoto(model.path)
        }
        model: DestinationFolderModel {
          id: destinationButtonModel
        }
      }      
    }
  }

  Connections {
    target: proxy
    onLoadingFinished: {
      replacingText.text = qsTr("Нет изображений")
      photoModel.setData(proxy.getImagesPathList())

      if (elementsCount > 0) {
        loader.setSource("OnePhotoItem.qml",
                         {"photoModel": photoModel,
                          "fileOperationHandler": fileOperationHandler})
      }
    }
  }

  Component.onCompleted: {
    replacingText.text = qsTr("Загрузка изображений")

    destinationButtonModel.init()
    proxy.startLoading(folderSet.sourcePath)
  }
}
