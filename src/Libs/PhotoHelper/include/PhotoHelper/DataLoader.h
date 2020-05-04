#ifndef PHOTOHELPER_DATALOADER_H
#define PHOTOHELPER_DATALOADER_H

#include <QtCore/QFileInfo>
#include <QtCore/QFutureWatcher>
#include <QtCore/QObject>

class QQmlPropertyMap;

namespace PhotoHelper {

/*!
 * \brief Асинхронно загружает пути до фото в исходном каталоге и
 * фото в каталогах назначения
 */
class DataLoader : public QObject
{
  Q_OBJECT
public:
  DataLoader();

  //! Запуск загрузки фото в исходном каталоге и в каталогах назначения
  //! sourcePath - исходный каталог с фото
  //! destinationPathList - список путей до каталогов назначения
  Q_INVOKABLE void startLoading(const QString &sourcePath,
                                const QStringList &destinationPathList);

  //! Возвращает список путей до фото в исходном каталоге
  Q_INVOKABLE QStringList getSourcePhotoPathList();

  //! Возвращает кеш с путями до фото в каталогах назначения
  Q_INVOKABLE QQmlPropertyMap* getDestinationPathPhotosCache();

signals:
  void loadingFinished();

protected slots:
  void startLoadingFiles();
  void onLoadingFinished();

private:
  QStringList m_destinationPathList;  ///< Список путей до каталогов назначения

  QStringList m_sourcePhotoPathList;  ///< Список путей до фото в исходном каталоге
  QQmlPropertyMap* m_destinationPhotoCache; ///< Кеш с путями до фото в каталогах назначения

  QFutureWatcher<QStringList> m_loadImagesFutureWatcher;
  QFuture<QStringList> m_loadImagesFuture;

  QFutureWatcher<QQmlPropertyMap*> m_loadFilesFutureWatcher;
  QFuture<QQmlPropertyMap*> m_loadFilesFuture;
};

}

#endif
