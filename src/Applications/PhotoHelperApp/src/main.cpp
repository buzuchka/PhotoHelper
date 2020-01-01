#include "Proxy.h"

#include <QApplication>

#include <PhotoHelper/ConfigManager.h>
#include <PhotoHelper/DestinationFolderModel.h>
#include <PhotoHelper/FileOperationHandler.h>
#include <PhotoHelper/FolderSet.h>
#include <PhotoHelper/PhotoModel.h>

#include <QStringList>

#include <QtQuick/QQuickView>

#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>

#include <QImageReader>

using namespace PhotoHelper;

void qmlRegisterTypes()
{
  qmlRegisterType<DestinationFolderModel>("DestinationFolderModel", 1, 0,
                                          "DestinationFolderModel");
  qmlRegisterType<FileOperationHandler>("FileOperationHandler", 1, 0,
                                        "FileOperationHandler");
  qmlRegisterType<FolderSet>("FolderSet", 1, 0, "FolderSet");
  qmlRegisterType<PhotoModel>("PhotoModel", 1, 0, "PhotoModel");
  qmlRegisterType<Proxy>("Proxy", 1, 0, "Proxy");
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  app.setWindowIcon(QIcon(":icons/logo"));
  app.setOrganizationName("Buzuchka");
  app.setOrganizationDomain("Photo Helper");
  app.setApplicationVersion("0.1");

  qmlRegisterTypes();

  Q_INIT_RESOURCE(PhotoHelper);

  auto *configManager = ConfigManager::getInstance();

  FileOperationHandler operationHandler;

  FolderSet folderSet;
  folderSet.setSourcePath(configManager->getSourceFolderPair());
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

  Proxy proxy;

  QQmlApplicationEngine engine;

  engine.rootContext()->setContextProperty("cppFolderSet",
                                           &folderSet);
  engine.rootContext()->setContextProperty("cppFileOperationHandler",
                                           &operationHandler);
  engine.rootContext()->setContextProperty("cppProxy",
                                           &proxy);

  engine.load(QUrl("qrc:/main.qml"));

  return app.exec();
}
