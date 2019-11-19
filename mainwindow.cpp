#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    connect(&server, &Proxy::payloadReceived, this, &MainWindow::onPayloadReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_gateButton_clicked()
{
//    QString string = "joaozinho gosta de azul";
//    ui->clientRequest->setPlainText(string);
    ui->gateButton->setEnabled(false);
    ui->clientRequest->setEnabled(false);
    QString text =  ui->clientRequest->toPlainText();

    qDebug() << text;
    emit gateOpened(text);
}

void MainWindow::onPayloadReceived(const QString &message)
{
    ui->gateButton->setEnabled(true);
    ui->clientRequest->setEnabled(true);
    ui->clientRequest->setPlainText(message);
//  Qstream text =  ui->clientRequest->toPlainText();
//    ui->clientRequest->setText(string);

}
