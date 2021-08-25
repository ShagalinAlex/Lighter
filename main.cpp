#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "serialadapter.h"
#include "rgbdapter.h"
#include "myconicalgradient.h"
#include "avaservice.h"
#include "gateadapter.h"
#include "buttonadapter.h"
#include "bme280adapter.h"
#include "compressoradapter.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setOrganizationName("Shagalin A.M.");
    app.setOrganizationDomain("shagalinam@gmail.com");
    app.setApplicationName("Home Application");


    QQmlApplicationEngine engine;

    SerialAdapter::declareQML();
    GateAdapter::declareQML();
    ButtonAdapter::declareQML();
    RGBAdapter::declareQML();
    BME280Adapter::declareQML();
    CompressorAdapter::declareQML();

    MyConicalGradient::declareQML();
    AvaService::declareQML();

    DataAdapter::declareQML();

    engine.load(QUrl(QStringLiteral("qrc:/AvaInterface.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
