#ifndef PHOTOHELPER_FILEOPERATIONHANDLER_H
#define PHOTOHELPER_FILEOPERATIONHANDLER_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QFileInfo>
#include <QVariant>

#include <QtQml/QQmlPropertyMap>

namespace PhotoHelper {

enum RightOrientation
{
  Normal = 0, // exif = 1
  Right,      // exif = 6
  UpsideDown, // exif = 3
  Left,       // exif = 8
  Undefined   // no exif data
};

class PHOTOHELPER_EXPORT FileOperationHandler
{
public:
  //! Возвращает кеш с путями до файлов в каталогах destinationPathList
  static QQmlPropertyMap* getDestinationPathFilesCache(QStringList const& destinationPathList);

  //! Копирование файла filePath в папку destinationPath
  //! filePath - путь до файла, который копируем
  //! destinationPath - путь назначения
  //! destinationFileName - имя файла назначения с расширением
  static void copyFile(const QString &filePath,
                       const QString &destinationPath,
                       const QString &destinationFileName = QString());

  void copyFiles(const QStringList &filePathList,
                             const QString &destinationPath);

  static void deleteFile(const QString &filePath);
  void deleteFiles(const QStringList &filePathList);

  static void deleteFileFromFolder(QString const& filePath,
                                   QString const& folderPath);
  void deletePhotosFromFolder(QStringList const& photoFilePathList,
                                          QString const& folderPath);

  static QStringList getImagesPathList(const QString &path);
  QStringList getImagesOrientationList(const QString &path);

  //! Возвращает ориентацию изображения
  static int getImageOrientation(const QString &path);

  //! Поворот изображения вправо
  static void rotateRightImage(const QString &filePath);

  //! Поворот нескольких изображений вправо
  void rotateRightImages(const QStringList &filePathList);

private:
  FileOperationHandler() = delete;
};

} // !PhotoHelper

#endif
