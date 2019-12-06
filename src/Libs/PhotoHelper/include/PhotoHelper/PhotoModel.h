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
      PathRole
  };

  PhotoModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void setData(const QStringList &data);

  //! Удаление элемента
  Q_INVOKABLE void deleteItem(int index);

  //! Возвращает путь по индексу
  Q_INVOKABLE QString getFilePath(int index);

  //! Возвращает имя файла по индексу
  Q_INVOKABLE QString getFileName(int index);

private:
  QStringList m_data;
};

} // !PhotoHelper

#endif
