#ifndef PROXY_H
#define PROXY_H

#include <QtCore/QFutureWatcher>
#include <QtCore/QObject>

class Proxy : public QObject
{
  Q_OBJECT
public:
  Proxy();

  Q_INVOKABLE void startLoading(QString const& path);
  Q_INVOKABLE QStringList getImagesPathList();

signals:
  void loadingFinished();

protected slots:
  void onLoadingFinished();

private:
  QStringList m_imagePathList;

  QFutureWatcher<QStringList> m_futureWatcher;
  QFuture<QStringList> m_future;
};

#endif // PROXY_H
