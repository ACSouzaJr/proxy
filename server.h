#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class Proxy : public QObject
{
    Q_OBJECT
public:
    explicit Proxy(QObject *parent = nullptr);

// emit
signals:
    void payloadReceived(const QString &message);

// listen
public slots:
    void run();

};

#endif // SERVER_H
