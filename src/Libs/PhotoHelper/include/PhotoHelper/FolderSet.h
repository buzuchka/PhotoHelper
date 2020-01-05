#ifndef PHOTOHELPER_FOLDERSET_H
#define PHOTOHELPER_FOLDERSET_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QObject>
#include <QVariant>

namespace PhotoHelper {

//! Пути до папки для разбора и папок назначения
class PHOTOHELPER_EXPORT FolderSet : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString sourceName
             READ getSourceName
             NOTIFY sourceNameChanged)
  Q_PROPERTY(QString sourcePath
             READ getSourcePath
             NOTIFY sourcePathChanged)
  Q_PROPERTY(QVariantList destinationPathList
             READ getDestinationVariantList
             NOTIFY destinationPathListChanged)
public:
  FolderSet();

  Q_INVOKABLE QString getSourceName() const;
  Q_INVOKABLE QString getSourcePath() const;
  Q_INVOKABLE void setSourcePath(const QString &name,
                                 const QString &path);
  void setSourcePath(const FolderConfigPair &folderPair);

  Q_INVOKABLE QString getDestinationPath(int index) const;
  Q_INVOKABLE QString getDestinationName(int index) const;
  Q_INVOKABLE void setDestinationPath(int index,
                                      const QString &name,
                                      const QString &path);

  Q_INVOKABLE void setLastOperatedIndex(int index);
  Q_INVOKABLE int getLastOperatedIndex() const;

  Q_INVOKABLE QVariantList getDestinationVariantList() const;

  Q_INVOKABLE FolderConfigList getDestinationPathList() const;
  void setDestinationPathList(const FolderConfigList &list);

signals:
  void sourceNameChanged();
  void sourcePathChanged();
  void destinationPathListChanged();
  void lastOperatedIndexChanged();

private:
  FolderConfigPair m_sourcePath;           ///< Путь до папки для разбора
  int m_lastOperatedIndex;                 ///< Индекс последнего обработанного изображения
  FolderConfigList m_destinationPathList;  ///< Пути до папок назначения
};

} // !PhotoHelper

#endif
