#ifndef PHOTOHELPER_PHOTOMODEL_H
#define PHOTOHELPER_PHOTOMODEL_H

#include <photohelper_export.h>

#include <QAbstractListModel>
#include <QStringList>

namespace PhotoHelper {

class PHOTOHELPER_EXPORT PhotoModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum Roles {
      NameRole = Qt::UserRole + 1,
      PathRole,
      SelectedRole
  };

  PhotoModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void setData(const QStringList &data);

  Q_INVOKABLE void setSelectedIndexes(QList<int> const& indexes);

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

private:
  QStringList m_data;
  QList<int> m_selectedIndexes;
};

} // !PhotoHelper

#endif
