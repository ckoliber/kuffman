#include <QGuiApplication>
#include <QQuickStyle>
#include <QQmlContext>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <kuffmanconnection.h>

int main(int argc, char *argv[]){
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<KuffmanConnection>("Kuffman",1,0,"KuffmanConnection");

    QLoggingCategory::setFilterRules("default.debug=true");

    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    QPM_INIT(engine);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
