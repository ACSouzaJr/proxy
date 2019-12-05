#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    treeWindow = new HypertextTree();

//    connect(this, &MainWindow::onNewAcessedLink, &treeWindow, &HypertextTree::insertChild);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_requestButton_clicked()
{
//    QString string = "joaozinho gosta de azul";
//    ui->clientRequest->setPlainText(string);

    ui->requestButton->setEnabled(false);
    ui->clientRequest->setEnabled(false);
    QString text =  ui->clientRequest->toPlainText();
//    ui->clientRequest->clear();

    qDebug() << text;
    emit gateOpened(text);
}

void MainWindow::onPayloadReceived(const QString &message, serverStatus status)
{
    if (status == requestReceived)
    {
        ui->requestButton->setEnabled(true);
        ui->clientRequest->setEnabled(true);
        ui->clientRequest->setPlainText(message);
    }
    else if (status == responseReceived)
    {
        ui->responseButton->setEnabled(true);
        ui->serverResponse->setEnabled(true);
        ui->serverResponse->setPlainText(message);
    }
//  Qstream text =  ui->clientRequest->toPlainText();
//    ui->clientRequest->setText(string);

}

void MainWindow::on_responseButton_clicked()
{
    ui->responseButton->setEnabled(false);
    ui->serverResponse->setEnabled(false);
    QString text =  ui->serverResponse->toPlainText();
//    ui->serverResponse->clear();

//    qDebug() << text;
    emit responseFromServer(text);
}


void MainWindow::on_spiderButton_clicked()
{
    ui->spiderButton->setEnabled(false);
    ui->dumperButton->setEnabled(false);
    treeWindow->show();
    emit spiderClicked(host);
}

void MainWindow::onHostExtracted(const std::string host)
{
    ui->spiderButton->setEnabled(true);
    ui->dumperButton->setEnabled(true);
    this->host = host;
}

void MainWindow::onAppendRoot(QTreeWidgetItem *rootItem)
{
    treeWindow->addTreeRoot(rootItem);
}

void MainWindow::onNewAcessedLink(QTreeWidgetItem *parent, QTreeWidgetItem *childItem)
{
    treeWindow->addTreeChild(parent, childItem);
}

void MainWindow::on_dumperButton_clicked()
{
    ui->dumperButton->setEnabled(false);
    emit dumperClicked(host);
}
