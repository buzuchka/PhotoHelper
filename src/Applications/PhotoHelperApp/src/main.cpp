#include <QApplication>

#include <PhotoHelper/ConfigManager.h>
#include <PhotoHelper/DataLoader.h>
#include <PhotoHelper/DestinationFolderModel.h>
#include <PhotoHelper/FileOperationHandler.h>
#include <PhotoHelper/FolderSet.h>
#include <PhotoHelper/PhotoController.h>
#include <PhotoHelper/PhotoModel.h>

#include <QStringList>

#include <QtQuick/QQuickView>

#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>

using namespace PhotoHelper;

void qmlRegisterTypes()
{
  qmlRegisterType<DataLoader>("DataLoader", 1, 0, "DataLoader");
  qmlRegisterType<DestinationFolderModel>("DestinationFolderModel", 1, 0,
                                          "DestinationFolderModel");
  qmlRegisterType<FolderSet>("FolderSet", 1, 0, "FolderSet");
  qmlRegisterType<PhotoController>("PhotoController", 1, 0, "PhotoController");
  qmlRegisterType<PhotoModel>("PhotoModel", 1, 0, "PhotoModel");
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  app.setWindowIcon(QIcon(":icons/logo"));
  app.setOrganizationName("Buzuchka");
  app.setOrganizationDomain("Photo Helper");
  app.setApplicationVersion("0.1");

  qmlRegisterTypes();

  qRegisterMetaType<QQmlPropertyMap*>("QQmlPropertyMap");
  qRegisterMetaType<DestinationFolderModel*>("DestinationFolderModel*");
  qRegisterMetaType<FolderSet*>("FolderSet*");
  qRegisterMetaType<PhotoModel*>("PhotoModel*");

  Q_INIT_RESOURCE(PhotoHelper);

  auto *configManager = ConfigManager::getInstance();

  FolderSet folderSet;
  folderSet.setSourcePath(configManager->getSourceFolderPair());
  folderSet.setLastOperatedIndex(configManager->getLastOperatedIndex());
  folderSet.setDestinationPathList(configManager->getDestinationPathList());

  QObject::connect(&folderSet, &FolderSet::sourcePathChanged,
          [configManager, &folderSet]()
  {
    configManager->setSourcePath({folderSet.getSourceName(),
                                  folderSet.getSourcePath()});
  });
  QObject::connect(&folderSet, &FolderSet::destinationPathListChanged,
          [configManager, &folderSet]()
  {
    configManager->setDestinationPathList(folderSet.getDestinationPathList());
  });
  QObject::connect(&folderSet, &FolderSet::lastOperatedIndexChanged,
          [configManager, &folderSet]()
  {
    configManager->setLastOperatedIndex(folderSet.getLastOperatedIndex());
  });

  QQmlApplicationEngine engine;

  engine.rootContext()->setContextProperty("cppFolderSet",
                                           &folderSet);

  engine.load(QUrl("qrc:/main.qml"));

  return app.exec();
}
