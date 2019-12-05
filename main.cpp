#include "mainwindow.h"

#include <QApplication>
#include <QThread>
#include <regex>
#include "server.h"
#include "serverstatus.h"
#include "toolkit.h"
#include <stdint.h>
#include <iostream>

int main(int argc, char *argv[])
{
    uint16_t port;
    std::string flag;

    if (argc == 1)
    {
        port = 8228;
    }
    else if (argc == 3)
    {
        flag = argv[1];
        if (flag.compare("-p") != 0)
        {
            std::cout << "Argumento " << argv[1] << " invalido! " << std::endl;
            return 0;
        }
        port = std::stoi(argv[2]);
    }

    QApplication a(argc, argv);
    MainWindow w;

    QThread proxyServer, toolkitThread;
    Proxy server(port);
    server.moveToThread(&proxyServer);

    Toolkit tools;
    tools.moveToThread(&toolkitThread);

    qRegisterMetaType<serverStatus>();
    qRegisterMetaType<std::string>();

    QObject::connect(&server, &Proxy::payloadReceived, &w, &MainWindow::onPayloadReceived, Qt::QueuedConnection);
    QObject::connect(&w, &MainWindow::gateOpened, &server, &Proxy::onGateOpened, Qt::QueuedConnection);
    QObject::connect(&w, &MainWindow::responseFromServer, &server, &Proxy::onResponseFromServer, Qt::QueuedConnection);
    QObject::connect(&server, &Proxy::hostExtracted, &w, &MainWindow::onHostExtracted, Qt::QueuedConnection);

    QObject::connect(&w, &MainWindow::spiderClicked, &tools, &Toolkit::spider, Qt::QueuedConnection);
    QObject::connect(&w, &MainWindow::dumperClicked, &tools, &Toolkit::recursiveClient, Qt::QueuedConnection);

    QObject::connect(&tools, &Toolkit::newAcessedLink, &w, &MainWindow::onNewAcessedLink, Qt::QueuedConnection);
    QObject::connect(&tools, &Toolkit::appendRoot, &w, &MainWindow::onAppendRoot, Qt::QueuedConnection);

//    QObject::connect(&proxyServer, &QThread::started, [=](){ tools->recursiveClient("samotabr.com"); });
     QObject::connect(&proxyServer, &QThread::started, &server, &Proxy::createServerSocket, Qt::QueuedConnection);


    proxyServer.start();
    toolkitThread.start();

    w.show();
    return a.exec();
}
