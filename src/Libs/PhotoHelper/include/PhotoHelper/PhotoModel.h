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
  Q_PROPERTY(QStringList destinationPathList
             READ getDestinationPathList
             WRITE setDestinationPathList
             NOTIFY destinationPathListChanged)
  Q_PROPERTY(QStringList destinationPathNameList
             READ getDestinationPathNameList
             WRITE setDestinationPathNameList
             NOTIFY destinationPathNameListChanged)
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

  Q_INVOKABLE void setData(const QStringList &pathList);

  //! Удаление элемента
  Q_INVOKABLE void deleteItem(int index);

  //! Удаление нескольких элементов
  Q_INVOKABLE void deleteItems(QList<int> const& indexes);

  //! Возвращает путь по индексу
  Q_INVOKABLE QString getFilePath(int index);

  //! Возвращает список путей по индексам
  Q_INVOKABLE QStringList getFilePathList(QList<int> const& indexes);

  //! Обновляет данные после копирования файла
  Q_INVOKABLE void onFileCopied(int index, QString const& folderPath);

  //! Возвращает имя файла по индексу
  Q_INVOKABLE QString getFileName(int index);

  Q_INVOKABLE QList<int> selectedIndexes() const;
  Q_INVOKABLE void setSelectedIndexes(const QList<int> &list);

  Q_INVOKABLE void rotateRightSelectedIndexes();

  Q_INVOKABLE void setDestinationPathList(QStringList const& pathList);
  Q_INVOKABLE QStringList getDestinationPathList();

  Q_INVOKABLE void setDestinationPathNameList(QStringList const& nameList);
  Q_INVOKABLE QStringList getDestinationPathNameList();

  //! Отправляет сигнал об обновлении элемента
  Q_INVOKABLE void emitUpdateData(int index);

  //! Задает индекс элемента, который необходимо загрузить
  Q_INVOKABLE void setLastOperatedIndex(int index);

  //! Удаляет данные модели
  Q_INVOKABLE void clear();



  Q_INVOKABLE void setDestinationPathFilesCache(QQmlPropertyMap*);

  int getElementsCount() const;

  void onOrientationChanged(int index);

  int getOrientation(int index) const;

  void deletePhotoFromFolder(int index, QString const& path);

private:

  QStringList getContainsColors(int index) const;

signals:
  void selectedIndexesChanged();
  void destinationPathListChanged();
  void destinationPathNameListChanged();

protected:
  bool canFetchMore(const QModelIndex &parent) const override;
  void fetchMore(const QModelIndex &parent) override;

private:
  QStringList m_pathList;                ///< Список путей до изображений
  QList<int> m_selectedIndexes;          ///< Индексы выделенных элементов
  unsigned int m_fetchedItemCount;       ///< Количество загруженных элементов
  unsigned int m_lastOperatedIndex;

  QStringList m_destinationPathList;     ///< Пути до папок назначения
  QStringList m_destinationPathNameList; ///< Названия папок назначения

  mutable QHash<QString, QStringList> m_containsColorsCache;
  // Список файлов в целевых директориях
  mutable QHash<QString, QStringList> m_destinationPathFilesCache;
  // Кэш ориентаций изображений
  mutable QHash<QString /*photoPath*/, int /*orientation*/> m_orientationCache;
};

} // !PhotoHelper

#endif
