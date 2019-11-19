#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class Proxy : public QObject
{
    Q_OBJECT
public:
    explicit Proxy(QObject *parent = nullptr);
    void run();

// emit
signals:
    void payloadReceived(const QString &message);

// listen
public slots:
};

#endif // SERVER_H
