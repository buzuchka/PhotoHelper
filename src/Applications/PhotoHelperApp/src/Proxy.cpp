#include "Proxy.h"

#include <PhotoHelper/FileOperationHandler.h>

#include <QtConcurrent/QtConcurrent>

#include <QtCore/QFutureWatcher>

using namespace PhotoHelper;

Proxy::Proxy()
{
  connect(&m_futureWatcher, &QFutureWatcher<QStringList>::finished,
          this, &Proxy::onLoadingFinished);
}

void Proxy::startLoading(const QString &path)
{
  m_future = QtConcurrent::run(
        FileOperationHandler::getImagesPathList, path);
  m_futureWatcher.setFuture(m_future);
}

QStringList Proxy::getImagesPathList()
{
  return m_imagePathList;
}

void Proxy::onLoadingFinished()
{
  m_imagePathList = m_future.result();
  emit loadingFinished();
}
