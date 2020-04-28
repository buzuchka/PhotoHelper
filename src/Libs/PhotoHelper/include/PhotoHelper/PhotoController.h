#ifndef PHOTOHELPER_PHOTOCONTROLLER_H
#define PHOTOHELPER_PHOTOCONTROLLER_H

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
  Q_PROPERTY(QList<bool> isFolderContainsCurrentPhoto
             READ isFolderContainsCurrentPhoto
             NOTIFY isFolderContainsCurrentPhotoChanged)
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

  //! Удаляет текущее фото из исходной папки
  Q_INVOKABLE void deleteCurrentPhotoFromSource();

  //! Удаляет текущее фото из папки назначения
  Q_INVOKABLE void deleteCurrentPhotoFromDestination(QString const& path);

  //! Поворачивает текущее фото вправо
  Q_INVOKABLE void rotateCurrentPhotoRight();

  //! Возвращает флаг корректности ориентации текущего фото
  Q_INVOKABLE bool isCurrentPhotoOrientationCorrect() const;

  //! Возвращает флаги содержания каталогами текущего фото
  Q_INVOKABLE QList<bool> isFolderContainsCurrentPhoto() const;

signals:
  void elementsCountChanged();
  void currentIndexChanged();
  void currentPhotoNameChanged();
  void isCurrentPhotoOrientationCorrectChanged();
  void isFolderContainsCurrentPhotoChanged();

private:
  //! Возвращает путь до текущего фото
  QString getCurrentPhotoPath() const;

private:
  PhotoModel * m_photoModel;
  DestinationFolderModel * m_destinationFolderModel;

  int m_currentIndex; ///< Текущий индекс в m_photoModel
};

}

#endif
