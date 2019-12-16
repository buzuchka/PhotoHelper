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

  return m_pathList.size();
}

QVariant PhotoModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) {
      return QVariant();
  }

  switch (role) {
  case NameRole:
      return QFileInfo(m_pathList.at(index.row())).fileName();
  case PathRole:
      return m_pathList.at(index.row());
  case SelectedRole:
    return m_selectedIndexes.contains(index.row());
  case OrientationRole:
    return m_orientationList.at(index.row()).toInt();
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
  roles[OrientationRole] = "orientation";

  return roles;
}

void PhotoModel::setData(const QStringList &pathList,
                         const QStringList &orientationList)
{
  beginResetModel();
  m_pathList = pathList;
  m_orientationList = orientationList;
  endResetModel();
}

void PhotoModel::deleteItem(int index)
{
  beginRemoveRows(QModelIndex(), index, index);
  m_pathList.removeAt(index);
  endRemoveRows();
}

void PhotoModel::deleteItems(const QList<int> &indexes)
{
  QList<int> sortedList = indexes;
  std::sort(sortedList.begin(), sortedList.end());

  for(int i = sortedList.count() - 1; i >= 0; --i)
    deleteItem(sortedList.at(i));
}

QString PhotoModel::getFilePath(int index)
{
  if(index < 0)
    return QString();

  return m_pathList.at(index);
}

QStringList PhotoModel::getFilePathList(const QList<int> &indexes)
{
  QStringList list;
  for(int i = 0; i < indexes.count(); ++i)
    list.append(m_pathList.at(indexes.at(i)));
  return list;
}

QString PhotoModel::getFileName(int index)
{
  if(index < 0 || (m_pathList.count() == 0))
    return QString();

  QFileInfo fileInfo(m_pathList.at(index));
  return fileInfo.fileName();
}

QList<int> PhotoModel::selectedIndexes() const
{
  return m_selectedIndexes;
}

void PhotoModel::setSelectedIndexes(const QList<int> &indexes)
{
  m_selectedIndexes = indexes;
  emit selectedIndexesChanged();
}

QStringList PhotoModel::orientationList() const
{
  return m_orientationList;
}

int PhotoModel::getOrientation(int index)
{
  return m_orientationList.at(index).toInt();
}

void PhotoModel::rotateRight(int index)
{
  int oldOrientation = m_orientationList.at(index).toInt();
  int newOrientation = 0;

  if(oldOrientation < 3)
    newOrientation = oldOrientation + 1;

  m_orientationList[index] = QString::number(newOrientation);

  QModelIndex ind = createIndex(index, index);
  emit dataChanged(ind, ind);

  emit orientationListChanged();
}

void PhotoModel::rotateRightSelectedIndexes()
{
  for(int ind : m_selectedIndexes)
    rotateRight(ind);
}

} // !PhotoHelper
