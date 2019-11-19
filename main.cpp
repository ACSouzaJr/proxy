#include "mainwindow.h"

#include <QApplication>
#include <QThread>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Proxy server;
    QThread proxyServer;

//    server.connect(&w, &MainWindow::onPayloadReceived, &Proxy::payloadReceived );
//    connect(&server, &Proxy::payloadReceived, this, &MainWindow::onPayloadReceived);
    QObject::connect(&server, &Proxy::payloadReceived, &w, &MainWindow::onPayloadReceived);
//            connect(&server, &Proxy::payloadReceived, this, &MainWindow::onPayloadReceived);
    server.moveToThread(&proxyServer);
//    connect(childrenWatcherThread, SIGNAL(started()),
//                childrenWatcher, SLOT(watch()));
    QObject::connect(&proxyServer, &QThread::started, &server, &Proxy::run);
    proxyServer.start();

    w.show();
    return a.exec();
}
