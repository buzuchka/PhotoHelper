#include "DestinationFolderModel.h"
#include "ConfigManager.h"

#include <QColor>
#include <QCryptographicHash>

namespace PhotoHelper {

QColor GetColorByName(const QString& name)
{
  QColor color;
  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(name.toUtf8());
  const auto& hashStr = hash.result().toHex();
  QString colorStr = QString("#") + hashStr.left(6);
  color.setNamedColor(colorStr);
  return color;
}

DestinationFolderModel::DestinationFolderModel(QObject *parent)
  : QAbstractListModel(parent)
{
}

int DestinationFolderModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return m_data.size();
}

QVariant DestinationFolderModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) {
      return QVariant();
  }

  switch (role) {
  case NameRole:
      return m_data.at(index.row()).first;
  case ColorRole:
    return GetColorByName(m_data.at(index.row()).first);
  case PathRole:
      return m_data.at(index.row()).second;
  case ContainsRole:
    if(m_containsCurrentPhotoList.count() > 0)
      return m_containsCurrentPhotoList.at(index.row());
  default:
      return QVariant();
  }
}

QHash<int, QByteArray> DestinationFolderModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[NameRole] = "name";
  roles[ColorRole] = "color";
  roles[PathRole] = "path";
  roles[ContainsRole] = "contains";
  return roles;
}

void DestinationFolderModel::init()
{
  auto *configManager = ConfigManager::getInstance();
  setFolderData(configManager->getDestinationPathList());
}

void DestinationFolderModel::setFolderData(const FolderConfigList &destinationPathList)
{
  beginResetModel();
  m_data = destinationPathList;
  for(int i = 0; i < m_data.count(); ++i)
    m_containsCurrentPhotoList.append(false);
  endResetModel();
}

void DestinationFolderModel::setContainsCurrentPhotoList(
    QList<bool> containsCurrentPhotoList)
{
  beginResetModel();
  m_containsCurrentPhotoList = containsCurrentPhotoList;
  endResetModel();
}

} // !PhotoHelper
