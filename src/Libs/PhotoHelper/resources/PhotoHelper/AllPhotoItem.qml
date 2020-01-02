import QtQuick 2.0
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  property string currentPhotoIndexText: view.count + " " + qsTr("фото")
  property string currentPhotoNameText: ""

  property int photoSize: 200
  property int photoSpacing: 10

  property int previousIndex: -1 // индекс предыдущего выбранного элемента
  property int outsideIndex: -1  // индекс элемента, который необходимо установить текущим выбранным

  // Для передачи при переключении между режимами
  property int mainCurrentIndex1: photoModel.selectedIndexes.length > 0 ?
                                     photoModel.selectedIndexes[0] :
                                     0

  function forwardClicked() {
  }

  function backClicked() {
  }

  function copyPhoto(path) {
      fileOperationHandler.copyFiles(
            photoModel.getFilePathList(photoModel.selectedIndexes),
            path)
  }

  function deletePhoto() {
      fileOperationHandler.deleteFiles(
            photoModel.getFilePathList(photoModel.selectedIndexes))
      photoModel.deleteItems(photoModel.selectedIndexes)
      photoModel.setSelectedIndexes([])
  }

  function rotateRightPhoto() {
    fileOperationHandler.rotateRightImages(
          photoModel.getFilePathList(photoModel.selectedIndexes))
    photoModel.rotateRightSelectedIndexes()
  }

  function setRowRange(select, first, last) {
    var selectedIndexes = photoModel.selectedIndexes
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
    photoModel.setSelectedIndexes(selectedIndexes)
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

      model: photoModel

      delegate:  Item {
        id: photoDelegate

        width: GridView.view.cellWidth
        height: GridView.view.cellHeight

        Rectangle {
          property bool isSelected: photoModel.selectedIndexes.indexOf(index) !== -1

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

            Image {
              id: photo

              anchors.centerIn: parent

              width: (model.orientation === 0) || (model.orientation === 2) ?
                       parent.width : parent.height
              height: (model.orientation === 1) || (model.orientation === 3) ?
                       parent.height : parent.width

              verticalAlignment: (model.orientation === 0) || (model.orientation === 2) ?
                                   Image.AlignBottom : Image.AlignVCenter

              fillMode: Image.PreserveAspectFit;
              source: "file:" + model.path

              rotation: 90 * model.orientation
            }
          }

          Text {
            id: contactInfo

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
              var hasIndex = photoModel.selectedIndexes.indexOf(index) !== -1
              if (mouse.modifiers & Qt.ControlModifier) {
                setRowRange(!hasIndex, index, index)
              } else {
                photoModel.setSelectedIndexes(
                      (photoModel.selectedIndexes.length === 1 && hasIndex) ? [] : [index])
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
    view.forceActiveFocus()

    if(outsideIndex > 0) {
      view.positionViewAtIndex(outsideIndex, GridView.Beginning)
      photoModel.setSelectedIndexes([outsideIndex])
    }
  }
}

