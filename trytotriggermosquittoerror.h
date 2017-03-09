#ifndef TRYTOTRIGGERMOSQUITTOERROR_H
#define TRYTOTRIGGERMOSQUITTOERROR_H

#include <QObject>
#include <qmqtt/qmqtt_client.h>
#include <QScopedPointer>
#include <qmqtt/qmqtt_message.h>
#include <QTimer>

class TryToTriggerMosquittoError : public QObject
{
    Q_OBJECT
    QScopedPointer<QMQTT::Client> mMqttClient;
    QTimer mPublishTimer;
public:
    explicit TryToTriggerMosquittoError(const QString &hostname, const int port, const QString &username, const QString &password, QObject *parent = 0);
    void doStuff();

signals:

private slots:
    void onReceived(const QMQTT::Message &message);
    void onError(const QMQTT::ClientError error);
    void onConnect();

    /**
     * @brief keepAlivePublish can be called manually or by timer, to make the other end keep publishing stuff
     */
    void keepAlivePublish();

public slots:
};

#endif // TRYTOTRIGGERMOSQUITTOERROR_H
