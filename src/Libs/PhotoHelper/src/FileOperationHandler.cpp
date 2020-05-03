#include "FileOperationHandler.h"

#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QDir>

#include <exiv2/exiv2.hpp>

#include <unordered_map>

namespace PhotoHelper {

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
QString FileOperationHandler::copyFile(const QString &filePath,
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
      return QString();
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

          return copyFile(filePath, destinationPath +
                          QDir::separator(),
                          (oldFileName.replace(start, mas.size(), QString::number(++i)) +
                           "." +
                           fromFileInfo.suffix()));
        }
      }
      else {
        return copyFile(filePath,
                        destinationPath,
                        (oldFileName + " (2)." + toFileInfo.suffix()));
      }
    }
  }

  QFile::copy(filePath, toFilePath);
  return QFileInfo(toFilePath).fileName();
}

void FileOperationHandler::deleteFile(const QString &filePath)
{
  QFile::remove(filePath);
}

void FileOperationHandler::deleteFileFromFolder(const QString &photoFilePath,
                                                const QString &folderPath)
{
  QString destFileName;
  if(FileOperationHandler::isFolderContainsFile(folderPath,
                                                photoFilePath,
                                                destFileName))
    QFile::remove(folderPath + QDir::separator() + destFileName);
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

int FileOperationHandler::getImageOrientation(const QString &filePath)
{
  RightOrientation orientation = Undefined;
  try
  {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdWString());
    assert (image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData& ed = image->exifData();
    if (ed.empty())
      qDebug() << filePath + ": No Exif data found in the file";

    auto orientationExif = ed["Exif.Image.Orientation"].toLong();
    orientation = orientationByExifNumber(orientationExif);
  }
  catch(std::exception const& ex)
  {
    qDebug() << ex.what();
  }

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

bool FileOperationHandler::isFolderContainsFile(QString const& folderPath,
                                                QString const& filePath,
                                                QString &fileName)
{
  QFileInfo sourceFileInfo(filePath);
  QString sourceFileName = sourceFileInfo.baseName();

  // Получить первые цифры из названия фото, если оно начинается на IMG_
  if(sourceFileName.startsWith("IMG_"))
    sourceFileName = sourceFileName.mid(4, 4);

  auto destPhotoInfoList =
      QDir(folderPath).entryInfoList({"*" + sourceFileName + "*"});

  for(auto const& destPhotoInfo : destPhotoInfoList)
  {
    if(sourceFileInfo.size() == destPhotoInfo.size() &&
       sourceFileInfo.lastModified() == destPhotoInfo.lastModified())
    {
      fileName = destPhotoInfo.fileName();
      return true;
    }
  }
  return false;
}

} // !PhotoHelper
