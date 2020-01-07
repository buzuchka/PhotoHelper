#include "PhotoModel.h"

#include "FileOperationHandler.h"

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
  return parent.isValid() ? 0 : m_pathList.size();
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
    return getOrientation(index.row());
  case ContainsRole:
    return FileOperationHandler::getContainsFolderColors(m_pathList.at(index.row()),
                                                         m_destinationPathList,
                                                         m_destinationPathNameList);
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
  roles[ContainsRole] = "dircontains";

  return roles;
}

void PhotoModel::setData(const QStringList &pathList)
{
  beginResetModel();
  m_pathList = pathList;
  endResetModel();

  emit elementsCountChanged();
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

void PhotoModel::rotateRight(int index)
{
  FileOperationHandler::rotateRightImage(m_pathList.at(index));
  emitUpdateData(index);
}

void PhotoModel::rotateRightSelectedIndexes()
{
  for(int ind : m_selectedIndexes)
    rotateRight(ind);
}

int PhotoModel::elementsCount() const
{
  return m_pathList.size();
}

void PhotoModel::setDestinationPathList(const QStringList &pathList)
{
  m_destinationPathList = pathList;
  emit destinationPathListChanged();
}

QStringList PhotoModel::getDestinationPathList()
{
  return m_destinationPathList;
}

void PhotoModel::setDestinationPathNameList(const QStringList &nameList)
{
  m_destinationPathNameList = nameList;
  emit destinationPathNameListChanged();
}

QStringList PhotoModel::getDestinationPathNameList()
{
  return m_destinationPathNameList;
}

void PhotoModel::emitUpdateData(int index)
{
  QModelIndex ind = createIndex(index, index);
  emit dataChanged(ind, ind);
}

int PhotoModel::getOrientation(int index)const
{
  QString filePath(m_pathList.at(index));

  if(m_orientationCache.contains(filePath))
    return m_orientationCache.value(filePath);

  int orientation = FileOperationHandler::getImageOrientation(filePath);
  m_orientationCache.insert(filePath, orientation);
  return orientation;
}

} // !PhotoHelper
