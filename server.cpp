#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <QDebug>
#include "htmlutils.h"
#include <iostream>

Proxy::Proxy(QObject *parent, uint16_t port) : QObject(parent)
{
    this->port = port;
}

void Proxy::run()
{
    createServerSocket();
    int valread;
    char client_request[2048] = {0};
    qDebug() << "Reading message from client\n";

    valread = read(server_socket, client_request, 2048); //Blocked until message is sent from the client
    qDebug() << client_request;

    QString payload = QString::fromUtf8(client_request);
    //    QString string = "joaozinho gosta de azul";
    emit payloadReceived(payload);
}

void Proxy::createServerSocket()
{
    int opt = 1;
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the selected port
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    qDebug() << "Waiting for a client to initiate connection\n";
    if ((server_socket = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) < 0) //Blocked until connect is called in the client
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    qDebug() << "Server accepted connection\n";
}

void Proxy::onGateOpened(const QString &message)
{
    std::string request = HtmlUtils::formatRequest(message.toStdString(), '\n', "\r\n");
    std::cout << request;
    // Extract host
}
