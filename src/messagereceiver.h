#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>

class MessageReceiver : public QObject
{
    Q_OBJECT
public:
    explicit MessageReceiver(QObject *parent = 0);
public slots:
    void receivedMessage( int instanceId, QByteArray message );
signals:
    QString requestedFile(QString fileName);
};

#endif // MESSAGERECEIVER_H
