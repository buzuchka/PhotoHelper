#ifndef PHOTOHELPER_PHOTOMODEL_H
#define PHOTOHELPER_PHOTOMODEL_H

#include <photohelper_export.h>

#include <QObject>
#include <QStringList>

namespace PhotoHelper {

class PHOTOHELPER_EXPORT PhotoModel : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QStringList data
             READ data
             NOTIFY dataChanged)
public:
  PhotoModel(QObject *parent = nullptr);

  QStringList data() const;
  Q_INVOKABLE void setData(const QStringList &data);

  //! Удаление элемента
  Q_INVOKABLE void deleteItem(int index);

  //! Возвращает путь по индексу
  Q_INVOKABLE QString getFilePath(int index);

signals:
  void dataChanged();

private:
  QStringList m_data;
};

} // !PhotoHelper

#endif
