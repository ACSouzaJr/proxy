﻿#include "mainwindow.h"
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


void MainWindow::on_requestButton_clicked()
{
//    QString string = "joaozinho gosta de azul";
//    ui->clientRequest->setPlainText(string);

    ui->requestButton->setEnabled(false);
    ui->clientRequest->setEnabled(false);
    QString text =  ui->clientRequest->toPlainText();

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
