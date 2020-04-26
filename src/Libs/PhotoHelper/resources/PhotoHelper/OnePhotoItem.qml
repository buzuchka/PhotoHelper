import QtQuick 2.5
import QtQuick.Layouts 1.13

Item {
  id: root

  property var photoModel
  property var fileOperationHandler

  property string currentPhotoIndexText: (mainCurrentIndex + 1) + " / " + elementsCount + " " + qsTr("фото")
  property string currentPhotoNameText

  property int mainCurrentIndex: 0

  property int outsideIndex: -1

  property bool once: true

  function updateFocus() {
    photoListView.forceActiveFocus()
  }

  function forwardClicked() {
    if(mainCurrentIndex < elementsCount - 1) {
      mainCurrentIndex++
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
      updateCurrentNameLabel()
      updateFocus()
    }
  }

  function backClicked() {
    if(elementsCount > 0 && mainCurrentIndex >= 1) {
      mainCurrentIndex--
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
      updateCurrentNameLabel()
      updateFocus()
    }
  }

  function copyPhoto(path) {
    fileOperationHandler.copyFile(
          photoModel.getFilePath(mainCurrentIndex),
          path)
    photoModel.onFileCopied(mainCurrentIndex, path);
    photoModel.emitUpdateData(mainCurrentIndex)
    updateFocus()
  }

  function deletePhoto() {
    fileOperationHandler.deleteFile(photoModel.getFilePath(mainCurrentIndex))
    photoModel.deleteItem(mainCurrentIndex)
    if(mainCurrentIndex == elementsCount - 1)
      mainCurrentIndex--
    updateCurrentNameLabel()
    updateFocus()
  }

  function deletePhotoFromFolder(folderPath) {
    fileOperationHandler.deletePhotoFromFolder(
          photoModel.getFilePath(mainCurrentIndex),
          folderPath)
    photoModel.emitUpdateData(mainCurrentIndex)
  }

  function rotateRightPhoto() {
    photoModel.rotateRight(mainCurrentIndex)
    updateFocus()
  }

  function updateCurrentNameLabel() {
    currentPhotoNameText = photoModel.getFileName(mainCurrentIndex)
  }

  ColumnLayout {
    anchors.fill: parent

    ListView {
      id: photoListView

      Layout.fillWidth: true
      Layout.fillHeight: true

      clip: true
      interactive: false
      orientation: ListView.Horizontal

      model: photoModel

      delegate: Item {
        id: photoWrapper

        width: ListView.view.width === 0 ? 600 : ListView.view.width
        height: ListView.view.height

        clip: true

        Flickable {
          id: flick

          anchors.fill: parent

          Rectangle {
            id: photoWrapperRect

            width: flick.width
            height: flick.height

            transform: Scale {
              id: scaler

              origin.x: pinchArea.m_x2
              origin.y: pinchArea.m_y2
              xScale: pinchArea.m_zoom2
              yScale: pinchArea.m_zoom2
            }

            // Изображение
            Image {
              id: photo

              z: dragArea.z+1
              anchors.centerIn: parent

              width: (model.orientation === 0) || (model.orientation === 2) ?
                       parent.width : parent.height
              height: (model.orientation === 0) || (model.orientation === 2) ?
                       parent.height : parent.width

              fillMode: Image.PreserveAspectFit
              source: "file:" + model.path

              rotation: 90 * model.orientation
            }

            PinchArea {
              id: pinchArea
              anchors.fill: parent
              property real m_x1: 0
              property real m_y1: 0
              property real m_y2: 0
              property real m_x2: 0
              property real m_zoom1: 0.5
              property real m_zoom2: 1
              property real m_max: 2
              property real m_min: 0.5

              function reset() {
                m_x2 = 0
                m_y2 = 0
                m_zoom2 = 1
              }

              onPinchStarted: {
                  console.log("Pinch Started")
                  m_x1 = scaler.origin.x
                  m_y1 = scaler.origin.y
                  m_x2 = pinch.startCenter.x
                  m_y2 = pinch.startCenter.y
                  rect.x = rect.x + (pinchArea.m_x1-pinchArea.m_x2)*(1-pinchArea.m_zoom1)
                  rect.y = rect.y + (pinchArea.m_y1-pinchArea.m_y2)*(1-pinchArea.m_zoom1)
              }
              onPinchUpdated: {
                  console.log("Pinch Updated")
                  m_zoom1 = scaler.xScale
                  var dz = pinch.scale-pinch.previousScale
                  var newZoom = m_zoom1+dz
                  if (newZoom <= m_max && newZoom >= m_min) {
                      m_zoom2 = newZoom
                  }
              }
              MouseArea {
                  id: dragArea
                  hoverEnabled: true
                  anchors.fill: parent
                  drag.target: photoWrapperRect
                  drag.filterChildren: true

                  onWheel: {
                      console.log("Wheel Scrolled")
                      pinchArea.m_x1 = scaler.origin.x
                      pinchArea.m_y1 = scaler.origin.y
                      pinchArea.m_zoom1 = scaler.xScale
                      pinchArea.m_x2 = mouseX
                      pinchArea.m_y2 = mouseY

                      var newZoom
                      if (wheel.angleDelta.y > 0) {
                          newZoom = pinchArea.m_zoom1+0.1
                          if (newZoom <= pinchArea.m_max) {
                              pinchArea.m_zoom2 = newZoom
                          } else {
                              pinchArea.m_zoom2 = pinchArea.m_max
                          }
                      } else {
                          newZoom = pinchArea.m_zoom1-0.1
                          if (newZoom >= pinchArea.m_min) {
                              pinchArea.m_zoom2 = newZoom
                          } else {
                              pinchArea.m_zoom2 = pinchArea.m_min
                          }
                      }
                      photoWrapperRect.x = photoWrapperRect.x + (pinchArea.m_x1-pinchArea.m_x2)*(1-pinchArea.m_zoom1)
                      photoWrapperRect.y = photoWrapperRect.y + (pinchArea.m_y1-pinchArea.m_y2)*(1-pinchArea.m_zoom1)

                      console.debug(photoWrapperRect.width+" -- "+photoWrapperRect.height+"--"+photoWrapperRect.scale)

                  }

                  onDoubleClicked: {
                    pinchArea.reset()

                  }
              }
            }
          }
        }

        // Индикаторы наличия в папках
        Row {
          anchors {
            top: parent.top
            right: parent.right
          }

          spacing: 5

          Repeater {
            model: dircontains

            Rectangle {
              color: modelData
              width: 20
              height: 20
              border.width: 2
            }
          }
        }
      }

      Keys.onLeftPressed: {
        backClicked()
      }
      Keys.onRightPressed: {
        forwardClicked()
      }

      // Костыль для корректного отображения конкретного элемента при загрузке программы
      onCountChanged: {
        if(count > 1) {
          if(once) {
            forwardClicked()
            backClicked()
            once = false
          }
        }
      }
    }
  }
  Component.onCompleted: {
    if(outsideIndex > 0) {
      mainCurrentIndex = outsideIndex
      photoListView.positionViewAtIndex(mainCurrentIndex, ListView.Beginning)
    }
    updateCurrentNameLabel()
    updateFocus()
  }
}
