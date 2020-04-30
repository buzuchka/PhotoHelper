#include "PhotoController.h"

#include <PhotoHelper/DestinationFolderModel.h>
#include <PhotoHelper/FileOperationHandler.h>
#include <PhotoHelper/FolderSet.h>
#include <PhotoHelper/PhotoModel.h>

#include <QColor>

namespace PhotoHelper {

QColor GetColorByName(const QString& name);

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

  m_currentIndex = folderSet->getLastOperatedIndex();
  emit currentIndexChanged();

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
  QString currentPhotoPath = getCurrentPhotoPath();
  FileOperationHandler::deleteFileFromFolder(currentPhotoPath, path);
  m_photoModel->onPhotoDeletedFromDestination(m_currentIndex, path);
  updateDestinationFolderModelContainsState();
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

QString PhotoController::getCurrentPhotoPath() const
{
  return m_photoModel->getFilePath(m_currentIndex);
}

void PhotoController::updateDestinationFolderModelContainsState()
{
  QList<bool> states = m_photoModel->getContainsState(m_currentIndex);
  m_destinationFolderModel->setContainsCurrentPhotoList(states);
}

}
