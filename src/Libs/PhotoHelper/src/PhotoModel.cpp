#include "PhotoModel.h"

#include <QDir>
#include <QFileInfo>

#include <QDebug>

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
  beginResetModel();
  m_data = pathList;
  endResetModel();
}

void PhotoModel::setSelectedIndexes(const QList<int> &indexes)
{
  m_selectedIndexes = indexes;
}

void PhotoModel::deleteItem(int index)
{
  beginRemoveRows(QModelIndex(), index, index);
  m_data.removeAt(index);
  endRemoveRows();
}

void PhotoModel::deleteItems(const QList<int> &indexes)
{
  QList<int> sortedList = indexes;
  std::sort(sortedList.begin(), sortedList.end());

  for(int i = sortedList.count() - 1; i >= 0; --i)
  {
    beginRemoveRows(QModelIndex(), sortedList.at(i), sortedList.at(i));
    m_data.removeAt(sortedList.at(i));
    endRemoveRows();
  }
}

QString PhotoModel::getFilePath(int index)
{
  if(index < 0)
    return QString();

  return m_data.at(index);
}

QStringList PhotoModel::getFilePathList(const QList<int> &indexes)
{
  QStringList list;
  for(int i = 0; i < indexes.count(); ++i)
    list.append(m_data.at(indexes.at(i)));
  return list;
}

QString PhotoModel::getFileName(int index)
{
  if(index < 0 || (m_data.count() == 0))
    return QString();

  QFileInfo fileInfo(m_data.at(index));
  return fileInfo.fileName();
}

} // !PhotoHelper
