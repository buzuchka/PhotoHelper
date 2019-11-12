#include <QApplication>

#include <PhotoHelper/ConfigManager.h>
#include <PhotoHelper/DestinationFolderModel.h>
#include <PhotoHelper/FileOperationHandler.h>
#include <PhotoHelper/FolderSet.h>
#include <PhotoHelper/PhotoModel.h>

#include <QStringList>

#include <QtQuick/QQuickView>

#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

#include <QImageReader>

using namespace PhotoHelper;

void qmlRegisterTypes()
{
  qmlRegisterType<DestinationFolderModel>("DestinationFolderModel", 1, 0,
                                          "DestinationFolderModel");
  qmlRegisterType<FileOperationHandler>("FileOperationHandler", 1, 0, "FileOperationHandler");
  qmlRegisterType<FolderSet>("FolderSet", 1, 0, "FolderSet");
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

  QQuickView viewer;

  QObject::connect(viewer.engine(), &QQmlEngine::quit,
                   &viewer, &QWindow::close);

  viewer.setTitle(QStringLiteral("QML Photo Helper"));

  viewer.rootContext()->setContextProperty("cppFolderSet",
                                           &folderSet);
  viewer.rootContext()->setContextProperty("cppFileOperationHandler",
                                           &operationHandler);

  viewer.setSource(QUrl("qrc:/main.qml"));
  viewer.setResizeMode(QQuickView::SizeRootObjectToView);
  viewer.show();

  return app.exec();
}
