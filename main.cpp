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
