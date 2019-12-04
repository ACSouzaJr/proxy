#include "mainwindow.h"

#include <QApplication>
#include <QThread>
#include <regex>
#include "server.h"
#include "serverstatus.h"
#include "toolkit.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(simpletreemodel);

    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();

    QThread proxyServer, toolkitThread;
    Proxy *server = new Proxy();
    server->moveToThread(&proxyServer);

    Toolkit *tools = new Toolkit();
    tools->moveToThread(&toolkitThread);

    qRegisterMetaType<serverStatus>();
    qRegisterMetaType<std::string>();

    QObject::connect(server, &Proxy::payloadReceived, w, &MainWindow::onPayloadReceived, Qt::QueuedConnection);
    QObject::connect(w, &MainWindow::gateOpened, server, &Proxy::onGateOpened, Qt::QueuedConnection);
    QObject::connect(w, &MainWindow::responseFromServer, server, &Proxy::onResponseFromServer, Qt::QueuedConnection);
    QObject::connect(server, &Proxy::hostExtracted, w, &MainWindow::onHostExtracted, Qt::QueuedConnection);

    QObject::connect(w, &MainWindow::spiderClicked, tools, &Toolkit::spider, Qt::QueuedConnection);
    QObject::connect(w, &MainWindow::dumperClicked, tools, &Toolkit::recursiveClient, Qt::QueuedConnection);

    QObject::connect(tools, &Toolkit::newAcessedLink, w, &MainWindow::onNewAcessedLink, Qt::QueuedConnection);
    QObject::connect(tools, &Toolkit::appendRoot, w, &MainWindow::onAppendRoot, Qt::QueuedConnection);

//    QObject::connect(&proxyServer, &QThread::started, [=](){ tools->recursiveClient("samotabr.com"); });
     QObject::connect(&proxyServer, &QThread::started, server, &Proxy::createServerSocket, Qt::QueuedConnection);


    proxyServer.start();
    toolkitThread.start();

    w->show();
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
