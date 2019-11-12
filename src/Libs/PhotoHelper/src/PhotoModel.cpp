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
  for(QString & path : data) {
    if(!path.startsWith("file///"))
      path.prepend("file:///");
  }

  m_data = data;
  emit dataChanged();
}

void PhotoModel::deleteItem(int index)
{
  m_data.removeAt(index);
  emit dataChanged();
}

QString PhotoModel::getFilePath(int index)
{
  QString path = m_data.at(index);
  return path.replace("file:///","");
}

} // !PhotoHelper
