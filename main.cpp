#include "mainwindow.h"

#include <QApplication>
#include <QThread>
#include <regex>
#include "server.h"
#include "serverstatus.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QThread proxyServer;
    Proxy server;
    server.moveToThread(&proxyServer);

    qRegisterMetaType<serverStatus>();

    QObject::connect(&server, &Proxy::payloadReceived, &w, &MainWindow::onPayloadReceived);
    QObject::connect(&w, &MainWindow::gateOpened, &server, &Proxy::onGateOpened);
    QObject::connect(&w, &MainWindow::responseFromServer, &server, &Proxy::onResponseFromServer);
    QObject::connect(&proxyServer, &QThread::started, &server, &Proxy::createServerSocket);

    proxyServer.start();

    w.show();
    return a.exec();
}


// Qtree
//#include "treemodel.h"

//#include <QApplication>
//#include <QFile>
//#include <QTreeView>

//int main(int argc, char *argv[])
//{
//    Q_INIT_RESOURCE(simpletreemodel);

//    QApplication app(argc, argv);

//    QFile file(":/default.txt");
//    file.open(QIODevice::ReadOnly);
//    TreeModel model(file.readAll());
//    file.close();

//    QTreeView view;
//    view.setModel(&model);
//    view.setWindowTitle(QObject::tr("Simple Tree Model"));
//    view.show();
//    return app.exec();
//}
