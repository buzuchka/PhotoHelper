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

QColor GetColorByName(const QString& name);

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
  if (!index.isValid())
      return QVariant();

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
  roles[SelectedRole] = "selected";
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

void PhotoModel::rotateRightSelectedIndexes()
{
  //for(int ind : m_selectedIndexes)
  //  rotateRight(ind);
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

void PhotoModel::setLastOperatedIndex(int index)
{
  m_lastOperatedIndex = index;
}

void PhotoModel::clear()
{
  setData({});
  setSelectedIndexes({});
  m_lastOperatedIndex = 0;
  m_orientationCache.clear();
}

void PhotoModel::onFileCopied(int index, const QString &folderPath)
{
  // Убираем путь для перегенерации
  m_containsColorsCache.remove(m_pathList.at(index));

  auto files = m_destinationPathFilesCache.value(folderPath);
  m_destinationPathFilesCache.remove(folderPath);
  QFileInfo fileInfo(m_pathList.at(index));
  files.push_back(fileInfo.fileName());
  m_destinationPathFilesCache.insert(folderPath, files);

  emitUpdateData(index);
}

void PhotoModel::deletePhotoFromFolder(int index, const QString &folderPath)
{
  auto files = m_destinationPathFilesCache.value(folderPath);
  m_destinationPathFilesCache.remove(folderPath);
  QFileInfo fileInfo(m_pathList.at(index));
  files.push_back(fileInfo.fileName());
  m_destinationPathFilesCache.insert(folderPath, files);

  emitUpdateData(index);
}

void PhotoModel::setDestinationPathFilesCache(QQmlPropertyMap * cache)
{
  for(QString & key : cache->keys())
    m_destinationPathFilesCache.insert(key, cache->value(key).toStringList());
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

QStringList PhotoModel::getContainsColors(int index) const
{
  QString filePath(m_pathList.at(index));

  if(m_containsColorsCache.contains(filePath))
    return m_containsColorsCache.value(filePath);

  QStringList colorList;
  QFile file(filePath);

  QFileInfo fileInfo(file);
  QString fileName = fileInfo.baseName();

  // Получить первые цифры из названия фото, если оно начинается на IMG_
  if(fileName.startsWith("IMG_"))
    fileName = fileName.mid(4, 4);

  for(int i = 0; i < m_destinationPathList.count(); ++i)
  {
    QStringList destFileInfoList =
        m_destinationPathFilesCache.value(m_destinationPathList.at(i));

    for(QString const& destFileName : destFileInfoList)
    {
      QFileInfo destFileInfo(m_destinationPathList.at(i) + QDir::separator() + destFileName);
      if(destFileInfo.baseName().contains(fileName) &&
         (destFileInfo.size() == file.size()) &&
         (destFileInfo.lastModified() == fileInfo.lastModified())) {
        colorList.append(GetColorByName(m_destinationPathNameList.at(i)).name());
        break;
      }
    }
  }

  m_containsColorsCache.insert(filePath, colorList);
  return colorList;
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
