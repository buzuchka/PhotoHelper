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
  return m_data.at(index);
}

} // !PhotoHelper
