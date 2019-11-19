#include "server.h"

Proxy::Proxy(QObject *parent) : QObject(parent)
{

}

void Proxy::run()
{
    QString string = "joaozinho gosta de azul";
    emit payloadReceived(string);
}
