import QtQuick 2.0
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  property int photoSize: 200
  property int photoSpacing: 10

  property var selectedIndexes: []
  property int previousIndex: -1
  property int updateCounter: 0 // for force re-evaliation of values in delegates

  ListView {
    id: photoListView
    model: photoModel
    delegate: Text {}
    visible: false
  }

  function copyPhoto(path) {
    if(selectedIndexes.length > 0) {
      fileOperationHandler.copyFiles(
            photoModel.getFilePathList(selectedIndexes),
            path)
    }
  }

  function deletePhoto() {
    if(selectedIndexes.length > 0) {
      fileOperationHandler.deleteFiles(
            photoModel.getFilePathList(selectedIndexes))
      photoModel.deleteItems(selectedIndexes)
      console.log(selectedIndexes)
      selectedIndexes = []
      photoModel.setSelectedIndexes(selectedIndexes)
      updateCounter++
    }
  }

  function setRowRange(select, first, last) {
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

      model: photoModel

      delegate:  Item {
        id: photoDelegate

        width: GridView.view.cellWidth
        height: GridView.view.cellHeight

        Rectangle {
          property bool isSelected: updateCounter && model.selected

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
              var hasIndex = selectedIndexes.indexOf(index) !== -1
              if (mouse.modifiers & Qt.ControlModifier) {
                setRowRange(!hasIndex, index, index)
              } else {
                selectedIndexes = (selectedIndexes.length == 1 && hasIndex) ? [] : [index]
                photoModel.setSelectedIndexes(selectedIndexes)
              }
            }
            previousIndex = index
            updateCounter++
          }
        }
      }
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true

      Layout.maximumHeight: 50

      Text {
          Layout.fillWidth: true
          Layout.fillHeight: true
          verticalAlignment: Text.AlignVCenter

          text: photoListView.count + " " + qsTr("фото")
      }
    }
  }
}

