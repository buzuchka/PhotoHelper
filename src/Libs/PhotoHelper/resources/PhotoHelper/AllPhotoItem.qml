import QtQuick 2.0
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  property int photoSize: 200
  property int photoSpacing: 10

  property int previousIndex: -1 // индекс предыдущего выбранного элемента
  property int outsideIndex: -1  // индекс элемента, который необходимо установить текущим выбранным

  property int mainCurrentIndex: photoModel.selectedIndexes.lenght > 0 ?
                                   photoModel.selectedIndexes[0] :
                                   0

  function copyPhoto(path) {
      fileOperationHandler.copyFiles(
            photoModel.getFilePathList(photoModel.selectedIndexes),
            path)
  }

  function deletePhoto() {
      fileOperationHandler.deleteFiles(
            photoModel.getFilePathList(selectedIndexes))
      photoModel.deleteItems(photoModel.selectedIndexes)
      photoModel.setSelectedIndexes([])
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

          Image {
            id: photo

            Layout.fillWidth: true
            Layout.fillHeight: true

            verticalAlignment: Image.AlignBottom

            cache: false
            fillMode: Image.PreserveAspectFit;
            source: "file:" + model.path
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
                factor = 2.0;
            else
                factor = 0.5;

            if(view.cellWidth * factor >= photoSize &&
               view.cellWidth * factor < view.height)
              view.cellWidth = view.cellWidth * factor
          }
          else
            wheel.accepted = false
        }
      }
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Layout.maximumHeight: 30

      Text {
          Layout.fillWidth: true
          Layout.fillHeight: true
          verticalAlignment: Text.AlignVCenter

          text: view.count + " " + qsTr("фото")
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

