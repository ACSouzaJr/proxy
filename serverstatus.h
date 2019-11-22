#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H
#include <QMetaEnum>
enum serverStatus { requestReceived, responseReceived, waiting };

Q_DECLARE_METATYPE(serverStatus)

#endif // SERVERSTATUS_H
