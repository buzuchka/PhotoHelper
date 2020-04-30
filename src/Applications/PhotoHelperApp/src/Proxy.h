#ifndef PROXY_H
#define PROXY_H

#include <QtCore/QFileInfo>
#include <QtCore/QFutureWatcher>
#include <QtCore/QObject>

class QQmlPropertyMap;

class Proxy : public QObject
{
  Q_OBJECT
public:
  Proxy();

  Q_INVOKABLE void startLoading(const QString &path,
                                const QStringList &destinationPathList);

  Q_INVOKABLE QStringList getImagesPathList();
  Q_INVOKABLE QQmlPropertyMap* getDestinationPathFilesCache();

signals:
  void loadingFinished();

protected slots:
  void startLoadingFiles();
  void onLoadingFinished();

private:
  QStringList m_imagePathList;
  QQmlPropertyMap* m_filesCache;

  QStringList m_destinationPathList;

  QFutureWatcher<QStringList> m_loadImagesFutureWatcher;
  QFuture<QStringList> m_loadImagesFuture;

  QFutureWatcher<QQmlPropertyMap*> m_loadFilesFutureWatcher;
  QFuture<QQmlPropertyMap*> m_loadFilesFuture;
};

#endif // PROXY_H
