#ifndef PHOTOHELPER_FILEOPERATIONHANDLER_H
#define PHOTOHELPER_FILEOPERATIONHANDLER_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QObject>
#include <QVariant>

namespace PhotoHelper {

enum RightOrientation
{
  Normal = 0, // exif = 1
  Right,      // exif = 6
  UpsideDown, // exif = 3
  Left        // exif = 8
};

class PHOTOHELPER_EXPORT FileOperationHandler : public QObject
{
  Q_OBJECT

public:
  FileOperationHandler();

  //! Копирование файла filePath в папку destinationPath
  //! filePath - путь до файла, который копируем
  //! destinationPath - путь назначения
  //! destinationFileName - имя файла назначения с расширением
  Q_INVOKABLE void copyFile(const QString &filePath,
                            const QString &destinationPath,
                            const QString &destinationFileName = QString());

  Q_INVOKABLE void copyFiles(const QStringList &filePathList,
                             const QString &destinationPath);

  Q_INVOKABLE void deleteFile(const QString &filePath);
  Q_INVOKABLE void deleteFiles(const QStringList &filePathList);

  Q_INVOKABLE QStringList getImagesPathList(const QString &path);
  Q_INVOKABLE QStringList getImagesOrientationList(const QString &path);

  // Поворот изображения вправо
   Q_INVOKABLE void rotateRightImage(const QString &filePath);

  // Поворот нескольких изображений вправо
  Q_INVOKABLE void rotateRightImages(const QStringList &filePathList);
};

} // !PhotoHelper

#endif
