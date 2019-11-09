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

const QString sourceDirName = QLatin1String("SourceDir");
const QString destinationDirsName = QLatin1String("DestinationDirs");

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

QString ConfigManager::getSourcePath()
{
  return getStringValue(sourceDirName);
}

void ConfigManager::setSourcePath(const QString &path)
{
  setValue(sourceDirName, path);
}

QStringList ConfigManager::getDestinationPathList()
{
  return getStringListValue(destinationDirsName);
}

void ConfigManager::setDestinationPathList(const QStringList &pathList)
{
  setValue(destinationDirsName, pathList);
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
