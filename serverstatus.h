#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H
#include <QMetaEnum>
#include <string>
enum serverStatus { requestReceived, responseReceived, waiting };

Q_DECLARE_METATYPE(serverStatus)
Q_DECLARE_METATYPE(std::string)

#endif // SERVERSTATUS_H
