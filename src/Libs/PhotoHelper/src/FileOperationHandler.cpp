#include "FileOperationHandler.h"

#include <QDir>

namespace PhotoHelper {

FileOperationHandler::FileOperationHandler()
{
}

void FileOperationHandler::copyFile(const QString &filePath,
                                    const QString &destinationPath)
{
  QString toFilePath(destinationPath +
                     "/" +
                     QFileInfo(filePath).fileName());

  QFile::copy(filePath, toFilePath);
}

void FileOperationHandler::deleteFile(const QString &filePath)
{
  QFile::remove(filePath);
}

QStringList FileOperationHandler::getImagesPathList(const QString &path)
{
  auto nameList = QDir(path).entryList(
        QStringList() << "*.jpg" << "*.jpeg",
        QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

  for(auto & name : nameList)
    name.prepend(path + "/");

  return nameList;
}


} // !PhotoHelper
