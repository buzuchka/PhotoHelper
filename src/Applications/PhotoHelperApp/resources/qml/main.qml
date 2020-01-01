import QtQuick 2.11

import 'qrc:/PhotoHelper'

InitialDialog {
    id: appWindow

    title: "QML Photo Helper"

    folderSet: cppFolderSet
    fileOperationHandler: cppFileOperationHandler
    proxy: cppProxy

    Component.onCompleted: show()
}
