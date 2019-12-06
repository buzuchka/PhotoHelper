import QtQuick 2.0
import QtQuick.Layouts 1.13

Item {
  id: root

  property var model

  property int photoSize: 200
  property int photoSpacing: 10

  property var selectedIndexes: []
  property int previousIndex: -1
  property int updateCounter: 0 // for force re-evaliation of values in delegates

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
      model.setSelectedIndexes(selectedIndexes)
  }

  GridView {
    id: view

    anchors.fill: parent

    cellWidth: photoSize
    cellHeight: cellWidth

    clip: true
    focus: true

    model: root.model

    delegate:  Item {
      id: photoDelegate

      property var view: GridView.view

      width: view.cellWidth
      height: view.cellHeight

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
              root.model.setSelectedIndexes(selectedIndexes)
            }
          }
          previousIndex = index
          updateCounter++
        }
      }
    }
  }
}

