#include "FileOperationHandler.h"

#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QDir>

#include <exiv2/exiv2.hpp>

#include <unordered_map>

namespace PhotoHelper {

QColor GetColorByName(const QString& name);

std::unordered_map<int, RightOrientation> exifOrientationMap = {
  {1, RightOrientation::Normal},
  {6, RightOrientation::Right},
  {3, RightOrientation::UpsideDown},
  {8, RightOrientation::Left}
};

RightOrientation orientationByExifNumber(int exifNumber)
{
  return exifOrientationMap.at(exifNumber);
}

std::unordered_map<RightOrientation, int> orientationExifMap = {
  {RightOrientation::Normal,     1},
  {RightOrientation::Right,      6},
  {RightOrientation::UpsideDown, 3},
  {RightOrientation::Left,       8}
};

int exifNumberByOrientation(RightOrientation o)
{
  return orientationExifMap.at(o);
}

//==============================================================================
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
        QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
        QDir::Time | QDir::Reversed);

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
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdWString());
    assert (image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData& ed = image->exifData();
    if (ed.empty())
      std::string error = filePath.toStdString() + ": No Exif data found in the file";

    auto orientationExif = ed["Exif.Image.Orientation"].toLong();
    RightOrientation orientation = orientationByExifNumber(orientationExif);
    orientationList.push_back(QString::number(orientation));
  }
  return orientationList;
}

// Поворот изображения вправо
void FileOperationHandler::rotateRightImage(const QString &filePath)
{
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdWString());
  assert (image.get() != nullptr);
  image->readMetadata();
  Exiv2::ExifData& ed = image->exifData();
  if (ed.empty())
    std::string error = filePath.toStdString() + ": No Exif data found in the file";

  QString exifOrientationTag("Exif.Image.Orientation");

  auto oldOrientationExif = ed[exifOrientationTag.toStdString().c_str()].toLong();
  auto oldOrientation = orientationByExifNumber(oldOrientationExif);

  auto newOrientation = RightOrientation::Normal;

  if(oldOrientation < RightOrientation::Left)
    newOrientation = static_cast<RightOrientation>(oldOrientation + 1);

  auto newOrientationExif = exifNumberByOrientation(newOrientation);

  ed[exifOrientationTag.toStdString().c_str()] = uint16_t(newOrientationExif);
  image->setExifData(ed);
  image->writeMetadata();
}

void FileOperationHandler::rotateRightImages(const QStringList &pathList)
{
  for(auto & path : pathList)
    rotateRightImage(path);
}

int FileOperationHandler::getImageOrientation(const QString &filePath)
{
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdWString());
  assert (image.get() != 0);
  image->readMetadata();
  Exiv2::ExifData& ed = image->exifData();
  if (ed.empty())
    qDebug() << filePath + ": No Exif data found in the file";

  auto orientationExif = ed["Exif.Image.Orientation"].toLong();
  RightOrientation orientation = orientationByExifNumber(orientationExif);

  return static_cast<int>(orientation);
}

QQmlPropertyMap* FileOperationHandler::getDestinationPathFilesCache(QStringList const& destinationPathList)
{
  QQmlPropertyMap *map = new QQmlPropertyMap;

  for(auto const& path : destinationPathList) {
    auto files = QDir(path).entryList(
    {"*.jpg", "*.jpeg"},
    QDir::Files |
    QDir::NoSymLinks |
    QDir::NoDotAndDotDot);
    map->insert(path, files);
  }
  return map;
}


} // !PhotoHelper
