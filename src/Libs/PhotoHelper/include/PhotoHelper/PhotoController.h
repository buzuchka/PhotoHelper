#ifndef PHOTOHELPER_PHOTOCONTROLLER_H
#define PHOTOHELPER_PHOTOCONTROLLER_H

#include <QHash>
#include <QMap>
#include <QObject>

class QQmlPropertyMap;

namespace PhotoHelper {

class DestinationFolderModel;
class FolderSet;
class PhotoModel;

class PhotoController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int elementsCount
             READ getElementsCount
             NOTIFY elementsCountChanged)
  Q_PROPERTY(int currentIndex
             READ getCurrentIndex
             NOTIFY currentIndexChanged)
  Q_PROPERTY(QString currentPhotoName
             READ getCurrentPhotoName
             NOTIFY currentPhotoNameChanged)
  Q_PROPERTY(bool isCurrentPhotoOrientationCorrect
             READ isCurrentPhotoOrientationCorrect
             NOTIFY isCurrentPhotoOrientationCorrectChanged)
  Q_PROPERTY(QList<int> selectedIndexes
             READ selectedIndexes
             WRITE setSelectedIndexes
             NOTIFY selectedIndexesChanged)
public:
  PhotoController(QObject * parent = nullptr);

  Q_INVOKABLE PhotoModel* getPhotoModel();

  Q_INVOKABLE DestinationFolderModel* getDestinationFolderModel();

  Q_INVOKABLE int getElementsCount() const;

  Q_INVOKABLE void onStartReloadData(FolderSet * folderSet);

  Q_INVOKABLE void onFinishReloadData(QStringList const& sourcePhotoPathList,
                                      QQmlPropertyMap* destinationPathPhotosCache);

  Q_INVOKABLE int getCurrentIndex() const;

  Q_INVOKABLE void increaseCurrentIndex();
  Q_INVOKABLE void decreaseCurrentIndex();

  //! Возвращает имя текущего фото
  Q_INVOKABLE QString getCurrentPhotoName() const;

  //! Копирует текущее фото в папку path
  Q_INVOKABLE void copyCurrentPhoto(QString const& path);

  //! Копирует выбранные фото в папку path
  Q_INVOKABLE void copySelectedPhotos(QString const& path);

  //! Удаляет текущее фото из исходной папки
  Q_INVOKABLE void deleteCurrentPhotoFromSource();

  //! Удаляет выбранные фото из исходной папки
  Q_INVOKABLE void deleteSelectedPhotosFromSource();

  //! Удаляет текущее фото из папки назначения
  Q_INVOKABLE void deleteCurrentPhotoFromDestination(QString const& path);

  //! Удаляет выбранные фото из папки назначения
  Q_INVOKABLE void deleteSelectedPhotosFromDestination(QString const& path);

  //! Поворачивает текущее фото вправо
  Q_INVOKABLE void rotateCurrentPhotoRight();

  //! Поворачивает выбранные фото вправо
  Q_INVOKABLE void rotateSelectedPhotosRight();

  //! Возвращает флаг корректности ориентации текущего фото
  Q_INVOKABLE bool isCurrentPhotoOrientationCorrect() const;

  Q_INVOKABLE QList<int> selectedIndexes() const;
  Q_INVOKABLE void setSelectedIndexes(const QList<int> &list);

signals:
  void elementsCountChanged();
  void currentIndexChanged();
  void currentPhotoNameChanged();
  void isCurrentPhotoOrientationCorrectChanged();
  void selectedIndexesChanged();

private:
  //! Возвращает путь до текущего фото
  QString getCurrentPhotoPath() const;

  //! Возвращает список цветов, соответствующих каталогов, в которых содержится
  //! текущее фото
  QStringList getCurrentContainsColors() const;

  //! Обновляет состояние модели папок назначения
  void updateDestinationFolderModelContainsState();

private:
  PhotoModel * m_photoModel;
  DestinationFolderModel * m_destinationFolderModel;

  int m_currentIndex;           ///< Текущий индекс в m_photoModel
  QList<int> m_selectedIndexes; ///< Индексы выделенных элементов
};

}

#endif
