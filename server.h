#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <stdint.h>

class Proxy : public QObject
{
    Q_OBJECT
public:
    explicit Proxy(QObject *parent = nullptr, uint16_t port = 8228);

// emit
signals:
    void payloadReceived(const QString &message);

// listen
public slots:
    void run();
    void onGateOpened(const QString &message);

private:
    int server_socket;
    uint16_t port;
    void createServerSocket();
};

#endif // SERVER_H
