#include "trytotriggermosquittoerror.h"
#include <QDateTime>

TryToTriggerMosquittoError::TryToTriggerMosquittoError(const QString &hostname, const int port, const QString &username, const QString &password, QObject *parent) : QObject(parent)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QString clientID = QString("TryingToBreakSSL_%1").arg(qrand());

    mMqttClient.reset(new QMQTT::Client(hostname, port, true, true)); // TODO: I ignoreSelfSigned can be false and it still works?
    mMqttClient->setUsername(username);
    mMqttClient->setPassword(password);
    mMqttClient->setCleanSession(true);
    mMqttClient->setClientId(clientID);

    connect(mMqttClient.data(), &QMQTT::Client::received, this, &TryToTriggerMosquittoError::onReceived);
    connect(mMqttClient.data(), &QMQTT::Client::error, this, &TryToTriggerMosquittoError::onError);
    connect(mMqttClient.data(), &QMQTT::Client::connected, this, &TryToTriggerMosquittoError::onConnect);
}

void TryToTriggerMosquittoError::doStuff()
{
    mMqttClient->connectToHost();
}

void TryToTriggerMosquittoError::onReceived(const QMQTT::Message &message)
{
    //return;
    QString line = QString("%1: %2").arg(message.topic()).arg(QString::fromUtf8(message.payload()));
    qDebug() << line;
}

void TryToTriggerMosquittoError::onError(const QMQTT::ClientError error)
{
    QString line = QString("Error occurred. QMQTT error code %1").arg(error);
    qDebug() << line;
}

void TryToTriggerMosquittoError::onConnect()
{
    const QString subscribePath = "N/847e4062cb99/#";

    QString line = QString("Connection established. Publishing %1").arg(subscribePath);
    qDebug() << line;

    mMqttClient->subscribe(subscribePath, 0);
    mMqttClient->subscribe("#", 0);


    connect(&mPublishTimer, &QTimer::timeout, this, &TryToTriggerMosquittoError::keepAlivePublish);
    mPublishTimer.setSingleShot(false);
    mPublishTimer.start(50000);
    keepAlivePublish();
}

void TryToTriggerMosquittoError::keepAlivePublish()
{
    const QString publishPath = "R/847e4062cb99/system/0/Serial";
    qDebug() << QString("Publishing %1 for keepalive").arg(publishPath);
    const int qos = 2;
    QMQTT::Message message(qrand(), publishPath, QByteArray(), qos);
    mMqttClient->publish(message);
}
