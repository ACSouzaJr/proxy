#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"
#include "serverstatus.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void gateOpened(const QString &message);
    void responseFromServer(const QString &message);
    void spiderClicked(const std::string);

public slots:
    void onPayloadReceived(const QString &message, serverStatus);
    void onHostExtracted(const std::string host);

private slots:
    void on_requestButton_clicked();
    void on_responseButton_clicked();
    void on_spiderButton_clicked();

private:
    Ui::MainWindow *ui;
    std::string host;
};
#endif // MAINWINDOW_H
