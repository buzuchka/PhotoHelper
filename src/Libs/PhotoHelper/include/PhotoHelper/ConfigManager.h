#ifndef PHOTOHELPER_CONFIGMANAGER_H
#define PHOTOHELPER_CONFIGMANAGER_H

#include <photohelper_export.h>

#include <QStringList>
#include <QReadWriteLock>

class QSettings;
class QVariant;

namespace PhotoHelper {

class PHOTOHELPER_EXPORT ConfigManager
{
public:
  ~ConfigManager();

  static ConfigManager* getInstance();

  QString getSourcePath();
  void setSourcePath(const QString& path);

  QStringList getDestinationPathList();
  void setDestinationPathList(const QStringList & pathList);

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
