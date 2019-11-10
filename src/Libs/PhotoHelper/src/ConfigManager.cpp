#include "ConfigManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QMutexLocker>
#include <QSettings>
#include <QString>
#include <QTextCodec>

namespace {
QMutex mutex;
}

namespace PhotoHelper {

const QString sourceDirNameProperty = QLatin1String("SourceDirName");
const QString sourceDirPathProperty = QLatin1String("SourceDirPath");

const QString destinationPathListNameGroup("DestinationPathList");
const QString folderNameProperty("FolderName");
const QString folderPathProperty("FolderPath");

ConfigManager* ConfigManager::m_configManager = nullptr;

ConfigManager::ConfigManager(const QString &fileName)
{
    QDir dir(qApp->applicationDirPath());

    QString iniFilePath = dir.absolutePath() + "/" +
            (fileName.isEmpty() ? "settings.ini" : fileName);

    m_settings = new QSettings(iniFilePath, QSettings::IniFormat);
    m_settings->setIniCodec(QTextCodec::codecForName("UTF8"));
}

ConfigManager::~ConfigManager()
{
  delete m_settings;
}

ConfigManager *ConfigManager::getInstance()
{
  QMutexLocker locker(&mutex);
  Q_UNUSED(locker);

  if(!m_configManager)
    m_configManager = new ConfigManager;
  return m_configManager;
}

FolderConfigPair ConfigManager::getSourceFolderPair()
{
  return FolderConfigPair(getStringValue(sourceDirNameProperty),
                          getStringValue(sourceDirPathProperty));
}

void ConfigManager::setSourcePath(const FolderConfigPair &folderPair)
{
  setValue(sourceDirNameProperty, folderPair.first);
  setValue(sourceDirPathProperty, folderPair.second);
}

FolderConfigList ConfigManager::getDestinationPathList()
{
  QReadLocker locker(&m_readWriteLock);
  Q_UNUSED(locker);

  FolderConfigList list;

  int size = m_settings->beginReadArray(destinationPathListNameGroup);

  for (int i = 0; i < size; ++i) {
      m_settings->setArrayIndex(i);

      list.append({m_settings->value(folderNameProperty).toString(),
                   m_settings->value(folderPathProperty).toString()});
  }
  m_settings->endArray();

  return list;
}

void ConfigManager::setDestinationPathList(const FolderConfigList &folderList)
{
  QWriteLocker locker(&m_readWriteLock);
  Q_UNUSED(locker);

  if (folderList.isEmpty())
      return;

  m_settings->beginWriteArray(destinationPathListNameGroup);

  for (int i = 0; i < folderList.size(); ++i) {
      m_settings->setArrayIndex(i);
      m_settings->setValue(folderNameProperty, folderList.at(i).first);
      m_settings->setValue(folderPathProperty, folderList.at(i).second);
  }

  m_settings->endArray();
}

QString ConfigManager::getStringValue(const QString &propertyName,
                                      const QString &defaultValue)
{
    QWriteLocker locker(&m_readWriteLock);
    Q_UNUSED(locker);

    return getValue(propertyName, defaultValue).toString();
}

QStringList ConfigManager::getStringListValue(const QString &propertyName, const QStringList &defaultValue)
{
    QWriteLocker locker(&m_readWriteLock);
    Q_UNUSED(locker);

    return getValue(propertyName, defaultValue).toStringList();
}

QVariant ConfigManager::getValue(const QString &propertyName,
                                 const QVariant &defaultValue)
{
  if (!m_settings->allKeys().contains(propertyName) ||
          (m_settings->value(propertyName).toByteArray().isEmpty() &&
           m_settings->value(propertyName).toStringList().isEmpty())) {
      m_settings->setValue(propertyName, defaultValue);
  }
  return m_settings->value(propertyName, defaultValue);
}

void ConfigManager::setValue(const QString &propertyName,
                             const QVariant &value)
{
    QWriteLocker locker(&m_readWriteLock);
    Q_UNUSED(locker);

    if (propertyName.isEmpty())
        return;

    m_settings->setValue(propertyName, value);
}

} // !PhotoHelper
