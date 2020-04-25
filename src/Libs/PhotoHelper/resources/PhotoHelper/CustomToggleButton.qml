import QtQuick 2.0
import QtQuick.Controls 2.12

CustomButton {
  id: control

  property color specialColor: "#d91c26"

  checkable: true

  background: Rectangle {
    anchors.fill: parent
    border.color:  specialColor
    border.width: control.down ? 5 : 3
    radius: 2

    Rectangle {
      anchors.fill: parent
      anchors.margins: 5
      opacity: control.checked ? 0.3 : 0.1
      color: control.checked ? specialColor : control.hovered ? "#dadce0" : "transparent"
    }
  }
}
