#include <QApplication>

#include <PhotoHelper/ConfigManager.h>

#include <QStringList>

#include <QtQuick/QQuickView>

#include <QtQml/QQmlEngine>

using namespace PhotoHelper;

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  Q_INIT_RESOURCE(PhotoHelper);

  auto *d = ConfigManager::getInstance();
  d->setSourcePath("ololo");
  d->setDestinationPathList({"popopp", "sjkkjsk"});

  auto fs  = d->getSourcePath();
  auto ds = d->getDestinationPathList();

  QQuickView viewer;

  QObject::connect(viewer.engine(), &QQmlEngine::quit,
                   &viewer, &QWindow::close);

  viewer.setTitle(QStringLiteral("QML Photo Helper"));

  /*ScatterData *data = new ScatterData;
  TwoAxesChartController * viewController = new TwoAxesChartController;

   viewer.rootContext()->setContextProperty("viewController",
                                            viewController);
*/

  viewer.setSource(QUrl("qrc:/main.qml"));
  viewer.setResizeMode(QQuickView::SizeRootObjectToView);
  viewer.show();

  return app.exec();
}
