#include "PhotoController.h"

#include <PhotoHelper/DestinationFolderModel.h>
#include <PhotoHelper/FileOperationHandler.h>
#include <PhotoHelper/FolderSet.h>
#include <PhotoHelper/PhotoModel.h>

namespace PhotoHelper {

PhotoController::PhotoController(QObject *parent)
  : QObject(parent)
  , m_photoModel(new PhotoModel)
  , m_destinationFolderModel(new DestinationFolderModel)
  , m_currentIndex(0)
{
  connect(this, &PhotoController::currentIndexChanged,
          this, &PhotoController::currentPhotoNameChanged);
  connect(this, &PhotoController::currentIndexChanged,
          this, &PhotoController::isCurrentPhotoOrientationCorrectChanged);
  connect(this, &PhotoController::currentIndexChanged,
          this, &PhotoController::isFolderContainsCurrentPhotoChanged);
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
  m_photoModel->clear();
  m_photoModel->setLastOperatedIndex(folderSet->getLastOperatedIndex());
  m_photoModel->setDestinationPathList(folderSet->getDestinationPathListAsList());
  m_photoModel->setDestinationPathNameList(folderSet->getDestinationPathNameListAsList());

  m_destinationFolderModel->init();
}

void PhotoController::onFinishReloadData(const QStringList &sourcePhotoPathList,
                                         QQmlPropertyMap *destinationPathPhotosCache)
{
  m_photoModel->setData(sourcePhotoPathList);
  m_photoModel->setDestinationPathFilesCache(destinationPathPhotosCache);

  emit elementsCountChanged();
}

int PhotoController::getCurrentIndex() const
{
  return m_currentIndex;
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
  FileOperationHandler::copyFile(m_photoModel->getFilePath(m_currentIndex), path);
  m_photoModel->onFileCopied(m_currentIndex, path);
}

void PhotoController::deleteCurrentPhotoFromSource()
{
  FileOperationHandler::deleteFile(getCurrentPhotoPath());
  m_photoModel->deleteItem(m_currentIndex);
  if(m_currentIndex == getElementsCount() - 1)
    decreaseCurrentIndex();
  emit elementsCountChanged();
}

void PhotoController::deleteCurrentPhotoFromDestination(QString const& path)
{
  FileOperationHandler::deleteFileFromFolder(getCurrentPhotoPath(), path);
  m_photoModel->deletePhotoFromFolder(m_currentIndex, path);
}

void PhotoController::rotateCurrentPhotoRight()
{
  FileOperationHandler::rotateRightImage(getCurrentPhotoPath());
  m_photoModel->onOrientationChanged(m_currentIndex);
}

bool PhotoController::isCurrentPhotoOrientationCorrect() const
{
  return m_photoModel->getOrientation(m_currentIndex) < RightOrientation::Undefined;
}

QList<bool> PhotoController::isFolderContainsCurrentPhoto() const
{
  QList<bool> l;
  l << false;
  return l;
}

QString PhotoController::getCurrentPhotoPath() const
{
  return m_photoModel->getFilePath(m_currentIndex);
}

}
