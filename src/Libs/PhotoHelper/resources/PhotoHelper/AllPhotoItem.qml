import QtQuick 2.0
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoController

  property string currentPhotoIndexText: elementsCount + " " + qsTr("фото")
  property string currentPhotoNameText: ""

  property int photoSize: 200
  property int photoSpacing: 10

  property int previousIndex: -1 // индекс предыдущего выбранного элемента

  function updateFocus() {
    view.forceActiveFocus()
  }

  function forwardClicked() {
  }

  function backClicked() {
  }

  function copyPhoto(path) {
    photoController.copySelectedPhotos(path)
  }

  function deletePhoto() {
    photoController.deleteSelectedPhotosFromSource()
  }

  function deletePhotoFromFolder(folderPath) {
    photoController.deleteSelectedPhotosFromDestination(folderPath)
  }

  function rotateRightPhoto() {
    photoController.rotateSelectedPhotosRight()
  }

  function setRowRange(select, first, last) {
    var selectedIndexes = photoController.selectedIndexes
    var start = first <= last ? first : last
    var stop = first <= last ? last : first
    for (var i = start ; i <= stop ; ++ i) {
        var indexAt = selectedIndexes.indexOf(i)
        if (select) {
            if (indexAt < 0)
                selectedIndexes.push(i)
        } else {
            if (indexAt > -1)
                selectedIndexes.splice(indexAt, 1);
        }
    }
    photoController.setSelectedIndexes(selectedIndexes)
  }

  ColumnLayout {
    anchors.fill: parent

    GridView {
      id: view

      Layout.fillWidth: true
      Layout.fillHeight: true

      cellWidth: photoSize
      cellHeight: cellWidth

      clip: true
      focus: true
      cacheBuffer: 20

      model: photoController.getPhotoModel()

      delegate:  Item {
        id: photoDelegate

        width: GridView.view.cellWidth
        height: GridView.view.cellHeight

        Rectangle {
          property bool isSelected: photoController.selectedIndexes.indexOf(index) !== -1

          anchors.fill: parent
          color: "lightsteelblue";
          visible: isSelected
        }

        ColumnLayout {
          anchors.fill: parent
          anchors.margins: photoSpacing

          Item {
            id: photoWrapper

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            // Изображение
            Image {
              id: photo

              asynchronous: true
              cache: true

              anchors.centerIn: parent

              width: (model.orientation === 0) ||
                     (model.orientation === 2) ||
                     (model.orientation === 4) ?
                       parent.width : parent.height
              height: (model.orientation === 0) ||
                      (model.orientation === 2) ||
                      (model.orientation === 4) ?
                       parent.height : parent.width

              verticalAlignment: (model.orientation === 0) ||
                                 (model.orientation === 2) ||
                                 (model.orientation === 4) ?
                                   Image.AlignBottom : Image.AlignVCenter

              fillMode: Image.PreserveAspectFit;
              source: "file:" + model.path

              rotation: 90 * model.orientation
            }

            // Индикаторы наличия в папках
            Row {
              anchors {
                top: parent.top
                right: parent.right
              }

              spacing: photoSize / 20

              Repeater {
                model: dircontains

                Rectangle {
                  color: modelData
                  width: photoSize / 10
                  height: width
                  border.width: 1
                }
              }
            }
          }

          Text {
            Layout.fillWidth: true

            text: model.name
            horizontalAlignment: Text.AlignHCenter
          }
        }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            if (mouse.modifiers & Qt.ShiftModifier) {
              setRowRange(true, previousIndex, index)
            } else {
              var hasIndex = photoController.selectedIndexes.indexOf(index) !== -1
              if (mouse.modifiers & Qt.ControlModifier) {
                setRowRange(!hasIndex, index, index)
              } else {
                photoController.setSelectedIndexes(
                      (photoController.selectedIndexes.length === 1 && hasIndex) ? [] : [index])
              }
            }
            previousIndex = index
          }
        }
      }

      MouseArea {
        property var factor

        anchors.fill: parent
        propagateComposedEvents: true
        onWheel: {
          if (wheel.modifiers & Qt.ControlModifier) {
            if (wheel.angleDelta.y > 0)
                factor = 1.25;
            else
                factor = 0.75;

            if(view.cellWidth * factor >= photoSize &&
               view.cellWidth * factor < view.height)
              view.cellWidth = view.cellWidth * factor
          }
          else
            wheel.accepted = false
        }
      }
    }
  }

  Component.onCompleted: {
    photoController.switchToSelectedIndexes()
    view.positionViewAtIndex(photoController.currentIndex, GridView.Beginning)
    updateFocus()
  }
}

