#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <stdint.h>
#include <netinet/in.h>
#include "serverstatus.h"

class Proxy : public QObject
{
    Q_OBJECT
public:
    explicit Proxy(uint16_t port = 8228, QObject *parent = nullptr);

// emit
signals:
    void payloadReceived(const QString &message, serverStatus);
    void hostExtracted(const std::string host);

// listen
public slots:
    void createServerSocket();
    void onGateOpened(const QString &message);
    void onResponseFromServer(const QString &message);

private:
    int server_socket;
    int client_socket;
    int server_fd;
    struct sockaddr_in address;
    std::string host;
    uint16_t port;
    void readFromClient();
    void createClientSocket(std::string hostname, uint16_t serverPort);
};

#endif // SERVER_H
