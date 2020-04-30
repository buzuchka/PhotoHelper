import QtQuick 2.11

import 'qrc:/PhotoHelper'

MainWindow {
    id: appWindow

    title: "QML Photo Helper"

    folderSet: cppFolderSet
    fileOperationHandler: cppFileOperationHandler
    proxy: cppProxy

    Component.onCompleted: showMaximized()
}
