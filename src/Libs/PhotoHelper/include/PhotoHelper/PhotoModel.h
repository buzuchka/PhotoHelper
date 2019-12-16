#ifndef PHOTOHELPER_PHOTOMODEL_H
#define PHOTOHELPER_PHOTOMODEL_H

#include <photohelper_export.h>

#include <QAbstractListModel>
#include <QStringList>

namespace PhotoHelper {

class PHOTOHELPER_EXPORT PhotoModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(QList<int> selectedIndexes
             READ selectedIndexes
             WRITE setSelectedIndexes
             NOTIFY selectedIndexesChanged)
  Q_PROPERTY(QStringList orientationList
             READ orientationList
             NOTIFY orientationListChanged)
public:
  enum Roles {
    NameRole = Qt::UserRole + 1,
    PathRole,
    SelectedRole,
    OrientationRole
  };

  PhotoModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void setData(const QStringList &pathList,
                           const QStringList &orientationList);

  //! Удаление элемента
  Q_INVOKABLE void deleteItem(int index);

  //! Удаление нескольких элементов
  Q_INVOKABLE void deleteItems(QList<int> const& indexes);

  //! Возвращает путь по индексу
  Q_INVOKABLE QString getFilePath(int index);

  //! Возвращает список путей по индексам
  Q_INVOKABLE QStringList getFilePathList(QList<int> const& indexes);

  //! Возвращает имя файла по индексу
  Q_INVOKABLE QString getFileName(int index);

  Q_INVOKABLE QList<int> selectedIndexes() const;
  Q_INVOKABLE void setSelectedIndexes(const QList<int> &list);

  Q_INVOKABLE QStringList orientationList() const;

  Q_INVOKABLE int getOrientation(int index);

  Q_INVOKABLE void rotateRight(int index);
  Q_INVOKABLE void rotateRightSelectedIndexes();

signals:
  void selectedIndexesChanged();
  void orientationListChanged();

private:
  QStringList m_pathList;
  QStringList m_orientationList;
  QList<int> m_selectedIndexes;
};

} // !PhotoHelper

#endif
