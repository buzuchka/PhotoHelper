#include "FileOperationHandler.h"

#include <QDateTime>
#include <QDir>

namespace PhotoHelper {

FileOperationHandler::FileOperationHandler()
{
}

void FileOperationHandler::copyFile(const QString &filePath,
                                    const QString &destinationPath,
                                    const QString &destinationFileName)
{
  QFileInfo fromFileInfo(filePath);

  QString toFilePath = destinationPath + QDir::separator();

  if(!destinationFileName.isEmpty())
    toFilePath.append(destinationFileName);
  else
    toFilePath.append(fromFileInfo.fileName());

  if(QFile::exists(toFilePath))
  {
    QFileInfo toFileInfo(toFilePath);
    // Если это один и тот же файл, выходим
    if((fromFileInfo.size() == toFileInfo.size()) &&
       (fromFileInfo.lastModified() == toFileInfo.lastModified()))
      return;
    else
    {
      // Если это разные файлы, но с одинаковыми именами, меняем имя назначения
      QFile toFile(toFilePath);
      QString oldFileName = toFileInfo.baseName();
      int i = 0;
      if(oldFileName.endsWith("[0-9])"))
      {
        int start = oldFileName.lastIndexOf(QRegExp("[0-9]"));
        QString mas = oldFileName.mid(start,1);
        if(!mas.isEmpty())
        {
          i = mas.toInt();

          copyFile(filePath, destinationPath +
                   QDir::separator(),
                   (oldFileName.replace(start, mas.size(), QString::number(++i)) +
                    "." +
                    fromFileInfo.suffix()));
          return;
        }
      }
      else {
        copyFile(filePath,
                 destinationPath,
                 (oldFileName +
                  " (2)." +
                  toFileInfo.suffix()));
        return;
      }
    }
  }

  QFile::copy(filePath, toFilePath);
}

void FileOperationHandler::deleteFile(const QString &filePath)
{
  QFile::remove(filePath);
}

void FileOperationHandler::deleteFiles(const QStringList &filePathList)
{
  for(auto const& filePath : filePathList)
    QFile::remove(filePath);
}

QStringList FileOperationHandler::getImagesPathList(const QString &path)
{
  auto nameList = QDir(path).entryList(
        QStringList() << "*.jpg" << "*.jpeg",
        QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

  for(auto & name : nameList)
    name.prepend(path + QDir::separator());

  return nameList;
}

} // !PhotoHelper
