/*
* VLC-Qt QML Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QQmlApplicationEngine>
#include <QOpenGLContext>

#include "mdkplayer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("mdk QML Player");
    QCoreApplication::setOrganizationName("mdk QML Player demo");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<QmlMDKPlayer>("MDKPlayer", 1, 0, "MDKPlayer");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/video.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
