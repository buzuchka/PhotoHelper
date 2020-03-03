#include "Proxy.h"

#include <PhotoHelper/FileOperationHandler.h>

#include <QtConcurrent/QtConcurrent>

#include <QtCore/QFutureWatcher>

#include <QtQml/QQmlPropertyMap>

using namespace PhotoHelper;

Proxy::Proxy()
{
  connect(&m_loadImagesFutureWatcher, &QFutureWatcher<QStringList>::finished,
          this, &Proxy::startLoadingFiles);
  connect(&m_loadFilesFutureWatcher, &QFutureWatcher<QStringList>::finished,
          this, &Proxy::onLoadingFinished);
}

void Proxy::startLoading(const QString &path,
                         const QStringList &destinationPathList)
{
  m_destinationPathList = destinationPathList;

  m_loadImagesFuture = QtConcurrent::run(
        FileOperationHandler::getImagesPathList, path);

  m_loadImagesFutureWatcher.setFuture(m_loadImagesFuture);
}

QStringList Proxy::getImagesPathList()
{
  return m_imagePathList;
}

QQmlPropertyMap* Proxy::getDestinationPathFilesCache()
{
  return m_filesCache;
}

void Proxy::startLoadingFiles()
{
  m_loadFilesFuture = QtConcurrent::run(
        FileOperationHandler::getDestinationPathFilesCache, m_destinationPathList);
  m_loadFilesFutureWatcher.setFuture(m_loadFilesFuture);
}

void Proxy::onLoadingFinished()
{
  m_imagePathList = m_loadImagesFuture.result();
  m_filesCache = m_loadFilesFuture.result();
  emit loadingFinished();
}


