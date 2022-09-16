#include <QDebug>
#include "messagereceiver.h"

MessageReceiver::MessageReceiver(QObject *parent) : QObject(parent)
{
}

void MessageReceiver::receivedMessage(int instanceId, QByteArray message)
{
    emit requestedFile(QString::fromUtf8(message));
}
