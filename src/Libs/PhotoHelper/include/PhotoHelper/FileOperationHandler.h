#ifndef PHOTOHELPER_FILEOPERATIONHANDLER_H
#define PHOTOHELPER_FILEOPERATIONHANDLER_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QObject>
#include <QVariant>

namespace PhotoHelper {

class PHOTOHELPER_EXPORT FileOperationHandler : public QObject
{
  Q_OBJECT

public:
  FileOperationHandler();

  //! Копирование файла filePath в папку destinationPath
  Q_INVOKABLE void copyFile(const QString &filePath,
                            const QString &destinationPath);

  Q_INVOKABLE void deleteFile(const QString &filePath);

  Q_INVOKABLE QStringList getImagesPathList(const QString &path);
};

} // !PhotoHelper

#endif
