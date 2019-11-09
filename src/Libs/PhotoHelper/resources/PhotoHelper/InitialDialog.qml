import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3

Item {
  id: root

  width: 800
  height: 300

  Column {
    id: column

    anchors.fill: parent
    anchors.margins: 10

    spacing: 10


    Repeater {
      id: repeater

      property int clickedIndex: 0

      model: 5

      Item {
        property alias folderPath: pathTextField.text

        implicitWidth: parent.width
        implicitHeight: chooseFolderButton.height

        RowLayout{
          anchors.fill: parent

          spacing: 10

          TextField {
            id: nameTextField

            width: 500

            text: index == 0 ? qsTr("Для разбора") : ""
            placeholderText: qsTr("Введите имя")
          }
          TextField {
            id: pathTextField

            Layout.fillWidth: true
          }
          Button {
            id: chooseFolderButton

            height: 40

            text: qsTr("Выберите папку")

            onClicked: {
              repeater.clickedButtonIndex = index
              fileDialog.open()
            }
          }
        }
      }
    }
  }

  FileDialog {
    id: fileDialog

    title: qsTr("Выберите папку")
    selectFolder: true
    selectMultiple: false
    folder: shortcuts.home

    onAccepted: {
      repeater.itemAt(repeater.clickedIndex).folderPath = fileDialog.fileUrl
      close()
    }
    onRejected: {
      close()
    }
  }
}
