#include "FileOperationHandler.h"

#include <QDateTime>
#include <QDir>

#include <exiv2/exiv2.hpp>

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
                 (oldFileName + " (2)." + toFileInfo.suffix()));
        return;
      }
    }
  }

  QFile::copy(filePath, toFilePath);
}

void FileOperationHandler::copyFiles(const QStringList &filePathList,
                                     const QString &destinationPath)
{
  for(auto const& filePath : filePathList)
    copyFile(filePath, destinationPath);
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

QStringList FileOperationHandler::getImagesOrientationList(const QString &path)
{
  QStringList orientationList;

  auto pathList = getImagesPathList(path);
  for(auto &filePath : pathList)
  {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdString());
    assert (image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData& ed = image->exifData();
    if (ed.empty())
      std::string error = filePath.toStdString() + ": No Exif data found in the file";

    // Ориентация изображения (в какую сторону требуется повернуть)
    enum Orientation
    {
      Normal = 0, // exif = 1
      Left,       // exif = 8
      UpsideDown, // exif = 3
      Right       // exif = 6
    };

    auto orientationExif = ed["Exif.Image.Orientation"].toLong();
    Orientation orientation = Normal;
    if(orientationExif == 8)
      orientation = Right;
    else if(orientationExif == 3)
      orientation = UpsideDown;
    else if(orientationExif == 6)
      orientation = Left;

    orientationList.push_back(QString::number(orientation));
  }
  return orientationList;
}

void FileOperationHandler::setImageOrientation(const QString &filePath,
                                               int orientation)
{
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdString());
  assert (image.get() != 0);
  image->readMetadata();
  Exiv2::ExifData& ed = image->exifData();
  if (ed.empty())
    std::string error = filePath.toStdString() + ": No Exif data found in the file";

  // Ориентация изображения (в какую сторону требуется повернуть)
  enum Orientation
  {
    Normal = 0, // exif = 1
    Left,       // exif = 8
    UpsideDown, // exif = 3
    Right       // exif = 6
  };

  int orientationExif = 1;

  if(orientation == 1)
    orientationExif = 8;
  else if(orientation == 2)
    orientationExif = 3;
  else if(orientation == 3)
    orientationExif = 6;

  ed["Exif.Image.Orientation"] = uint16_t(orientationExif);
  image->setExifData(ed);
  image->writeMetadata();
}

} // !PhotoHelper
