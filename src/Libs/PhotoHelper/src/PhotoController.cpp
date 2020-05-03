#include "PhotoController.h"

#include <PhotoHelper/DestinationFolderModel.h>
#include <PhotoHelper/FileOperationHandler.h>
#include <PhotoHelper/FolderSet.h>
#include <PhotoHelper/PhotoModel.h>

#include <QColor>

#include <QtQml/QQmlEngine>

namespace PhotoHelper {

QColor GetColorByName(const QString& name);

PhotoController::PhotoController(QObject *parent)
  : QObject(parent)
  , m_photoModel(new PhotoModel)
  , m_destinationFolderModel(new DestinationFolderModel)
  , m_currentIndex(0)
{
  QQmlEngine::setObjectOwnership(m_photoModel, QQmlEngine::CppOwnership);
  QQmlEngine::setObjectOwnership(m_destinationFolderModel, QQmlEngine::CppOwnership);

  connect(this, &PhotoController::currentIndexChanged,
          this, &PhotoController::currentPhotoNameChanged);
  connect(this, &PhotoController::currentIndexChanged,
          this, &PhotoController::isCurrentPhotoOrientationCorrectChanged);
  connect(this, &PhotoController::currentIndexChanged,
          this, &PhotoController::updateDestinationFolderModelContainsState);
}

PhotoModel *PhotoController::getPhotoModel()
{
  return m_photoModel;
}

DestinationFolderModel *PhotoController::getDestinationFolderModel()
{
  return m_destinationFolderModel;
}

int PhotoController::getElementsCount() const
{
  return m_photoModel->getElementsCount();
}

void PhotoController::onStartReloadData(FolderSet *folderSet)
{
  auto folderPathList = folderSet->getDestinationPathListAsList();
  auto folderNameList = folderSet->getDestinationPathNameListAsList();

  QList<QPair<QString /*folderPath*/, QString /*color*/>> folderPathColorCache;

  for(int i = 0; i < folderPathList.count(); ++i)
    folderPathColorCache.push_back({folderPathList.at(i),
                                    GetColorByName(folderNameList.at(i)).name()});

  m_photoModel->clear();
  m_photoModel->setLastOperatedIndex(folderSet->getLastOperatedIndex());
  m_photoModel->setFolderPathColorCache(folderPathColorCache);

  setCurrentIndex(folderSet->getLastOperatedIndex());

  m_destinationFolderModel->init();
}

void PhotoController::onFinishReloadData(const QStringList &sourcePhotoPathList,
                                         QQmlPropertyMap *destinationPathPhotosCache)
{
  m_photoModel->setDestinationPathFilesCache(destinationPathPhotosCache);
  m_photoModel->setData(sourcePhotoPathList);

  updateDestinationFolderModelContainsState();

  emit elementsCountChanged();
}

int PhotoController::getCurrentIndex() const
{
  return m_currentIndex;
}

void PhotoController::setCurrentIndex(int index)
{
  m_currentIndex = index;
  emit currentIndexChanged();
}

void PhotoController::increaseCurrentIndex()
{
  m_currentIndex++;
  emit currentIndexChanged();
}

void PhotoController::decreaseCurrentIndex()
{
  m_currentIndex--;
  emit currentIndexChanged();
}

QString PhotoController::getCurrentPhotoName() const
{
  return m_photoModel->getFileName(m_currentIndex);
}

void PhotoController::copyCurrentPhoto(const QString &path)
{
  QString copiedPhotoName = FileOperationHandler::copyFile(
        m_photoModel->getFilePath(m_currentIndex), path);

  if(copiedPhotoName.isEmpty())
    return;

  m_photoModel->onPhotoCopied(m_currentIndex, path, copiedPhotoName);
  updateDestinationFolderModelContainsState();
}

void PhotoController::copySelectedPhotos(const QString &path)
{
  for(int i = 0; i < m_selectedIndexes.count(); ++i)
  {
    QString copiedPhotoName = FileOperationHandler::copyFile(
          m_photoModel->getFilePath(m_selectedIndexes.at(i)), path);

    if(copiedPhotoName.isEmpty())
      continue;

    m_photoModel->onPhotoCopied(m_selectedIndexes.at(i), path, copiedPhotoName);
  }
  updateDestinationFolderModelContainsState();
}

void PhotoController::deleteCurrentPhotoFromSource()
{
  FileOperationHandler::deleteFile(getCurrentPhotoPath());
  m_photoModel->deleteItem(m_currentIndex);
  if(m_currentIndex == getElementsCount() - 1)
    decreaseCurrentIndex();
  emit elementsCountChanged();
}

void PhotoController::deleteSelectedPhotosFromSource()
{
  for(int i = 0; i < m_selectedIndexes.count(); ++i)
  {
    FileOperationHandler::deleteFile(
          m_photoModel->getFilePath(m_selectedIndexes.at(i)));
    m_photoModel->deleteItem(m_selectedIndexes.at(i));
  }

  setSelectedIndexes({});
}

void PhotoController::deleteCurrentPhotoFromDestination(QString const& path)
{
  QString currentPhotoPath = getCurrentPhotoPath();
  FileOperationHandler::deleteFileFromFolder(currentPhotoPath, path);
  m_photoModel->onPhotoDeletedFromDestination(m_currentIndex, path);
  updateDestinationFolderModelContainsState();
}

void PhotoController::deleteSelectedPhotosFromDestination(const QString &path)
{
  for(int i = 0; i < m_selectedIndexes.count(); ++i)
  {
    FileOperationHandler::deleteFileFromFolder(
          m_photoModel->getFilePath(m_selectedIndexes.at(i)), path);
    m_photoModel->onPhotoDeletedFromDestination(m_selectedIndexes.at(i), path);
  }
  updateDestinationFolderModelContainsState();
}

void PhotoController::rotateCurrentPhotoRight()
{
  FileOperationHandler::rotateRightImage(getCurrentPhotoPath());
  m_photoModel->onOrientationChanged(m_currentIndex);
}

void PhotoController::rotateSelectedPhotosRight()
{
  for(int i = 0; i < m_selectedIndexes.count(); ++i)
  {
    if(m_photoModel->getOrientation(m_selectedIndexes.at(i)) == RightOrientation::Undefined)
      continue;

    FileOperationHandler::rotateRightImage(m_photoModel->getFilePath(
                                             m_selectedIndexes.at(i)));
    m_photoModel->onOrientationChanged(m_selectedIndexes.at(i));
  }
}

bool PhotoController::isCurrentPhotoOrientationCorrect() const
{
  return m_photoModel->getOrientation(m_currentIndex) < RightOrientation::Undefined;
}

QString PhotoController::getCurrentPhotoPath() const
{
  return m_photoModel->getFilePath(m_currentIndex);
}

void PhotoController::updateDestinationFolderModelContainsState()
{
  if(!m_selectedIndexes.isEmpty())
  {
    QList<bool> states =  m_photoModel->getContainsState(m_currentIndex);
    for(int i = 1; i < m_selectedIndexes.count(); ++i)
    {
      QList<bool> currentStates =
          m_photoModel->getContainsState(m_selectedIndexes.at(i));

      for(int i = 0; i < states.count(); ++i)
      {
        if(states.at(i) != currentStates.at(i))
          states[i] = false;
      }
    }
    m_destinationFolderModel->setContainsCurrentPhotoList(states);
  }
  else
  {
    QList<bool> states = m_photoModel->getContainsState(m_currentIndex);
    m_destinationFolderModel->setContainsCurrentPhotoList(states);
  }
}

QList<int> PhotoController::selectedIndexes() const
{
  return m_selectedIndexes;
}

void PhotoController::setSelectedIndexes(const QList<int> &indexes)
{
  m_selectedIndexes = indexes;
  emit selectedIndexesChanged();
}

void PhotoController::switchToSelectedIndexes()
{
  setSelectedIndexes({m_currentIndex});
}

void PhotoController::switchToCurrentIndex()
{
  if(m_selectedIndexes.count() > 0)
    setCurrentIndex(m_selectedIndexes.at(0));
}

}
