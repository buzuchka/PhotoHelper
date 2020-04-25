import QtQuick 2.0
import QtQuick.Controls 2.12

Button {
  id: control

  background: Rectangle {
    anchors.fill: parent
    opacity: enabled ? 1 : 0.3
    color: control.hovered ? "#dadce0" : "transparent"
    border.color: control.down ? "#6d6d6e" : "transparent"
    border.width: 1
    radius: 2
  }
}
