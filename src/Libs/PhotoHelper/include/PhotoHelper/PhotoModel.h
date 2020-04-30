#ifndef PHOTOHELPER_PHOTOMODEL_H
#define PHOTOHELPER_PHOTOMODEL_H

#include <photohelper_export.h>

#include <QAbstractListModel>
#include <QFileInfo>
#include <QStringList>

class QQmlPropertyMap;

namespace PhotoHelper {

class PHOTOHELPER_EXPORT PhotoModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(QList<int> selectedIndexes
             READ selectedIndexes
             WRITE setSelectedIndexes
             NOTIFY selectedIndexesChanged)
public:
  enum Roles {
    NameRole = Qt::UserRole + 1,
    PathRole,
    SelectedRole,
    OrientationRole,
    ContainsRole
  };

  explicit PhotoModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  //! Установка списка путей до фото
  void setData(const QStringList &photoPathList);

  //! Удаляет данные модели
  void clear();

  //! Возвращает количество элементов в модели
  int getElementsCount() const;

  //! Возвращает путь по индексу
  QString getFilePath(int index);

  //! Возвращает имя файла по индексу
  QString getFileName(int index);

  //! Задает индекс элемента, который необходимо загрузить
  void setLastOperatedIndex(int index);

  //! Задает кэш путей до папок назначения и соответствующих цветов
  void setFolderPathColorCache(const QList<QPair<QString, QString> > &map);

  //! Преобразовывает cache в m_destinationPathFilesCache
  void setDestinationPathFilesCache(QQmlPropertyMap * cache);

  //! Возвращает ориентацию элемента
  int getOrientation(int index) const;

  //! Действие на изменение ориентации фото
  void onOrientationChanged(int index);

  //! Удаление элемента
  void deleteItem(int index);

  //! Обновляет данные после копирования фото
  void onPhotoCopied(int index,
                     QString const& folderPath,
                     QString const& copiedPhotoName);

  //! Обновляет данные после удаления фото из папки назначения
  void onPhotoDeletedFromDestination(int index,
                                     QString const& folderPath);

  //! Возвращает статус содержания в папках назначения для индекса
  QList<bool> getContainsState(int index);

  Q_INVOKABLE QList<int> selectedIndexes() const;
  Q_INVOKABLE void setSelectedIndexes(const QList<int> &list);

//  //! Удаление нескольких элементов
//  Q_INVOKABLE void deleteItems(QList<int> const& indexes);
//
//  //! Возвращает список путей по индексам
//  Q_INVOKABLE QStringList getFilePathList(QList<int> const& indexes);
//
//  Q_INVOKABLE void rotateRightSelectedIndexes();
//
//  Q_INVOKABLE void setDestinationPathList(QStringList const& pathList);
//  Q_INVOKABLE QStringList getDestinationPathList();
//
//  Q_INVOKABLE void setDestinationPathNameList(QStringList const& nameList);
//  Q_INVOKABLE QStringList getDestinationPathNameList();
//
//  void deletePhotoFromFolder(int index, QString const& path);

signals:
  void selectedIndexesChanged();

protected:
  bool canFetchMore(const QModelIndex &parent) const override;
  void fetchMore(const QModelIndex &parent) override;

private:
  //! Отправляет сигнал об обновлении элемента
  void emitUpdateData(int index);

  //! Возвращает список цветов, соответствующих каталогов
  QStringList getContainsColors(int index) const;

private:
  QStringList m_pathList;                ///< Список путей до изображений

  unsigned int m_fetchedItemCount;       ///< Количество загруженных элементов
  unsigned int m_lastOperatedIndex;

  // Кэш  путей до папок назначения и соответствующих цветов
  QList<QPair<QString /*folderPath*/, QString /*color*/>> m_folderPathColorCache;

  // Список файлов в целевых директориях
  mutable QHash<QString /*folderPath*/, QStringList /*fileNames*/> m_destinationPathFilesCache;

  // Кэш ориентаций фото
  mutable QHash<QString /*photoPath*/, int /*orientation*/> m_orientationCache;

  // Кэш цветов каталогов, в которых содержатся фото
  mutable QHash<QString /*photoPath*/, QStringList /*colors*/> m_containsColorsCache;

  QList<int> m_selectedIndexes;          ///< Индексы выделенных элементов
};

} // !PhotoHelper

#endif
