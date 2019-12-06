import QtQuick 2.0
import QtQuick.Layouts 1.13

Item {
  id: root

  property var model

  property int photoSize: 200
  property int photoSpacing: 10

  GridView {
    id: view

    anchors.fill: parent

    cellWidth: photoSize
    cellHeight: cellWidth

    clip: true
    focus: true

    highlight: Rectangle {
      color: "lightsteelblue";
    }

    model: root.model

    delegate:  Item {
      id: photoDelegate

      property var view: GridView.view

      width: view.cellWidth
      height: view.cellHeight

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

          MouseArea {
            anchors.fill: parent
            onClicked: view.currentIndex = model.index
          }
        }

        Text {
          id: contactInfo

          Layout.fillWidth: true

          text: model.name
          horizontalAlignment: Text.AlignHCenter
        }
      }
    }
  }
}

