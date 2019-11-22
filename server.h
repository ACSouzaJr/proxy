#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <stdint.h>
#include "serverstatus.h"

class Proxy : public QObject
{
    Q_OBJECT
public:
    explicit Proxy(QObject *parent = nullptr, uint16_t port = 8228);

// emit
signals:
    void payloadReceived(const QString &message, serverStatus);

// listen
public slots:
    void run();
    void onGateOpened(const QString &message);
    void onResponseFromServer(const QString &message);

private:
    int server_socket;
    int client_socket;
    uint16_t port;
    void createServerSocket();
    void createClientSocket(std::string hostname);
};

#endif // SERVER_H
