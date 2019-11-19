#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

void MainWindow::onPayloadReceived(const QString &message)
{
    ui->clientRequest->setPlainText(message);
//  Qstream text =  ui->clientRequest->toPlainText();
//    ui->clientRequest->setText(string);

}
