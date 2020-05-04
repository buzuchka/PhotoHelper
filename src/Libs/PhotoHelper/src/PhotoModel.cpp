#include "PhotoModel.h"

#include "FileOperationHandler.h"

#include <QColor>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QString>

#include <QtQml/QQmlPropertyMap>

#include <QDebug>

namespace PhotoHelper {

PhotoModel::PhotoModel(QObject *parent)
  : QAbstractListModel(parent)
  , m_fetchedItemCount(0)
{
}

int PhotoModel::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : static_cast<int>(m_fetchedItemCount);
}

QVariant PhotoModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= m_pathList.count())
      return QVariant();

  switch (role) {
  case NameRole:
      return QFileInfo(m_pathList.at(index.row())).fileName();
  case PathRole:
      return m_pathList.at(index.row());
  case OrientationRole:
    return getOrientation(index.row());
  case ContainsRole:
    return getContainsColors(index.row());
  default:
      return QVariant();
  }
}

QHash<int, QByteArray> PhotoModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[NameRole] = "name";
  roles[PathRole] = "path";
  roles[OrientationRole] = "orientation";
  roles[ContainsRole] = "dircontains";

  return roles;
}

void PhotoModel::setData(const QStringList &pathList)
{
  beginResetModel();
  m_pathList = pathList;
  m_fetchedItemCount = 0;
  endResetModel();
}

void PhotoModel::deleteItem(int index)
{
  beginRemoveRows(QModelIndex(), index, index);
  m_pathList.removeAt(index);
  endRemoveRows();
}

void PhotoModel::onPhotoCopied(int index,
                               const QString &path,
                               QString const& copiedPhotoName)
{
  auto files = m_destinationPathFilesCache.value(path);
  files.push_back(copiedPhotoName);

  m_destinationPathFilesCache.remove(path);
  m_destinationPathFilesCache.insert(path, files);

  m_containsColorsCache.remove(m_pathList.at(index));

  emitUpdateData(index);
}

void PhotoModel::onPhotoDeletedFromDestination(int index,
                                               const QString &path)
{
  m_containsColorsCache.remove(m_pathList.at(index));

  emitUpdateData(index);
}

QList<bool> PhotoModel::getContainsState(int index)
{
  QList<bool> states;

  if(m_pathList.empty() || index >= m_pathList.count()) {
    for(int i = 0; i < m_folderPathColorCache.count(); ++i)
      states.push_back(false);
    return states;
  }

  for(int i = 0; i < m_folderPathColorCache.count(); ++i)
  {
    QString tmp;
    if(FileOperationHandler::isFolderContainsFile(m_folderPathColorCache.at(i).first,
                                                  m_pathList.at(index),
                                                  tmp))
      states.push_back(true);
    else
      states.push_back(false);
  }
  return states;
}

QString PhotoModel::getFilePath(int index)
{
  if(index < 0)
    return QString();

  return m_pathList.at(index);
}

QString PhotoModel::getFileName(int index)
{
  if(index < 0 || (m_pathList.count() == 0) || (index > m_pathList.count()))
    return QString();

  QFileInfo fileInfo(m_pathList.at(index));
  return fileInfo.fileName();
}

int PhotoModel::getElementsCount() const
{
  return m_pathList.size();
}

void PhotoModel::onOrientationChanged(int index)
{
  m_orientationCache.remove(m_pathList.at(index));
  emitUpdateData(index);
}

void PhotoModel::emitUpdateData(int index)
{
  QModelIndex ind = createIndex(index, index);
  emit dataChanged(ind, ind);
}

void PhotoModel::setLastOperatedIndex(int index)
{
  m_lastOperatedIndex = index;
}

void PhotoModel::setFolderPathColorCache(const QList<QPair<QString, QString>> &map)
{
  m_folderPathColorCache = map;
}

void PhotoModel::clear()
{
  setData({});
  m_lastOperatedIndex = 0;
  m_orientationCache.clear();
}

int PhotoModel::getOrientation(int index)const
{
  if(m_pathList.isEmpty() || index >= m_pathList.count())
    return 0;

  QString filePath(m_pathList.at(index));

  if(m_orientationCache.contains(filePath))
    return m_orientationCache.value(filePath);

  int orientation = FileOperationHandler::getImageOrientation(filePath);
  m_orientationCache.insert(filePath, orientation);
  return orientation;
}

QStringList PhotoModel::getContainsColors(int index) const
{
  QStringList colorList;

  if(index >= m_pathList.count())
    return colorList;

  QString filePath(m_pathList.at(index));

  if(m_containsColorsCache.contains(filePath))
    return m_containsColorsCache.value(filePath);

  for(int i = 0; i < m_folderPathColorCache.count(); ++i)
  {
    QString tmp;
    if(FileOperationHandler::isFolderContainsFile(m_folderPathColorCache.at(i).first,
                                                  filePath,
                                                  tmp))
      colorList.push_back(m_folderPathColorCache.at(i).second);
  }
  m_containsColorsCache.insert(filePath, colorList);
  return colorList;
}

void PhotoModel::setDestinationPathFilesCache(QQmlPropertyMap * cache)
{
  for(QString & key : cache->keys())
    m_destinationPathFilesCache.insert(key, cache->value(key).toStringList());
}

bool PhotoModel::canFetchMore(const QModelIndex &parent) const
{
  if (parent.isValid())
    return false;
  return (m_fetchedItemCount < m_pathList.size());
}

void PhotoModel::fetchMore(const QModelIndex &parent)
{
  if (parent.isValid())
    return;

  int chunkSize = 25;

  if(chunkSize > m_pathList.count())
    chunkSize = m_pathList.count();

  int remainder = m_pathList.size() - m_fetchedItemCount;
  int itemsToFetch = qMin(chunkSize, remainder);

  if (itemsToFetch <= 0)
    return;

  if(m_lastOperatedIndex > 0)
  {
    int t = (m_lastOperatedIndex+1) / chunkSize;
    itemsToFetch = chunkSize * (t + 1);
    m_lastOperatedIndex = 0;
  }

  beginInsertRows(QModelIndex(),
                  m_fetchedItemCount,
                  m_fetchedItemCount + itemsToFetch - 1);

  m_fetchedItemCount += itemsToFetch;

  endInsertRows();

  emitUpdateData(parent.row());
}

} // !PhotoHelper
