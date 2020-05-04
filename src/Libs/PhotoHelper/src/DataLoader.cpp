#include "DataLoader.h"

#include <PhotoHelper/FileOperationHandler.h>

#include <QtConcurrent/QtConcurrent>

#include <QtQml/QQmlPropertyMap>

namespace PhotoHelper {

DataLoader::DataLoader()
{
  connect(&m_loadImagesFutureWatcher, &QFutureWatcher<QStringList>::finished,
          this, &DataLoader::startLoadingFiles);
  connect(&m_loadFilesFutureWatcher, &QFutureWatcher<QStringList>::finished,
          this, &DataLoader::onLoadingFinished);
}

void DataLoader::startLoading(const QString &path,
                              const QStringList &destinationPathList)
{
  m_destinationPathList = destinationPathList;

  m_loadImagesFuture = QtConcurrent::run(
        FileOperationHandler::getImagesPathList, path);

  m_loadImagesFutureWatcher.setFuture(m_loadImagesFuture);
}

QStringList DataLoader::getSourcePhotoPathList()
{
  return m_sourcePhotoPathList;
}

QQmlPropertyMap* DataLoader::getDestinationPathPhotosCache()
{
  return m_destinationPhotoCache;
}

void DataLoader::startLoadingFiles()
{
  m_loadFilesFuture = QtConcurrent::run(
        FileOperationHandler::getDestinationPathFilesCache, m_destinationPathList);
  m_loadFilesFutureWatcher.setFuture(m_loadFilesFuture);
}

void DataLoader::onLoadingFinished()
{
  m_sourcePhotoPathList = m_loadImagesFuture.result();
  m_destinationPhotoCache = m_loadFilesFuture.result();
  emit loadingFinished();
}

}
