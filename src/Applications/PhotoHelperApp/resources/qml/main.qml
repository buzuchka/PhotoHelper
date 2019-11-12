import QtQuick 2.11

import 'qrc:/PhotoHelper'

OnePhotoWindow {
    id: appWindow

    folderSet: cppFolderSet
    fileOperationHandler: cppFileOperationHandler
}
