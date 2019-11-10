#include "FolderSet.h"

#include <QVariantMap>

namespace PhotoHelper {

FolderSet::FolderSet()
{
}

QString FolderSet::getSourceName() const
{
  return m_sourcePath.first;
}

QString FolderSet::getSourcePath() const
{
  return m_sourcePath.second;
}

void FolderSet::setSourcePath(const QString &name,
                              const QString &path)
{
  m_sourcePath = {name, path};
  emit sourceNameChanged();
  emit sourcePathChanged();
}

void FolderSet::setSourcePath(const FolderConfigPair &folderPair)
{
  m_sourcePath = folderPair;
  emit sourceNameChanged();
  emit sourcePathChanged();
}

QVariantList FolderSet::getDestinationVariantList() const
{
  QVariantList list;
  for(int i = 0; i < m_destinationPathList.size(); ++i)
  {
    QVariantMap pair;
    pair.insert("first", m_destinationPathList.at(i).first);
    pair.insert("second", m_destinationPathList.at(i).second);
    list << pair;
  }
  return list;
}

FolderConfigList FolderSet::getDestinationPathList() const
{
  return m_destinationPathList;
}

void FolderSet::setDestinationPathList(const FolderConfigList &list)
{
  m_destinationPathList = list;
  emit destinationPathListChanged();
}

QString FolderSet::getDestinationName(int index) const
{
    return m_destinationPathList.at(index).first;
}

QString FolderSet::getDestinationPath(int index) const
{
  return m_destinationPathList.at(index).second;
}

void FolderSet::addDestinationPath(const QString &name,
                                   const QString &path)
{
  m_destinationPathList.append({name, path});
  emit destinationPathListChanged();
}

} // !PhotoHelper
