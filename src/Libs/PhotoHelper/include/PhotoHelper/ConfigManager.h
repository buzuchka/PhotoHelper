#ifndef PHOTOHELPER_CONFIGMANAGER_H
#define PHOTOHELPER_CONFIGMANAGER_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QStringList>
#include <QReadWriteLock>

class QSettings;
class QVariant;

namespace PhotoHelper {

//! Менеджер для работы с настройками
class PHOTOHELPER_EXPORT ConfigManager
{
public:
  ~ConfigManager();

  static ConfigManager* getInstance();

  FolderConfigPair getSourceFolderPair();
  void setSourcePath(const FolderConfigPair &path);

  void setLastOperatedIndex(int index);
  int getLastOperatedIndex();

  FolderConfigList getDestinationPathList();
  void setDestinationPathList(const FolderConfigList & folderList);

private:
  explicit ConfigManager(const QString &fileName = QString());

  /*!
   * Возвращает строковое значение по заданному свойству
   * @param[in] propertyName Название свойства
   * @param[in] defaultValue Значение по умолчанию
   */
  QString getStringValue(const QString &propertyName,
                         const QString &defaultValue = QString());

  /*!
   * Возвращает список строк по заданному свойству
   * @param[in] propertyName Название свойства
   * @param[in] defaultValue Значение по умолчанию
   */
  QStringList getStringListValue(const QString &propertyName,
                                 const QStringList &defaultValue = QStringList());

  void setValue(const QString &propertyName,
                const QVariant &value);

  QVariant getValue(const QString &propertyName,
                    const QVariant &defaultValue);

private:
  static ConfigManager *m_configManager;
  QSettings *m_settings;
  mutable QReadWriteLock m_readWriteLock;
};

} // !PhotoHelper

#endif
