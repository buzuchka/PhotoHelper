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

  ListView {
    id: photoListView
    model: photoModel
    delegate: Text {}
    visible: false
  }

  RowLayout {
    anchors.fill: parent
    anchors.margins: 10

    ColumnLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Text {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignCenter

        visible: photoListView.count == 0
        text: qsTr("Нет изображений")
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

        visible: photoListView.count != 0
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
          enabled: isCurrentAllPhotoItem()
          onClicked: loader.setSource("OnePhotoItem.qml",
                                      {"photoModel": photoModel,
                                       "fileOperationHandler": fileOperationHandler,
                                       "outsideIndex": loader.item.selectedIndexes[0]
                                      })
        }

        Button {
          Layout.fillWidth: true

          text: "All"
          enabled:isCurrentOnePhotoItem()
          onClicked: loader.setSource("AllPhotoItem.qml",
                                      {"photoModel": photoModel,
                                       "fileOperationHandler": fileOperationHandler,
                                       "outsideIndex": loader.item.mainCurrentIndex
                                      })
        }
      }

      Button {
        id: rotateButton

        Layout.fillWidth: true

        text: qsTr("Повернуть")
      }

      Button {
        id: deleteButton

        Layout.fillWidth: true

        text: qsTr("Удалить")
        enabled: photoListView.count > 0
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
          action: Action {
            text: model.name
            shortcut: "Ctrl+"+(index+1)
          }
          onClicked: loader.item.copyPhoto(model.path)
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

    if (photoListView.count > 0) {
      loader.setSource("OnePhotoItem.qml",
                       {"photoModel": photoModel,
                        "fileOperationHandler": fileOperationHandler})
    }
  }
}
