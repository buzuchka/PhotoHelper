#include "DestinationFolderModel.h"

#include "ConfigManager.h"

namespace PhotoHelper {

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
  case PathRole:
      return m_data.at(index.row()).second;
  default:
      return QVariant();
  }
}

QHash<int, QByteArray> DestinationFolderModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[NameRole] = "name";
  roles[PathRole] = "path";
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
  endResetModel();
}

} // !PhotoHelper
