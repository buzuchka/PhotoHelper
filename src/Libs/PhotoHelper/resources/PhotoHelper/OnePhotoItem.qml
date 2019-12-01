import QtQuick 2.5

Item {
  id: photoWrapper

  property string photoFilePath
  property alias rotation: photo.rotation

  clip: true

  Image {
    id: photo

    anchors.fill: parent

    cache: false
    fillMode: Image.PreserveAspectFit;
    source: "file:" + photoFilePath

    rotation: 0
  }
}
