#ifndef PHOTOHELPER_FILEOPERATIONHANDLER_H
#define PHOTOHELPER_FILEOPERATIONHANDLER_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QFileInfo>
#include <QObject>
#include <QVariant>

#include <QtQml/QQmlPropertyMap>

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

  static QStringList getImagesPathList(const QString &path);
  Q_INVOKABLE QStringList getImagesOrientationList(const QString &path);

  //! Возвращает ориентацию изображения
  static int getImageOrientation(const QString &path);

  //! Поворот изображения вправо
  static void rotateRightImage(const QString &filePath);

  //! Поворот нескольких изображений вправо
  Q_INVOKABLE void rotateRightImages(const QStringList &filePathList);

  static QQmlPropertyMap* getDestinationPathFilesCache(QStringList const& destinationPathList);
};

} // !PhotoHelper

#endif
