#ifndef PHOTOHELPER_DESTINATIONFOLDERMODEL_H
#define PHOTOHELPER_DESTINATIONFOLDERMODEL_H

#include <photohelper_export.h>

#include <PhotoHelper/Types.h>

#include <QAbstractListModel>

namespace PhotoHelper {

class FolderSet;

class PHOTOHELPER_EXPORT DestinationFolderModel : public QAbstractListModel
{
  Q_OBJECT
public:
    enum Roles
    {
      NameRole = Qt::UserRole + 1,
      ColorRole,
      PathRole,
      ContainsRole
    };

    enum State
    {
      Checked,
      Unchecked,
      Undefined
    };
    Q_ENUM(State)

    DestinationFolderModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    void init();

    void setFolderData(const FolderConfigList &destinationPathList);

    void setContainsCurrentPhotoList(QList<bool> containsCurrentPhotoList);

private:
    FolderConfigList m_data;
    QList<bool> m_containsCurrentPhotoList;
};

} // !PhotoHelper

#endif
