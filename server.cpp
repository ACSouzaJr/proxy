#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <QDebug>
#include "htmlutils.h"
#include <iostream>
#include <sstream>
#include "serverstatus.h"

Proxy::Proxy(QObject *parent, uint16_t port) : QObject(parent)
{
    this->port = port;
}

bool is_valid_host(const string host, int server_socket)
{
    bool host_valid = true;
    if (host == "detectportal.firefox.com" ||
            host == "g.symcd.com" ||
            host == "sr.symcd.com" ||
            host == "ocsp.digicert.com" ||
            host == "ocsp.pki.goog")
    {
        host_valid = false;
        close(server_socket);
    }

    return host_valid;
}

void Proxy::readFromClient()
{
    int addrlen = sizeof(address);
    int valread;
    char client_request[2048] = {0};
    string str;

    do {

        bzero(client_request, sizeof(client_request));
        qDebug() << "Waiting for a client to initiate connection\n";
        if ((server_socket = accept(server_fd, (struct sockaddr *)&address,
                                    (socklen_t *)&addrlen)) < 0) //Blocked until connect is called in the client
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        qDebug() << "Server accepted connection\n";
        qDebug() << "Reading message from client\n";

        valread = read(server_socket, client_request, sizeof(client_request)); //Blocked until message is sent from the client
        qDebug() << client_request;

    }while (!is_valid_host(HtmlUtils::extractHost(client_request), server_socket));

    QString payload = QString::fromUtf8(client_request);
    //    QString string = "joaozinho gosta de azul";
    emit payloadReceived(payload, requestReceived);

}

void Proxy::createServerSocket()
{
    int opt = 1;

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
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Read message from client
    readFromClient();
}

void Proxy::onGateOpened(const QString &message)
{
    string request = message.toStdString();
    // Extract host
    string host = HtmlUtils::extractHost(request);

    // format request
    std::string requestFormatted = HtmlUtils::formatRequest(request, '\n', "\r\n");
//    std::cout << host;
    qDebug("\n\t%s\n", host.c_str());

    // connect to server
    createClientSocket(host);

    // send request to server
//    HtmlUtils::replaceInHeader(requestFormatted, "keep-alive", "close");
//    // format
//    std::string str2 = "keep-alive";
//    std::size_t found = requestFormatted.find(str2);
//    // std::cout << found;
//    requestFormatted.replace(found, str2.length(), "close");
//    // end format
    requestFormatted = HtmlUtils::replaceInHeader(requestFormatted, "Connection: ", "close");
    requestFormatted = HtmlUtils::replaceInHeader(requestFormatted, "Accept-Encoding: ", "identity");

    send(client_socket, requestFormatted.c_str(), strlen(requestFormatted.c_str()), 0);

    // receive response from server
    std::stringstream buffer;
    char cur;

    while (recv(client_socket, &cur, 1, 0) > 0){
        buffer << cur;
    }

    // close connection
    close(client_socket);

    // emit response received
    QString responsePayload = QString::fromUtf8(buffer.str().c_str());

    emit payloadReceived(responsePayload, responseReceived);
}

void Proxy::onResponseFromServer(const QString &message)
{
    string server_response = message.toStdString();
    // Format request

//    server_response = HtmlUtils::formatRequest(server_response, '\n', "\r\n");
    qDebug("%s", server_response.c_str());
    qDebug() << "Sending response to client \n";

    send(server_socket, server_response.c_str() , strlen(server_response.c_str()), 0);

    qDebug() << "response sent \n";

    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
    readFromClient();
}

void Proxy::createClientSocket(string hostname)
{
    struct sockaddr_in serv_addr;
    struct hostent *host_entry;    /* server host name information        */
    char *IPbuffer;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("\n Socket creation error \n");
            exit(EXIT_FAILURE);
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(80);

        host_entry = gethostbyname(hostname.c_str());
        if (host_entry == (struct hostent *) 0)
        {
            perror("Gethostbyname failed\n");
            exit(EXIT_FAILURE);
        }


        IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
//        std::cout << IPbuffer;

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, IPbuffer, &serv_addr.sin_addr) <= 0)
        {
            perror("\nInvalid address/ Address not supported \n");
            exit(EXIT_FAILURE);
        }

        qDebug() << "connecting to server \n";
        if (::connect(client_socket, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("\nConnection Failed \n");
            exit(EXIT_FAILURE);
        }

        qDebug() << "connected to server successfully\n";
}
