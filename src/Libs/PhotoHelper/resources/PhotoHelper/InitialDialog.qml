import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.13

import FolderSet 1.0
import FileOperationHandler 1.0

Window {
  id: root

  property FolderSet folderSet
  property FileOperationHandler fileOperationHandler

  property int firstColumnWidth: 200

  function saveModel(model, set) {
    for(var i = 0; i < model.count; ++i) {
      var listItem = model.get(i);
      set.setDestinationPath(i, listItem.name, listItem.path)
    }
  }

  function loadSet(set, model) {
    model.clear()
    var list = set.getDestinationVariantList()
    for(var i = 0; i < list.length; ++i) {
      model.append({name: list[i].first,
                    path: list[i].second})
    }

    if(model.count === 0)
      model.append({name: "", path: ""})
  }

  signal destinationChooseFolderButtonClicked(int index)

  width: 800
  height: 350

  ColumnLayout {
    id: column

    anchors.fill: parent
    anchors.margins: 10

    spacing: 10

    RowLayout {
      spacing: 10

      TextField {
        id: sourceNameTextField
        implicitWidth: firstColumnWidth
        enabled: false
        color: "black"
      }
      TextField {
        id: sourcePathTextField
        Layout.fillWidth: true
        enabled: false
        placeholderText: qsTr("Выберите папку")
      }
      Button {
        id: sourceChooseFolderButton
        text: qsTr("Выберите папку")

        onClicked: sourceFolderDialog.open()
      }
    }

    ListView {
      id: listView

      Layout.fillWidth: true
      Layout.fillHeight: true

      clip: true

      delegate: Item {
        property alias folderPath: pathTextField.text

        implicitWidth: parent.width
        implicitHeight: chooseFolderButton.height

        RowLayout {
          anchors.fill: parent

          spacing: 10

          TextField {
            id: nameTextField
            implicitWidth: firstColumnWidth

            text: model.name
            color: acceptableInput ? "black" : "red"
            validator: RegExpValidator {
              regExp: /[a-zA-Z0-9а-яА-я]{1,20}/
            }
            placeholderText: qsTr("Введите имя")
            onEditingFinished: listModel.get(index).name = text
          }
          TextField {
            id: pathTextField

            Layout.fillWidth: true

            text: model.path
            enabled: false
            onEditingFinished: listModel.get(index).path = text

          }
          Button {
            id: chooseFolderButton
            text: qsTr("Выберите папку")
            onClicked: destinationChooseFolderButtonClicked(index)
          }
        }
      }

      model: ListModel {
        id: listModel
    }
    }

    RowLayout {
      Layout.fillWidth: true

      Item {
        Layout.fillWidth: true

        height: addFolderButton.height

        RowLayout {
          Layout.alignment: Qt.AlignLeft

          ToolButton {
            id: addFolderButton


            text: qsTr("Добавить")

            onClicked: {
              listModel.append({name: "", path: ""})
            }
          }

          ToolButton {
            id: deleteFolderButton

            text: qsTr("Удалить")

            enabled: listModel.count > 0 ? true : false

            onClicked: {
              listModel.remove(listModel.count - 1)
            }
          }
        }
      }

      ToolButton {
        id: continueButton

        Layout.alignment: Qt.AlignRight

        text: qsTr("Дальше")

        enabled: listModel.count > 0 ? true : false

        onClicked: {
          folderSet.setSourcePath(sourceNameTextField.text,
                                  sourcePathTextField.text)
          saveModel(listModel, folderSet)

          root.visible = false

          var component = Qt.createComponent("MainWindow.qml")
          var window    = component.createObject(root,
                                                 {folderSet: root.folderSet,
                                                  fileOperationHandler: root.fileOperationHandler,
                                                  title: root.title})
          window.show()
        }
      }
    }
  }

  Component.onCompleted: {
    sourceNameTextField.text = folderSet.getSourceName()
    if(sourceNameTextField.text.length === 0)
      sourceNameTextField.text = qsTr("Исходный каталог")

    sourcePathTextField.text = folderSet.getSourcePath()

    loadSet(folderSet, listModel)
  }

  Connections {
    target: root
    onDestinationChooseFolderButtonClicked: {
      destinationFolderDialog.clickedIndex = index
      destinationFolderDialog.open()
    }
  }

  FileDialog {
    id: sourceFolderDialog

    title: qsTr("Выберите папку")
    selectFolder: true
    selectMultiple: false
    folder: (sourcePathTextField.text.length != 0) ?
              sourcePathTextField.text :
              shortcuts.home

    onAccepted: {
      var fileUrlString = sourceFolderDialog.fileUrl.toString()
      if(fileUrlString.startsWith("file:///"))
        fileUrlString = fileUrlString.replace("file:///","")
      if(fileUrlString.startsWith("file://"))
        fileUrlString = fileUrlString.replace("file:","")
      sourcePathTextField.text = fileUrlString
      close()
    }
    onRejected: {
      close()
    }
  }

  FileDialog {
    id: destinationFolderDialog

    property int clickedIndex: 0

    title: qsTr("Выберите папку")
    selectFolder: true
    selectMultiple: false
    folder: shortcuts.home

    onAccepted: {
      var fileUrlString = destinationFolderDialog.fileUrl.toString()
      if(fileUrlString.startsWith("file:///"))
        fileUrlString = fileUrlString.replace("file:///","")
      if(fileUrlString.startsWith("file://"))
        fileUrlString = fileUrlString.replace("file:","")
      listModel.get(clickedIndex).path = fileUrlString
      close()
    }
    onRejected: {
      close()
    }
  }
}
