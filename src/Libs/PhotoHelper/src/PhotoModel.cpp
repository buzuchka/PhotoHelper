#include "PhotoModel.h"

#include <QDir>
#include <QFileInfo>

namespace PhotoHelper {

PhotoModel::PhotoModel(QObject *parent)
  : QAbstractListModel(parent)
{
}

int PhotoModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return m_data.size();
}

QVariant PhotoModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) {
      return QVariant();
  }

  switch (role) {
  case NameRole:
      return QFileInfo(m_data.at(index.row())).fileName();
  case PathRole:
      return m_data.at(index.row());
  case SelectedRole:
    return m_selectedIndexes.contains(index.row());
  default:
      return QVariant();
  }
}

QHash<int, QByteArray> PhotoModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[NameRole] = "name";
  roles[PathRole] = "path";
  roles[SelectedRole] = "selected";

  return roles;
}

void PhotoModel::setData(const QStringList &pathList)
{
  beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
  auto data = pathList;
  m_data = pathList;
  endInsertRows();

  QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
  emit dataChanged(index, index);
}

void PhotoModel::setSelectedIndexes(const QList<int> &indexes)
{
  m_selectedIndexes = indexes;
}

void PhotoModel::deleteItem(int index)
{
  m_data.removeAt(index);

  QModelIndex ind = createIndex(index, 0, static_cast<void *>(0));
  emit dataChanged(ind, ind);
}

QString PhotoModel::getFilePath(int index)
{
  if(index < 0)
    return QString();

  return m_data.at(index);
}

QString PhotoModel::getFileName(int index)
{
  if(index < 0)
    return QString();

  QFileInfo fileInfo(m_data.at(index));
  return fileInfo.fileName();
}

} // !PhotoHelper
