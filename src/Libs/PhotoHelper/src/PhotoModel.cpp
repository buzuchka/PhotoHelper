#include "PhotoModel.h"

#include <QDir>

namespace PhotoHelper {

PhotoModel::PhotoModel(QObject *parent)
  : QObject(parent)
{
}

QStringList PhotoModel::data() const
{
  return m_data;
}

void PhotoModel::setData(const QStringList &pathList)
{
  auto data = pathList;

  m_data = pathList;
  emit dataChanged();
}

void PhotoModel::deleteItem(int index)
{
  m_data.removeAt(index);
  emit dataChanged();
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
